#pragma once
#include "srecords.h"

// FN FOR IF WRITING BYTE TO RECORD
void s1rec_write_byte(s1record *s1rec, uint8_t data) {		
	if(s1rec->addr_set == false) {							// if s1record's address isn't set
		initialize_s1(s1rec);								// initialize s1record
		s1rec->addr_set = true;								// set address set flag to true
	}
	if (global_loc != (s1rec->address + s1rec->index))		// if global loc is not what record would expect
			emit_s1(s1rec);									// location has changed, emit s1rec	
	if (s1rec->index >= MAX_SZ) 							// if s1record's data field is full
		emit_s1(s1rec);										// enit s1record
	s1rec->data[s1rec->index] = data;						// set s1rec[index] data byte
	s1rec->index++;											// increment record's index
	s1rec->length++;										// increment record's length
	s1rec->checksum += data;								// add data to record's checksum
	global_loc++;											// increment global location counter
}

// OVERLOADED FN FOR IF WRITING WORD TO RECORD
void s1rec_write_word(s1record *s1rec, uint16_t data) {		
	uint8_t hibyte = (data & 0xff00) >> 8;					// break up 16 bit data (word) into high
	uint8_t lobyte = (data & 0xff);							// and low bytes to be written separately
	s1rec_write_byte(s1rec, lobyte);						// write low byte first (little endien)
	s1rec_write_byte(s1rec, hibyte);						// write high byte second (little endien)
}

void emit_s1(s1record *s1rec) {
	s1count++;												// increment s1 count used for s5 records
	diagout << "EMIT S1REC" << endl;						// diagnostic message
	out << s1rec->type;										// output type
	out << HEX2(s1rec->length);								// output formatted length
	out << HEX4(s1rec->address);							// output formatted address
	for (int i = 0; i < s1rec->index; i++)					// loop through data 
		out << HEX2(s1rec->data[i]);						// output formatted data byte
	s1rec->chkcmp = ~(s1rec->checksum + s1rec->length);		// add length to checksum and takes one's comp
	out << HEX2(s1rec->chkcmp);								// output formatted ~checksum
	out << s1rec->termination;								// output carriage return and line feed
	initialize_s1(s1rec);									// initialize new s1 record
	s1rec->addr_set = true;									// ensure addr_set flag stays true
}

void emit_s5(s5record *s5rec) {
	diagout << "EMIT S5REC" << endl;						// diagnostic message
	s5rec->count = s1count;									// count = number of s1 records
	out << s5rec->type;										// output type
	out << HEX2(s5rec->length);								// output formatted length
	out << HEX4(s5rec->count);								// output formatted count
	s5rec->checksum += s5rec->length;						// add length to checksum
	s5rec->checksum += s5rec->count;						// add count to checksum
	s5rec->chkcmp = ~(s5rec->checksum);						// take ones comp of checksum
	out << HEX2(s5rec->chkcmp);								// output checksum
	out << s5rec->termination;								// output termination character
}

void emit_s9(s9record *s9rec) {
	diagout << "EMIT S9REC" << endl;						// diagnostic message
	out << s9rec->type;										// output type
	out << HEX2(s9rec->length);								// output formatted length
	out << HEX4(s9rec->address);							// otuput formatted starting execution address
	s9rec->chkcmp = ~(s9rec->checksum + s9rec->length);		// ones complement of checksum+length
	out << HEX2(s9rec->chkcmp);								// output checksum
	out << s9rec->termination;								// output termination 
}



// initialize s1 record object
void initialize_s1(s1record *s1rec) {
	s1rec->addr_set = false;
	s1rec->index = 0;
	s1rec->type = "S1";
	s1rec->length = LEN_CONST;
	s1rec->address = global_loc;
	memset(s1rec->data, 0, sizeof(s1rec->data));
	s1rec->checksum = 0;
	s1rec->chkcmp = 0;
	s1rec->termination = '\n';
	s1rec->checksum += (global_loc & 0xff);
	s1rec->checksum += (global_loc & 0xff00) >> 8;
}

// initialize s5 record object
void initialize_s5(s5record *s5rec) {
	s5rec->type = "S5";
	s5rec->length = LEN_CONST;
	s5rec->count = 0;
	s5rec->checksum = 0;
	s5rec->chkcmp = 0;
	s5rec->termination = '\n';
}

// initialize s9 record object
void initialize_s9(s9record *s9rec) {
	s9rec->type = "S9";
	s9rec->length = LEN_CONST;
	s9rec->address = starting_addr;
	s9rec->checksum = 0;
	s9rec->chkcmp = 0;
	s9rec->termination = '\n';
	s9rec->checksum += (starting_addr & 0xff);
	s9rec->checksum += (starting_addr & 0xff00) >> 8;
}