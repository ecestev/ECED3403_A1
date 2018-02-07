#pragma once
#include "directives.h"

using namespace std;

void initialize_dirs(directives *dirs) {
	
	dirs->mnem[0] = "ORG";
	dirs->args[0] = 1;

	dirs->mnem[1] = "STRING";
	dirs->args[1] = 1;
	
	dirs->mnem[2] = "WORD";
	dirs->args[2] = 1;
	
	dirs->mnem[3] = "BSS";
	dirs->args[3] = 1;
	
	dirs->mnem[4] = "BYTE";
	dirs->args[4] = 1;

	dirs->mnem[5] = "EQU";
	dirs->args[5] = 1;
	
	dirs->mnem[6] = "ALIGN";
	dirs->args[6] = 0;
	
	dirs->mnem[7] = "END";
	dirs->args[7] = 0;

	dirs->mnem[8] = "ASCII";
	dirs->args[8] = 1;

}

int valDirOp(record *testrecord) {

	// NOTE: 65536 will be used as the error code since LC can
	// have values 0 --> 65535 all directives will increment the
	// LC except ORG & END and ORG may increment or decrement
	// PC so can not use -1 as error code as I have been
	// this file has minimum commenting as all functioncalls and
	// progression is very self-explanatory 
	if (testrecord->mnem == "ALIGN") {
		if (testrecord->value % 2 != 0) {
			testrecord->srcOpData = 1;
			return 1;
		}
		testrecord->srcOpData = -1;
		return 0;
	}

	if (testrecord->mnem == "BSS") {
		int value = 0;
		while(true){
			bool ishex = is_hex(testrecord->srcop);
			if (ishex) {
				value = get_hex(testrecord->srcop);
				break;
			}
			bool isdec = is_dec(testrecord->srcop);
			if (isdec) {
				value = get_dec(testrecord->srcop);
				break;
			}
			return 65536;
		}
		if ((testrecord->value + value) < 65536)
			if ((testrecord->value + value) > 0)
				if (value > 0) {
					testrecord->srcOpData = value;
					return value;
				}
		return 65536;
	}

	
	if (testrecord->mnem == "STRING") {
		int value = testrecord->srcop.length();
		return value;
	}

	if (testrecord->mnem == "EQU") {
		// get value of testrecord->srcop (dec, hex, or label)
		if (validate_label(testrecord->srcop))
			return -1;				// tell calling function testrecord->srcop is a label
		if (is_hex(testrecord->srcop))
			return -2;				// tell calling function testrecord->srcop is a hex
		if (is_dec(testrecord->srcop))
			return -3;				// tell calling function testrecord->srcop is a decimal
		return 65536;
	}

	if (testrecord->mnem == "ORG") {
		int value = 0;
		while (true) {
			bool ishex = is_hex(testrecord->srcop);
			if (ishex) {
				value = get_hex(testrecord->srcop);
				value -= testrecord->value;
				break;
			}
			bool isdec = is_dec(testrecord->srcop);
			if (isdec) {
				value = get_dec(testrecord->srcop);
				value -= testrecord->value;
				break;
			}
			return 65536;
		}
		if (value < 65536)
			if (value + testrecord->value > 0) {
				testrecord->srcOpData = value;
				return value;
			}
		return 65536;
	}

	if (testrecord->mnem == "BYTE") {
		int value = 0;
		while (true) {
			bool ishex = is_hex(testrecord->srcop);
			if (ishex) {
				value = get_hex(testrecord->srcop);
				break;
			}
			bool isdec = is_dec(testrecord->srcop);
			if (isdec) {
				value = get_dec(testrecord->srcop);
				break;
			}
			return 65536;
		}
		if (value > -129 && value < 256) {
			testrecord->srcOpData = value; 
			return 1;
		}
		return 65536;
	}

	if (testrecord->mnem == "WORD") {
		int value = 0;
		while (true) {
			bool ishex = is_hex(testrecord->srcop);
			if (ishex) {
				value = get_hex(testrecord->srcop);
				break;
			}
			bool isdec = is_dec(testrecord->srcop);
			if (isdec) {
				value = get_dec(testrecord->srcop);
				break;
			}
			return 65536;
		}
		if (value > -32769 && value < 65536) {
			testrecord->srcOpData = value;
			return 2;
		}
		return 65536;
	}

	
	if (testrecord->mnem == "END") {

	}

	return 0;
}



// Validates Directive by Comparing to list of supported directives and sets record
// values approperiately
void validateDir(string testdir, record *test_record, directives *dir_list) {
	for (unsigned i = 0; i < testdir.length(); i++)
		testdir[i] = toupper(testdir[i]);

	for (int j = 0; j < 9; j++) {
		if (testdir == dir_list->mnem[j]) {
			test_record->found = true;
			test_record->dirtype = dir_list->args[j];
			if (testdir == "ASCII")
				test_record->mnem = "STRING";
			else
			test_record->mnem = dir_list->mnem[j];
			break;
		}
	}
}