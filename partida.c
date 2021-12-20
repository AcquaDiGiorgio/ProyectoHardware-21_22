#include "gestor_output.h"

enum estado_juego{
	principio 	= 0,
	jugando 		= 1,
	final 			= 2
};

static volatile enum estado_juego estadoActual;

void pasar_a_estado(enum estado_juego estado)
{
	estadoActual = estado;
}

void dibujar()
{
	switch(estadoActual)
	{
		case principio:
			enviar_info();
			break;
		
		case jugando:
			inicializar_tablero();
			break;
		
		case final:
			break;
	}
}
