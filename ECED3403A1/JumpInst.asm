;		
; Jump; conditional jumps: PC = PC + (2×offset with sign ext.)		
; Test jump instructions - some addressing modes are invalid		
; 5 May 2017		
; Caveat emptor		
;		
;	Org	$4,000								; HERE WHY ,000
	Org	$2000
Target2		; Reserve a location
;	ORG	$2,000								; HERE WHY ,000
	ORG $1000
; JNE/JNZ - Jump if not equal/zero		
Loop1	jne	Target1
	JNZ	Target2
	JnE	Loop1
Self1	jnz	Self1
	jne	R6
; JEQ/JZ - Jump if equal/zero		
Loop2	jeq	Target1
	jz	Target2
	JEQ	Loop2
Self2	JZ	Self2
;	jeq	 @R4								; HERE
;	JZ										; HERE
; JNC/JLO - Jump if no carry/lower		
Loop3	jnc	Target1
	JLO	Target2
	JNC	Loop3
Self3	jlo	Self3
	jlo	Target3
; JC/JHS - Jump if carry/higher or same		
Loop4	JC	Target1
	jhs	Target2
	jc	Loop4
Self4	JHS	Self4
	jc	Target3
; JN - Jump if negative		
Loop5	jne	Target1
	JNZ	Target2
	JnE	Loop5
Self5	jnz	Self5
;	jne	 @R6+								; HERE
; JGE - Jump if greater or equal		
Loop6	jge	Target1
	JGE	Target2
	jge	Loop6
Self6	JGE	Self6
;	jg	Self6								; HERE
; JL - Jump if less
Loop7	JL	Target1
	JL	Target2
	JL	Loop7
Self7	JL	Self7
;	jl	self7								; HERE - CASE SENSITIVE 
	jl Self7
; JMP - Jump (unconditionally)		
Loop8	jmp	Target1
	jmp	Target2
	jmp	Loop8
Self8	jmp	Self8
;	jmp	jmp									; can not jump to an instruction, jmp would be added as label that would be undefined
Target3;
; Forward reference		
	ORG	$0
Target1		; Reserve a location
	END
