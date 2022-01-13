#include "cola.h"
#include "temporizador.h"
#include "gestor_pulsacion.h"
#include "gestor_alarmas.h"
#include "gestor_IO.h"
#include "gestor_energia.h"
#include "gestor_comandos.h"
#include "gestor_output.h"
#include "sudoku_p2.h"
#include "partida.h"
#include "access.h"

#include "pantalla.h"

struct index{
		int aLeer;			// Evento a ser ejecutado
		int aEscribir;  // Espacio libre para introducir un evento
};

static volatile struct index indice;
extern int status_mode(void);

void cola_guardar_eventos(event_t idEvento, uint32_t auxData)
{
		int indiceAux;
		
		lock();
		
		indiceAux = indice.aEscribir;				// Creamos un �ndice auxiliar	
		indice.aEscribir++;									// aumentamos el �ndice
		
		if (indice.aEscribir == MAX_EVENTS)
		{
				// Si hemos llegado al final de la cola, ponemos el �ndice al 0
				indice.aEscribir = 0;	
		}

		unlock();
		
		// Comprobamos que donde queremos introducir el evento, no hay otro esperando a ser le�do
 		if(eventList[indiceAux].ready == FALSE)
		{
				// Preparamos el evento
				eventList[indiceAux].id = idEvento;
				eventList[indiceAux].auxData = auxData;
					
				if (status_mode() == USER)
				{
						eventList[indiceAux].marcaTemporal = clock_gettime(); 
				}
				else
				{
						eventList[indiceAux].marcaTemporal = temporizador_leer();
				}
				
				eventList[indiceAux].ready = TRUE;		
				return; // Salimos de la funci�n
		}
		WD_feed();
		while(1){}
}

void tratar_alarma(uint32_t auxData){
		switch(auxData){ 				// La acci�n a realizar depende de sus datos auxiliares
				case EV_GPIO_REF:			// Refrescar las salidas de la GPIO
						IO_refrescarSalidas();
						break;
				
				case EV_POWER:				// Poner el procesador en modo Power Down
						PM_power_down();
						IO_reiniciarEstadoAnterior();
						break;
				
				case EV_CHECK_PULS:		// Comprobar la pulsaci�n de los botones
						gestion_eint1(EV_CHECK_PULS);
						gestion_eint2(EV_CHECK_PULS);
						break;
				
				case EV_LED_ERR:			// Apagar el led de error de la GPIO
						IO_quitarLedErr();
						break;
				
				case EV_LATIDO:				// Apagar el led del latido de la GPIO
						IO_latidoLed();
						break;
				
				case EV_FIN_PARTIDA:
						partida_terminar("Partida ha sido muy larga", 25);
						break;
				
				case EV_FEED_WATCHDOG:
						WD_feed();
						break;
				
				case EV_COMMAND_CONFIRM:
						alarma_parar(LED_CANCELAR-1);
						IO_apagar_led_cancelar();
						comando_aceptar();
						break;
				
				case EV_LED_CANCELAR:
						IO_cancelarLed();
						break;
				
				default:
						break;
		}
}

void cola_leer_evento()
{ 	
		event_t id;
		estado_juego_t estado_partida;
		uint32_t auxData;
		int fila, columna, valor;
	
		// Sacamos la informaci�n del evento y liberamos su espacio
		id = eventList[indice.aLeer].id;
		auxData = eventList[indice.aLeer].auxData;
		eventList[indice.aLeer].ready = FALSE;
		
		// Aumentamos en 1 el �ndice
		indice.aLeer++;

		// Estamos al final de la cola de procesos
		if(indice.aLeer == MAX_EVENTS)
			indice.aLeer = 0;	
		
		// Acci�n dependendiendo del identificador del evento
		switch (id)
		{
				case SET_ALARMA:
						// Gestionar evento de la alarma
						tratar_alarma(auxData);
						break;
				
				// Gestionar el evento de timer0
				case SET_TIMER_0:
						alarma_gestionar_alarmas();
						break;
				
				// Gestionar interrupci�n externa 1
				case SET_EXT_INT_1:
						switch(energia_estado_actual())
						{
							case DESPIERTO:
									alarma_parar(LED_CANCELAR-1);
									IO_apagar_led_cancelar();
									alarma_add_alarma_PD();
									
									comando_aceptar();
									break;
								
							case DORMIDO:
									energia_actualizar_estado(NULL_EVENT);
									break;
							
							default:
								break;
						}			
						break;
			
				// Gestionar interrupci�n externa 2
				case SET_EXT_INT_2:
						switch(energia_estado_actual())
						{
							case DESPIERTO:
									alarma_parar(LED_CANCELAR-1);
									IO_apagar_led_cancelar();
									alarma_add_alarma_PD();
									
									comando_cancelar();
									break;
								
							case DORMIDO:
									energia_actualizar_estado(NULL_EVENT);
									break;
							
							default:
									break;
						}			
						break;
				
				// Gestionar entrada de la UART
				case SET_UART_SEND_CHR:
						alarma_add_alarma_PD();
						estado_partida = partida_obtener_estado();
						if(estado_partida == MODO_PRINCIPIO)
						{
								partida_empezar();
						}
						else
						{
								comando_recibir_caracter(auxData);
						}			
						break;
				
				case SET_WRITE_COMMAND:
						alarma_add_alarma_PD();
						estado_partida = partida_obtener_estado();
						if (estado_partida == MODO_JUGANDO)
						{
								valor = auxData & 0xFF;
								columna = (auxData >> 0x8) & 0xFF;
								fila = (auxData >> 0x10) & 0xFF;
								sudoku_introducir_valor(fila, columna, valor);
								comando_permitir_cancelacion();
								partida_mostrar();
						}						
						break;
				
				case SET_RESET_COMMAND:
						alarma_add_alarma_PD();
						estado_partida = partida_obtener_estado();
						if (estado_partida == MODO_JUGANDO)
						{
								partida_terminar("Usuario a escrito #RST!",23);
						}						
						break;
				
				case SET_NEW_COMMAND:
						alarma_add_alarma_PD();					
						estado_partida = partida_obtener_estado();
						if (estado_partida == MODO_TERMINANDO)
						{
								sudoku_reiniciar();
								partida_preprar();
						}							
						break;				
				
				default:
						break;
		}
}

boolean cola_hay_evento()
{
		return eventList[indice.aLeer].ready;
}

void scheduler()
{	
		indice.aLeer = 0;
		indice.aEscribir = 0;
		
		while(1)
		{
				if(cola_hay_evento() == TRUE){
						cola_leer_evento();
				}else{
						PM_idle();
				}		
		}
}
