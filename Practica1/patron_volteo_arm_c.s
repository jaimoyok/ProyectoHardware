
	PRESERVE8
	AREA codigo, CODE, READONLY
	EXPORT patron_volteo_arm_c
	IMPORT ficha_valida

patron_volteo_arm_c
;r0 = *tablero, r1 = *longitud, r2 = FA, r3 = CA, 
;r4 = SF, r5 = SC, r6 = color
		PUSH {lr,fp}
        mov fp,sp
		sub sp, sp, #4
		PUSH {r4-r10}		;guardo los registros que voy a utilizar
		add r4,fp,#8	;se fija el registro en el primer parametro de la pila
        ldm r4,{r4-r6}	;se hace una lectura m�ltiple de los 3 parametros
		;recoloco las variables
		;r1 = FA, r2 = CA, r7 = *longitud 
		mov r7,r1
		mov r1,r2
		mov r2,r3

		sub r3,fp,#4		;r3 = *posicion valida
		add r1,r1,r4		;FA = FA + SF
		add r2,r2,r5		;CA = CA + SC

		;casilla = r8 = ficha_valida(tablero, FA, CA, &posicion_valida)
		PUSH {r0-r3}
		bl ficha_valida 
		mov r8,r0 
		POP {r0-r3}	
		
		mov r9, #0 ; r9 =longitud = 0
while 	ldr r10, [r3] ; r3= posicion_valida
		cmp r10, #1   ;pocicion_valida = 1
		bne end_while
		cmp r8,r6	;casilla != color
		beq end_while
		add r1,r1,r4		;FA = FA + SF
		add r2,r2,r5		;CA = CA + SC
		add r9, r9, #1		; logitud ++

		;casilla = r8 = ficha_valida(tablero, FA, CA, &posicion_valida)
		PUSH {r0-r3}
		bl ficha_valida 
		mov r8,r0 
		POP {r0-r3}

		b while

end_while 
		mov r0, #0  ;r0 = NO_ENCONTRADO
		ldr r10, [r3]
		cmp r10, #1   ;pocicion_valida = 1
		bne _else
		cmp r8,r6	;casilla == color
		bne _else
		cmp r9, #0
		movne r0, #1

		str r9, [r7] ; Se guarda la longitud
_else	POP {r4-r10}
		add sp, sp, #4
		POP{fp,pc}
    END 