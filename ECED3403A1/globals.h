/**********************************************************
 *	Stephnen Sampson
 *	Summer 2017
 *	THIS FILE CONTAINS GLOBAL VARIABLES. MACROS, AND
 *	CONSTANTS USED THROUGHOUT THE PROGRAM. THESE WILL
 *	BE ACCESSIBLE ANYWHERE GLOBALS.H IS INCLUDED
 *********************************************************/

#pragma once
#include <list>

extern int global_loc;			// global location counter
extern int unknown_records;		// count of unknown records
extern bool second_pass;		// flag for second pass
extern int s1count;				// count of s1 records used to create s5 record
extern bool instr_found;		// flag: used to specify starting address for execution
extern bool start_addr_set;		// flag: has a starting address has been defined
extern int starting_addr;		// starting address for execution (for s9 record)
extern std::ofstream fout;		// output filestream for srecord
extern std::ostream &out;		// output filestream for diagnostics
extern std::ofstream fsym;		// output filestream for symbol table
extern std::ostream &symout;	// output filestream for diagnostics
extern std::ofstream fdiag;		// output filestream for diagnostics
extern std::ostream &diagout;	// output filestream for diagnostics
extern std::ofstream flist;		// output filestream for srecord
extern std::ostream &listout;	// output filestream for diagnostics

#define HEX2( x ) uppercase << setw(2) << setfill('0') << hex << (int)(x)
#define HEX4( x ) uppercase << setw(4) << setfill('0') << hex << (int)(x)
#define MAX_SZ 32				// maximum number of bytes in srecord
#define LEN_CONST 3				// length of length field + address field
#define LINE_LEN 256			// maximum allowable line length
#define ERR_VAL 65536			// used as an error flag
#define NUL	'\0'				// NULL charact

