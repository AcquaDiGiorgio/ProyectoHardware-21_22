//#include "botones.h"
#include <LPC210X.H>                            // LPC21XX Peripheral Registers

//Funcion para inicializar los botones
void eint_init (void);

//Devuelve el valor de la variable para detectar una nueva pulsacion
int button_nueva_pulsacion_1(void);
int button_nueva_pulsacion_2(void);

//Resetea la variable correspondiente a 0
void button_clear_nueva_pulsacion_1(void);
void button_clear_nueva_pulsacion_2(void);
