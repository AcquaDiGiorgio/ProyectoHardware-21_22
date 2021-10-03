	AREA candidatos_propagar_arm, CODE, READONLY
	EXPORT candidatos_propagar_arm
	
; INICIALIZACION
	MOV		R6,R0					;R0=entrada    R6=cuadricula
	MOV		R7,R1					;R1=entrada    R7=fila
	MOV		R8,R2					;R2=entrada    R8=columna
	MOV		R4,#0x0					;R4=i
	MOV		R5,#0x0					;R5=j

; leer valor
	ADD		R9,R6,R7,LSL #5    
	ADD		R0,R9,R8,LSL #1			;R0=dir(celda[fila][columna])
	LDRB	R0,[R0]					;mete en r4 el primer byte de dir(celda[fila][columna])
	AND		R3,R0,#0xF				;R3 = valor(celda[fila][columna]) en el que estamos

;celda_eliminar candidato
bucle_columnas
	ADD		R9,R9,R5,LSL #1
	LDRH	R0,[R9]					;mete en r0 el contenido de la celda 
	;eliminar candidato
	ADD		R1,R3,#6
	MOV		R2,#0x1    
	MOV		R1,R2,LSL R1
	ORR		R0,R0,R1				;Pone en R0 el contenido anterior | 0x1 desplazado
	STRH	R0,[R9]					;Guarda el calculo anterior en memoria de R9

; bucle check
	ADD		R5,R5,#0x1
	CMP		R5,#0x9
	BLT		bucle_columnas
	
; colocamos R9 en cuadricula[0][columna]
	ADD		R9,R6,R8,LSL #1

;celda_eliminar candidato
bucle_filas
	ADD		R9,R9,R4,LSL #5
	LDRH	R0,[R9]					;mete en r0 el contenido de la celda 
	;eliminar candidato
	ADD		R1,R3,#6
	MOV		R2,#0x1    
	MOV		R1,R2,LSL R1
	ORR		R0,R0,R1				;Pone en R0 el contenido anterior | 0x1 desplazado
	STRH	R0,[R9]					;Guarda el calculo anterior en memoria de R9

; bucle check
	ADD		R4,R4,#0x1
	CMP		R4,#0x9
	BLT		bucle_filas
	
;determinar fronteras región;    	;init_region[NUM_FILAS] = {0, 0, 0, 3, 3, 3, 6, 6, 6};
	MOV 	R3,#0					;init_i
	MOV 	R4,#0					;init_j
	MOV 	R5,#0					;end_i
	MOV		R7,#0					;end_j

;init_i = init_region[fila];
	CMP 	R1,#3					;comparamos fila>=3
	MOVHS 	R3,#3					;init_i = init_region[fila];
	
	CMP 	R1,#6       		 	;comparamos fila>=6
	MOVHS 	R3,#6    

	;init_j = init_region[columna];
	CMP 	R2,#3					;comparamos fila<6
	MOVHS 	R4,#3					;init_j = init_region[columna];     

	CMP 	R2,#6					;comparamos fila<6
	MOVHS 	R4,#6        


