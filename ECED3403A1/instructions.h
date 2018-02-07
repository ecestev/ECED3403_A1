/****************************************************************************************
*	Stephnen Sampson
*	Summer 2017
*	
*	This file contains functions to initialize the instruction
*	list, validate an instruction, and validate an operand's 
*	addressing mode. *
*
*	Format		Mode			Incr	Mode	OP1		OP2		LBL		JUMP	As	Ad
*	@Rn			Indirect		0		1		Y		N		N		Y			N/A
*	@Rn+		Indirect+		0		2		Y		N		N		Y			N/A
*	Rn			Direct			0		3		Y		Y		N		Y
*	x(Rn)		Indexed			2		4		Y		Y		N		Y
*	ADDR(Rn)	Indexed			2		5		Y		Y		Y		Y
*	&ADDR		Absolute		2		6		Y		Y		Y		Y
*	ADDR		Relative		2		7		Y		Y		Y		Y
*	#ADDR		Immediate		2		8		Y		N		Y		N			N/A
*	#x			Immediate		2		9		Y		N		N		N			N/A
***************************************************************************************/

#pragma once
#include "functions.h"

using namespace std;

struct instructions {
	string mnem[31];	// array to store mnemonics for supported instructions
	int type[31];		// array to store types for supported instructions
	int opcode[31];		// array to store opcodes for supported instructions
	bool bw[31];		// array to store support for B/W specifier
};

void initialize_insts(instructions *inst);
void validateInst(string testinst, record *test_record, instructions *instr_list);
int valAddMode(string SRCDST, record *testrecord, list<symtblrec> *symtbl);