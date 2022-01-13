;
;
;

	AREA    status_mode, CODE, READONLY
	EXPORT 	status_mode

	STMFD   SP!, {LR}          ; Store LR
	
	MRS		R0, CPSR
	AND		R0, R0, #0x1F

	LDMFD   SP!, {PC}         ; Restore LR
	
	END