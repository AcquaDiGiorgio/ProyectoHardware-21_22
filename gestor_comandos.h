#ifndef GESTOR_COMANDOS_H
#define GESTOR_COMANDOS_H

	typedef enum {
		OVERSIZED_COMMAND,
		NOT_A_COMMAND,
		BAD_CHECKSUM
	}command_err;

	void init_serial (void); /* Initialize Serial Interface       */
		
	void preparar_nuevo_comando(void);
	void recibir_caracter(char chr);
	void detectar_comando(void);
	void reiniciar_partida(void);
	void nueva_partida(void);
	void introducir_jugada(int[]);
	void lanzar_error(command_err err);
	int to_uint(char);

#endif //GESTOR_COMANDOS_H
