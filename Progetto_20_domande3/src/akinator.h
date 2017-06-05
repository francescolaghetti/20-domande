/*
 * akinator.h
 *
 *  Created on: 03 giu 2017
 *      Author: Laghetti
 */

#ifndef AKINATOR_H_
#define AKINATOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>



struct tree{
	char doma_risp[52];//stringa dove è contenuta la domanda (se è un nodo interno) o la risposta (se è una foglia). 52 caratteri perchè sono |50 risposta| |1 eventuale punto interrogativo da aggiungere| |1 /0|
	int prec;//indice del nodo superiore
	int sx;//indice del sottonodo sinistro
	int dx;//indice del sottonodo destro
	int conta; //conta il numero della domanda per sapere se è arrivato a 20;
	bool foglia;//if 0 nodo interno, if 1 foglia
	bool pieno;//if 0 vuoto, if 1 pieno
};

struct salvataggio{
	char risposta[51];//stringa contenente l'animale a cui aveva pensato l'utente durante la partita salvata
	bool pieno;//if 0 vuoto, if 1 pieno
	int ind_nod;//indice del nodo contenente la domanda a cui si era arrivati
};

void analisi_nodo(int);
char* richiesta_stringa();//VERIFICATO
int strspc(char*);//VERIFICATO
bool richiesta_si_no();//VERIFICATO
void nodo_vuoto(int, int, int);//VERIFICATO
void sostituzione_nodo(int, int);
void lista_risposte(int/*, int*/);//VERIFICATO
int richiesta_comando(int indice);
void fine_partita();
void sottonodi(int, int, char*, int, int);
void lista_salvataggi ();
void elimina_salvataggio();
void crea_salvataggio(int, int);
void menu_principale ();
void schermata_di_spiegazione (int, int);
void sequenza_di_chiusura();
void carica_salvataggio();
char* punto_interrogativo(char*);



#endif /* AKINATOR_H_ */
