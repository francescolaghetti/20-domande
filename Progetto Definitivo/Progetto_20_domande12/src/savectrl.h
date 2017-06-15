/*
 * savectrl.h
 *
 *  Created on: 09 giu 2017
 *      Author: Laghetti
 */

#ifndef SAVECTRL_H_
#define SAVECTRL_H_

#include "inputctrl.h"
#include "gamectrl.h"

struct salvataggio{
	char risposta[51];//stringa contenente l'animale a cui aveva pensato l'utente durante la partita salvata
	bool pieno;//if 0 vuoto, if 1 pieno
	int ind_nod;//indice del nodo contenente la domanda a cui si era arrivati
};

void lista_salvataggi ();
void elimina_salvataggio();
void crea_salvataggio(int, int);
void carica_salvataggio();

#endif /* SAVECTRL_H_ */
