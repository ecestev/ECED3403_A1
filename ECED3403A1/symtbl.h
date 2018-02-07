/************************************************************************
*	Stephnen Sampson
*	Summer 2017
*
*	This file contains structure definitions for two different record
*	structures - one used during the passes and one written to the 
*	symbol table as well as functions used to intialize the symbol
*	table, add a symbol the the symbol table, look-up the location of 
*	a symbol in the symbol table, and update an entry in the symbol 
*	table (used for forward referended symbols).
*
************************************************************************/

#pragma once
#include <string>
#include "globals.h"

using namespace std;

struct record {
	int linenum = -1;			// line number within .asm file
	int value = 0;				// program counter at this point
	int incrValue = 0;			// amount line increments LC by
	int insttype = -1;			// instruction type (zero|one|two operand)
	int dirtype = -1;			// directive type
	bool found = false;			// instruction|directive found flag

	string label;				// record label
	string mnem;				// record instruction|directive
	int instrincr = 0;			// amount instr increments LC by

	string srcop;				// left hand operand (optional)	
	string srcOpLbl;			// label of left hand operand
	int srcOpData = -1;			// location of source label
	string srcOpMode;			// index of left hand operand
	string srcOpIndx;			// index of left hand operand
	int srcincr = 0;			// amount srcop increments LC by

	string dstop;				// right hand operand (optional)
	string dstOpLbl;			// label of right hand operand
	int dstOpData = -1;			// location of dest label
	string dstOpMode;			// index of right hand operand
	string dstOpIndx;			// index of right hand operand
	int dstincr = 0;			// amount dstop increments LC by

	int opcode = 0;				// opcpde indicating which instruction
	int source = 0;				// reg. used to determine source op address
	int Ad = 0;					// destination op addressing mode
	int BW = 0;					// byte (1) / word (0)
	int As = 0;					// source op addressing mode
	int dest = 0;				// reg. used to determine dest op address
};

struct symtblrec {
	string label;				// record label
	int value = 0;				// program counter at this point
	string type = "UNKNOWN";	// known, unknown, register
};

void initSymTbl(list<symtblrec> *symtbl);
int processLabel(string toprocess, int pc, bool fw, list<symtblrec> *symtbl);
int symtbl_Lookup(string toprocess, list<symtblrec> *symtbl);
void symentry_Update(string to_update, int address, list<symtblrec> *symtbl);