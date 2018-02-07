/**********************************************************************************************
*
*	Stephnen Sampson
*	B00568374
*	May 2017
*	ECED 3403 - Computer Architecture
*	Assignment #1
*		
*	This file contains:

**********************************************************************************************/

#include "functions.h"
#include <iomanip>

// Validates label based on criteria outlined in assignment
bool validate_label(string testlabel) {
	if (isalpha(testlabel[0]) == 0)
		return false;
	for (unsigned i = 0; i < testlabel.length(); i++) {
		if (isalnum(testlabel[i]) == 0)
			return false;
	}
	return true;
}

// check if string is valid. series of characters enclosed in double quotation marks
bool validate_string(record *testrecord, istringstream *iss, istream_iterator<string> *iit, istream_iterator<string> *eos) {
	int pos = -1;
	string token = **iit;
	if (token[0] == '\"') {							// if token starts with double quote
		while (*iit != *eos) {						// while there are tokens to grab
			++*iit;									// grab the next token
			if (*iit != *eos) {						// if not end of stream
				token = token + " " + **iit;		// catonate it with the current string
			}
		}
		pos = token.length() - 1;
		if (token[pos] == '\"') {					// if token ends with double quotes
			token = token.substr(1, (pos - 1));		// strip away opening and closing double quotes
			testrecord->srcop = token;				// store token in record's srcop field
			return true;							// operand was valid string
		}
		return false;
	} 
	return false;
}

// check if register is valid (R0 -> R15)
bool validate_register(string SRCDST, string testreg, record *testrecord) {
	if (toupper(testreg[0]) == 'R') {
		if (testreg[1] - '0' > -1) {
			if (testreg[2] == NUL) {
				testreg = testreg.substr(1);
				if(SRCDST=="SRC"){
					testrecord->source = stoi(testreg);
				} else {
					testrecord->dest = stoi(testreg);
				}
				return true;
			}
			if (testreg[2] - '0' < 6) {
				if (testreg[3] == NUL) {
					testreg = testreg.substr(1);
					if (SRCDST == "SRC") {
						testrecord->source = stoi(testreg);
					}
					else {
						testrecord->dest = stoi(testreg);
					}
					return true;
				}
			}
			else
				return false;
		}
		else
			return false;
	}
	return false;
}

// validate hexadecimal value
bool is_hex(string testhex) {
	// checking for $ denoting hex
	if (testhex[0] == '$') {
		testhex = testhex.substr(1);
		for (unsigned i = 0; i < testhex.length(); i++) {
			if (isxdigit(testhex[i]) == 0) {
				return false;
			}
		}
		return true;
	}
	// checking for 0x denoting hex
	if (testhex[0] == '0' && testhex[1] == 'x') {
		testhex = testhex.substr(2);
		for (unsigned i = 0; i < testhex.length(); i++) {
			if (isxdigit(testhex[i]) == 0) {
				return false;
			}
		}
		return true;
	}
	return false;
}

// get hexadecimal value... suggest calling only after verifying hex value
int get_hex(string gethex) {
	int pos = gethex.find('0x');
	if (pos < gethex.length()) {
		gethex = gethex.substr(pos+1);
	} else {
		pos = pos = gethex.find('$');
		if (pos < gethex.length())
			gethex = gethex.substr(pos+1);
	}
	stringstream str;
	str << hex << gethex;
	int temp;
	str >> hex >> temp;
	return temp;
}

// validate decimal value
bool is_dec(string testdec) {
	// checking for decimal
	if (testdec[0] == '-')
		testdec = testdec.substr(1);
	if (isdigit(testdec[0]) != 0) {
		for (unsigned i = 0; i < testdec.length(); i++) {
			if (isdigit(testdec[i]) == 0) {
				return false;
			}
		}
		return true;
	}
	return false;
}

// get decimal value... suggest calling only after verifying dec value
int get_dec(string getdec) {
	stringstream str;
	int temp;
	str << getdec;
	str >> temp;
	return temp;
}

int SWAPB(uint16_t to_SWAP) {
	uint8_t hibyte = (to_SWAP & 0xff00) >> 8;				// 
	uint8_t lobyte = (to_SWAP & 0xff);						// 
	to_SWAP = lobyte << 8 | hibyte;							// 
	return to_SWAP;											// 
}

int HIGH_B(uint16_t twobytes) {
	uint8_t hibyte = (twobytes & 0xff00) >> 8;				// 
	return hibyte;											//
}

int LOW_B(uint16_t twobytes) {
	uint8_t lobyte = (twobytes & 0xff);						// 
	return lobyte;											// 
}

void init_list() {
	listout << setw(6) << setfill(' ') << "ADDRESS";
	listout << setw(6) << setfill(' ') << "MNEM";
	listout << setw(11) << setfill(' ') << "OP1";
	listout << setw(11) << setfill(' ') << "OP2";
	listout << setw(16) << setfill(' ') << "MACHINE INST";
	listout << endl << endl;
}