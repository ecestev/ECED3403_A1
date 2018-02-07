/************************************************************************
*	Stephnen Sampson
*	Summer 2017
*
*	This file contains functions to initialize the directive
*	list, validate a directive and validate a directive's operand
*
************************************************************************/

#pragma once
#include "functions.h"

using namespace std;


struct directives {
	string mnem[9];
	int args[9];
};

void initialize_dirs(directives *dirs);
void validateDir(string testdir, record *test_record, directives *dir_list);
int valDirOp(record *testrecord);