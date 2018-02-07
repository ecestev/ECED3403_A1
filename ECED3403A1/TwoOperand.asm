;
; Two-operand arithmetic		
; Test of two-operand instruction -- not all work
; 5 May 17
; Caveat emptor
;
; MOV - Move source to destination		
	mov	r1,r11						
	mov	r5,Array(r7)				
	mov	Data,r10					
	mov	&Data,Data
	mov	 @r7,Array(R6)
	mov	 @R5,@r5
;	mov	 @R10+							; HERE		CHECK! MISSING DEST OPERAND
	mov	#1,Array()					
	mov.w	r0,r10
	mov.w	r5,Array(r7)
	mov.w	Data,r10
	mov.w	&Data,Data
;	m0v.w	 Array(R6),@R7				; HERE		CHECK! INVALID INST + UNSUPPORTED DEST ADD MODE
	mov.w	 @R5,@r5+
;	moo.w	 @R17+,Data					; HERE		CHECK! R17 DNE
	mov.w	#8,Array(r7)			
	mov.w	#$ffff,Data				
	mov.b	r0,r20					
	mov.x	r5+,Array(r7)
;	mov.b	Data+,r10					; HERE		UNSURE! LABEL+
	mov.b	&Data,Data2				
	mov.b	 Array(R6),@R7			
;	miv.b	 @R5,@r5+					; HERE		CHECK! UNSUPPORTED DEST ADD MODE
;	mov.b	 @R17+,Data					; HERE		CHECK! R17 DNE
	mov.b	#8,Array(r7)
; ADD - Add source to destination		
	ADD	r0,r10
	ADD	r5,Array(r7)
	ADD	Data,r10
	ADD	&Data,Data
	ADD	 @r7,Array(R6)
;	Adx	 @R5,@r5						; HERE		CHECK! UNSUPPORTED DEST ADD MODE
;	ADD	 @R10+							; HERE 		CHECK! MISSING DEST OPERAND
	ADD	#1,Array()
	ADD.w	r0,r10
	ADD.w	r5,Array(r7)
	ADD.w	Data,r10
	add.w	#-120,Data
	ADD.w	&Data,Data
	ADD.w	 Array(R6),@R7
	ADD.w	 @R5,@r5+
;	ADD.w	 @R17+,Data					; HERE		CHECK! R17 DNE
	ADD.w	#8,Array(r7)
	ADD.b	r0,r20
	ADD.b	r5+,Array(r7)
;	ADDi.b	Data+,r10					; HERE		CHECK! INVALID INST
	ADD.b	&Data,Data2
	ADD.b	 Array(R6),@R7
	ADD.b	 @R5,@r5+
	add.b	#$8000,R4
;	ADD.b	 @R17+,Data					; HERE		CHECK! R17 DNE
	ADD.b	#8,Array(r7)
;ADDC - Add source and carry to destination		
	Addc	r0,r10
	Addc	r5,Array(r7)
	Addc	Data,r10
	Addc	&Data,Data
	Addc	 @r7,Array(R6)
	Addc	 @R5,@r5
;	Addc.wx	 @R10+						; HERE		CHECK! INVALID INST
	Addc	#1,Array()
	Addc.w	r0,r10
	Addc.w	r5,Array(r7)
	Addc.w	Data,r10
	Addc.w	&Data,Data
	Addc.w	 Array(R6),@R7
	Addc.w	 @R5,@r5+
Data
;	Addc.w	 @R17+,Data					; HERE		CHECK! R17 DNE
	Addc.w	#8,Array(r7)
	Addc.b	r0,r20
	Addc.b	r5+,Array(r7)
;	Addc.b	Data+,r10					; HERE		UNSURE LABEL+
;	Addo.b	&Data,Data2					; HERE		CHECK! INVALID INST
	Addc.b	 Array(R6),@R7
	Addc.b	 @R5,@r5+
;	Addc.b	 @R17+,Data					; HERE		CHECK! R17
	Addc.b	#8,Array(r7)
	addc.w	#-2,Data2
;SUBC - Subtract source from destination (with carry)		
	SUBC	r0,r10							
	SUBC	r5,Array(r7)					
	SUBC	Data,r10						
	SUBC	&Data,Data						
	SUBC	 @r7,Array(R6)					
	SUBC	 @R5,@r5						
;	SUBC	 @R10+						; HERE		CHECK! MISSING DEST OPERAND
	SUBC	#1,Array()												
	SUBC.w	r0,r10							
Array
	SUBC.w	r5,Array(r7)					
	SUBC.w	Data,r10						
	SUBC.w	&Data,Data						
	SUBC.w	 Array(R6),@R7					
	SUBC.uu	 @R5,@r5+						
;	SUBC.w	 @R17+,Data					; HERE		CHECK! R17
	SUBC.w	#8,Array(r7)					
	SUBC.b	r0,r20							
	SUBC.b	r5+,Array(r7)					
;	SUBC.b	Data+,r10					; HERE		UNSURE LABEL+
	SUBC.b	&Data,Data2						
	SUBC.b	 Array(R6),@R7					
	SUBC.b	 @R5,@r5+i						
	SUBC.w	@R5,#$34												
;	SUBC.b	 @R17+,Data					; HERE		CHECK! R17
	SUBC.b	#8,Array(r7)					
;SUB - Subtract source from destination		
	sub	r0,r10
	sub	r5,Array(r7)
	sub	Data,r10
	sub	&Data,Data
	sub	 @r7,Array(R6)
	sub	 @R5,@r5
;	sub	 @R10+							; HERE
	sub	#1,Array()
;	subw	r0,r10						; HERE
Data2
	sub.w	r5,Array(r7)
	sub.w	Data,r10
;	suw	&Data,Data						; HERE
	sub.w	 Array(R6),@R7
	sub.w	 @R5,@r5+
;	sub.w	 @R17+,Data					; HERE
	sub.w	#8,Array(r7)
	sub.b	r0,r20
	sub.b	#$44,#-4
	sub.b	r5+,Array(r7)
;	subb	Data+,r10					; HERE
	sub.b	&Data,Data2
	sub.b	 Array(R6),@R7
	sub.b	 @R5,@r5+
;	sub.b	 @R17+,Data					; HERE
	sub.b	#8,Array(r7)
;CMP - Compare source with destination		
	CmP	r0,r10
	cmp	#$ffff1,R10
	CmP	r5,Array(r7)
	CmP	Data,r10
	CmP	&Data,Data
	CmP	 @r7,Array(R6)
	CmP	 @R5,@r5
;	CmP	 @R10+							; HERE
	CmP	#1,Array()
	cmp	#-32769,Data
	CmP.w	r0,r10
	CmP.w	r5,Array(r7)
	CmP.w	Data,r10
;	ComP.w	&Data,Data					; HERE
	CmP.w	 Array(R6),@R7
	CmP.w	 @R5,@r5+
;	CmP.w	 @R17+,Data					; HERE
	CmP.w	#8,Array(r7)
	CmP.b	r0,r20
	CmP.b	r5+,Array(r7)
;	CmP.b	Data+,r10					; HERE
	cmp.b	#-200,R5
	CmP.b	&Data,Data2
	CmP.b	 Array(R6),@R7
	CmP.	 @R5,@r5+
;	CmP.b	 @R17+,Data					; HERE
;BIT - Test bits of source AND destination		
	bIt	r0,r10
	bIt	r5,Array(r7)
	bIt	Data,r10
	bIt	&Data,Data
	bIt	 @r7,Array(R6)
	bIt	 @R5,@r5
;	bLt	 @R10+							; HERE	
	bIt	#1,Array()
	bIt.w	r0,r10
	bIt.w	r5,Array(r7)
	bIt.w	Data,r10
	bIt.w	&Data,Data
	bIt.w	 Array(R6),@R7
	bIt.w	 @R5,@r5+
;	blt.w	 @R17+,Data					; HERE
	bIt.w	#8,Array(r7)
	bIt.b	r0,r20
	bIt.b	r5+,Array(r7)
;	bIt.b	Data+,r10					; HERE
	bIt.b	&Data,Data2
	bIt.b	 Array(R6),@R7
;	boIt.b	 @R5,@r5+					; HERE
;	bit.b	#f0e0,Array(r5)				; ADDED TO SYMBOL TABLE>>>>> WHY???? HEX VALUE NOT PREFIXED WITH $ IS VALID LABEL TECHNICALLY...
	bit.b	#$f0e0,Array(r5)			; FIXED VERSION...
;	bIt.b	 @R17+,Data					; HERE
;DADD - Decimal add source to destination (with carry)		
	DADD	r0,r10
;	DAD	r5,Array(r7)					; HERE
	DADD	Data,r10
	DADD	&Data,Data
	DADD	 @r7,Array(R6)
	DADD	 @R5,@r5
;	DADD	 @R10+						; HERE	
	DADD	#1,Array()
;	DADo.w	r0,r10						; HERE
	DADD.w	r5,Array(r7)
	DADD.w	Data,r10
	DADD.w	&Data,Data
	DADD.w	 Array(R6),@R7
	DADD.w	 @R5,@r5+
;	DADD.w	 @R17+,Data					; HERE
;	DANc.w	#8,Array(r7)				; HERE
	DADD.b	r0,r20
	DADD.b	r5+,Array(r7)
;	DADD.b	Data+,r10					; HERE
	DADD.b	&Data,Data2
	DADD.b	 Array(R6),@R7
	DADD.b	 @R5,@r5+
;	DADD.b	 @R17+,Data					; HERE
;BIT - Test bits of source AND destination		
	bIt	r0,r10
	bIt	r5,Array(r7)
	bIt	Data,r10
	bIt	&Data,Data
	bIt	 @r7,Array(R6)
	bIt	 @R5,@r5
;	bIt	 @R10+							; HERE
	bIt	#1,Array()
	bit.w	r0,r10
	bIt.w	r5,Array(r7)
	bIt.w	Data,r10
	bIt.w	&Data,Data
	bIt.w	 Array(R6),@R7
	bIt.w	 @R5,@r5+
;	blt.w	 @R17+,Data					; HERE
	bIt.w	#8,Array(r7)
	bIt.b	r0,r20
	bIt.b	r5+,Array(r7)
;	bIt.b	Data+,r10					; HERE
	bIt.b	&Data,Data2
	bIt.b	 Array(R6),@R7
;	but.b	 @R5,@r5+					; HERE
;	bIt.b	 @R17+,Data					; HERE
;BIC - Bit clear (dest & ~src)		
	bic	r0,r10
	bic	r5,Array(r7)
	bic	Data,r10
	bic	&Data,Data
	bic	 @r7,Array(R6)
	bic	 @R5,@r5
;	bic	 @R10+							; HERE
	bic	#1,Array()
	bic.w	r0,r10
	bic.w	r5,Array(r7)
	bic.w	Data,r10
	bic.w	&Data,Data
	bic.w	 Array(R6),@R7
	bic.w	 @R5,@r5+
;	bic.w	 @R17+,Data					; HERE
	bic.w	#8,Array(r7)
	bic.b	r0,r20
	bic.b	r5+,Array(r7)
;	bicpen.b	Data+,r10				; HERE
	bic.b	&Data,Data2
	bic.b	 Array(R6),@R7
	bic.b	 @R5,@r5+
;	bic.b	 @R17+,Data					; HERE
	bic.b	#8,Array(r7)
	bic	#$ffff,Data
;BIS - Bit set (logical OR)		
	bis	r0,r10
	bis	r5,Array(r7)
	bis	Data,r10
	bis	&Data,Data
	bis	 @r7,Array(R6)
	bis	 @R5,@r5
;	bis	 @R10+							; HERE
;	bison	#1,Array()					; HERE
	bis.w	r0,r10
	bis.w	r5,Array(r7)
	bis.w	Data,r10
	bis.w	&Data,Data
	bis.w	 Array(R6),@R7
	bis.w	 @R5,@r5+
;	bis.w	 @R17+,Data					; HERE
	bis.w	#8,Array(r7)
	bis	#-1,Data
	bis.b	r0,r20
	bis.b	r5+,Array(r7)
;	bis.b	Data+,r10					; HERE
	bis.b	&Data,Data2
	bis.b	 Array(R6),@R7
;	bis.b	 @R17+,Data					; HERE
;	bliss.b	 @R5,@r5+					; HERE
	bis.b	#8,Array(r7)
;XOR - Exclusive or source with destination		
	XoR	r0,r10
	XoR	r5,Array(r7)
	XoR	Data,r10
	XoR	&Data,Data
	XoR	 @r7,Array(R6)
	XoR	 @R5,@r5
;	XoR	 @R10+
	xor	#-128,r10
	XoR	#1,Array()
	XoR.w	r0,r10
;	ZoRo.w	r5,Array(r7)
	XoR.w	Data,r10
	XoR.w	&Data,Data
	XoR.w	 Array(R6),@R7
	XoR.w	 @R5,@r5+
;	XoR.w	 @R17+,Data
	XoR.w	#8,Array(r7)
	XoR.b	r0,r20
	XoR.b	r5+,Array(r7)
;	XoR.b	Data+,r10
	XoR.b	&Data,Data2
	XoR.b	 Array(R6),@R7
;	X0R.b	 @R5,@r5+
;	XoR.b	 @R17+,Data
	XoR.b	#8,Array(r7)
;AND - Logical AND source with destination (dst & src)		
	and	r0,r10
	and	r5,Array(r7)
	and	Data,r10
	and	&Data,Data
	and	 @r7,Array(R6)
	and	 @R5,@r5	
;	and	 @R10+						
	and	#1,Array()
	and.w	r0,r10
	and.w	r5,Array(r7)
	and.w	Data,r10
	and.w	&Data,Data
	and.w	 Array(R6),@R7
	and.w	 @R5,@r5+
;	and.w	 @R17+,Data				
	and.w	#8,Array(r7)
	and.w	#$0,Data
	and.w	#-0,r6							; HERE
	and.b	r0,r20
	and.b	r5+,Array(r7)			
;	and.b	Data+,r10				
	and.b	&Data,Data2
	and.b	 Array(R6),@R7
	and.b	 @R5,@r5+
;	and.b	 @R17+,Data				
	and.b	#8,Array(r7)
;		
	end
;
Array	BSS	5
Data	byte	4

