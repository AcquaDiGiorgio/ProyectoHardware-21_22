#include <LPC210x.H>  
#include "temporizador.h"
#include "cola.h"
#include "gestor_alarmas.h"

int main (void) {
	temporizador_iniciar();
	crear_alarma_unica(EXT_INT_1,100);
	scheduler();
}
