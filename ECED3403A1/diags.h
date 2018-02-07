/************************************************************************
*	Stephnen Sampson
*	Summer 2017
*
*	This file contains functions used to write to the diagnostic file.
*
************************************************************************/

#pragma once
#include "globals.h"
#include "symtbl.h"
#include "iostream"

void symtblwrite(ostream& symout, list<symtblrec> *symtbl);
void P1_Error(int errType, int line);
void P2_Error(int errType, int line);