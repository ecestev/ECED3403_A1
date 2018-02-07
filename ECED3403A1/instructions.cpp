#pragma once
#include "instructions.h"
#include <iostream>

using namespace std;

void initialize_insts(instructions *inst) {

	inst->mnem[0] = "ADD";
	inst->type[0] = 2;
	inst->opcode[0] = 0x5000;
	inst->bw[0] = true;

	inst->mnem[1] = "ADDC";
	inst->type[1] = 2;
	inst->opcode[1] = 0x6000;
	inst->bw[1] = true;

	inst->mnem[2] = "AND";
	inst->type[2] = 2;
	inst->opcode[2] = 0xf000;
	inst->bw[2] = true;

	inst->mnem[3] = "BIC";
	inst->type[3] = 2;
	inst->opcode[3] = 0xc000;
	inst->bw[3] = true;

	inst->mnem[4] = "BIS";
	inst->type[4] = 2;
	inst->opcode[4] = 0xd000;
	inst->bw[4] = true;

	inst->mnem[5] = "BIT";
	inst->type[5] = 2;
	inst->opcode[5] = 0xb000;
	inst->bw[5] = true;

	inst->mnem[6] = "CALL";
	inst->type[6] = 1;
	inst->opcode[6] = 0x1280;
	inst->bw[6] = false;

	inst->mnem[7] = "CMP";
	inst->type[7] = 2;
	inst->opcode[7] = 0x9000;
	inst->bw[7] = true;

	inst->mnem[8] = "DADD";
	inst->type[8] = 2;
	inst->opcode[8] = 0xa000;
	inst->bw[8] = true;

	inst->mnem[9] = "JC";
	inst->type[9] = 3;
	inst->opcode[9] = 0x2c00;
	inst->bw[9] = false;

	inst->mnem[10] = "JEQ";
	inst->type[10] = 3;
	inst->opcode[10] = 0x2400;
	inst->bw[10] = false;

	inst->mnem[11] = "JGE";
	inst->type[11] = 3;
	inst->opcode[11] = 0x3400;
	inst->bw[11] = false;


	inst->mnem[12] = "JHS";
	inst->type[12] = 3;
	inst->opcode[12] = 0x2c00;
	inst->bw[12] = false;

	inst->mnem[13] = "JL";
	inst->type[13] = 3;
	inst->opcode[13] = 0x3800;
	inst->bw[13] = false;

	inst->mnem[14] = "JLO";
	inst->type[14] = 3;
	inst->opcode[14] = 0x2800;
	inst->bw[14] = false;

	inst->mnem[15] = "JMP";
	inst->type[15] = 3;
	inst->opcode[15] = 0x3c00;
	inst->bw[15] = false;

	inst->mnem[16] = "JN";
	inst->type[16] = 3;
	inst->opcode[16] = 0x3000;
	inst->bw[16] = false;

	inst->mnem[17] = "JNC";
	inst->type[17] = 3;
	inst->opcode[17] = 0x2800;
	inst->bw[17] = false;

	inst->mnem[18] = "JNE";
	inst->type[18] = 3;
	inst->opcode[18] = 0x2000;
	inst->bw[18] = false;

	inst->mnem[19] = "JNZ";
	inst->type[19] = 3;
	inst->opcode[19] = 0x2000;
	inst->bw[19] = false;

	inst->mnem[20] = "JZ";
	inst->type[20] = 3;
	inst->opcode[20] = 0x2400;
	inst->bw[20] = false;

	inst->mnem[21] = "MOV";
	inst->type[21] = 2;
	inst->opcode[21] = 0x4000;
	inst->bw[21] = true;

	inst->mnem[22] = "PUSH";
	inst->type[22] = 1;
	inst->opcode[22] = 0x1200;
	inst->bw[22] = true;

	inst->mnem[23] = "RETI";
	inst->type[23] = 0;
	inst->opcode[23] = 0x1300;
	inst->bw[23] = false;

	inst->mnem[24] = "RRA";
	inst->type[24] = 1;
	inst->opcode[24] = 0x1100;
	inst->bw[24] = true;

	inst->mnem[25] = "RRC";
	inst->type[25] = 1;
	inst->opcode[25] = 0x1000;
	inst->bw[25] = true;

	inst->mnem[26] = "SUB";
	inst->type[26] = 2;
	inst->opcode[26] = 0x8000;
	inst->bw[26] = true;

	inst->mnem[27] = "SUBC";
	inst->type[27] = 2;
	inst->opcode[27] = 0x7000;
	inst->bw[27] = true;

	inst->mnem[28] = "SWPB";
	inst->type[28] = 1;
	inst->opcode[28] = 0x1080;
	inst->bw[28] = false;

	inst->mnem[29] = "SXT";
	inst->type[29] = 1;
	inst->opcode[29] = 0x1180;
	inst->bw[29] = false;

	inst->mnem[30] = "XOR";
	inst->type[30] = 2;
	inst->opcode[30] = 0xe000;
	inst->bw[30] = true;

}

// validates addressing mode for source|destination operand and set's 
// approperiat record fields based on results. 
int valAddMode(string SRCDST, record *testrecord, list<symtblrec> *symtbl) {
	/**********************************************************************/	
	/*								SETUP                                 */
	/**********************************************************************/
	string operand;						// operand being tested
	bool dst = false;					// flag: source operand
	bool src = false;					// flag: dest operand
	if (SRCDST == "SRC") {				// if SRC passed to function
		src = true;						// set src flag 
		operand = testrecord->srcop;	// set record's source op field
	}
	else if (SRCDST == "DST") {			// if DST passed to function
		dst = true;						// set dst flag
		operand = testrecord->dstop;	// set record's dest op field
	}
	else return -1;						// if neither SRC or DST are passed: error

	/**********************************************************************/	
	/*		  Check Indirect @Rn and Indirect Auto Increment '@Rn+'		  */
	/*						 SRC ONLY / INCR = 0                          */
	/**********************************************************************/
	if (operand[0] == '@') {			// if operand starts with '@'
		if (dst == true)				// if this is a dst operand
			return -2;					// invalid addressing mode for destination
		string testreg = operand.substr(1);
		bool incr = false;				// flag for ind vs ind+
		int pos = testreg.find('+');	// if '+' is found
		if (pos < testreg.length()) {	// take substring removing '+'
			testreg = testreg.substr(0, pos);
			incr = true;				// and set incr flag
		}
		if (validate_register(SRCDST, testreg, testrecord) == false)
			return -1;					// if invalid register return error
		if (incr == true) {				// determined to be indirect w/increment
				testrecord->As = 3;		// set As value 3
				return 2;				// return mode 2 (Indirect+)
		}							
			testrecord->As = 2;			// else set As value to 2
			return 1;					// return mode 1 (Indirect)
	}

	/**********************************************************************/
	/*						Check Register Direct 'Rn'					  */
	/*							SRC|DST / INCR = 0                        */
	/**********************************************************************/
	if (validate_register(SRCDST, operand, testrecord) == true) {
		if(src==true)					// if operand is valid reg and src is true
			testrecord->As = 0;			// set As to 0
		if(dst==true)					// if operand is valid reg and dst is true
			testrecord->Ad = 0;			// set Ad to 0
		return 3;						// reutrn mode 3 (Direct)
	}

	/**********************************************************************/
	/*							Check Indexed 'x(Rn)'					  */
	/*							SRC|DST / INCR = 2                        */
	/**********************************************************************/
	int pos1 = operand.find('(');		// find braces that may be enclosing a
	int pos2 = operand.find(')');		// register and store their positions
	if (pos1 < operand.length() && pos2 < operand.length()) {
		int len = pos2 - pos1;			// how many characters are between braces
		// break up the string, supposedly in format x(Rn) into 'x' and 'Rn'
		// by creating two substrings - one up until the opening brace and the
		// other containing the characters enclosed in braces
		string teststring = operand.substr(0, pos1);
		string testreg = operand.substr(pos1 + 1, len - 1);
		// ensure 'x' is not a valid register and that 'Rn' is.
		if (validate_register(SRCDST, teststring, testrecord) == true)
			return -1;
		if (validate_register(SRCDST, testreg, testrecord) == false)
			return -1;
		// check if 'x' is a valid label and handle it accordingly
		// if we are working on source operand, set record's label
		// field, and increment values. If second pass, set As/Ad and 
		// data field to the location of the label in the symbol table
		if (validate_label(teststring)) {
			if (src == true) {			
				testrecord->srcOpLbl = teststring;
				testrecord->srcincr = 2;
				if (second_pass == true) {
					testrecord->As = 1;
					testrecord->srcOpData = symtbl_Lookup(teststring, symtbl);
				}
			}
			if (dst == true) {
				testrecord->dstOpLbl = teststring;
				testrecord->dstincr = 2;
				if (second_pass == true) {
					testrecord->Ad = 1;
					testrecord->dstOpData = symtbl_Lookup(teststring, symtbl);
				}
			}
			return 5;					// return mode 5: indexed (label)
		}
		// check if 'x' is a valid hex value and handle it accordingly as
		// above, except use the hex value to set the src data field 
		if (is_hex(teststring)) {
			if (src == true) {
				testrecord->srcincr = 2;
				if (second_pass == true) {
					testrecord->As = 1;
					testrecord->srcOpData = get_hex(teststring);
				}
			}
			if (dst == true) {
				testrecord->dstincr = 2;
				if (second_pass == true) {
					testrecord->Ad = 1;
					testrecord->dstOpData = get_hex(teststring);
				}
			}
			return 4;					// return mode 4: indexed (value)
		}
		// check if 'x' is a valid dec value and handle it accordingly as
		// above, except use the dec value to set the src data field 
		if (is_dec(teststring)) {
			if (src == true) {
				testrecord->srcincr = 2;
				if (second_pass == true) {
					testrecord->As = 1;
					testrecord->srcOpData = stoi(teststring);
				}
			}
			if (dst == true) {
				testrecord->dstincr = 2;
				if (second_pass == true) {
					testrecord->Ad = 1;
					testrecord->dstOpData = stoi(teststring);
				}
			}
			return 4;					// return mode 4: indexed (value)
		}
		return -1;						// error: no valid label/value
	}

	/**********************************************************************/
	/*							Check Immediate '#x'					  */
	/*							SRC ONLY / INCR = 2                       */
	/**********************************************************************/
	if (operand[0] == '#') {	// check if first character is '#'
		if (dst == true)		// if it is and we are analyzing des op
			return -2;			// invalid addressing mode for destination
		// otherwise proceed. First strip '#' and see if 'x' is a valid label
		// or hex/dec value. Set As and OpData field in record appropriately 
		// if second pass.
		string teststring = operand.substr(1);
		// handle case where 'x' is a label
		if (validate_label(teststring)) {
			testrecord->srcOpLbl = teststring;
			testrecord->srcincr = 2;
			if (second_pass == true) {
				testrecord->As = 3;
				testrecord->srcOpData = symtbl_Lookup(teststring, symtbl);
			}
			return 8;					// return mode 8 : immediate (label)
		}
		// handle case where 'x' is a hex value
		if (is_hex(teststring)) {
			testrecord->srcincr = 2;
			if (second_pass == true) {
				testrecord->As = 3;
				testrecord->srcOpData = get_hex(teststring);
			}
			return 9;					// return mode 9: immediate (value)
		}
		// handle case where 'x' is a dec value
		if (is_dec(teststring)) {
			testrecord->srcincr = 2;
			if (second_pass == true) {
				testrecord->As = 3;
				testrecord->srcOpData = get_dec(teststring);
			}
			return 9;					// return mode 9: immediate (value)
		}
		return -1;						// error: no valid label/value
	}

	/**********************************************************************/
	/*						Check ABSOLUTE '&ADDR'						  */
	/*						  SRC|DST / INCR = 2				          */
	/*                     NOTE: SOURCE = SR (R2)						  */
	/**********************************************************************/
	if (operand[0] == '&') {			// check if fist character is '&'
		// and strip it away. Proceed to verify that 'ADDR' is a valid label.
		// set As/Ad, op label, increment and op data fields. Set source
		// to status register (R2)
		string testlabel = operand.substr(1);
		if (validate_label(testlabel)) {
			if (src == true) {
				testrecord->srcOpLbl = testlabel;
				testrecord->srcincr = 2;
				if (second_pass == true) {
					testrecord->As = 1;
					testrecord->source = 2;
					testrecord->srcOpData = symtbl_Lookup(testlabel, symtbl);
				}
			}
			if (dst == true) {
				testrecord->dstOpLbl = testlabel;
				testrecord->dstincr = 2;
				if (second_pass == true) {
					testrecord->Ad = 1;
					testrecord->dest = 2;
					testrecord->dstOpData = symtbl_Lookup(testlabel, symtbl);
				}
			}
			return 6;					// return mode 6: absolute
		}
		return -1;						// error: no valid label
	}

	/**********************************************************************/
	/*							Check RELATIVE 'ADDR'					  */
	/*							 SRC|DST / INCR = 2                       */
	/**********************************************************************/
	// check if ADDR is a valid label or hex/dec value. Set record's increment
	// and if it is a label also set the op label field. If it is the second 
	// pass set As/Ad values and store the value of ADDR (either hex/dec or
	// location from symbol table lookup) in record's op data field.
	int value = -1;
	bool valid = false; 
	bool label = false;
	if(validate_label(operand)){
		valid = true;
		label = true;
		if (second_pass == true)
			value = symtbl_Lookup(operand, symtbl);
	} else if(is_hex(operand)) {
		valid = true;
		if(second_pass==true) {
			value = get_hex(operand);
		}
	}
	else if(is_dec(operand)) {
		valid = true;
		if (second_pass == true) {
			value = get_dec(operand);
		}
	}
	if (valid) {
		if (src == true) {
			if(label==true)
				testrecord->srcOpLbl = operand;
			testrecord->srcincr = 2;
			if (second_pass == true) {
				testrecord->As = 1;
				testrecord->srcOpData = (value - (global_loc+2));
			}
		}
		if (dst == true) {
			if(label==true)
				testrecord->dstOpLbl = operand;
			testrecord->dstincr = 2;
			if (second_pass == true) {
				testrecord->Ad = 1;
				testrecord->dstOpData = (value - (global_loc + 2));
			}
		}
		return 7;						// return mode 7: relative
	}

	// NO VALID ADDRESSING MODE FOUND
	return -1;							// error: no valid label/value
}

/*********************************************************************************/
/*					Validates Instruction by Comparing to list 
/*		of supported instructions and sets record values approperiately                                                                              
/*********************************************************************************/
void validateInst(string testinst, record *test_record, instructions *instr_list) {
	bool bwfound = false;				// flag for BW specifier
	// convert testinst to all uppercase
	for (unsigned i = 0; i < testinst.length(); i++)
		if (isalpha(testinst[i]) != 0)
			testinst[i] = toupper(testinst[i]);
	// check if inst contains .B or .W specifier
	int pos = testinst.find('.');		// search string for '.'
	if (pos < testinst.length()) {		// if '.' found in instruction
		if (testinst[pos + 1] == 'B') {	// .B found in instruction
			bwfound = true;				// set byte flag
			test_record->BW = 1;		// set BW value to 1 (byte)
		}
		if (testinst[pos + 1] == 'W') {	// .W found in instruction
			bwfound = true;				// set word flag
			test_record->BW = 0;		// set BW falue to 0 (word)
		}
		// remove .X from test string to compare to list of instructions
		testinst = testinst.substr(0, pos);
	} else {							// .B or .W not found in instruction
		test_record->BW = 0;			// word assummed
	}

	// iterate through list until instruction is found and set record fields
	// accdordingly. Re-write using binary search at a later date. 
	for (int j = 0; j < 31; j++) {
		if (testinst == instr_list->mnem[j]) {
			if (bwfound==true)
				if (instr_list->bw[j] != true)
					break;
			test_record->found = true;
			test_record->insttype = instr_list->type[j];
			test_record->mnem = instr_list->mnem[j];
			test_record->opcode = instr_list->opcode[j];
			break;
		}
	}
}
