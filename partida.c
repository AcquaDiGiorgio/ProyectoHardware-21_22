#include "partida.h"
#include "gestor_output.h"
#include "temporizador.h"
#include "gestor_alarmas.h"
#include "constantes_comunes.h"

static volatile estado_juego_t estadoActual = principio;
static volatile uint32_t partida_minutos;
static volatile uint32_t partida_segundos;
static volatile char* razon_fin;
static volatile int razon_len;

void partida_cambiar_estado(estado_juego_t estado)
{
	estadoActual = estado;
}

void partida_preprar(void)
{
		partida_cambiar_estado(principio);
		partida_mostrar();
}

void partida_empezar(void)
{
		// alarma_crear_alarma_unica(0, EV_FIN_PARTIDA, SEGUNDO * 5);
		partida_cambiar_estado(jugando);
		partida_minutos 	= RTC_leer_minutos();
		partida_segundos 	= RTC_leer_segundos();
		partida_mostrar();
}

void partida_terminar(volatile char *razon, int length)
{
		partida_cambiar_estado(final);
		razon_fin = razon; 
		razon_len = length;
		partida_minutos		= RTC_leer_minutos()  - partida_minutos;
		partida_segundos 	= RTC_leer_segundos() - partida_segundos;
		partida_mostrar();
}

estado_juego_t partida_obtener_estado(void)
{
		return estadoActual;
}

void partida_mostrar(void)
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
			mostrar_final(partida_minutos, partida_segundos, razon_fin, razon_len);
			break;
	}
}
