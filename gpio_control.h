#ifndef GPIO_CONTROL_H
#define GPIO_CONTROL_H

#include <stdlib.h>
#include <inttypes.h>

void GPIO_iniciar (void);
int32_t GPIO_leer (int bit_inicial, int num_bits);
void GPIO_escribir (int bit_inicial, int num_bits, int32_t valor);
void GPIO_marcar_entrada (int bit_inicial, int num_bits);
void GPIO_marcar_salida (int bit_inicial, int num_bits);

#endif //GPIO_CONTROL_H
