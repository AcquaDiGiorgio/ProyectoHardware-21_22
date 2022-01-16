;
; Función que obtiene del registro CPRS el modo en el que se
; encuentra el sistema en este momento (User, Irq o Fiq)

AREA    status_mode, CODE, READONLY
EXPORT 	status_mode

STMFD   SP!, {LR}          ; Store LR

MRS		R0, CPSR
AND		R0, R0, #0x1F

LDMFD   SP!, {PC}         ; Restore LR

END