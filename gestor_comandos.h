#ifndef GESTOR_COMANDOS_H
#define GESTOR_COMANDOS_H

	typedef enum {
		OVERSIZED_COMMAND,
		NOT_A_COMMAND,
		BAD_CHECKSUM
	}command_err;
		
	// Funciones accesibles por el resto
	void comando_reiniciar(void);
	void comando_preparar_nuevo(void);
	void comando_recibir_caracter(char chr); 
	void comando_detectar(void);
	
	void reiniciar_partida(void);
	void nueva_partida(void);
	void introducir_jugada(int[]);
	void lanzar_error(command_err err);
	
	void comando_permitir_cancelacion(void);
	void comando_cancelar(void);
	void comando_aceptar(void);

#endif //GESTOR_COMANDOS_H
