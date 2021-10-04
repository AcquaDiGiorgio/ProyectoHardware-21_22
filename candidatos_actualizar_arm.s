	AREA candidatos_actualizar_arm, CODE, READONLY
	EXPORT candidatos_actualizar_arm
	IMPORT candidatos_propagar_arm
	
	MOV		R12,SP
	STMDB   SP!,{R4-R12,R14}
	
; INICIALIZACION
	MOV		R6,R0					;R0=entrada    	R6=cuadricula
	MOV		R7,#0x0					;R7=celdas_vacias
	MOV		R4,#0x0					;R4=i
	MOV		R5,#0x0					;R5=j
; EJECUCION
bucle1
	ADD		R1,R6,R4,LSL #5			;mete en r1 valor de cuadricula + (i desplazado 5 izq)
	ADD		R1,R1,R5,LSL #1			;mete en r1 valor de r1 + (j desplazado 1)
	LDRB	R0,[R1]					;mete en r0 el valor de r0 + [j desplazado 1 izq]
	AND		R0,R0,#0x7F				;and r0 0x007F
	STRH	R0,[R1]					;guarda en [r1] ½ palabra de r0 (valor de la celda)

	ADD		R5,R5,#0x1				;guarda en r0 j++
	CMP		R5,#0x9	        	    ;compara j con 16
	BLT		bucle1					;salta a la primera bucle1 si es menor
	MOV		R5,#0					;pone j=0

	ADD		R4,R4,#0x1				;i++
	CMP		R4,#0x9					;comara i con 144 (9*16)
	BLT		bucle1					;salta a bucle1 si es menor
	MOV		R4,#0					;pone i=0

bucle2
	ADD		R1,R6,R4,LSL #5			;mete en r0 valor de cuadricula + (i desplazado 5 izq)
	ADD		R1,R1,R5,LSL #1			;mete en r1 valor de r1 + (j desplazado 1)
	LDRH	R0,[R1]					;mete en r0 el valor de r0 + [j desplazado 1 izq]

	AND		R0,R0,#0xF				;sacamos el valor número que contiene la celda
	CMP		R0,#0x0					;celda contiene un 0
	ADDEQ	R7,R7,#1				;celdas_vacias++ si no es pista
	
	MOVNE	R0,R6
	MOVNE	R1,R4
	MOVNE	R2,R5
	
	BLNE	candidatos_propagar_arm	;llama a candidatos_propagar_arm si es pista

	ADD		R5,R5,#0x1				;j++
	CMP		R5,#0x9					;compara j con 9
	BLT		bucle2					;salta a la primera bucle2 si es menor
	MOV		R5,#0					;pone j=0

	ADD		R4,R4,#0x1				;i++
	CMP		R4,#0x9					;comara i con 9
	BLT		bucle2					;salta a bucle2 si es menor

	MOV		R0, R7					;pone en r0 (return) celdas_vacias
	LDMDB   SP,{R4-R12,R14}
	MOV		SP,R12
	BX		R14
	
	END
			
			