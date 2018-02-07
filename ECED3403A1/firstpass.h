/**********************************************************
*	Stephnen Sampson
*	Summer 2017
*	
*	This file contains the main program flow for the first 
*	pass of my two-pass cross-assembler for the 
*	Texas Instrument's MSP-430 microcomputer
*
*	Lines read from an input file containing potential 
*	records are passed to this function to be analyzed.
*
*	Labels are identified and stored in the symbol table
*	if determined to be valid and not a dublicate.
*
*	Instructions/Directives and their operands are 
*	identified and validated
*
*	The program counter is updated as records are analyzed 
*	so that it can be stored in the symbol table with a 
*	label when one is found.
*
**********************************************************/

#pragma once
#include "instructions.h"
#include "directives.h"

using namespace std;

int firstPass(string to_parse, int linenum, int locCnt, 
	instructions *instr_list, directives *dir_list, list<symtblrec> *symtbl);