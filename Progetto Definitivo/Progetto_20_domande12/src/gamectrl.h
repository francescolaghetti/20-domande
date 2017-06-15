/*
 * gamectrl.h
 *
 *  Created on: 09 giu 2017
 *      Author: Laghetti
 */

#ifndef GAMECTRL_H_
#define GAMECTRL_H_

#include"treectrl.h"
#include"savectrl.h"

void menu_principale ();
void nuova_partita ();
void analisi_nodo(int);
void procedura_risposta (int);
void procedura_domande_finite(int, int);
void procedura_domanda (int);
void lista_risposte(int);
void schermata_di_spiegazione (int, int);
void fine_partita();

#endif /* GAMECTRL_H_ */
