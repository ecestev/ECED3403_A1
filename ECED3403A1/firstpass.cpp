#pragma once
#include "firstpass.h"

using namespace std;

/********************************************************************************************
*                      FUNCTION TO PARSE LINE AND RETURN RESULT
*******************************************************************************************/

int firstPass(string to_parse, int linenum, int locCnt, instructions *instr_list, directives *dir_list, list<symtblrec> *symtbl) {
	// Declarations and Initizlizations for function
	record *rec = new record;							// record being processed
	int increment = 0;									// amount to increment location counter by
	int addrMode1 = -1;									// addressing mode for operand 1
	int addrMode2 = -1;									// addressing mode for operand 2
	int errType = 0;									// stores error type indicator
	int expectedOperands = -1;							// number of operands expected
	int foundOperands = -1;								// number of operands found	
	int result = 0;										// stores result of label validation
	rec->value = locCnt;								// store value of LC in record
	rec->linenum = linenum;								// store value of linenum in record

	// If line is too long no processing is to be done on the line
	if (to_parse.size() > LINE_LEN) {					// if line is too long
		errType = -2;									// error: line too long
		return errType;									// return errType
	}													

	// If line starts with comment no processing is to be done on the line
	if(to_parse[0] == ';') {							// if line starts with comment
		errType = -3;									// ignore comment line
		return errType;									// return errType
	}													

	// Strip away comments from string
	string::size_type pos = to_parse.find(';', 0);		// find comment if it exist
	to_parse = to_parse.substr(0, pos);					// strip away comment if it exists

	// initialize istream iterator on istringstream so we can easily iterate through the line
	string token;										// token to be analyzed
	istringstream *iss = new istringstream(to_parse);	// string stream iss is pointer to
	istream_iterator<string> *iit =						// istream iterator iit is pointer to 
		new istream_iterator<string>(*iss);				// a new istream iterator
	istream_iterator<string> *eos =						// istream iterator eos is pointer to
		new istream_iterator<string>;					// a new end-of-stream iterator

	// check if first token is an instruction or directive if not, handle it as a label
	if (*iit != *eos) {									// check if iterator is pointing to end-of-stream
		token = **iit;									// store what iterator is pointing to in token
			validateInst(token, rec, instr_list);		// check if first token is an instruction
			if (rec->insttype > -1)						// if valid instruction found
				rec->instrincr = 2;						// instruction increments LC by 2
			if (rec->found == false)					// first token is not an instruction
				validateDir(token, rec, dir_list);		// check if first token is a directive
			if(rec->found==false) {
				if (validate_label(token)) {			// check if label is valid
					rec->label = token;					// store label if valid
					result = processLabel(rec->label, locCnt, false, symtbl);
					if (result == -1) {					// label exists in symbol table
						errType = -5;					// error: duplicate label
						return errType;					// return errType
					}
					if (result == 1) {					// if label does not exist in symbol tabel
						symtblrec *newrecord = new symtblrec;// create new symtblrec
						newrecord->label = rec->label;	// set record's label
						newrecord->type = "KNOWN";		// set record's type
						newrecord->value = locCnt;		// set record's location counter
						symtbl->push_back(*newrecord);	// push record to symbol table
					}
					result = 0;							// reset variable for later use
				}
				else {									// label is not valid	
					errType = -4;						// error: invalid label
					return errType;						// return errType
				}
			}
	} else {											// iterator points to end-of-stream
		errType = -1;									// empty line
		return errType;									// return error
	}

	// if an instruction or directive hasn't been found yet (first token was a label)
	// get next token and ensure it is a valid instruction|directive. If not, error & exit
	if (rec->found == false) {							// instruction|directive not found yet												
		++*iit;											// increment iterator position
		if (*iit != *eos) {								// check if iterator is pointing to end-of-stream
			token = **iit;								// store what iterator is pointing to in token
			validateInst(token, rec, instr_list);		// check if second token is an instruction
			if (rec->insttype > -1)						// if valid instruction found
				rec->instrincr = 2;						// instruction increments LC by 2
			if (rec->found == false)					// second token is not an instruction
				validateDir(token, rec, dir_list);		// check if first token is a directive
			if (rec->found == false) {					// no valid instruction or directive found
				errType = -6;							// error: invalid inst|dir
				return errType;							// return errType 
			}												
		}													
	}	
	if (rec->found == false)							// if no record has been found at this point
		return -1;										// line contained just a label. Exit and do not incr LC
	
	// Special handler for end directive to specify starting address if end has an opernad to and 
	// stop pass 
	if (rec->mnem == "END") {							// if end directive found	
		++*iit;											// increment stream iterator to next pos
		if (*iit != *eos) {								// if we have not reached the end of string
			token = **iit;								// store token
			if (validate_label(token)) {				// if token is a valid label
				starting_addr = symtbl_Lookup(token, symtbl);	// set starting address
				start_addr_set = true;					// set flag that start address has been defined
			}											
			else if (is_hex(token)) {					// if token is a hex value
				starting_addr = get_hex(token);			// set starting address to value of token
				start_addr_set = true;					// set flag that start address has been defined
			}											
			else if (is_dec(token)) {					// if token is a dec value
				starting_addr = get_dec(token);			// set starting address to value of token
				start_addr_set = true;					// set flag that start address has been defined
			}
		}
		errType = -9;									// set error flag to end directive found
		return errType;									// return errType
	}

	// define values for # operands expected and # operands found. Also ensure there is nothing left in the 
	// line after the operands, otherwise it is an error.
	if (rec->dirtype > -1) {							// if a directive was found														
		expectedOperands = rec->dirtype;				// set number of expected operands for directive 
	}													
	if (rec->insttype > -1) {							// if an instruction was found
		if (rec->insttype == 3)							// if jump instruction
			expectedOperands = 1;						// we expect one operand
		else 											// else not a jump instruction
			expectedOperands = rec->insttype;			// set number of expected operands based on inst type
		if (instr_found == false) {						// if this is the first instruction we have encountered
			starting_addr = locCnt;						// set starting address for execution (used in s9 record)
			instr_found = true;							// flag that instruction has been found
		}
	}
	++*iit;												// increment stream iterator to next pos
	if (*iit != *eos) {									// if we have not reached the end of string
		token = **iit;									// get next token
		if (rec->mnem == "STRING") {					// if directive is string
			if ((validate_string(rec, iss, iit, eos))) {// if operand is valid string
				foundOperands = 1;						// set found operands
				valDirOp(rec);							// validate directive operand
			} else {									// else operand is not valid string
				errType = -7;							// error: invalid operand(s)
				return errType;							// return errType
			}											
		} else {										// if inst|dir is anything other than STRING
			pos = token.find(',', 0);					// see if comma separated operands exist
			if (pos < LINE_LEN)							// if comma found in token
				foundOperands = 2;						// two operands have been found
			else										// otherwise
				foundOperands = 1;						// one operand has been foudn
		}												
	} else {											// else if we have reached the end of the string
		foundOperands = 0;								// no operands have been found
	}													
	// ensure there is nothing left in the string and throw error if there is
	++*iit;												// increment stream iterator to next pos
	if (*iit != *eos) {									// if we have not reached the end of string
		errType = -8;									// error: invalid record format
		return errType;									// return errType 
	}													


	// see if instruction has the proper number of operands. If so, validate the operande
	// otherwise, set error type and return error type
	if(expectedOperands != foundOperands) {				// expectedOperands != foundOperands
		errType = -7;									// error: invalid operand(s)	
		return errType;									// return errType 
	}		
	
	// HANDLE ZERO OPERAND INSTRUCTIONS / DIRECTIVES
	if (foundOperands == 0) {							// if inst|dir has zero operands
		if (rec->insttype > -1) 						// if it is an instruction
			;											// RETI no handler needed on first pass
		if (rec->dirtype > -1) {						// if if is a directive
			return (locCnt + valDirOp(rec));			// increment LC by amount dictated by directive
		}											 
	}												

	// HANDLE ONE OPERAND INSTRUCTIONS / DIRECTIVES
	if (foundOperands == 1) {							// if that number is 1
		if(rec->mnem != "STRING")						// so long as it wasn't a string
			rec->srcop = token;							// store operand in record
		if (rec->insttype > -1) {						// if it was an instruction that was found
			addrMode1 = valAddMode("SRC", rec, symtbl);	// validate the addressing mode 
			if (addrMode1 < 0) {						// valid source operand was found
				if (addrMode1 == -1)					// Invalid Operand Format
					errType = -7;						// error: invalid operand(s)
				if (addrMode1 == -2)					// Invalid Addressing Mode
					errType = -10;						// error: invalid operand(s)
				return errType;							// return errType 
			}											
			if (rec->insttype == 3) {					// if instruction is jump instruction
				if (addrMode1 != 7){					// if addressing mode is not relative
					errType = -7;						// error: invalid operand(s)
					return errType;						// return errType
				}										
			}											
			rec->srcincr = 0;							// operand does not increment LC for jumps
		}												
		if (rec->dirtype > -1) {						// otherwise it was a directive that was found
			int temp = valDirOp(rec);					// validate directive operand
			if (temp == ERR_VAL) { 						// if value returned is not ERR_VAL (65536)
				errType = -7;							// error: invalid operand
				return errType;							// return error type
			}
			if (rec->mnem == "EQU") {					// check if directive was EQU
				if (temp == -1) {						// if temp = -1 (flag telling us it is a label)
					int addr = symtbl_Lookup(rec->srcop, symtbl);
					if (addr != -1)						
						symentry_Update(rec->label, addr, symtbl);					
				}									
				if (temp == -2)							// if temp = -2 (flag telling us it is a hex)
					symentry_Update(rec->label, get_hex(rec->srcop), symtbl);
				if (temp == -3) {						// if temp = -3 (flag telling us it is a dec)
					symentry_Update(rec->label, get_dec(rec->srcop), symtbl);
				}
			} else										// else directive was not EQU
				rec->instrincr = temp;					// increment LC by value from valDirOp(rec)									
		}												
	}													

	// HANDLE TWO OPERAND INSTRUCTIONS / DIRECTIVES
	if (foundOperands == 2) {							// if that number is 2
		rec->srcop = token.substr(0, pos);				// store operand1 in record
		rec->dstop = token.substr(pos+1);				// store operand2 in record
		addrMode1 = valAddMode("SRC", rec, symtbl);		// get addressing mode for operand1
		if (addrMode1 < 0) {							// valid source operand was found
			if (addrMode1 == -1)						// Invalid Operand Format
				errType = -7;							// error: invalid operand(s)
			if (addrMode1 == -2)						// Invalid Addressing Mode
				errType = -10;							// error: invalid operand(s)
			return errType;								// return errType 
		}												
		addrMode2 = valAddMode("DST", rec, symtbl);		// get addressing mode for destination operand
		if (addrMode2 < 0) {							// valid dest operand was found
			if (addrMode2 == -1)						// Invalid Operand Format
				errType = -7;							// error: invalid operand(s)
			if (addrMode2 == -2)						// Invalid Addressing Mode
				errType = -10;							// error: invalid operand(s)
			return errType;								// return errType 
		}											 
	}												 
	
	if (!rec->srcOpLbl.empty()) {						// if operand contains a label
		if (processLabel(rec->srcOpLbl, NUL, true, symtbl) == 1) { // if label does not exist in symbol table
			symtblrec *newrecord = new symtblrec;		// create new symtblrec
			newrecord->label = rec->srcOpLbl;			// set record's label
			newrecord->type = "UNKNOWN";				// set record's type
			newrecord->value = NUL;						// set record's location counter
			symtbl->push_back(*newrecord);				// push record to symbol table
			unknown_records++;							// increment unknown record count (forward reference)
		}												
	}													
	if (!rec->dstOpLbl.empty()) {						// if operand contains a label
		if (processLabel(rec->dstOpLbl, NUL, true, symtbl) == 1) { // if label does not exist in symbol table
			symtblrec *newrecord = new symtblrec;		// create new symtblrec
			newrecord->label = rec->dstOpLbl;			// set record's label
			newrecord->type = "UNKNOWN";				// set record's type
			newrecord->value = NUL;						// set record's location counter
			symtbl->push_back(*newrecord);				// push record to symbol table
			unknown_records++;							// increment unknown record count (forward reference)
		}												
	}													
	
	rec->incrValue = rec->instrincr + rec->srcincr + rec->dstincr;
	locCnt += rec->incrValue;							// update location counter
	return locCnt;										// return location coutner
}


