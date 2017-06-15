/*
 * savectrl.c
 *
 *  Created on: 09 giu 2017
 *      Author: Laghetti
 */

#include "savectrl.h"

void lista_salvataggi (){

	struct salvataggio salva;
	FILE *fsave;
	int i;

	fsave=fopen("salvataggi", "rb+");
	if(fsave==NULL){//controllo sull'esistenza del file
		system("cls");
		printf("____________________\n\nERRORE 006: file \"salvataggi.bin\" non trovato\n____________________\n\n");
	    fclose(fsave);
	    getch();
	    exit(1);
	}
	for (i=0; i<10; i++){
	fseek(fsave, i*sizeof(struct salvataggio), SEEK_SET);
	fread(&salva, sizeof(struct salvataggio), 1, fsave);

	printf ("%c)",ftell(fsave)/sizeof(struct salvataggio)+64);


	if (salva.pieno==0)
		printf ("___SLOT_VUOTO\n");
	else
		printf ("...%s\n",salva.risposta);}
	fclose(fsave);

}

void crea_salvataggio(int indice, int provenienza){

	FILE *fsave;
	int n;
	char tmp[10];
	char *mdl;
	bool reit=1;
	bool conferma=1;
	struct salvataggio salva;

	while(reit==1){

		//STAMPA PREMESSE E LISTA SALVATAGGI
		printf ("_________________________\n\nSCHERMATA DI SALVATAGGIO\n_________________________\n\n");
		lista_salvataggi ();
		printf ("\nscrivi INDIETRO per tornare indietro o scegli il salvataggio dove vuoi salvare con la lettera corrispondente: ");

		//ACQUISIZIONE E CONTROLLO SCELTA DELL'UTENTE
		fgets(tmp, 10, stdin);//salva i primi 9 caratteri inseriti in tmp
		if(tmp[strlen(tmp)-1]=='\n'){//se l'ultimo carattere è il \n lo fa diventare \0. devo eliminare il \n perchè  se fossero stati inseriti esattamente 50 caratteri si avrebbe l'array [50char][\n][\0] e si avrebbe strlen(tmp)>50 anche se non è vero
			tmp[strlen(tmp)-1]='\0';
		}
		if(strlen(tmp)>8){//potrei scrivere semplicemente else perchè se \n è presente in tmp vuol dire che ci sono al massimo 5o caratteri
			while(getchar()!='\n');//assorbe tutti i caratteri rimasti nel buffer fino a quando non incontra \n
			system("cls");
			printf ("__________________________________________\n\nErrore 001: comando immesso non valido.\n__________________________________________\n");

		}

		//ANALISI SCELTA UTENTE

		else if (strlen(tmp)==1){//inserisce un solo carattere
			n = *tmp;
			if((n>=65 && n<=74) || (n>=97 && n<=106)){//controlla che abbia inserito una lettera dalla A alla J
				if (n>=65 && n<=74){//codice ASCII per le lettere da A a J
					n=n-64;
				}
				else if (n>=97 && n<=106){//codice ASCII per le lettere da a a j
					n=n-96;
				}
				//LETTURA DEL SALVATAGGIO SELEZIONATO
				fsave=fopen("salvataggi", "rb+");
				if(fsave==NULL){//controllo sull'esistenza del file
					system("cls");
					printf("____________________\n\nERRORE 006: file \"salvataggi.bin\" non trovato\n____________________\n\n");
					fclose(fsave);
					getch();
					exit(1);
				}
				fseek(fsave, (n-1)*sizeof(struct salvataggio), SEEK_SET);
				fread(&salva, sizeof(struct salvataggio), 1, fsave);
				fclose(fsave);

				//CONTROLLO SE LO SLOT SALVATAGGIO E' OCCUPATO
				if(salva.pieno==1){//salvataggio occupato
					//richiesta conferma sovrascrittura
					printf("\nil salvataggio selezionato è già occupato. Vuoi sovrascriverlo? ");
					//acquisizione conferma sovrascrittura
					conferma=richiesta_si_no();
					if(conferma==0){//NO alla sovrascrittura
						system("cls");
					}
				}
				if(conferma==1){
					//ACQUISIZIONE ANIMALE PENSATO
					printf("\ninserisci l'animale che avevi pensato: ");
					mdl=richiesta_stringa();
					strcpy(salva.risposta, mdl);
					free(mdl);
					//SCRITTURA DEL SALVATAGGIO
					salva.pieno=1;
					salva.ind_nod=indice;
					fsave=fopen("salvataggi", "rb+");
					if(fsave==NULL){//controllo sull'esistenza del file
						system("cls");
						printf("____________________\n\nERRORE 006: file \"salvataggi.bin\" non trovato\n____________________\n\n");
						fclose(fsave);
						getch();
						exit(1);
					}
					fseek(fsave, (n-1)*sizeof(struct salvataggio), SEEK_SET);
					fwrite(&salva, sizeof(struct salvataggio), 1, fsave);
					fclose(fsave);
					system("cls");
					printf ("_________________________\n\nsalvataggio avvenuto correttamente nello slot %c\n_________________________\n\n", n+64);
				}
			}
			else{
				system("cls");
				printf ("__________________________________________\n\nErrore 001: comando immesso non valido.\n__________________________________________\n");
			}
		}
		else{//inserisce più di un carattere
			if(!strcmp(tmp, "INDIETRO") || !strcmp(tmp, "indietro")){
				if (provenienza==0){// da domanda
					system("cls");
					analisi_nodo(indice);
					reit=0;
				}
				else if (provenienza==1){//da finale partita
					system("cls");
					menu_principale();
					reit=0;
				}
				else if (provenienza==2){//da finale partita
					system("cls");
					//sequenza_di_chiusura();
					reit=0;
				}
			}
			else{
				system("cls");
				printf ("__________________________________________\n\nErrore 001: comando immesso non valido.\n__________________________________________\n");
			}
		}
	}
}

void elimina_salvataggio(){


	FILE *fsave;
	int n;
	bool si_no;
	char tmp[10];
	struct salvataggio salva;
	bool reit=1;

	while(reit==1){

		//STAMPA PREMESSE E LISTA SALVATAGGI
		printf ("_________________________\n\nSCHERMATA DI ELIMINAZIONE DEI SALVATAGGI\n_________________________\n\n");
		lista_salvataggi();
		printf ("\nscrivi INDIETRO per tornare indietro o scegli il salvataggio che vuoi cancellare con la lettera corrispondente: ");

		//ACQUISIZIONE E CONTROLLO SCELTA UTENTE
		fgets(tmp, 10, stdin);//salva i primi 9 caratteri inseriti in tmp
		if(tmp[strlen(tmp)-1]=='\n'){//se l'ultimo carattere è il \n lo fa diventare \0. devo eliminare il \n perchè  se fossero stati inseriti esattamente 50 caratteri si avrebbe l'array [50char][\n][\0] e si avrebbe strlen(tmp)>50 anche se non è vero
			tmp[strlen(tmp)-1]='\0';
		}
		if(strlen(tmp)>8){//potrei scrivere semplicemente else perchè se \n è presente in tmp vuol dire che ci sono al massimo 5o caratteri
			while(getchar()!='\n');//assorbe tutti i caratteri rimasti nel buffer fino a quando non incontra \n
			system("cls");
			printf ("__________________________________________\n\nErrore 001: comando immesso non valido.\n__________________________________________\n");
		}

		//ANALISI SCELTA UTENTE

		else if (strlen(tmp)==1){//inserisce un solo carattere
			n = *tmp;//salva il codice ASCII del carattere inserito in n
			if ((n>=65 && n<=74) || (n>=97 && n<=106)){//ha inserito una lettera da A a J o da a a j
				if (n>=65 && n<=74){//codice ASCII per le lettere da A a J
					n=n-64;
				}
				else if (n>=97 && n<=106){//codice ASCII per le lettere da a a j
					n=n-96;
				}

				//LETTURA  DELLO SLOT SALVATAGGIO SELEZIONATO
				fsave=fopen("salvataggi", "rb+");
				if(fsave==NULL){//controllo sull'esistenza del file
					system("cls");
					printf("____________________\n\nERRORE 006: file \"salvataggi.bin\" non trovato\n____________________\n\n");
					fclose(fsave);
					getch();
					exit(1);
				}
				fseek(fsave, (n-1)*sizeof(struct salvataggio), SEEK_SET);
				fread(&salva, sizeof(struct salvataggio), 1, fsave);
				fclose(fsave);

				//CONTROLLO SE LO SLOT SALVATAGGIO E' VUOTO
				if(salva.pieno==1){//salvataggio occupato
					//RICHIESTA DI CONFERMA DI ELIMINAZIONE
					printf("\nSei sicuro di voler eliminare il salvataggio nello slot %c? ", n+64);
					//ACQUISIZIONE CONFERMA
					si_no=richiesta_si_no();
					//ANALISI CONFERMA
					if(si_no==0){//NO alla sovrascrittura
						system("cls");
					}
					else{//SI alla sovrascrittuta
						//SOVRASCRITTURA SALVATAGGIO
						salva.pieno=0;
						fsave=fopen("salvataggi", "rb+");
						if(fsave==NULL){//controllo sull'esistenza del file
							system("cls");
							printf("____________________\n\nERRORE 006: file \"salvataggi.bin\" non trovato\n____________________\n\n");
							fclose(fsave);
							getch();
							exit(1);
						}
						fseek(fsave, (n-1)*sizeof(struct salvataggio), SEEK_SET);
						fwrite(&salva, sizeof(struct salvataggio), 1, fsave);
						//fseek(fsave, (n-1)*sizeof(struct salvataggio), SEEK_SET);
						//fread(&salva, sizeof(struct salvataggio), 1, fsave);
						fclose(fsave);
						system("cls");
						printf ("_________________________\n\neliminazione del salvataggio avvenuta correttamente nello slot %c\n_________________________\n\n", n+64);
					}
				}
				else{//salvataggio vuoto
					system("cls");
					printf ("_________________________\n\nnon c'è alcun salvataggio da eliminare nello slot %c\n_________________________\n\n", n+64);
				}



			}
			else {
					system("cls");
					printf ("__________________________________________\n\nErrore 001: comando immesso non valido.\n__________________________________________\n");
			}
		}
		else{//inserisce più di un carattere
			if(!strcmp(tmp, "INDIETRO") || !strcmp(tmp, "indietro")){
				system("cls");
				menu_principale();
				reit=0;
			}
			else{
				system("cls");
				printf ("__________________________________________\n\nErrore 001: comando immesso non valido.\n__________________________________________\n");
			}
		}
	}
}


void carica_salvataggio(){


	FILE *fsave;
	int n;
	char tmp[10];//se usavo *tmp non funzionava
	struct salvataggio salva;
	bool reit=1;

	while(reit==1){
		//STAMPA DELLE PREMESSE E DELLA LISTA DEI SALVATAGGI
		printf ("_________________________\n\nSCHERMATA DI CARICAMENTO DEI SALVATAGGI\n_________________________\n\n");
		lista_salvataggi ();
		printf ("\nscrivi INDIETRO per tornare indietro o scegli il salvataggio che vuoi caricare con la lettera corrispondente: ");

		//ACQUISIZIONE E CONTROLLO DELLA SCELTA DELL'UTENTE
		fgets(tmp, 10, stdin);//salva i primi 9 caratteri inseriti in tmp
		if(tmp[strlen(tmp)-1]=='\n'){//se l'ultimo carattere è il \n lo fa diventare \0. devo eliminare il \n perchè  se fossero stati inseriti esattamente 50 caratteri si avrebbe l'array [50char][\n][\0] e si avrebbe strlen(tmp)>50 anche se non è vero
			tmp[strlen(tmp)-1]='\0';
		}
		if(strlen(tmp)>8){//potrei scrivere semplicemente else perchè se \n è presente in tmp vuol dire che ci sono al massimo 5o caratteri
			while(getchar()!='\n');//assorbe tutti i caratteri rimasti nel buffer fino a quando non incontra \n
			system("cls");
			printf ("__________________________________________\n\nErrore 001: comando immesso non valido.\n__________________________________________\n");
		}

		//ANALISI DELLA SCELTA INSERITA DALL'UTENTE

		else if (strlen(tmp)==1){//inserisce un solo carattere
			n = *tmp;//salva il codice ASCII del carattere inserito in n
			if ((n>=65 && n<=74) || (n>=97 && n<=106)){//ha inserito una lettera da A a J o da a a j
				if (n>=65 && n<=74){//codice ASCII per le lettere da A a J
					n=n-64;
				}
				else if (n>=97 && n<=106){//codice ASCII per le lettere da a a j
					n=n-96;
				}
				//LETTURA DEL SALVATAGGIO SELEZIONATO
				fsave=fopen("salvataggi", "rb+");
				if(fsave==NULL){//controllo sull'esistenza del file
					system("cls");
					printf("____________________\n\nERRORE 006: file \"salvataggi.bin\" non trovato\n____________________\n\n");
					fclose(fsave);
					getch();
					exit(1);
				}
				fseek(fsave, (n-1)*sizeof(struct salvataggio), SEEK_SET);
				fread(&salva, sizeof(struct salvataggio), 1, fsave);
				fclose(fsave);

				//ANALISI DEL SALVATAGGIO
				if(salva.pieno==0){//salvataggio vuoto
					system("cls");
					printf("_________________________\n\nIl salvataggio scelto è vuoto. Selezionane un altro o torna INDIETRO.\n_________________________\n\n");
				}
				else{//salvataggio pieno
					system("cls");
					printf ("_________________________\n\nHai deciso di caricare il salvataggio dello slot %c\nL'animale che avevi pensato per questa partita è %s\nPremi un tasto qualsiasi quando sei pronto\n_________________________\n\n", n+64, salva.risposta);
					getch();
					system("cls");
					//analisi_nodo(salva.ind_nod);
					procedura_domanda(salva.ind_nod);
					reit=0;
				}
			}

			else{
				system("cls");
				printf ("__________________________________________\n\nErrore 001: comando immesso non valido.\n__________________________________________\n");
			}
		}
		else{//inserisce più di un carattere
			if(!strcmp(tmp, "INDIETRO") || !strcmp(tmp, "indietro")){
				system("cls");
				menu_principale();
				reit=0;
			}
			else{
				system("cls");
				printf ("__________________________________________\n\nErrore 001: comando immesso non valido.\n__________________________________________\n");
			}
		}
	}
}
