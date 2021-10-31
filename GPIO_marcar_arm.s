;Funcion que devuelve una mascara de bits
	AREA GPIO_marcar_arm, CODE, READONLY
	EXPORT GPIO_marcar_arm

	MOV		IP,SP
	STMDB   SP!,{R4-R10,FP,IP,LR}
	
; INICIALIZACION
	MOV R2,#0xFFFFFFFF ;Se crean dos mascaras para seleccionar los bits
	MOV R3,#0xFFFFFFFF
	MOV R2,R2,LSL R0 ;Eliminamos los bit_iniciales de la derecha, R2= FF0...0
	
	MOV R4, #32 ;Elliminamos los n bits de la izquiera, siendo n = (32 - (bit_inicial + num_bits))
	ADD R5,R1,R0
	SUB R5,R4,R5
	MOV R3,R3,LSR R5 ;R3 = 0..0FF
	
	AND R3,R2,R3 ;Creamos la mascara para seleccionar los bits que queremos, R3 = 0..FF..0
	
	MOV	R0, R3					;pone en r0 (return)
	LDMIA   SP,{R4-R10,FP,SP,PC}
	
	END	