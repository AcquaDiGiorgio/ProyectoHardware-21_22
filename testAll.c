#include <LPC210x.H>  
#include "temporizador.h"
#include "cola.h"
#include "gestor_alarmas.h"

int main (void) {
	temporizador_iniciar();
	crear_alarma_unica(5,500);
	scheduler();
}
