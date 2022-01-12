#ifndef PARTIDA_H
#define PARTIDA_H

		typedef enum{
				MODO_PRINCIPIO 		= 0,
				MODO_JUGANDO 			= 1,
				MODO_TERMINANDO 	= 2
		}estado_juego_t;


		void partida_cambiar_estado(estado_juego_t estado);
		estado_juego_t partida_obtener_estado(void);
		
		void partida_preprar(void);
		void partida_empezar(void);
		void partida_terminar(volatile char *razon, int length);
		
		void partida_mostrar(void);
		
#endif // PARTIDA_H
