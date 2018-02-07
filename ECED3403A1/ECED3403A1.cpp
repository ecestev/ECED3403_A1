/****************************************************************************************
*	Stephnen Sampson
*	B00568374
*	May 2017
*	ECED 3403 - Computer Architecture
*	Assignment #1
*
*	This file contains the main program flow for the
*	two-pass cross-assembler for the Texas Instrument's
*	MSP-430 microcomputer		
****************************************************************************************/

#pragma once
#include <fstream>
#include "globals.h"										// variables modified across various files
#include "firstpass.h"										// validate records and build symbol table
#include "secondpass.h"										// generate machine loadable instructions and data
#include "experimental/filesystem"							// path(), filename(), and replace_extension()
#include "symtbl.h"											// record structures and symtbl functions
#include "diags.h"											// error handlers
namespace fs = std::experimental::filesystem;				// for when performing filesystem operations
using namespace std;										// namespace generally used throughout program

/****************************************************************************************
*							  INITIALIZE GLOBAL VARS
****************************************************************************************/
int global_loc = 0;											// initialization for location counter
int unknown_records = 0;									// initialization for unknown record count
bool second_pass = false;									// initialization for second pass flag
bool start_addr_set = false;								// initialization for address flag
bool instr_found = false;									// initialization for instruction found flag
int starting_addr = 0;										// initialization for starting address
int s1count = 0;											// initialization for s1 count
ofstream fout;												// initialization for ofstream fout
ostream &out = fout;										// initialization for ostream &out
ofstream fsym;												// initialization for ofstream fout
ostream &symout = fsym;										// initialization for ostream &out
ofstream fdiag;												// initialization for ofstream fout
ostream &diagout = fdiag;									// initialization for ostream &out
ofstream flist;												// output filestream for list file
ostream &listout = flist;									// output filestream for list file

int main(int argc, char *argv[]){
	/************************************************************************************
	*                             DECLARATIONS & INITIALIZATIONS
	************************************************************************************/
	instructions *inst_list = new instructions;				// create instruction list
	directives *dir_list = new directives;					// create directive list
	list<symtblrec> *symbol_table = new list<symtblrec>;	// initialize pointer to new list for symbol table
	string oneline;											// stores line of input for processing 
	string file_name;										// variable to store filename
	ifstream inFile;										// input file stream
	int result = 0;											// stores error type or program counter if valid record
	int line_num = 0;										// stores line number being worked on
	int err_cnt = 0;										// stores error count
	int locationCounter = 0;								// stores value of program counter
	initialize_insts(inst_list);							// initialize instruction list
	initialize_dirs(dir_list);								// initialize directive list
	initSymTbl(symbol_table);								// initialize symbol table

	if (argc != 2) {										// if improper command line arguments are provided
		P1_Error(-1000, 0);									// output user instructions
		getchar();											// hold console for user input
		exit(0);											// exit
	} 
	

	diagout << " Attempting to open " << argv[1] << endl;	// diagnostic message
	if (fopen(argv[1], "r") != nullptr) {					// if file specified as arg can be opened
		diagout << " Successfully opened " << file_name;	// success message
		diagout << endl;									// formatting 
		file_name = argv[1];								// set filename
		inFile.open(file_name);								// set input file stream
		fout.close();										// close globally declared ofstream		
		fout.open(fs::path(argv[1]).filename().				// and open new ofstream with name of
			replace_extension("_SRECORDS.s19"));			// input file w/modified extension
		fsym.close();										// close globally declared ofstream
		fsym.open(fs::path(argv[1]).filename().				// and open new ofstream iwth name of
			replace_extension("_SYMTBL.txt"));				// input file w/modified extension
		fdiag.close();										// close globally declared ofstream
		fdiag.open(fs::path(argv[1]).filename().			// and open new ofstream with name of
			replace_extension("_DIAGS.txt"));				// input file w/modified extension
		flist.close();										// close globally declared ofstream
		flist.open(fs::path(argv[1]).filename().			// and open new ofstream with name of
			replace_extension("_LIST.lis"));				// input file w/modified extension
	} else {												// otherwise file could not be opened
		cerr << " Unable to open input file." << endl;		// error message
		return -1;											// quit with code '-1'
	}														

	/************************************************************************************
	*                                   START FIRST PASS
	************************************************************************************/
	diagout << " ----------------------------------------------------------" << endl;
	diagout << "                     STARTING FIRST PASS                   " << endl;
	diagout << " ----------------------------------------------------------" << endl << endl;
	diagout << " Parsing " << fs::path(argv[1]).filename() << " and Identifying Records" << endl;
	while (inFile.peek() != EOF) {							// next read != EOF indicator & END not reached
		line_num++;											// increment line number counter
		getline(inFile, oneline);							// read first line from input file
		result = firstPass(oneline, line_num, locationCounter, inst_list, dir_list, symbol_table);
		if (result >= 0) {									// if result is not error code (neg value)
			locationCounter = result;						// update location counter value
		} else {											// result is an error code
			if (result == -9)								// if END directive found
				break;										// stop parsing records
			if (result != -3 && result != -1) 				// if error count should be incremented
				err_cnt++;									// increment error count
			P1_Error(result, line_num);						// output error message
		}													// end 'result is an error code'
	}														// EOF|END reached

	/************************************************************************************
	*                                   END FIRST PASS
	************************************************************************************/
	diagout << " Successfully parsed " << fs::path(argv[1]).filename() << endl;
	diagout << " Number of Errors: " << err_cnt << endl;
	diagout << " Number of Unknown Labels: " << unknown_records << endl << endl;
	symtblwrite(symout, symbol_table);						// write symbol table to diagout or fout
	diagout << endl;										// spaces to improve viewing
	if(err_cnt > 0) {										// errors found during first pass
		inFile.close();										// close file opened for reading
		fout.close();										// close ofstream
		fsym.close();										// close ofstream
		fdiag.close();										// close ofstream
		return 0;											// exit
	}														// end 'errors found during first pass'
	if(unknown_records!=0) {								// if there are unknown records
		inFile.close();										// close file opened for reading
		fout.close();										// close ofstream
		fsym.close();										//close ofstream
		fdiag.close();										// close ofstream
		return 0;											// exit
	}														// end 'if there are unknown records'
	
	/************************************************************************************
	*                                   START SECOND PASS
	************************************************************************************/
	diagout << " ----------------------------------------------------------" << endl;
	diagout << "                    STARTING SECOND PASS                   " << endl;
	diagout << " ----------------------------------------------------------" << endl << endl;
	second_pass = true;										// falg used in some functions
	inFile.clear();											// clear state bit failbit
	inFile.seekg(0);										// rewind stream position to start of file	
	line_num = 0;											// stores line number being worked on
	global_loc = 0;											// globally accessible location counter
	s1record *s1rec = new s1record;							// create new s1record object
	initialize_s1(s1rec);									// initialize s1 record
	init_list();											// initialize list file
	while (inFile.peek() != EOF) {							// next read != EOF indicator & END not reached
		line_num++;											// increment line number counter
		getline(inFile, oneline);							// read first line from input file
		result = secondPass(oneline, line_num, inst_list, dir_list, symbol_table, s1rec);
		if (result == -9) 									// if END directive found
			break;											// stop parsing records
		if(result < 0)
			P2_Error(result, line_num);						// handle any second pass err/warn
	}														// EOF|END has been reached

	/************************************************************************************
	*                                   END SECOND PASS
	************************************************************************************/
	if (s1rec->index>0)										// if current srecord contains data
		emit_s1(s1rec);										// emit srecord
	inFile.close();											// close file opened for reading
	s5record *s5rec = new s5record;							// create new s5record object
	s9record *s9rec = new s9record;							// create new s9record object
	initialize_s5(s5rec);									// initialize s5 record
	initialize_s9(s9rec);									// initialize s9 record
	emit_s5(s5rec);											// emit the s5 record
	emit_s9(s9rec);											// emit the s9 record
	diagout << endl;										// formatting
	diagout << " ----------------------------------------------------------" << endl;
	diagout << "                       END OF PROGRAM                      " << endl;
	diagout << " ----------------------------------------------------------" << endl << endl;
	fdiag.close();											// close ofstream 
	fout.close();											// close ofstream
	fsym.close();											// close ofstream
	cout << "DONE..." << endl;								// write to console
	cin.get();												// hold console for user input
    return 0;												// exit

	/************************************************************************************
	*                                 END OF PROGRAM
	************************************************************************************/
}
