; This test file has no errors. It is designed to test the assembler output. 
;
; AUTHOR: Gary Hubley
; 13/06/16
;

	
LABEL2	EQU	17		
LABEL	EQU 	$35			
ADDR	EQU	$2000			
ORG	$500
INDRCT	
MOV	@R2,R4		
SWPB 	@R4+					
mov	@R3+,R13		
	
ADDC	@R11,R5
ORG $1000		
IMM	add	#IMM1,R12		
ADD	#425,ADDR(R3)	
add	#42,R14	
ORG $1500
ABS	
add	&LABEL,R2		
mov	&ADDR,R3
sub 	&ADDR2,R4		
sub	&ADDR2,&ADDR	
ORG $2000
ALPHA	MOV	LABEL,LABEL4
ADD	$12(R3),LABEL4(R2)	
SXT	R12	
MOV	IND(R4),R2		
mov	0(R11),A22(R3)	
mov	R56,ADDR(R12)	
LABEL4	EQU	$1002
ADDR2	EQU	$2006
A22	EQU	14
IMM1	EQU	57
IND	EQU	69
ORG	$1700 			
START
RRC 	R4		
PUSH 	@R5		
BYTE 3			
ALIGN			
ASCII 	"FOO BAR"					; HERE	
ALIGN			
BLOCK BSS	6	
Lab6 EQU 666
Lab7 EQu $02EB
ASCII "asdfasdfasdf"				; HERE
ADD	ADDR(R1),0(R2)		
ADD	$FF(R1),LABEL2	
label EQU 123 
MOV #123,label
ADD #123,R6		
SUBC &label,&label		
SUB R9,R10 				
CMP #$AbCd,&label					; HERE
DADD #$AB,label(R6) 	
BIT label(R7),label		
add	@R5,&LABEL			
add	@R7+,R3			
ADD	@R10+,&ABS	
BIS @R7+,label(R7)		
XOR @R4,R10				
AND &label,R13			
R56 	EQU	22
RETI
ALIGN
BYTE	$65
align
add	#LABEL,R14
Lab4 EQU $2EB
sub 	#R3,LABEL	
add	#$FF,R13
BYTE	85
BYTE	53
ALIGN
ORG	$1000
MOV	#1,R4    	
SWPB	R4      
Labelly41
Loop	add	@R7,Data(R5)	
ADD	Data(R5),R4	
ADDC	#1,R5	
org	$2000
Data8 bss	4 
Label96

lab08
add @R6+,&Loop
last
org $2200
jmp Label96
add	#8,R6 
add	#-1,R7 
label40
Data
END