#include <LPC210x.H>  
#include "temporizador.h"
#include "cola.h"
#include "gestor_alarmas.h"
#include "gpio_control.h"
#include "gestor_pulsacion.h"

int main (void) {
	temporizador_iniciar();
	GPIO_iniciar();
	eint_init();
	
	crear_alarma_periodica(GPIO_REFRESH,EV_GPIO,200);
	crear_alarma_unica(POW_DOWN,EV_POWER,15000);
	crear_alarma_periodica(PULSACION,CHECK_PULS,100);
	scheduler();
}
