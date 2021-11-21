#ifndef GESTOR_IO_H
#define GESTOR_IO_H

#include <inttypes.h>

void initIO(void);

void refrescarSalidas(void);
void escribirValor(void);
void eliminarValor(void);
void quitarLedErr(void);
void checkFinPartida(uint8_t fila, uint8_t columna, uint8_t valor);
void overflow(void);

#endif //GESTOR_IO_H
