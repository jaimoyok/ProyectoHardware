	  

AREA codigo, CODE, READONLY
PRESERVE8
EXPORT patron_volteo_arm_arm	
 DIM    EQU   8

patron_volteo_arm_arm
;r0 = *tablero, r1 = *longitud, r2 = FA, r3 = CA, 
;r4 = SF, r5 = SC, r6 = color
;r7 = posicion_valida ,r8 = casilla
		PUSH {lr}
        PUSH{fp}
        mov fp,sp
		PUSH {r4-r10}		;guardo los registros que voy a utilizar
        ldr r4, [fp,#8]
        ldr r5, [fp, #12]
        ldr r6, [fp,#16]



		
		add r2,r2,r4		;FA = FA + SF
		add r3,r3,r5		;CA = CA + SC

                ;suponemos que no es valida la posicion
                mov r7, #0      ;posicion valida = false
                mov r8, #0      ;casilla = VACIA
                ;comprobamos si lo que suponemos es cierto
                cmp r2, DIM     
                bhs noValida
                cmp r2, #0 
                blo noValida
                cmp r3, #DIM
                bhs noValida
                cmp r3, #0
                bhs noValida
                add r9, r3, r2, LSL #3          ;r9 = @tablero[fa][ca]
                ldr r9, [r1,r9]                 ;r9 = tablero[fa][ca]
                cmp r9, #0             ; tablero[fa][ca]= VACIA
                beq noValida
                ;si es valida actualizamos valores
                mov r7, #1
                mov r8 ,r9



		
noValida	ldr r10, [r1] ; r10 =longitud


while 	cmp r7, #1   ;pocicion_valida = 1
		bne end_while
		cmp r8,r6	;casilla != color
		beq end_while
		add r1,r1,r4		;FA = FA + SF
		add r2,r2,r5		;CA = CA + SC
		add r10, r10, #1		; logitud ++

		;suponemos que no es valida la posicion
                mov r7, #0      ;posicion valida = false
                mov r8, #0      ;casilla = VACIA
                ;comprobamos si lo que suponemos es cierto
                cmp r2, DIM     
                bhs noValidaW
                cmp r2, #0 
                blo noValidaW
                cmp r3, #DIM
                bhs noValidaW
                cmp r3, #0
                bhs noValidaW
                add r9, r3, r2, LSL #3          ;r9 = @tablero[fa][ca]
                ldr r9, [r1,r9]                 ;r9 = tablero[fa][ca]
                cmp r9, #0                      ; tablero[fa][ca]= VACIA
                beq noValidaW
                ;si es valida actualizamos valores
                mov r7, #1
                mov r8 ,r9

noValidaW		b while

end_while 
		mov r0, #0  ;r0 = NO_ENCONTRADO
		cmp r8, #1   ;pocicion_valida = 1
		bne els
		cmp r8,r6	;casilla == color
		bne els
		cmp r9, #0
		movls r0, #1

els		str r10, [r7] ; Se guarda la longitud
		POP {r4-r10}
		POP {fp,pc}
	
//POPs

    END 

/*;r0 = tablero, r1 = longitud, r2 = FA, r3 = CA, 
;r4 = SF, r5 = SC, r6 = color
; r7 = posicion_valida, r8 = DIM
AREA codigo, CODE, READONLY
	EXTERN patron_volteo_arm_c

patron_volteo_arm_c

//PUSHs

		            ldr r10, [r1] ; Se lee la longitud
dowhile 	
                    add r2,r2,r4
                    add r3,r3,r5
                    ;r8 = ficha_valida(tablero, FA, CA, &posicion_valida)
                    cmp r2, r8 
                    bhs ficha_valida_else
                    cmp r2, #0
                    blo ficha_valida_else
                    cmp r3, r8
                    bhs ficha_valida_else
                    cmp r3 , #0
                    blo ficha_valida_else
                    ldr r9, r1, ;r9 = tablero[FA][CA]
                    cmp r9, #0
                    movne r7, #1
                    movne r8, r9
ficha_valida_else   moveq r7, #0
                    moveq r8, #0
        
                    cmp r7, #1
                    bne end_while
                    cmp r8,r6	
                    addne r1, r1, #1
                    bne dowhile

end_while           mov r0, #0
                    cmp r7, #1
                    bne else
                    cmp r8,r6
                    bne else
                    cmp r10, #0
                    movls r0, #1    ;!((posicion_valida == 1) && (casilla == color) && (*longitud >0))
            else    str r10, [r1] ; Se guarda la longitud
                    ; devolver r0
	
//POPs

    END 
*/