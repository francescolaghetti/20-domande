/*
 * gamectrl.c
 *
 *  Created on: 09 giu 2017
 *      Author: Laghetti
 */

#include "gamectrl.h"

void menu_principale (){

	int scelta;
	char tmp[3];//stringa dove effettuerò il controllo
	bool reit=1;

	while(reit==1){
		//STAMPA A SCHERMO DEL MENU
		printf ("_________________________\n\nBENVENUTO NEL GIOCO DELLE 20 DOMANDE\n_________________________\n\n1)...NUOVA PARTITA\n2)...CARICA PARTITA\n3)...ELIMINA SALVATAGGIO\n4)...REGOLE E COMANDI\n5)...ESCI\n\nImmetti il numero corrispondente alla scelta che vuoi fare: ");

		//ACQUISIZIONE E CONTROLLO DELLA SCELTA DELL'UTENTE
		fgets(tmp, 3, stdin);//salva i primi 2 caratteri inseriti in tmp
		if(tmp[strlen(tmp)-1]=='\n'){//se l'ultimo carattere è il \n lo fa diventare \0. devo eliminare il \n perchè  se fossero stati inseriti esattamente 50 caratteri si avrebbe l'array [50char][\n][\0] e si avrebbe strlen(tmp)>50 anche se non è vero
			tmp[strlen(tmp)-1]='\0';
		}
		if(strlen(tmp)>1){//potrei scrivere semplicemente else perchè se \n è presente in tmp vuol dire che ci sono al massimo 5o caratteri
			while(getchar()!='\n');//assorbe tutti i caratteri rimasti nel buffer fino a quando non incontra \n
			system("cls");
			printf ("__________________________________________\n\nErrore 001: comando immesso non valido.\n__________________________________________\n");
		}

		//CONTROLLO DELLA SCELTA EFFETTUATA
		else{//non è stato riscontrato nessun errore
			scelta=atoi(tmp);//trasforma la scelta dell'utente in un int da usare nello switch
			switch(scelta){
				case 1:
					nuova_partita();
					reit=0;
					break;
				case 2:
					system("cls");
					carica_salvataggio();
					reit=0;
					break;
				case 3:
					system("cls");
					elimina_salvataggio();
					reit=0;
					break;
				case 4:
					system("cls");
					schermata_di_spiegazione(1, -500);
					reit=0;
					break;
				case 5:
					system("cls");
					//sequenza_di_chiusura();
					reit=0;
					break;
				default:
					system("cls");
					printf ("__________________________________________\n\nErrore 001: comando immesso non valido.\n__________________________________________\n");
					break;
			}
		}
	}
}

void nuova_partita(){
	system("cls");
	printf ("__________________________________________\n\nPer iniziare a giocare pensa ad un animale\nQuando sei pronto premi un tasto qualsiasi\n__________________________________________\n");
	getch();
	system("cls");
	analisi_nodo(0);
}

void analisi_nodo(int indice){

	struct tree nodo;
	FILE *ftree;

	//LETTURA DEL NODO INDICATO DALL'INDICE
	ftree=fopen("albero", "rb+");
	if(ftree==NULL){//controllo sull'esistenza del file
		system("cls");
		printf("____________________\n\nERRORE 006: file \"albero.bin\" non trovato\n____________________\n\n");
	    fclose(ftree);
	    getch();
	    exit(1);
	}
	fseek (ftree, indice*sizeof(struct tree), SEEK_SET);
	fread(&nodo, sizeof(struct tree), 1, ftree);
	fclose(ftree);

	//ANALISI DEL NODO
	  //NODO VUOTO
	if (nodo.pieno==0){
		procedura_domande_finite(indice, nodo.conta);
	}

	  //NODO PIENO
	else{

		//FOGLIA
		if (nodo.foglia==1){//ci troviamo in una foglia
			procedura_risposta(indice);
		}

		//NODO INTERNO
		else{//ci troviamo in un nodo interno
			procedura_domanda(indice);
		}
	}
}

void procedura_domande_finite(int indice, int contatore){

	//STAMPA DEL MESSAGGIO DI SCONFITTA
	printf("____________________\n\nMI HAI SCONFITTO! MI ARRENDO!\n____________________\n\n");

	//SOSTITUZIONE DEL NODO VUOTO E CREAZIONE DEI SUOI SOTTONODI
	//il controllo sull'aver posto 20 domande lo fa la funzione sostituzione_nodo()
	sostituzione_nodo(indice, contatore);
	fine_partita();
}

void procedura_risposta (int indice){

	struct tree nodo;
	bool si_no;
	FILE *ftree;

	//LETTURA NODO DA FILE
	ftree=fopen("albero", "rb+");
	if(ftree==NULL){//controllo sull'esistenza del file
		system("cls");
		printf("____________________\n\nERRORE 006: file \"albero.bin\" non trovato\n____________________\n\n");
	    fclose(ftree);
	    getch();
	    exit(1);
	}
	fseek (ftree, indice*sizeof(struct tree), SEEK_SET);
	fread(&nodo, sizeof(struct tree), 1, ftree);//per riportare il nodo conta al valore giusto
	fclose(ftree);

	//STAMPA DELLA RISPOSTA
	printf("____________________\n\nPROVO AD INDOVINARE!\n____________________\n\n");
	printf ("Stavi pensando a %s?\nRispondi con SI o NO: ", nodo.doma_risp);

	//ACQUISIZIONE DELLA RISPOSTA
	si_no=richiesta_si_no();

	//ANALISI DELLA RISPOSTA

		//INDOVINATO
	if(si_no==1){
		printf("____________________\n\nHO VINTO! HO INDOVINATO!\n____________________\n\n");
		//procedura di fine partita(gioca,menu,esci)
		fine_partita();
	}
		//NON INDOVINATO
	else{
		printf("____________________\n\nOH NO! NON HO INDOVINATO!\n____________________\n\n");
		if(nodo.conta==20){//ovvero siamo oltre la 20esima domanda, cioè siamo nell'ultimo livello dell'albero dove sono presenti solo foglie
			//volendo si può chiedere a quale animale pensasse ma ai fini del software è un dato che andrebbe perso
			//procedura di fine partita(gioca,menu,esci)//non si richiede nient'altro perchè si possono fare massimo 20 domande
			fine_partita();
		}
		else{//non si sono ancora poste 20 domande
			sostituzione_nodo(indice, nodo.conta/*contatore*/);
			fine_partita();}
		}

}

void procedura_domanda(int indice){

	FILE *ftree;
	struct tree nodo;
	int comando;
	bool si_no;
	bool reit=0;

	//LETTURA DEL NODO
	ftree=fopen("albero", "rb+");
	if(ftree==NULL){//controllo sull'esistenza del file
		system("cls");
		printf("____________________\n\nERRORE 006: file \"albero.bin\" non trovato\n____________________\n\n");
	    fclose(ftree);
	    getch();
	    exit(1);
	}
	fseek (ftree, indice*sizeof(struct tree), SEEK_SET);
	fread(&nodo, sizeof(struct tree), 1, ftree);
	fclose(ftree);

	while(reit==0){
		//STAMPA A VIDEO LE PREMESSE, LA LISTA E LA DOMANDA
		printf("____________________\n\nDOMANDA %d\nRICORDA: inserisci SI o NO per rispondere o uno degli altri 5 comandi\nSe non li ricordi inserisci COMANDI\n____________________\n\n", nodo.conta+1);
		lista_risposte(indice);
		printf("\nDOMANDA: %s\nInserisci la risposta o il comando: ", nodo.doma_risp);

		//ACQUISISCE IL COMANDO DELL'UTENTE
		comando=richiesta_comando(indice);

		//ANALIZZA IL COMANDO
		if(comando==0){//risposta NO
			system("cls");
			analisi_nodo(nodo.sx);
			reit=1;
		}
		else if(comando==1){//risposta SI
			system("cls");
			analisi_nodo(nodo.dx);
			reit=1;
		}
		else if(comando==2){//risposta COMANDI
			schermata_di_spiegazione (0, indice);
			reit=1;
		}
		else if(comando==3){//risposta INDIETRO
			system("cls");
			procedura_domanda(nodo.prec);
			reit=1;
		}
		else if(comando==4){//risposta SALVA
			system("cls");
			crea_salvataggio(indice, 0);
			reit=1;
		}
		else if(comando==5){//risposta MENU
			printf("Sei sicuro di voler tornare al menù? ");
			si_no=richiesta_si_no();
			if(si_no==1){//vuole tornare al menu principale
				printf("Vuoi salvare prima di tornare al menù? ");
				si_no=richiesta_si_no();
				if(si_no==0){//NON vuole salvare
					system("cls");
					menu_principale();
					reit=1;
				}
				if(si_no==1){//vuole salvare
					system("cls");
					crea_salvataggio(indice, 1);
					reit=1;
				}
			}
			else {//NON vuole tornare al menu principale
				system("cls");
			}
		}
		else if(comando==6){//risposta ESCI
			printf("Sei sicuro di voler uscire? ");
			si_no=richiesta_si_no();
			if(si_no==1){//vuole uscire
				printf("Vuoi salvare prima di uscire? ");
				si_no=richiesta_si_no();
				if(si_no==0){//NON vuole salvare
					system("cls");
					reit=1;
					//sequenza_di_chiusura();
				}
				if(si_no==1){//vuole salvare
					system("cls");
					crea_salvataggio(indice, 2);
					reit=1;
				}
			}
			else{//NON vuole uscire
				system("cls");
			}
		}
	}
}
void lista_risposte(int indice/*, int contatore*/){
	/* INFO
		 1.a
		 	 PARAMETRI DI INPUT:
		 	 	 puntatore a file
		 	 	 puntatore a struct tree
		 	 	 indice della domanda a cui si è arrivati (ftell(ftree)/sizeof(struct tree))
		 1.b
		 	 PARAMETRI DI OUTPUT:
		 	 	 stampa:
		 	 	 la lista delle domande precedenti con la relativa risposta
		 2
		 	 DESCRIZIONE:
		 	 	 stampa la lista delle domande precedenti con la relativa risposta
		 2.a
		 	 DETTAGLI IMPLEMENTATIVI SALIENTI:
		 	 	 nessuno
		 2.b
		 	 CASI LIMITE:
		 	 	 nessuno
		 2.c
		 	 INPUT:
		 	 	struct nell'indice 0:
				nodo->dx=2;
				nodo->sx=1;
				nodo->prec=-500;
				nodo->conta=0;
				nodo->foglia=0;
				nodo->pieno=1;
				nodo->doma_risp, "Ha 4 zampe?";

				struct nell'indice 1:
				nodo->dx=4;
				nodo->sx=3;
				nodo->prec=0;
				nodo->conta=1;
				nodo->foglia=0;
				nodo->pieno=1;
				nodo->doma_risp, "è arancione?";

				struct nell'indice 4:
				nodo->dx=6;
				nodo->sx=5;
				nodo->prec=1;
				nodo->conta=2;
				nodo->foglia=0;
				nodo->pieno=1;
				nodo->doma_risp, "Ha le strisce?";

				(ftree, nodo, 4)
		 	 OUTPUT
		 	 	Domande precedenti:
				2) è arancione?: SI
				1) Ha 4 zampe?: NO

		 3
		 	 AUTORI:
		 	 	 Francesco Laghetti
		 4
		 	 REVISIONE:
		 	 	 versione 1.0 18/05/2017
		 */

	struct tree nodo;
	FILE *ftree;
	int i, contatore, posizione;

	//STAMPA DELLA PREMESSA
	printf("Domande precedenti:\n");

	//POSIZIONAMENTO SUL NODO INDICATO DALL'INDICE E LETTURA DELLO STESSO
	posizione=indice;//ftell(ftree)/sizeof(struct tree);
	ftree=fopen("albero", "rb+");
	if(ftree==NULL){//controllo sull'esistenza del file
		system("cls");
		printf("____________________\n\nERRORE 006: file \"albero.bin\" non trovato\n____________________\n\n");
	    fclose(ftree);
	    getch();
	    exit(1);
	}
	fseek(ftree, indice*sizeof(struct tree), SEEK_SET);
	fread(&nodo, sizeof(struct tree), 1, ftree);
	fclose(ftree);

	//STAMPA DELLA LISTA DELLE DOMANDE
	contatore=nodo.conta;
	if(contatore==0){//si trova alla prima domanda
		printf("nessuna\n");//ergo non c'è nessuna domanda precedente
	}
	else{//NON si trova alla prima domanda
		for (i=contatore-1; i>=0; i--){//for che ripercorre a ritroso tutte le domande precedenti a quella dell'indice indicato
			ftree=fopen("albero", "rb+");
			if(ftree==NULL){//controllo sull'esistenza del file
				system("cls");
				printf("____________________\n\nERRORE 006: file \"albero.bin\" non trovato\n____________________\n\n");
				fclose(ftree);
				getch();
				exit(1);
			}
			fseek(ftree, nodo.prec*sizeof(struct tree), SEEK_SET);
			fread(&nodo, sizeof(struct tree), 1, ftree);
			printf("%d) %s: ",i+1,nodo.doma_risp);
			if(nodo.sx==posizione){
				printf("NO\n");
			}
			else if(nodo.dx==posizione){
				printf("SI\n");
			}
			posizione=ftell(ftree)/sizeof(struct tree)-1;//-1 perchè dopo fread il puntatore si sposta nel successivo indice
			fclose(ftree);
		}}
}

void schermata_di_spiegazione (int provenienza, int indice){

	//STAMPA A SCHERMO DELLE REGOLE E COMANDI
	system("cls");
	printf("__________________________________________\n\nREGOLE E COMANDI\n\nQuesto gioco si pone l’obbiettivo di sfidare il giocatore nel riuscire ad indovinare un qualsiasi animale da lui pensato in massimo 20 domande.\n"
			"Per cominciare non devi far altro che selezionare NUOVA PARTITA  oppure su CARICA PARTITA se vuoi riprendere una partita lasciata a metà.\n"
			"NOTA BENE: ricorda l’animale pensato per tutta la durata della partita!\n\n"
			"Durante la partita, avrai a disposizione ben 7 comandi:\n"
			"-SI: per rispondere affermativamente alla domanda che ti viene posta.\n"
			"-NO: per rispondere negativamente alla domanda che ti viene posta.\n"
			"-COMANDI: per ritornare in questa schermata.\n"
			"-INDIETRO: per tornare alla domanda precedente ed eventualmente dare una risposta diversa (NON puoi usare questo comando alla prima domanda!).\n"
			"-SALVA: ti permetterà di salvare la partita in corso. "
			"Ti verrà chiesto di inserire il nome dell’animale a cui stai pensando così potrai ricordartelo quando caricherai il salvataggio.\n"
			"-MENU: ti fa tornare al menù principale.\n"
			"-ESCI: ti fa uscire dal gioco.\n\n"
			"NOTA BENE: i comandi potrai scriverli solo tutto in maiuscolo o tutto in minuscolo!\n\n"
			"Premi un tasto qualsiasi per uscire da questa schermata.\n");

	//CONTROLLO SUL PREMERE QUALSIASI TASTO
	getch();

	//RITORNO ALLA PAGINA PRECEDENTE
	if (provenienza == 1){
		system("cls");
		menu_principale();}
	else
		system("cls");
		analisi_nodo(indice);
}

void fine_partita(){

		int scelta;
		bool reit=1;
		char tmp[3];//stringa dove effettuerò il controllo

		while(reit==1){
			//STAMPA DEL MESSAGGIO DI RINGRAZIAMENTO E DELLE SCELTE
			printf ("\n__________________________________________\n\nGRAZIE PER AVER GIOCATO!\n__________________________________________\n\nCosa vuoi fare ora?\n1)...NUOVA PARTITA\n2)...MENU PRINCIPALE\n3)...ESCI\n\nImmetti il numero corrispondente alla scelta che vuoi fare: ");

			//ACQUISIZIONE E CONTROLLO DELLA SCELTA DELL'UTENTE
			fgets(tmp, 3, stdin);//salva i primi 2 caratteri inseriti in tmp
			if(tmp[strlen(tmp)-1]=='\n'){//se l'ultimo carattere è il \n lo fa diventare \0. devo eliminare il \n perchè  se fossero stati inseriti esattamente 50 caratteri si avrebbe l'array [50char][\n][\0] e si avrebbe strlen(tmp)>50 anche se non è vero
				tmp[strlen(tmp)-1]='\0';
			}
			if(strlen(tmp)>1){//potrei scrivere semplicemente else perchè se \n è presente in tmp vuol dire che ci sono al massimo 5o caratteri
				while(getchar()!='\n');//assorbe tutti i caratteri rimasti nel buffer fino a quando non incontra \n
				system("cls");
				printf ("__________________________________________\n\nErrore 001: comando immesso non valido.\n__________________________________________\n");
			}

			//ANALISI DELLA SCELTA EFFETTUATA
			else{
				scelta=atoi(tmp);//trasforma la scelta dell'utente in un int da usare nello switch
				switch(scelta){
					case 1:
						nuova_partita();
						reit=0;
						break;
					case 2:
						system("cls");
						menu_principale();
						reit=0;
						break;
					case 3:
						system("cls");
						//sequenza_di_chiusura();
						reit=0;
						break;
					default:
						system("cls");
						printf ("__________________________________________\n\nErrore 001: comando immesso non valido.\n__________________________________________\n");
						break;
				}
			}
		}
}

