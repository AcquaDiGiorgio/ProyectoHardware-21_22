#include "gestor_IO.h"
#include "gpio_control.h"
#include "eventos.h"
#include "gestor_alarmas.h"
#include "sudoku_p2.h"
#include "temporizador.h"
#include "gestor_energia.h"
#include "constantes_comunes.h"

volatile uint32_t estadoAnterior = 0x0;
volatile boolean cambiandoLed = FALSE;
volatile boolean latido_on = FALSE;

//Inicializa el gestor IO
void initIO(void)
{
	// Iniciamos la GPIO
	GPIO_iniciar();
	
	// Creamos la alarma que refresca la GPIO
	// crear_alarma_periodica(GPIO_REFRESH,EV_GPIO_REF,200);
	
	// Marcamos Salidas
	GPIO_marcar_salida(0,14);
	GPIO_marcar_salida(30,2);
}

void refrescarSalidas(void)
{
	uint32_t estadoActual;
	uint32_t candidatos;
	uint8_t fila, columna, valor, pista;
	
	switch(cambiandoLed)
	{
		case TRUE:
			GPIO_clear_salida(0, 13);
			break;
		
		case FALSE:
			GPIO_clear_salida(0, 14);
			break;
	}
	
	// Obtenemos la fila y columna pedida
	fila = GPIO_leer(16,4)-1;
	columna = GPIO_leer(20,4)-1;
	
	// Obtenemos del sudoku el valor de esa celda
	if(fila < 8 && columna < 8)
	{
		valor = leer_celda(fila, columna);
		// Obtenemos los candidatos
		candidatos = leer_candidatos(fila, columna);
		
		// Marcamos el valor de la celda en la salida
		GPIO_escribir(0,3,valor);
		
		// Marcamos los candidatos de la celda en la salida
		GPIO_escribir(4,9,candidatos);
		
		// Escribimos el valor de la pista de esta celda en error
		GPIO_escribir(13,1,pista);
		
		if (es_pista(fila,columna))
			GPIO_escribir(13,1,1);
	}
	
	// Comprobamos el estado anterior de toda la GPIO
	estadoActual = GPIO_leer(0,31);
	if (estadoActual != estadoAnterior)
	{
		estadoAnterior = estadoActual;
		// crear_alarma_unica(POW_DOWN,EV_POWER,15 * SEGUNDO);
	}
}

void escribirValor(void)
{
	uint8_t fila,columna,valor; // Variables leidas de la GPIO
	boolean accesible;
	
	// Leemos la fila, columna y valor de la GPIO
	fila    = GPIO_leer(16,4)-1;
	columna = GPIO_leer(20,4)-1;
	valor   = GPIO_leer(24,4);
	
	// Miramos si ha pedido terminar
	checkFinPartida(fila+1,columna+1,valor);
	
	accesible = celdaAccesible(fila, columna);
	
	// Si es pista no hacemos nada
	if (accesible == TRUE && es_pista(fila,columna) == FALSE)
	{
		// Si no es pista lo introducimos
		introducirValorCelda(fila,columna,valor);
		candidatos_actualizar();
		
		// Si la jugada es errónea, encendemos el led
		if(celda_correcta(fila,columna) == FALSE)
		{
			cambiandoLed = TRUE;
			GPIO_escribir(13,1,1);
			// Creamos una alarma que apague el led
			crear_alarma_unica(LED_ERROR,EV_LED_ERR,1 * SEGUNDO);
		}			
	}
}

void eliminarValor(void)
{	
	uint8_t fila,columna,valor;
	boolean accesible;
	
	fila    = GPIO_leer(16,4)-1;
	columna = GPIO_leer(20,4)-1;
	valor   = GPIO_leer(24,4);
	
	// Miramos si ha pedido terminar
	checkFinPartida(fila+1,columna+1,valor);
	
	accesible = celdaAccesible(fila, columna);
	
	// Si no es una pista y además el procesador viene de un estado despierto
	if (accesible == TRUE && es_pista(fila,columna) == FALSE)
	{
		eliminarValorCelda(fila,columna);
		candidatos_actualizar();
	}
}

void checkFinPartida(uint8_t fila, uint8_t columna, uint8_t valor)
{
	if(fila == 0 && columna == 0 && valor == 0)
	{
		sudokuReiniciar();
		PM_power_down();
	}
}

void quitarLedErr(void)
{
	GPIO_clear_salida(13,1);
	cambiandoLed = FALSE;
}

void reiniciarEstadoAnterior(void)
{
	estadoAnterior = 0;
}

void latidoLed(void)
{
	switch(latido_on)	
	{
		case TRUE:
			GPIO_clear_salida(31,1);
			latido_on = FALSE;
			break;
		
		case FALSE:
			GPIO_escribir(31,1,1);
			latido_on = TRUE;
			break;
	}
}

void overflowLed(void)
{
	GPIO_escribir(30,1,1);
}
