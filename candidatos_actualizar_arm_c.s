	AREA candidatos_actualizar_arm_c, CODE, READONLY
	EXPORT candidatos_actualizar_arm_c
	;IMPORT candidatos_actualizar_c
	
	;LDR r1,=candidatos_actualizar_c
	;CMP r1,#0
	;BEQ candidatos_actualizar_c(r0)
	
	END