	AREA candidatos_propagar_arm, CODE, READONLY
	EXPORT candidatos_propagar_arm

	MOV		R12,SP
	STMDB   SP!,{R4-R12,R14}

; INICIALIZACION
	MOV		R6,R0					;R0=entrada    R6=cuadricula
	MOV		R7,R1					;R1=entrada    R7=fila
	MOV		R8,R2					;R2=entrada    R8=columna
	MOV		R4,#0x0					;R4=i
	MOV		R5,#0x0					;R5=j

; leer valor
	ADD		R12,R6,R7,LSL #5    
	ADD		R0,R12,R8,LSL #1		;R0=dir(celda[fila][columna])
	LDRB	R0,[R0]					;mete en r4 el primer byte de dir(celda[fila][columna])
	AND		R3,R0,#0xF				;R9 = valor(celda[fila][columna]) en el que estamos
	
	MOV		R10, #0x1  
;celda_eliminar candidato
bucle_columnas
	ADD		R9,R12,R5,LSL #1
	LDRH	R0,[R9]            		;mete en r4 el primer byte de la celda    
;eliminar candidato
	ADD		R1, R3, #6				;R3 = valor celda  
	MOV		R1, R10, LSL R1
	ORR		R0, R0, R1                
	STRH	R0, [R9]


; bucle check
	ADD    R5, R5, #0x1
	CMP    R5, #0x9
	BLT    bucle_columnas

	ADD		R12,R6,R8,LSL #1
	
	MOV		R10, #0x1  
bucle_filas
	ADD		R9,R12,R4,LSL #5
	LDRH	R0,[R9]					;mete en r4 el primer byte de la celda    
;eliminar candidato
	ADD		R1, R3, #6				;R3 = valor celda   
	MOV		R1, R10, LSL R1
	ORR		R0, R0, R1                
	STRH    R0, [R9]

; bucle check
	ADD		R4, R4, #0x1
	CMP		R4, #0x9
	BLT    	bucle_filas

;inicializacion i y j
	MOV 	R1,#0					;init_i
	MOV		R2,#0					;end_i
	MOV 	R10,#0					;init_j
	MOV 	R11,#0					;end_j

;init_i = init_region[fila];
	CMP 	R7,#3					;comparamos fila>=3
	MOVHS 	R1,#3					;init_i = init_region[fila];
	CMP 	R7,#6					;comparamos fila>=6
	MOVHS 	R1,#6    
	ADD 	R2,R1,#3				;end_i = init_i + 3;

;init_j = init_region[columna];
	CMP 	R8,#3					;comparamos fila<6
	MOVHS 	R10,#3					;init_j = init_region[columna];     
	CMP 	R8,#6					;comparamos fila<6
	MOVHS 	R10,#6    
	ADD 	R11,R10,#3				;end_j = init_j + 3;

;ini de la cuadricula a cuadricula[init_i][init_j]
	MOV		R4,R1					;i=init_i
	MOV		R5,R10					;j=init_j
	MOV		R8,#0x1
	
bucle_cuadrado
	ADD		R9,R6,R4,LSL #5
	ADD		R9,R9,R5,LSL #1
	LDRH	R0,[R9]					;mete en r0 el contenido de la celda 
	;eliminar candidato
	ADD		R7,R3,#6	    		;(7 + valor - 1) == (7 - 1 + valor) == (6 + valor)
	MOV		R7,R8,LSL R7
	ORR		R0,R0,R7				;Pone en R0 el contenido anterior | 0x1 desplazado
	STRH	R0,[R9]					;Guarda el calculo anterior en memoria de R9

; bucle check
	ADD		R4,R4,#0x1
	CMP		R4,R2
	BLT		bucle_cuadrado
	MOV		R4,R1
	ADD		R5,R5,#0x1
	CMP		R5,R11
	BLT		bucle_cuadrado
	
	MOV		R0,#0x0
	LDMDB   SP,{R4-R12,R14}
	MOV		SP,R12
	BX		R14
	END