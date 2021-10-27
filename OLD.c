#include <LPC210X.H>                            // LPC21XX Peripheral Registers
#include "OLD.h"
#include <stdlib.h>

//***********************************************//
//  Funcion que crea un dato para la lista       //
//***********************************************//
Info newInfo(int ID, int auxData, int timeStamp){
	Info inf;
	inf.ID_evento = ID;
	inf.auxData = auxData;
	inf.marca_temporal = timeStamp;
	return inf;
}


//***********************************************//
//  Funcion que inicializa una lista             //
//***********************************************//
void inicializarLista(Lista *l, Info x){
	//l = malloc(sizeof(Lista)); 
	
	struct nodo *nuevo;
	//nuevo = malloc(sizeof(struct nodo));
	
	nuevo->dato.ID_evento = x.ID_evento;
	nuevo->dato.auxData = x.auxData;
	nuevo->dato.marca_temporal = x.marca_temporal;
	nuevo->sig = nuevo;
	nuevo->ant = nuevo;
	
	l->ini=nuevo;
	l->tamano=1;
}

/***********************************************/
//  Funcion que devuelve el tamaño de la lista  /
/***********************************************/
int size(Lista *l){
    return l->tamano;
}

//********************************************************//
//  Funcion que inserta el dato al inicio de la lista     //
//********************************************************//
/*
void insertarPrimero(Lista *l,Info x)
{
    struct nodo *nuevo;
    nuevo=malloc(sizeof(struct nodo));
    
    nuevo->dato.ID_evento = x.ID_evento;
    nuevo->dato.auxData = x.auxData;
    nuevo->dato.marca_temporal = x.marca_temporal;

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
*/
//********************************************************//
//  Funcion que inserta el dato al final de la lista      //
//********************************************************//
void insertar(Lista *l, Info x)
{
    struct nodo *nuevo;
    nuevo=malloc(sizeof(struct nodo));
    
    nuevo->dato.ID_evento = x.ID_evento;
    nuevo->dato.auxData = x.auxData;
    nuevo->dato.marca_temporal = x.marca_temporal;
    
    if (l->ini == NULL){
			nuevo->sig = nuevo;
			nuevo->ant = nuevo;
      l->ini = nuevo;
			l->tamano=1;
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

//**************************************************************************//
//  Funcion que libera toda la lista en memoria, la borra por completo      //
//**************************************************************************//
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

//********************************************************//
//  Funcion que borra el dato al inicio de la lista       //
//********************************************************//
struct Informacion sacarElemento(Lista *l){
    if(l!=NULL){
        struct Informacion resultado;
        struct nodo *anterior = l->ini;
        if(size(l)!=0){
            resultado=l->ini->dato;
            if(size(l) != 1){
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
