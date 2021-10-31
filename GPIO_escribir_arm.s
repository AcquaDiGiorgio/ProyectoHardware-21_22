;Funcion auxiliar para escribir bits
	AREA GPIO_escribir_arm, CODE, READONLY
	EXPORT GPIO_escribir_arm

	MOV		IP,SP
	STMDB   SP!,{R4-R10,FP,IP,LR}
	
; INICIALIZACION
	MOV R4,R2,LSL R0
	
	MOV R5,#0xFFFFFFFF ;Elliminamos los n bits de la izquiera, siendo n = (32 - (bit_inicial + num_bits))
	MOV R6,#32
	ADD R7,R1,R0
	SUB R6,R6,R7
	MOV R5,R5,LSR R6 ;R3 = 0..0FF
	
	AND R3,R4,R5 ;Creamos la mascara para seleccionar los bits que queremos, R3 = 0..FF..0
	
	MOV	R0, R3					;pone en r0 (return)
	LDMIA   SP,{R4-R10,FP,SP,PC}
	
	END	