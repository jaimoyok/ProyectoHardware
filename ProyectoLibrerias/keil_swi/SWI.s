;/*****************************************************************************/
;/* SWI.S: SWI Handler                                                        */
;/*****************************************************************************/
;/* This file is part of the uVision/ARM development tools.                   */
;/* Copyright (c) 2005-2006 Keil Software. All rights reserved.               */
;/* This software may only be used under the terms of a valid, current,       */
;/* end user licence from KEIL for a compatible version of KEIL software      */
;/* development tools. Nothing else gives you the right to use this software. */
;/*****************************************************************************/

T_Bit           EQU     0x20

                PRESERVE8                      ; 8-Byte aligned Stack
                AREA    SWI_Area, CODE, READONLY
                ARM

                EXPORT  SWI_Handler
SWI_Handler   

                STMFD   SP!, {R12, LR}         ; Store R12, LR
                MRS     R12, SPSR              ; Get SPSR
                STMFD   SP!, {R8, R12}         ; Store R8, SPSR
                TST     R12, #T_Bit            ; Check Thumb Bit
                LDRNEH  R12, [LR,#-2]          ; Thumb: Load Halfword
                BICNE   R12, R12, #0xFF00      ; Extract SWI Number
                LDREQ   R12, [LR,#-4]          ; ARM:   Load Word
                BICEQ   R12, R12, #0xFF000000  ; Extract SWI Number
				
				
				
                LDR     R8, SWI_Count0
                CMP     R12, R8
                BHS     tabla1               ; Overflow		
                ADR     R8, SWI_Table0
				B 		end1
tabla1			LDR 	R8, SWI_Count1
				RSB 	R12, R12, #0xFF
				CMP		R12,R8
				BHS 	SWI_Dead
				ADR     R8, SWI_Table1
end1 			LDR     R12, [R8,R12,LSL #2]   ; Load SWI Function Address
				MOV     LR, PC                 ; Return Address
                BX      R12                    ; Call SWI Function 

                LDMFD   SP!, {R8, R12}         ; Load R8, SPSR
                MSR     SPSR_cxsf, R12         ; Set SPSR
                LDMFD   SP!, {R12, PC}^        ; Restore R12 and Return

SWI_Dead        B       SWI_Dead               ; None Existing SWI

SWI_Cnt0        EQU    (SWI_End0-SWI_Table0)/4
SWI_Count0       DCD     SWI_Cnt0

SWI_Cnt1         EQU    (SWI_End1-SWI_Table1)/4
SWI_Count1       DCD     SWI_Cnt1
	
                IMPORT  __SWI_0
				IMPORT __SWI_FF
				IMPORT __SWI_FE
				IMPORT __SWI_FD
				IMPORT __SWI_FC
SWI_Table0
                DCD     __SWI_0                ; SWI 0 Function Entry
SWI_End0

SWI_Table1
				DCD     __SWI_FF
				DCD     __SWI_FE
				DCD     __SWI_FD
				DCD     __SWI_FC

SWI_End1

            


                END

