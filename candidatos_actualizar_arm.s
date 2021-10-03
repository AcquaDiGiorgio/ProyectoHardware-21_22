	AREA candidatos_actualizar_arm, CODE, READONLY
	EXPORT candidatos_actualizar_arm
	
;	MOV r6, r0 
;	LDR r1, #0 ;Celdas Vacias
;	LDR r2, #0 ;i
;	LDR r3, #0 ;j
;	LDR r4,    ;NUM_FILAS
;	LDR r5,    ;NUM_COLUMNAS
;	
;	for1: 	AND r6, r6, #0x007F 	; cuadricula & 0x007F
;			AND r1, r1, #1			; celdas_vacias++
;			CMP r3, r5				; ¿j == NUM_COLUMNAS?
;			ADDNE r3, r3, #1		; si j != NUM_COLUMNAS -> j++
;			ADDNE r6, r6, #0x0002	; si j != NUM_COLUMNAS -> cuadricula[i][j+1]
;			BNE for1				; si j != NUM_COLUMNAS -> realizar el bucle de nuevo
;			MOVEQ r3, r3, #0		; si j == NUM_COLUMNAS -> poner j a 0
;			;Aqui termina el for interno (columnas)
;			CMP r2, r4				; ¿i == NUM_FILAS?
;			ADDNE r2, r2, #1		; si i != NUM_FILAS -> i++
;			ADDNE r6, r6, #0x00		; si i != NUM_FILAS -> cuadricula[i+1][j]
;			BNE for1				; si i != NUM_FILAS -> realizar el bucle de nuevo
;	for2:	
			
			