#ifndef GESTOR_COMANDOS_H
#define GESTOR_COMANDOS_H

	typedef enum {
		OVERSIZED_COMMAND,
		NOT_A_COMMAND,
		BAD_CHECKSUM
	}command_err;

	void init_serial (void); /* Initialize Serial Interface       */
		
	// Funciones accesibles por el resto
	void comando_reiniciar(void);
	void comando_preparar_nuevo(void);
	void comando_recibir_caracter(int chr);
	void comando_detectar(void);
	
	void reiniciar_partida(void);
	void nueva_partida(void);
	void introducir_jugada(int[]);
	void lanzar_error(command_err err);

#endif //GESTOR_COMANDOS_H
