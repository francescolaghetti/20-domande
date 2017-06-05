/*
 ============================================================================
 Name        : Progetto_20_domande2.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "akinator.h"

int main (){
	setbuf(stdout, NULL);



	/*int i;

	FILE *ftree;
	nodo->conta=0;
	strcpy (nodo->doma_risp, "");
	nodo->dx=-500;
	nodo->foglia=0;
	nodo->pieno=0;
	nodo->prec=-500;
	nodo->sx=-500;
	ftree=fopen ("albero", "wb");
	fwrite (nodo, sizeof(struct tree), 1, ftree);
	fclose(ftree);*/

	/*FILE *fsave;
	salva->ind_nod=-500;
	salva->pieno=0;
	strcpy (salva->risposta, "");
	fsave=fopen ("salvataggi", "wb");
	for (i=0; i<10; i++){
		fseek(fsave, i*sizeof(struct salvataggio), SEEK_SET);
		fwrite (nodo, sizeof(struct tree), 1, fsave);
	}
	fclose(fsave);*/

	menu_principale();
}
