#include "access.h"

void __swi(0xFF) enable_isr (void);
void __swi(0xFE) disable_isr (void);

void __swi(0xFD) enable_isr_fiq (void);
void __swi(0xFC) disable_isr_fiq (void);

void __swi(0xFB) enable_fiq (void);
void __swi(0xFA) disable_fiq (void);

static volatile state isr_state = ENABLE;
static volatile state fiq_state = ENABLE;

void lock(acceso_t acceso)
{
		switch(acceso)
		{
			case USER:			
				if(isr_state == ENABLE && fiq_state == ENABLE)
				{
						isr_state = DISABLE;
						fiq_state = DISABLE;
						disable_isr_fiq();
				}
				else if(fiq_state == ENABLE)
				{
						fiq_state = DISABLE;
						disable_fiq();
				}
				else if(isr_state == ENABLE)
				{
						isr_state = DISABLE;
						disable_isr();
				}
				break;
				
			case ISR:					
				if(fiq_state == ENABLE)
				{
						fiq_state = DISABLE;
						disable_fiq();
				}
				break;
				
			case FIQ:
				if(isr_state == ENABLE)
				{
						isr_state = DISABLE;
						disable_isr();
				}
				break;
		}
}

void unlock(acceso_t acceso)
{
		switch(acceso)
		{
			case USER:			
				if(isr_state == DISABLE && fiq_state == DISABLE)
				{
						isr_state = ENABLE;
						fiq_state = ENABLE;
						enable_isr_fiq();
				}
				else if(fiq_state == DISABLE)
				{
						fiq_state = ENABLE;
						enable_fiq();
				}
				else if(isr_state == DISABLE)
				{
						isr_state = ENABLE;
						enable_isr();
				}
				
				break;
				
			case ISR:
				if(fiq_state == DISABLE)
				{
						fiq_state = ENABLE;
						enable_fiq();
				}
				break;
				
			case FIQ:
				break;
		}
}
