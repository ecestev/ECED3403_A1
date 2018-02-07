/************************************************************************
*	Stephnen Sampson
*	Summer 2017
*
*	This file contains functions that are called from different 
*	various places. 
*	
************************************************************************/

#pragma once
#include <sstream>
#include <iterator>	
#include "symtbl.h"

using namespace std;

bool validate_label(string testlabel);
bool validate_string(record *testrecord, istringstream *iss, 
	istream_iterator<string> *iit, istream_iterator<string> *eos);
bool validate_register(string SRCDST, string testreg, record *testrecord);
bool is_hex(string testhex);
bool is_dec(string testdec);
int get_hex(string hex);
int get_dec(string dec);
int SWAPB(uint16_t to_SWAP);
int HIGH_B(uint16_t twobytes);
int LOW_B(uint16_t twobytes);
void init_list();