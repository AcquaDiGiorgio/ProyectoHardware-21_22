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

void cambiar_estado_partida(estado_juego_t estado)
{
	estadoActual = estado;
}

void preprar_partida(void)
{
		cambiar_estado_partida(principio);
		dibujar();
}

void empezar_partida(void)
{
		crear_alarma_unica(0, EV_FIN_PARTIDA, SEGUNDO * 5);
		cambiar_estado_partida(jugando);
		partida_minutos = RTC_leer_segundos();
		partida_segundos 	= RTC_leer_minutos();
		dibujar();
}

void terminar_partida(char *razon, int length)
{
		cambiar_estado_partida(final);
		razon_fin = razon; 
		razon_len = length;
		dibujar();
}

estado_juego_t obtener_estado_juego(void)
{
		return estadoActual;
}

void dibujar(void)
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
