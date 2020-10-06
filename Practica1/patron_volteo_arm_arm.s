	  

	PRESERVE8
	AREA cte, DATA
DIM		EQU 	8
	AREA codigo, CODE, READONLY
	EXPORT patron_volteo_arm_arm
		
patron_volteo_arm_arm
;r0 = *tablero, r1 = *longitud, r2 = FA, r3 = CA, 
;r4 = SF, r5 = SC, r6 = color
		PUSH {fp}
        mov fp,sp
		PUSH {r4-r9}		;guardo los registros que voy a utilizar
		add r4,fp,#4	;se fija el registro en el primer parametro de la pila
        ldm r4,{r4-r6}	;se hace una lectura m?ltiple de los 3 parametros
		
;r8 = casilla, no utilizamos posicion valida = (casilla > 0)
		
		add r2,r2,r4		;FA = FA + SF
		add r3,r3,r5		;CA = CA + SC
		
		;r8 = ficha_valiada()
		mov r8, #0
		cmp r2, #DIM
		bhs noValida
		cmp r2, #0
		blt noValida
		cmp r3, #DIM
		bhs noValida
		cmp r3, #0
		addge r8, r2, r3, LSL #3
		ldrge r8, [r0,r8]

noValida
		mov r9, #0 ; r9 =longitud = 0
		
		
while 	cmp r8, #0   ;pocicion_valida = 1
		beq end_while
		cmp r8,r6	;casilla != color
		beq end_while
		add r2,r2,r4		;FA = FA + SF
		add r3,r3,r5		;CA = CA + SC
		add r9, r9, #1		; logitud ++

		;r8 = ficha_valiada()
		mov r8, #0
		cmp r2, #DIM
		bhs end_while
		cmp r2, #0
		blt end_while
		cmp r3, #DIM
		bhs end_while
		cmp r3, #0
		blt end_while
		add r8, r3, r2 ,LSL #3
		ldr r8, [r0,r8]

		b while

end_while 
		mov r0, #0  ;r0 = NO_ENCONTRADO
		cmp r8,r6	;casilla == color
		bne _else
		cmp r9, #0
		movne r0, #1

		str r9, [r1] ; Se guarda la longitud
_else	POP {r4-r9}
		POP {fp}
		mov pc,lr
    END 
		