#pragma once
#include "secondpass.h"

using namespace std;

int secondPass(string to_parse, int linenum, instructions *instr_list, directives *dir_list, list<symtblrec> *symtbl, s1record *s1rec) {
	/**************************************************************************************************
										SETUP & INITIALIZATION
	**************************************************************************************************/
	record *rec = new record;								// record being processed
	stringstream *instr_stream = new stringstream;			// stream to read instruction into
	int addrMode1 = -1;										// addressing mode for op1
	int addrMode2 = -1;										// addressing mode for op2
	int errType = 0;										// error type
	string instr = "\0";									// insrtuction as string
	uint16_t instr_int = 0;									// instruction as int
	uint8_t highByte = 0;									// high byte of instruction
	uint8_t lowByte = 0;									// low byte of instruction
	rec->linenum = linenum;									// store line number in record
	rec->value = global_loc;								// store global location counter in record

	// If line starts with comment no processing is to be done on the line
	if(to_parse[0] == ';')									// if line starts with comment
		return -1;											// ignore line

	// Strip away comments from string
	string::size_type pos = to_parse.find(';', 0);			// find comment if it exist
	to_parse = to_parse.substr(0, pos);						// strip away comment if it exists

	// initialize istream iterator on istringstream so we can easily iterate through the line
	string token;											// token to be analyzed
	istringstream *iss = new istringstream(to_parse);		// string stream iss is pointer to
	istream_iterator<string> *iit =							// istream iterator iit is pointer to 
		new istream_iterator<string>(*iss);					// a new istream iterator
	istream_iterator<string> *eos =							// istream iterator eos is pointer to
		new istream_iterator<string>;						// a new end-of-stream iterator

/**************************************************************************************************
										BEGIN PARSING
**************************************************************************************************/

	if (*iit != *eos) {										// check if iterator is pointing to end-of-stream
		token = **iit;										// store what iterator is pointing to in token
		validateInst(token, rec, instr_list);				// check if first token is an instruction
		if (rec->insttype > -1) 							// if valid instruction found
			rec->instrincr = 2;								// location counter increment increased by 2
		if (rec->found == false)							// first token is not an instruction
			validateDir(token, rec, dir_list);				// check if first token is a directive
	}

	// if first token is not an instruction or directive we move to next token
	if (rec->found == false) {								// instr|dir not found yet
		++*iit;												// increment iterator position
		if (*iit != *eos) {									// check if iterator is pointing to end-of-stream
			token = **iit;									// store what iterator is pointing to in token
			validateInst(token, rec, instr_list);			// check if second token is an instruction
			if (rec->insttype > -1)							// if valid instruction found
				rec->instrincr = 2;							// location counter increment increased by 2
			if (rec->found == false)						// second token is not an instruction
				validateDir(token, rec, dir_list);			// check if first token is a directive
			if (rec->found == false) 						// no valid instruction or directive found
				return -1;									// return to main
		} else												// no more tokens
			return -1;										// return to main 
	}														// end case for intr|dir not found yet
	if (rec->mnem == "END") {								// if end directive was found
		int errType = -9;									// set error flag to 'END' found
		return errType;										// return error type to main
	}														// otherwise
	++*iit;													// increment stream iterator to next pos
	if (*iit != *eos) 										// if we have not reached the end of string
		token = **iit;										// get next token

/**************************************************************************************************
										HANDLE INSTRUCTION
**************************************************************************************************/
	if (rec->insttype >-1) {								// if we are dealing with an instruction
		listout << setw(6) << setfill(' ') << HEX4(global_loc) << "   ";
		listout << setw(6) << setfill(' ') << rec->mnem << " ";
		if (rec->insttype == 0) {							// if inst has zero operands (RETI)
			*instr_stream << bitset<16>(rec->opcode);		// OPCODE (9 bits + 0's for 7 bits = just opcode)
			listout << setw(22) << setfill(' ') << " ";
		} else if (rec->insttype == 1) {					// if inst|dir has one operand
			rec->srcop = token;								// store operand in record
			addrMode1 = valAddMode("SRC", rec, symtbl);		// validate the addressing mode 
			if (rec->insttype == 1) {						// if if is a one operand instruction
				*instr_stream << bitset<9>((rec->opcode >> 7));	// OPCODE (9 bits)
				*instr_stream << rec->BW;					// BW (1 bit)
				*instr_stream << bitset<2>(rec->As);		// AS (2 bits)
				*instr_stream << bitset<4>(rec->source);	// SOURCE (4 bits)
			}												
			listout << setw(10) << setfill(' ') << rec->srcop << " ";
			listout << setw(11) << setfill(' ') << " ";
		}
		else if (rec->insttype == 2) {						// if inst|dir has two operands		
			pos = token.find(',', 0);						// set position to where comma separates them
			rec->srcop = token.substr(0, pos);				// store operand1 in record
			rec->dstop = token.substr(pos + 1);				// store operand2 in record
			addrMode1 = valAddMode("SRC", rec, symtbl);		// get addressing mode for operand1
			addrMode2 = valAddMode("DST", rec, symtbl);		// get addressing mode for operand 2
			*instr_stream << bitset<4>((rec->opcode >> 12));// OPCODE (4 bits)
			*instr_stream << bitset<4>(rec->source);		// SOURCE (4 bits)
			*instr_stream << bitset<1>(rec->Ad);			// AD (1 bit)
			*instr_stream << rec->BW;						// BW (1 bit)
			*instr_stream << bitset<2>(rec->As);			// AS (2 bits)
			*instr_stream << bitset<4>(rec->dest);			// DESTINATION (4 bits)
			listout << setw(10) << setfill(' ') << rec->srcop << " ";
			listout << setw(10) << setfill(' ') << rec->dstop << " ";
		}
		else if (rec->insttype == 3) {						// if it is a jump instruction
			rec->srcop = token;								// store operand in record
			addrMode1 = valAddMode("SRC", rec, symtbl);		// validate the addressing mode 
			int dest = 0;									// location of destination from operand
			int jump = 0;									// length of jump
			if (addrMode1 == 7)								// if relative addressing mode
				dest = rec->srcOpData;						// destination address set by ValAddMode
			else {											// operand is none of the above
				errType = -2;								// illegal jump offset
				return errType;								// return error type to main
			}
	//		jump = dest - (global_loc + 2);					// jump length is difference between LC and Dest
			jump = dest;
			jump = jump >> 1;								// bit shift jump 
			if ((jump < -511) || (jump > 510)) { 			// if outside 10-bit signed range
				errType = -2;								// illegal jump offset
				return errType;								// return error type to main
			}												
			*instr_stream << bitset<6>((rec->opcode >> 10));// add jump opcode to  instr. stream
			*instr_stream << bitset<10>(jump);				// add jump offset to instr. stream
			rec->srcOpData = -1;							// clear srcOpData field set when valAddMode was called
			listout << setw(10) << setfill(' ') << rec->srcop << " ";
			listout << setw(11) << setfill(' ') << " ";
		}													
		*instr_stream >> instr;								// write instr_stream to string
		instr_int = stoi(instr, nullptr, 2);				// cast machine code to integet value (uint16_t)	
		listout << "   ";
		listout << HEX4(instr_int);
		if (global_loc % 2 != 0)							// if LC not even, alignment needed
			diagout << "LINE: " << linenum << ". Alignment Needed" << endl;
		s1rec_write_word(s1rec, instr_int);					// write machine instruction to srecord
		if (rec->srcOpData != -1) {							// if source operand has data for machine
			s1rec_write_word(s1rec, rec->srcOpData);		// write data to srecord
			listout << HEX4(rec->srcOpData);				// write data to list file
		}
		if (rec->dstOpData != -1) {							// if dest operand has data for machine
			s1rec_write_word(s1rec, rec->dstOpData);		// write data to srecord
			listout << HEX4(rec->dstOpData);				// write data to list file
		}
		listout << endl;
		return global_loc;									// return to main
	}														 

/**************************************************************************************************
										HANDLE DIRECTIVE
**************************************************************************************************/

	if (rec->dirtype > -1) {								// if instruction
		rec->srcop = token;									// set source operand to token
		valDirOp(rec);										// validate directive operand
		listout << setw(6) << setfill(' ') << HEX4(global_loc) << "   ";
		listout << setw(6) << setfill(' ') << rec->mnem << " ";
		
		// if it is a string, get the full string from validate_string()
		// and output the machine code to srecord. Also output the string 
		// and machine code to the list file
		if (rec->mnem == "STRING") {
			validate_string(rec, iss, iit, eos);
			listout << setw(20) << setfill(' ') << rec->srcop << "     ";
			for (unsigned i = 0; i < rec->srcop.length(); i++) {
				s1rec_write_byte(s1rec, int(rec->srcop[i]));// write data to srecord
				listout << HEX2(rec->srcop[i]);				// write data to list file
			}
			listout << endl;								// list file formatting
			return global_loc;								// return LC to main
		}

		// directive was not a string, output formatted source operand
		// to list file
		listout << setw(10) << setfill(' ') << rec->srcop << " ";
		listout << setw(14) << setfill(' ') << " ";
		if (rec->mnem == "EQU") {							// if EQU
			listout << endl;								// list file formatting
			return global_loc;								// return LC to main
		}
		if (rec->mnem == "ORG") {							// if ORG
			global_loc += rec->srcOpData;					// +/- LC by specified amount
			listout << endl;								// list file formatting
			return global_loc;								// return LC to main
		}
		if (rec->mnem == "ALIGN") {							// if Align
			if (rec->srcOpData == 1) 						// if increment is necessary
				global_loc++;								// increment LC by 1
			listout << endl;								// list file formatting
			return global_loc;								// return LC to main
		}	
		
		// if control gets here, source operand is data that needs to be loaded 
		// to the machine. Write this data to both the list file and srecord
		if (rec->mnem == "WORD") {							
			s1rec_write_word(s1rec, rec->srcOpData);		// write machine code to srec
			listout << HEX4(rec->srcOpData);				// write machine code to list
			listout << endl;								// list file formatting
			return global_loc;								// return LC to main
		}													
		if (rec->mnem == "BYTE") {							
			s1rec_write_byte(s1rec, rec->srcOpData);		// write machine code to srec		
			listout << HEX2(rec->srcOpData);				// write machine code to list
			listout << endl;								// list file formatting
			return global_loc;								// return LC to main
		}													
		if (rec->mnem == "BSS") {							
			for (int i = 0; i < rec->srcOpData; i++) {		// for as many bytes to be reserved
				s1rec_write_byte(s1rec, 0);					// write machine code to srec
				listout << HEX2(0);							// write machine code to list
			}
			listout << endl;								// list file formatting
			return global_loc;								// return LC to main
		}																
	}														
	return global_loc;										// return LC to main
}
/**************************************************************************************************
									 -- END OF FILE --
**************************************************************************************************/