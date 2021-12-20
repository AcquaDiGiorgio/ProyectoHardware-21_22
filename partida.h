#ifndef PARTIDA_H
#define PARTIDA_H

		typedef enum{
				principio 	= 0,
				jugando 		= 1,
				final 			= 2
		}estado_juego_t;


		void cambiar_estado_partida(estado_juego_t estado);
		
		void preprar_partida(void);
		void empezar_partida(void);
		void terminar_partida(char *razon, int length);
		
		estado_juego_t obtener_estado_juego(void);
		
		void dibujar(void);
		
#endif // PARTIDA_H
