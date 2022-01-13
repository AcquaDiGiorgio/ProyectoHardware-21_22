#include "access.h"

void __swi(0xFF) enable_isr (void);
void __swi(0xFE) disable_isr (void);

void __swi(0xFD) enable_isr_fiq (void);
void __swi(0xFC) disable_isr_fiq (void);

void __swi(0xFB) enable_fiq (void);
void __swi(0xFA) disable_fiq (void);

static volatile state isr_state = ENABLE;
static volatile state fiq_state = ENABLE;

void lock(void)
{
		disable_isr_fiq();
}

extern int status_mode(void);

void unlock()
{
		int acceso;
		acceso = status_mode();
		
		switch(acceso)
		{
			case USER:			
				enable_isr_fiq();
				break;
				
			case ISR:
					enable_fiq();
			
			case FIQ:
				break;
		}
}
