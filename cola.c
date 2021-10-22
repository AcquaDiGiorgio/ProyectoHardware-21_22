#include <LPC210X.H>                            // LPC21XX Peripheral Registers
#include "cola.h"
#include<stdio.h>
#include<stdlib.h>

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
typedef struct ListaIdentificar {
     struct nodo *ini;
     int tamano;
}Lista;

/***********************************************/
//  Funcion que inicializa una lista           //
/***********************************************/
void inicializacion(Lista *l){
		l->ini=NULL;
		l->tamano=0;
}

/***********************************************/
//  Funcion que devuelve el tamaño de la lista  //
/***********************************************/
int cantidad(Lista *l){
    return l->tamano;
}

/********************************************************/
//  Funcion que inserta el dato al inicio de la lista  //
/********************************************************/
void insertarPrimero(Lista *l,Info *x)
{
    struct nodo *nuevo;
    nuevo=malloc(sizeof(struct nodo));
    
    nuevo->dato.ID_evento = x->ID_evento;
    nuevo->dato.auxData = x->auxData;
    nuevo->dato.marca_temporal = x->marca_temporal;

    if (l->ini == NULL){
        nuevo->sig = nuevo;
        nuevo->ant = nuevo;
        l->ini = nuevo;
    }
    else{
        struct nodo *ultimo = l->ini->ant;
        nuevo->sig = l->ini;
        nuevo->ant = ultimo;
        l->ini->ant = nuevo;
        ultimo->sig = nuevo;
        l->ini = nuevo;
    }
    
    l->tamano++;
}

/********************************************************/
//  Funcion que inserta el dato al final de la lista  //
/********************************************************/
void insertarUltimo(Lista *l, Info *x)
{
    struct nodo *nuevo;
    nuevo=malloc(sizeof(struct nodo));
    
    nuevo->dato.ID_evento = x->ID_evento;
    nuevo->dato.auxData = x->auxData;
    nuevo->dato.marca_temporal = x->marca_temporal;
    
    if (l->ini == NULL){
        nuevo->sig = nuevo;
        nuevo->ant = nuevo;
        l->ini = nuevo;
    }
    else{
        struct nodo *ultimo = l->ini->ant;
        nuevo->sig = l->ini;
        nuevo->ant = ultimo;
        l->ini->ant = nuevo;
        ultimo->sig = nuevo;
    }
    
    l->tamano++;
}

/**************************************************************************/
//  Funcion que libera toda la lista en memoria, la borra por completo  //
/**************************************************************************/
void liberar(Lista *l){
    if (l->ini != NULL) {
        struct nodo *reco = l->ini->sig;
        struct nodo *bor;
        while (reco != l->ini)
        {
            bor = reco;
            reco = reco->sig;
            free(bor);
        }
        free(l->ini);
    }
    l->tamano=0;
    l->ini=NULL;
}

/********************************************************/
//  Funcion que borra el dato al inicio de la lista  //
/********************************************************/
struct Informacion borrarElemento(Lista *l){
    if(l!=NULL){
        struct Informacion resultado;
        struct nodo *anterior = l->ini;
        if(cantidad(l)!=0){
            resultado=l->ini->dato;
            if(cantidad(l) != 1){
                l->ini=l->ini->sig;
                l->ini->ant=anterior->ant;
                anterior->ant->sig=l->ini;
                free(anterior);
                l->tamano--;
            }
            else{
                l->tamano=0;
                l->ini=NULL;
            }
        }
        return resultado;
    }
}

/********************************************************/
//  Funcion escribe la lista de datos por pantalla      //
/********************************************************/
void imprimir(Lista *l){
    if (cantidad(l)!=0) {
        struct nodo *reco = l->ini;
        printf("{");
        do {
            printf("{%i,",reco->dato.ID_evento);
            printf("%i,",reco->dato.auxData);
            printf("%i}",reco->dato.marca_temporal);
            reco = reco->sig;
        } while (reco != l->ini);
        printf("}\n");
    }
}

