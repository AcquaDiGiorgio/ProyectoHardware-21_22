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
volatile boolean cancelado_on = FALSE;

const boolean modo_p2 = FALSE;

//Inicializa el gestor IO
void IO_init(void)
{
		// Iniciamos la GPIO
		GPIO_iniciar();
		
		// Creamos la alarma que refresca la GPIO
		alarma_crear_alarma_periodica(0,EV_GPIO_REF,200);
		
		// Marcamos Salidas
		if (modo_p2 == TRUE)
		{
			GPIO_marcar_salida(0,14);
			GPIO_marcar_salida(30,2);
		}
		else
		{
			GPIO_marcar_salida(0,14);
			GPIO_marcar_salida(16,16);
		}
}

void IO_refrescarSalidas(void)
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
				valor = sudoku_leer_valor(fila, columna);
				// Obtenemos los candidatos
				candidatos = sudoku_leer_candidatos(fila, columna);
				
				// Marcamos el valor de la celda en la salida
				GPIO_escribir(0,3,valor);
				
				// Marcamos los candidatos de la celda en la salida
				GPIO_escribir(4,9,candidatos);
				
				// Escribimos el valor de la pista de esta celda en error
				GPIO_escribir(13,1,pista);
				
				if (sudoku_es_pista(fila,columna))
						GPIO_escribir(13,1,1);
		}
		
		// Comprobamos el estado anterior de toda la GPIO
		estadoActual = GPIO_leer(0,31);
		if (estadoActual != estadoAnterior)
		{
				estadoAnterior = estadoActual;
				alarma_add_alarma_PD();
		}
}

void IO_escribirValor(void)
{
		uint8_t fila,columna,valor; // Variables leidas de la GPIO
		boolean accesible;
		
		// Leemos la fila, columna y valor de la GPIO
		fila    = GPIO_leer(16,4)-1;
		columna = GPIO_leer(20,4)-1;
		valor   = GPIO_leer(24,4);
		
		// Miramos si ha pedido terminar
		IO_checkFinPartida(fila+1,columna+1,valor);
		
		accesible = sudoku_celdaAccesible(fila, columna);
		
		// Si es pista no hacemos nada
		if (accesible == TRUE && sudoku_es_pista(fila,columna) == FALSE)
		{
				// Si no es pista lo introducimos
				sudoku_introducir_valor(fila,columna,valor);
				candidatos_actualizar();
				
				// Si la jugada es err�nea, encendemos el led
				if(sudoku_celda_correcta(fila,columna) == FALSE)
				{
						cambiandoLed = TRUE;
						GPIO_escribir(13,1,1);
						// Creamos una alarma que apague el led
						alarma_crear_alarma_unica(0,EV_LED_ERR,1 * SEGUNDO);
				}			
		}
}

void IO_eliminarValor(void)
{	
	uint8_t fila,columna,valor;
	boolean accesible;
	
	fila    = GPIO_leer(16,4)-1;
	columna = GPIO_leer(20,4)-1;
	valor   = GPIO_leer(24,4);
	
	// Miramos si ha pedido terminar
	IO_checkFinPartida(fila+1,columna+1,valor);
	
	accesible = sudoku_celdaAccesible(fila, columna);
	
	// Si no es una pista y adem�s el procesador viene de un estado despierto
	if (accesible == TRUE && sudoku_es_pista(fila,columna) == FALSE)
	{
		sudoku_eliminar_valor(fila,columna);
		candidatos_actualizar();
	}
}

void IO_checkFinPartida(uint8_t fila, uint8_t columna, uint8_t valor)
{
	if(fila == 0 && columna == 0 && valor == 0)
	{
		sudoku_reiniciar();
		energia_power_down();
	}
}

void IO_quitarLedErr(void)
{
	GPIO_clear_salida(13,1);
	cambiandoLed = FALSE;
}

void IO_reiniciarEstadoAnterior(void)
{
	estadoAnterior = 0;
}

void IO_latidoLed(void)
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

void IO_cancelarLed(void)
{
		switch(cancelado_on)	
		{
			case TRUE:
				IO_apagar_led_cancelar();
				break;
			
			case FALSE:
				GPIO_escribir(28,1,1);
				cancelado_on = TRUE;
				break;
		}
}

void IO_apagar_led_cancelar(void)
{
		GPIO_clear_salida(28,1);
		cancelado_on = FALSE;
}

void IO_overflowLed(void)
{
	GPIO_escribir(30,1,1);
}
