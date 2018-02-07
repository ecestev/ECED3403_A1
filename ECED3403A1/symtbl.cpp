#include "symtbl.h"

// intialize symbol table with registers
void initSymTbl(list<symtblrec> *symtbl) {
	for (int i = 0; i < 16; i++) {							
		string label = "R";									 
		symtblrec *newrecord = new symtblrec;				// create new symtblrec
		newrecord->label = label.insert(1, to_string(i));	// set record's label
		newrecord->type = "REGISTER";						// set record's type
		newrecord->value = i;								// set record's location counter
		symtbl->push_back(*newrecord);						// push record to symbol table
		delete newrecord;									// 
	}
}

// Ensures sumbol does not alreadt exist in symbol table. If it does and its type is 'known' then error.
// If it does and it's type is 'unknown', update type and value to current location counter value.
int processLabel(string toprocess, int pc, bool fw, list<symtblrec> *symtbl) {
	for (list<symtblrec>::iterator iterator = symtbl->begin(), end = symtbl->end(); iterator != end; ++iterator) {
		if (iterator->label == toprocess) {
			if (iterator->type == "UNKNOWN") {
				if (iterator->value == NUL) {
					if (pc > -1 && pc < 65536) {
						if (fw == false) {
							iterator->value = pc;
							iterator->type = "KNOWN";
							unknown_records--;
						}
					}
				}
				return 2;
			}
			return -1;
		}
	}
	return 1;
}

// Get address of label from symboltable
int symtbl_Lookup(string toprocess, list<symtblrec> *symtbl) {
	for (list<symtblrec>::iterator iterator = symtbl->begin(), end = symtbl->end(); iterator != end; ++iterator) {
		if (iterator->label == toprocess) {
			return iterator->value;
		}
	}
	return -1;
}

// update symbol table entry
void symentry_Update(string to_update, int address, list<symtblrec> *symtbl) {
	for (list<symtblrec>::iterator iterator = --symtbl->end(), end = symtbl->begin(); iterator != end; --iterator) {
		if (iterator->label == to_update) {
			iterator->value = address;
			break;
		}
	}
}