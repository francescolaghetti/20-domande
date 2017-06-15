/*
 * treectrl.h
 *
 *  Created on: 09 giu 2017
 *      Author: Laghetti
 */

#ifndef TREECTRL_H_
#define TREECTRL_H_

#include "inputctrl.h"
#include "gamectrl.h"

struct tree{
	char doma_risp[52];//stringa dove è contenuta la domanda (se è un nodo interno) o la risposta (se è una foglia). 52 caratteri perchè sono |50 risposta| |1 eventuale punto interrogativo da aggiungere| |1 /0|
	int prec;//indice del nodo superiore
	int sx;//indice del sottonodo sinistro
	int dx;//indice del sottonodo destro
	int conta; //conta il numero della domanda per sapere se è arrivato a 20;
	bool foglia;//if 0 nodo interno, if 1 foglia
	bool pieno;//if 0 vuoto, if 1 pieno
};

void nodo_vuoto(int, int, int);
void sostituzione_nodo(int, int);
void sottonodi(int, int, char*, int, int);

#endif /* TREECTRL_H_ */
