/**********************************************************
*	Stephnen Sampson
*	Summer 2017
*
*	This file contains the main program flow for the second
*	pass of my two-pass cross-assembler for the
*	Texas Instrument's MSP-430 microcomputer
*
*	Lines read from an input file containing already
*	validated records are passed to this function.
*
*	Labels are ignored as they were handled in the first pass
*
*	Machine loadable srecords for instructions/data are
*	created and output
*
**********************************************************/

#pragma once
#include <bitset>
#include "instructions.h"
#include "directives.h"
#include "srecords.h"

using namespace std;

int secondPass(string to_parse, int linenum, instructions *instr_list, 
	directives *dir_list, list<symtblrec> *symtbl, s1record *s1rec);
