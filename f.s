section .text

global f
f:
	push rbp
	mov rbp, rsp

; arguments location
; rdi 		, rsi		, rdx		, rcx 		, r8		, r9
; pBuffer	, W			, H			, A			, B			, C			, D			, S			
;	 		, [rbp - 40], [rbp - 32], [rbp - 24], [rbp - 16], [rbp - 8] , [rbp + 16], [rbp + 24]

; registers used
; r8 - x
; r9 - dx, pix_x in drawing
; r10 - y, pix_y in drawing

; alloc 6 stack variables
	sub rsp, 48

	mov [rbp - 8], r9	; C
	mov [rbp - 16], r8	; B
	mov [rbp - 24], rcx	; A
	mov [rbp - 32], rdx	; H (height)
	mov [rbp - 40], rsi	; W (width)

; load initial value to x
	mov r8, -2
	mov [rbp - 48], r8

; calculate y(x)
	fld QWORD [rbp - 24] 	; load A
	fld QWORD [rbp - 48] 	; load x
	fmul					; Ax
	fld QWORD [rbp - 16]	; load B
	fadd					; Ax + B
	fld QWORD [rbp - 48]	; load x
	fmul					; Ax^2 + Bx
	fld QWORD [rbp - 8]		; load C
	fadd					; Ax^2 + Bx + C
	fld QWORD [rbp - 48]	; load x
	fmul					; Ax^3 + Bx^2 + Cx
	fld QWORD [rbp + 16]	; load D
	fadd					; y(x) = Ax^3 + Bx^2 + Cx + D
	fstp QWORD [rbp - 48]	;
	mov r10, [rbp - 48]		; store y(x)

; check if y(x) is in <-4; 4> range
	cmp r10, 4				; if y(x) > 4
	jg end
	cmp -4, r10				; else if y(x) < -4
	jg end

; else: y(x) is in <-4; 4> - calculate pixel height
	mov [rbp - 48], r10		;
	fld QWORD [rbp - 48]	; load y(x)
	mov QWORD [rbp - 48], 4	;
	fld QWORD [rbp - 48]	; load 4
	fadd					; y + 4
	mov QWORD [rbp - 48], 8	;
	fld QWORD [rbp - 48]	; load 8
	fdiv					; (y + 4)/8
	fld QWORD [rbp - 32]	; load H
	fmul					; pix_y = (y + 4)/8 * H
	fistp QWORD [rbp - 48]	; convert to int
	mov r10, [rbp - 48]		; store pix_y

; find pixel in buffer
	mov rax, r10			; load pix_y to rax
	mul QWORD [rbp - 32]	; pix_y * W

; colour pixel
	mov [rdi + rax + 4], 0xFF

end:
	mov rsp, rbp
	pop rbp
	ret