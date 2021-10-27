#ifndef OLD_H
#define OLD_H

#include <inttypes.h>

//Struct que almacena la informacion del evento 
typedef struct Informacion {
    int ID_evento;
    int auxData;
    int marca_temporal;
}Info;

//Struct que representa un nodo de la lista circular
struct nodo {
    struct Informacion dato;
    struct nodo *sig;
    struct nodo *ant;
};

//Struct que representa una lista circular que almacena informacion de eventos
typedef struct Lista {
     struct nodo *ini;
     int tamano;
}Lista;

Info newInfo(int ID, int auxData, int timeStamp);
void inicializarLista(Lista *l, Info x);
int size(Lista *l);
//void insertarPrimero(Lista *l,Info x);
void insertar(Lista *l, Info x);
void liberar(Lista *l);
struct Informacion sacarElemento(Lista *l);

#endif //OLD_H
