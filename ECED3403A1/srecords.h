/************************************************************************
*	Stephnen Sampson
*	Summer 2017
*
*	This file contains the structures, write and emit functions for the
*	srecords. This program supports S1, S5, and S9 records. For S9 
*	records a starting address can be specified as an operand to END as
*	either a symbol or a hex/dec value. If no starting address is
*	specified, the S9 address will indicate the address of the first 
*	instruction to be executed. 
*
************************************************************************/

#pragma once
#include "globals.h"
#include <iostream>
#include <string>
#include <iomanip>


using namespace std;

struct s1record {
	bool addr_set;
	int index;					// index within data
	string type;				// two bytes for type
	uint16_t length;			// number of byte pairs left
	uint16_t address;			// tyo bytes for address
	uint16_t data[32];			// data
	uint16_t checksum;			// checksum before ones comp
	uint8_t chkcmp;				// checksum after ones comp
	string termination = "\n";	// termination characters
};

struct s5record {
	string type = "S5";			// two bytes for type
	uint16_t length = LEN_CONST;// number of byte pairs left
	uint16_t count = 0;			// tyo bytes for count
	uint16_t checksum = 0;		// checksum before ones comp
	uint8_t chkcmp = 0;			// checksum after ones comp
	string termination = "\n";	// termination characters
};

struct s9record {
	string type = "S9";			// two bytes for type
	uint16_t length = LEN_CONST;// number of byte pairs left
	uint16_t address = 0;		// tyo bytes for address
	uint16_t checksum = 0;		// checksum before ones comp
	uint8_t chkcmp = 0;			// checksum after ones comp
	string termination = "\n";	// termination characters
};

void initialize_s1(s1record *s1rec);
void initialize_s5(s5record *s5rec);
void initialize_s9(s9record *s9rec);
void s1rec_write_byte(s1record *s1rec, uint8_t data);
void s1rec_write_word(s1record *s1rec, uint16_t data);
void emit_s1(s1record *s1rec);
void emit_s5(s5record *s5rec);
void emit_s9(s9record *s9rec);