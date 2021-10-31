;Funcion auxiliar para leer bits	
	AREA GPIO_leer_arm, CODE, READONLY
	EXPORT GPIO_leer_arm

	MOV		IP,SP
	STMDB   SP!,{R4-R10,FP,IP,LR}
	
; INICIALIZACION
	MOV R3,0xFFFFFFFF ;Se crean dos mascaras para seleccionar los bits
	MOV R4,0xFFFFFFFF
	MOV R4,R4,LSL R0 ;Eliminamos los bit_iniciales de la derecha, R4= FF0...0
	
	MOV R5,#32	;Elliminamos los n bits de la izquiera, siendo n = (32 - (bit_inicial + num_bits))
	ADD R6,R1,R0
	SUB R6,R5,R6
	MOV R3,R3,LSR R6 ;R3 = 0..0FF
	
	AND R3,R4,R3 ;Creamos la mascara para seleccionar los bits que queremos, R3 = 0..FF..0
	
	AND R4,R2,R3 ;Aplicamos la mascara al valor que nos pasan como parametros
	MOV R4,R4,LSR R0 ;Eliminamos los bits de la derecha, para asi convertirlo a decimal
		
	MOV	R0, R4					;pone en r0 (return)
	LDMIA   SP,{R4-R10,FP,SP,PC}
	
	END	