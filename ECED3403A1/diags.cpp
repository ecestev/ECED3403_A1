#include "diags.h"

using namespace std;

// Writes formatted symbol table (to file or console)
void symtblwrite(ostream& symout, list<symtblrec> *symtbl) {
	int entry = 0;
	int temp = 0;
	symout << " ----------------------------------------------------------" << endl;
	symout << "                        SYMBOL TABLE                       " << endl;
	symout << " ----------------------------------------------------------" << endl << endl;
	symout << " ENTRY" << "     " << "LABEL" << "     " << "TYPE" << "      " << "LOCATION COUNTER" << endl << endl;
	for (list<symtblrec>::const_iterator iterator = symtbl->begin(), end = symtbl->end(); iterator != end; ++iterator) {
		entry++;
		symout << " " << dec << entry;
		if (entry > 999)
			symout << "      ";
		else if (entry > 99)
			symout << "       ";
		else if (entry > 9)
			symout << "        ";
		else
			symout << "         ";
		symout << iterator->label;
		int space = 10 - iterator->label.length();
		for (int i = 0; i < space; i++)
			symout << " ";
		symout << iterator->type;
		if (iterator->type == "KNOWN")
			symout << "      ";
		else if (iterator->type == "UNKNOWN")
			symout << "    ";
		else if (iterator->type == "REGISTER")
			symout << "   ";
		if (iterator->type != "UNKNOWN")
			symout << dec << iterator->value;
		else
			symout << " ";

		if (iterator->value > 999)
			symout << "  ";
		else if (iterator->value > 99)
			symout << "   ";
		else if (iterator->value > 9)
			symout << "    ";
		else
			symout << "     ";
		symout << "|  ";
		symout << hex << "0x" << iterator->value;
		symout << endl;
	}
	if (unknown_records > 0) {
		symout << endl << endl;
		symout << " ----------------------------------------------------------" << endl;
		symout << "   ERROR! ";
		symout << "First Pass Completed With " << unknown_records << " Unknown Labels!" << endl;
		symout << " ----------------------------------------------------------" << endl;
	}
}

// Print diagnostic error information based on records error type
void P1_Error(int errType, int line) {
	if (errType == -1)
		diagout << " Line: " << line << " - Ignored Due To: EMPTY LINE" << endl;
	if (errType == -2)
		diagout << " Line: " << line << " - Omitted Due To Error: Line too Long." << endl;
	if (errType == -3)
		diagout << " Line: " << line << " - Ignored Due To: Comment" << endl;
	if (errType == -4)
		diagout << " Line: " << line << " - Omitted Due To Error: Invalid Label." << endl;
	if (errType == -5)
		diagout << " Line: " << line << " - Omitted Due To Error: Duplicate Label." << endl;
	if (errType == -6)
		diagout << " Line: " << line << " - Omitted Due To Error: Unrecognized Instruction|Directive." << endl;
	if (errType == -7)
		diagout << " Line: " << line << " - Omitted Due To Error: Invalid Operand(s) for Instruction|Directive." << endl;
	if (errType == -8)
		diagout << " Line: " << line << " - Omitted Due To Error: Improper Record Format." << endl;
	if (errType == -9)
		diagout << " Line: " << line << " - End Directive Reached -- Pass Ending." << endl;
	if (errType == -10)
		diagout << " Line: " << line << " - Omitted Due To Error: Invalid Addressing Mode for Instruction." << endl;
	if (errType == -11)
		diagout << " Line: " << line << " - Omitted Due To Error: Required label not found in symbol table." << endl;
	if (errType == -1000) {
		cout << " ----------------------------------------------------------	" << endl;
		cout << "                     A SIMPLE ASSEMBLER						" << endl;
		cout << "         By: Stephen Sampson - Dalhousie University			" << endl;
		cout << "                        Summer 2017							" << endl;
		cout << " ----------------------------------------------------------	" << endl << endl;
		cout << "This assembler accepts a .asm file as a command line argument" << endl;
		cout << "and creates machine loadable SRECORDS from it. A Diagnostic" << endl;
		cout << "file and symbol table file are also output" << endl << endl;
		cout << "To use this program, simply drag and drop a .asm file onto" << endl;
		cout << "the assembler executable file" << endl;
	}
}

void P2_Error(int errType, int line) {
	if (errType == -2) {
		diagout << " Line: " << line << " - Illegal Jump Offset" << endl;
	}
}