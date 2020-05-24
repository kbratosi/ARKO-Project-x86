section .text

global f
f:
	push rbp
	mov rbp, rsp

; arguments location
; rdi 		, rsi		, rdx		, xmm0		, xmm1		, xmm2		, xmm3		, xmm4
; pBuffer	, W			, H			, A			, B			, C			, D			, S			
;	 		, [rbp - 8] , [rbp - 16], [rbp - 24], [rbp - 32], [rbp - 40], [rbp - 48], [rbp - 56]

; alloc 8 stack variables
	sub rsp, 64
	mov [rbp - 8], rsi		; W (width)
	mov [rbp - 16], rdx		; H (height)
	movsd [rbp - 24], xmm0	; A
	movsd [rbp - 32], xmm1	; B
	movsd [rbp - 40], xmm2	; C
	movsd [rbp - 48], xmm3	; D
	movsd [rbp - 56], xmm4	; S
; 		  [rbp - 64] - tmp

; registers used
; xmm0 - x
; r8 - tmp

; load initial value to x
    mov r8, -2
	cvtsi2sd xmm0, r8
	movsd [rbp - 64], xmm0

; calculate y(x)
    fld QWORD [rbp - 64] 	; load x
	fld QWORD [rbp - 24] 	; load A
	fmul					; Ax
	fld QWORD [rbp - 32]	; load B
	fadd					; Ax + B
	fld QWORD [rbp - 64]	; load x
	fmul					; Ax^2 + Bx
	fld QWORD [rbp - 40]	; load C
	fadd					; Ax^2 + Bx + C
	fld QWORD [rbp - 64]	; load x
	fmul					; Ax^3 + Bx^2 + Cx
	fld QWORD [rbp - 48]	; load D
	fadd					; y = Ax^3 + Bx^2 + Cx + D

; check if y is in <-4; 4) range
	mov QWORD [rbp - 64], -4	;
	fild QWORD [rbp - 64]		; load -4
	fcomip ST0, ST1				; if( y < -4 )
	ja loopSetup

	mov QWORD [rbp - 64], 4		;
	fild QWORD [rbp - 64]		; load 4
	fcomip ST0, ST1				; if( y >= 4 )
	jbe loopSetup

; else: y is in <-4; 4) - calculate pixel_y
	mov QWORD [rbp - 64], 4	;
	fild QWORD [rbp - 64]	; load 4
	fadd					; y + 4
	mov QWORD [rbp - 64], 8	;
	fild QWORD [rbp - 64]	; load 8
	fdiv					; (y + 4)/8
	fild QWORD [rbp - 16]	; load H
	fmul					; pixel_y = (y + 4)/8 * H
	fistp QWORD [rbp - 64]	;
	
; colour pixel( 0, pixel_y )
	mov rax, [rbp - 64]
	mul QWORD [rbp - 8]		; pixel_y * W
	mov DWORD [rdi + rax * 4], 0x000000FF

	jmp loop

loopSetup:
; remove y(x) from the stack
	fstp QWORD [rbp - 64]
	
loop:
; move current x to [rbp - 64]
	movsd [rbp - 64], xmm0

; calculate dx(x) = S / sqrt( (3Ax^2 + 2Bx + C)^2 + 1 )
	fld QWORD [rbp - 56]	; load S
	fld QWORD [rbp - 64]	; load x
	fld QWORD [rbp - 24]	; load A
	fmul					; Ax
	mov QWORD [rbp - 64], 3	
	fild QWORD [rbp - 64]	; load 3
	fmul					; 3Ax
	fld QWORD [rbp - 32]	; load B
	mov QWORD [rbp - 64], 2	
	fild QWORD [rbp - 64]	; load 2
	fmul					; 2B
	fadd					; 3Ax + 2B
	fld QWORD [rbp - 64]	; load x
	fmul					; 3Ax^2 + 2Bx
	fld QWORD [rbp - 40]	; load C
	fadd					; 3Ax^2 + 2Bx + C
	fld ST0					; 
	fmul					; (3Ax^2 + 2Bx + C)^2
	mov QWORD [rbp - 64], 1
	fild QWORD [rbp - 64]	; load 1
	fadd					; (3Ax^2 + 2Bx + C)^2 + 1
	fsqrt					; sqrt( (3Ax^2 + 2Bx + C)^2 + 1 )
	fdiv					; dx = S / sqrt( (3Ax^2 + 2Bx + C)^2 + 1 )

; x = x + dx
	fld QWORD [rbp - 64]	; load x
	fadd					; x + dx

; if( x >= 2 ) -> end loop
	mov QWORD [rbp - 64], 2		;
	fild QWORD [rbp - 64]		; load 2
	fcomip ST0, ST1				; if( x >= 2 )
	jbe end

; else: store new x in xmm0
	fst QWORD [rbp - 64] 	; store x in [rbp - 64]
	movsd xmm0, [rbp - 64]	

; calculate y(x)
	fld QWORD [rbp - 24] 	; load A
	fmul					; Ax
	fld QWORD [rbp - 32]	; load B
	fadd					; Ax + B
	fld QWORD [rbp - 64]	; load x
	fmul					; Ax^2 + Bx
	fld QWORD [rbp - 40]	; load C
	fadd					; Ax^2 + Bx + C
	fld QWORD [rbp - 64]	; load x
	fmul					; Ax^3 + Bx^2 + Cx
	fld QWORD [rbp - 48]	; load D
	fadd					; y = Ax^3 + Bx^2 + Cx + D

; check if y is in <-4; 4) range
	mov QWORD [rbp - 64], -4	;
	fild QWORD [rbp - 64]		; load -4
	fcomip ST0, ST1				; if( y < -4 )
	ja loopSetup

	mov QWORD [rbp - 64], 4		;
	fild QWORD [rbp - 64]		; load 4
	fcomip ST0, ST1				; if( y >= 4 )
	jbe loopSetup

; else: y is in <-4; 4) - calculate pixel_x
	movsd [rbp - 64], xmm0
	fld QWORD [rbp - 64]
	mov QWORD [rbp - 64], 2		;
	fild QWORD [rbp - 64]		; load 2
	fadd						; x + 2
	mov QWORD [rbp - 64], 4		;
	fild QWORD [rbp - 64]		; load 4
	fdiv						; (x + 2) / 4
	fld QWORD [rbp - 8]			; load W
	fmul						; pixel_x = (x + 2)/4 * W
	
; store pixel_x in r8	
	fistp QWORD [rbp - 64]
	mov r8, [rbp - 64]

; calculate pixel_y
	mov QWORD [rbp - 64], 4	;
	fild QWORD [rbp - 64]	; load 4
	fadd					; y + 4
	mov QWORD [rbp - 64], 8	;
	fild QWORD [rbp - 64]	; load 8
	fdiv					; (y + 4)/8
	fild QWORD [rbp - 16]	; load H
	fmul					; pixel_y = (y + 4)/8 * H
	fistp QWORD [rbp - 64]	;

; colour pixel( pixel_x, pixel_y )
	mov rax, [rbp - 64]
	mul QWORD [rbp - 8]		; pixel_y * W
	add rax, r8				; pixel_y * W + pixel_x
	mov DWORD [rdi + rax * 4], 0x000000FF

	jmp loop

end:
	fstp QWORD [rbp - 64]	; remove x from the stack
	mov rsp, rbp
	pop rbp
	ret