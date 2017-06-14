/*
 * treectrl.c
 *
 *  Created on: 09 giu 2017
 *      Author: Laghetti
 */

#include "treectrl.h"

void nodo_vuoto(int indice_nodo, int indice_prec, int contatore){
	/*INFO
			 1.a
			 	 PARAMETRI DI INPUT:
			 	 	 intero indice del nodo che si vuole creare vuoto
			 	 	 intero indice della domanda a cui si è arrivati (ftell(ftree)/sizeof(struct tree))
			 	 	 intero contatore del numero di domande
			 1.b
			 	 PARAMETRI DI OUTPUT:
			 	 	 nessuno
			 2
			 	 DESCRIZIONE:
			 	 	 crea un nodo vuoto nel indice indicato
			 2.a
			 	 DETTAGLI IMPLEMENTATIVI SALIENTI:
			 	 	 nessuno
			 2.b
			 	 CASI LIMITE:
			 	 	 il file contenente l'albero non viene trovato
			 	 	 non c'è abbastanza spazio per l'allocazione dinamica
			 2.c
			 	 INPUT:
			 	 	nodo nell'indice 0;
					nodo->dx=2;
					nodo->sx=1;
					nodo->prec=-500;
					nodo->conta=0;
					nodo->foglia=0;
					nodo->pieno=1;
					nodo->doma_risp, "Ha 4 zampe";

					(nodo->sx, ftell(ftree)/sizeof(struct tree)-1, nodo->conta)
			 	 SITUAZIONE FINALE:
			 	 	crea nell'indice nodo->sx un nodo vuoto ovvero con le seguenti caratteristiche
			 	 	nodo->pieno=0;
					nodo->foglia=0;
					nodo->conta=contatore+1;
					nodo->prec=indice_prec;
					nodo->sx=-500;
					nodo->dx=-500;
					nodo->doma_risp, "";
			 3
			 	 AUTORI:
			 	 	 Francesco Laghetti
			 4
			 	 REVISIONE:
			 	 	 versione 1.0 18/05/2017
			 */
	struct tree nodo;

	FILE *ftree;

	nodo.pieno=0;
	nodo.foglia=0;
	nodo.conta=contatore+1;
	nodo.prec=indice_prec;
	nodo.sx=-500;
	nodo.dx=-500;
	strcpy(nodo.doma_risp, "");
	ftree=fopen("albero", "rb+");
	if(ftree==NULL){//controllo sull'esistenza del file
		system("cls");
		printf("____________________\n\nERRORE 006: file \"albero.bin\" non trovato\n____________________\n\n");
	    fclose(ftree);
	    getch();
	    exit(1);
	}
	fseek (ftree, indice_nodo*sizeof(struct tree), SEEK_SET);
	fwrite(&nodo, sizeof(struct tree), 1, ftree);
	fclose(ftree);
}

void sostituzione_nodo(int indice, int contatore){
	/*INFO
			 1.a
			 	 PARAMETRI DI INPUT:
			 	 	 intero indice del nodo che si vuole sostituire
			 	 	 intero contatore del numero di domande
			 1.b
			 	 PARAMETRI DI OUTPUT:
			 	 	 nessuno
			 2
			 	 DESCRIZIONE:
			 	 	 acquisisce le informazioni sull'animale pensato dall'utente,
			 	 	 scopre quali sono i primi due indici liberi del file dove è salvato l'albero,
			 	 	 sovrascrive il nodo indicato con la domanda posta dall'utente a cui viene fatto un controllo sugli ultimi caratteri per capire la presenza di punti interrogativi,
			 	 	 infine richiama una funzione che creerà i due sottonodi nei due indici individuati scegliendo in quale creare una foglia e in quale creare un nodo vuoto in base al fatto che la risposta dell'utente è stata SI o NO
			 2.a
			 	 DETTAGLI IMPLEMENTATIVI SALIENTI:
			 	 	 scopre quali sono i primi indici liberi del file con fseek() utilizzando l'opzione seek_end
			 2.b
			 	 CASI LIMITE:
			 	 	 il file contenente l'albero non viene trovato
			 	 	 non c'è abbastanza spazio per l'allocazione dinamica
			 2.c
			 	 SITUAZIONE INIZIALE:
			 	 	 nodo nell'indice 2;
					 nodo->dx=-500;
					 nodo->sx=-500;
					 nodo->prec=0;
					 nodo->conta=1;
					 nodo->foglia=1;
					 nodo->pieno=1;
					 nodo->doma_risp, "gatto";

			 	 	 sostituzione_nodo(indice, nodo->conta)
			 	 INPUT:
					 "cavallo"
					 "ha la criniera"
					 "SI"

			 	 SITUAZIONE FINALE:
			 	 	sovrascrive il nodo iniziale con le seguenti informazioni
			 	 	nodo->pieno=1;
					nodo->foglia=0;
					nodo->sx=ind_sx;//primo indice libero del file
					nodo->dx=ind_dx;//secondo indice libero del file
					nodo->doma_risp, "ha la criniera?";

					e poi avvia la funzione sottonodi(ind_dx, ind_sx, animale, indice, contatore)
			 3
			 	 AUTORI:
			 	 	 Francesco Laghetti
			 4
			 	 REVISIONE:
			 	 	 versione 1.0 18/05/2017
			 */
	struct tree nodo;
	FILE *ftree;
	char *mdl;
	char animale[52], doma[52];
	//int ind_sx, ind_dx;
	bool si_no, conferma;
	bool reit=0;//bool che impedisce la reiterazione dopo la non conferma dei dati inseriti

	while(reit==0){
	//STAMPA DELLA LISTA DELLE DOMANDE PRECEDENTI
	lista_risposte(indice);//per ricordare all'utente quali domande sono già state fatte così non ne iserisce una uguale
	printf("\n");

	//ACQUISIZIONE ANIMALE PENSATO DALL'UTENTE
	printf ("A quale animale avevi pensato (MAX 50 caratteri)? ");
	mdl=richiesta_stringa();
	strcpy(animale, mdl);
	free(mdl);

	//CONTROLLO DELLA 20ESIMA DOMANDA
	if(contatore==20){//ovvero si è superata la 20a domanda e quindi si chiede solo l'animale a cui aveva pensato l'utente
		//RICHISTA CONFERMA INSERIMENTI
		printf("\nHai inserito:\n\nANIMALE = %s\n\nConfermi? ", animale);
		conferma=richiesta_si_no();
		if(conferma==0){//conferma=NO
			system("cls");
			printf("____________________\n\nREINSERISCI\n____________________\n\n");
		}
		else{//conferma=SI=1
			//SOSTITUZIONE DEL NODO VUOTO CON UNA FOGLIA
			//lettura del nodo vuoto
			ftree=fopen("albero", "rb+");
			if(ftree==NULL){//controllo sull'esistenza del file
				system("cls");
				printf("____________________\n\nERRORE 006: file \"albero.bin\" non trovato\n____________________\n\n");
				fclose(ftree);
				getch();
				exit(1);
			}
			fseek (ftree, indice*sizeof(struct tree), SEEK_SET);//va al nodo da sostituire
			fread (&nodo, sizeof (struct tree), 1, ftree);//legge il nodo da modificare
			fclose(ftree);
			//modifica di alcuni parametri per renderlo una foglia
			nodo.pieno=1;
			nodo.foglia=1;
			strcpy(nodo.doma_risp, animale);
			//sovrascrittura della foglia sul nodo vuoto
			ftree=fopen("albero", "rb+");
			if(ftree==NULL){//controllo sull'esistenza del file
				system("cls");
				printf("____________________\n\nERRORE 006: file \"albero.bin\" non trovato\n____________________\n\n");
				fclose(ftree);
				getch();
				exit(1);
			}
			fseek (ftree, indice*sizeof(struct tree), SEEK_SET);//va al nodo da sostituire
			fwrite (&nodo, sizeof (struct tree), 1, ftree);//apporta le modifiche
			fclose(ftree);
			reit=1;
		}
	}
	else{//NON si è arrivati alla 20esima domanda

		//acquisizione domanda dell'utente
		printf ("Quale domanda mi avrebbe aiutato ad identificarlo (MAX 50 caratteri)? ");
		mdl=punto_interrogativo(richiesta_stringa());
		strcpy(doma, mdl);
		free(mdl);

		//acquisizione della risposta alla domanda dell'utente
		printf ("A questa domanda avresti risposto SI o NO? ");
		si_no=richiesta_si_no();

		//richiesta conferma inserimenti
		printf("\nHai inserito:\n\nANIMALE = %s\nDOMANDA = %s\nRISPOSTA = ", animale, doma);
		if(si_no==0){
			printf("NO");
		}
		else{
			printf("SI");
		}
		printf("\n\nConfermi? ");
		conferma=richiesta_si_no();
		if(conferma==0){
			system("cls");
			printf("____________________\n\nREINSERISCI\n____________________\n\n");
		}
		else{//conferma=SI=1
			//lettura del nodo da sostituire
			ftree=fopen("albero", "rb+");
			if(ftree==NULL){//controllo sull'esistenza del file
				system("cls");
				printf("____________________\n\nERRORE 006: file \"albero.bin\" non trovato\n____________________\n\n");
				fclose(ftree);
				getch();
				exit(1);
			}
			fseek (ftree, indice*sizeof(struct tree), SEEK_SET);//va al nodo da sostituire
			fread (&nodo, sizeof (struct tree), 1, ftree);//legge il nodo da modificare
			fseek (ftree, 0, SEEK_END);//punta alla fine del file e quindi al suo primo indice libero
			nodo.sx=ftell(ftree)/sizeof(struct tree);
			nodo.dx=ftell(ftree)/sizeof(struct tree)+1;
			fclose(ftree);
			strcpy(nodo.doma_risp, doma);
			nodo.foglia=0;
			nodo.pieno=1;
			nodo.conta=contatore;
			ftree=fopen("albero", "rb+");
			if(ftree==NULL){//controllo sull'esistenza del file
				system("cls");
				printf("____________________\n\nERRORE 006: file \"albero.bin\" non trovato\n____________________\n\n");
				fclose(ftree);
				getch();
				exit(1);
			}
			fseek (ftree, indice*sizeof(struct tree), SEEK_SET);//punta di nuovo all'indice del nodo iniziale
			fwrite(&nodo, sizeof(struct tree), 1, ftree);
			fclose(ftree);
			if (si_no==0){//risposta NO
				sottonodi(nodo.sx, nodo.dx, animale, indice, contatore);}
			else{//risposta SI
				sottonodi(nodo.dx, nodo.sx, animale, indice, contatore);}
			reit=1;
	}}}}

void sottonodi(int leaf, int empty, char* animale, int indice, int contatore){
	/*INFO
			 1.a
			 	 PARAMETRI DI INPUT:
			 	 	 intero indice dove verrà creata la foglia
			 	 	 intero indice dove verrà creato il nodo vuoto
			 	 	 puntatore ad array di caratteri contenente l'animale pensato dall'utente
			 	 	 intero indice del nodo precedente
			 	 	 intero contatore del numero di domande
			 1.b
			 	 PARAMETRI DI OUTPUT:
			 	 	 nessuno
			 2
			 	 DESCRIZIONE:
			 	 	 crea una foglia nell'indice indicato da leaf con la risposta dell'utente contenuta in animale,
			 	 	 richiama una funzione che creerà un nodo vuoto nell'indice indicato da empty
			 2.a
			 	 DETTAGLI IMPLEMENTATIVI SALIENTI:
			 	 	 nessuno
			 2.b
			 	 CASI LIMITE:
			 	 	 il file contenente l'albero non viene trovato
			 	 	 non c'è abbastanza spazio per l'allocazione dinamica
			 2.c
			 	 SITUAZIONE FINALE:
					nell'indice indicato da leaf viene creato un nodo foglia con le seguenti caratteristiche;
						nodo.prec=indice;
						nodo.sx=-500;
						nodo.dx=-500;
						nodo.pieno=1;
						nodo.foglia=1;
						nodo.conta=contatore+1;
						nodo.doma_risp=animale;

					e poi avvia la funzione nodo_vuoto(empty, indice, contatore)
			 3
			 	 AUTORI:
			 	 	 Francesco Laghetti
			 4
			 	 REVISIONE:
			 	 	 versione 1.0 18/05/2017
			 */


	struct tree nodo;
	FILE *ftree;

	nodo.prec=indice;
	nodo.sx=-500;
	nodo.dx=-500;
	nodo.pieno=1;
	nodo.foglia=1;
	nodo.conta=contatore+1;
	strcpy(nodo.doma_risp, animale);
	ftree=fopen("albero", "rb+");
	if(ftree==NULL){//controllo sull'esistenza del file
		system("cls");
		printf("____________________\n\nERRORE 006: file \"albero.bin\" non trovato\n____________________\n\n");
	    fclose(ftree);
	    getch();
	    exit(1);
	}
	fseek (ftree, leaf*sizeof(struct tree), SEEK_SET); //si sposta sul nodo indicato da leaf ovvero il sottoalbero dove metterò la risposta
	fwrite(&nodo, sizeof(struct tree), 1, ftree);
	fclose(ftree);
	nodo_vuoto(empty, indice, contatore);//si sposta sul nodo indicato da empty ovvero il sottoalbero che sarà vuoto

}
