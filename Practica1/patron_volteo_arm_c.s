
	PRESERVE8
	AREA codigo, CODE, READONLY
	EXPORT patron_volteo_arm_c
	IMPORT ficha_valida

patron_volteo_arm_c
;r0 = *tablero, r1 = *longitud, r2 = FA, r3 = CA, 
;r4 = SF, r5 = SC, r6 = color
		PUSH {lr}			;para evitar guardar fp en la pila utilizamos directamente operaciones con sp
		sub sp, sp, #4		;reservo espacio para posicion valida  
		PUSH {r4-r9}		;guardo los registros que voy a utilizar
		add r4,sp,#32		;se fija el registro en el primer parametro de la pila
        ldm r4,{r4-r6}		;se hace una lectura m�ltiple de los 3 parametros
		;recoloco las variables
		;r1 = FA, r2 = CA, r7 = *longitud 
		mov r7,r1
		mov r1,r2
		mov r2,r3

		add r3,sp,#24		;r3 = @posicion valida
		add r1,r1,r4		;FA = FA + SF
		add r2,r2,r5		;CA = CA + SC

		;casilla = r8 = ficha_valida(tablero, FA, CA, &posicion_valida)
		PUSH {r0-r3}
		bl ficha_valida 
		mov r8,r0 
		POP {r0-r3}	
		
		mov r9, #0 			; r9 =longitud = 0, la longitud siempre es cero al inicio por lo que no leemos de memoria 
while 	cmp r8, #0 			; si la posicion no es valida es porque la casilla es vacia por lo que no hace falta buscar posicion valida de memoria
		beq end_while
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
		bne _else
		cmp r8,r6	;casilla == color
		bne _else
		cmp r9, #0
		movne r0, #1

		str r9, [r7] ; Se guarda la longitud
_else	POP {r4-r9}
		add sp, sp, #4
		POP{pc}
    END 