/*
 * akinator.c
 *
 *  Created on: 03 giu 2017
 *      Author: Laghetti
 */

#include "akinator.h"

int strspc(char *str){ //restituisce 1 qualora la stringa sia di lunghezza 0 e 2 se è composta interamente da spazi o tabulazioni
	/*INFO
	 1.a
	 	 PARAMETRI DI INPUT:
	 	 	 puntatore ad array di caratteri
	 1.b
	 	 PARAMETRI DI OUTPUT:
	 	 	 restituisce:
	 	 	 1 qualora la stringa sia di lunghezza 0
	 	 	 2 se è composta interamente da spazi o tabulazioni
	 	 	 0 in tutti gli altri casi
	 2
	 	 DESCRIZIONE:
	 	 	 controlla se la stringa è vuota o composta solo da spazi e tabulazioni
	 2.a
	 	 DETTAGLI IMPLEMENTATIVI SALIENTI:
	 	 	 controlla se la stringa è vuota o composta solo da spazi e tabulazioni con la funzione isspace()
	 2.b
	 	 CASI LIMITE:
	 	 	 nessuno
	 2.c
	 	 INPUT:
	 	 	 str*="    	"
	 	 SITUAZIONE FINALE:
	 	 	return 2
	 3
	 	 AUTORI:
	 	 	 Francesco Laghetti
	 4
	 	 REVISIONE:
	 	 	 versione 1.0 18/05/2017
	 */
	int c;
	size_t sl = strlen(str);
	if (!sl) return 1;
	for(c=0; c<(int) strlen(str); c++)
	if(!isspace(str[c])) return 0;
	return 2;
}

void analisi_nodo(int indice){

	struct tree *nodo=malloc(sizeof(struct tree));
	//struct salvataggio *salva=malloc(sizeof(struct salvataggio));
	char *mdl;
	int comando;
	bool si_no;
	FILE *ftree;

	//lettura del nodo indicato da indice
	ftree=fopen("albero", "rb+");
	fseek (ftree, indice*sizeof(struct tree), SEEK_SET);
	fread(nodo, sizeof(struct tree), 1, ftree);
	fclose(ftree);

	//analisi del nodo
	if (nodo->pieno==0){//NODO VUOTO
		printf("____________________\n\nMI HAI SCONFITTO! MI ARRENDO!\n____________________\n\n");
		if(nodo->conta==20){//ovvero siamo oltre la 20esima domanda, cioè siamo nell'ultimo livello dell'albero dove sono presenti solo foglie
				printf ("A quale animale avevi pensato (MAX 50 caratteri)? ");
				mdl=richiesta_stringa();
				/*strcpy(animale, mdl);
				strcpy(nodo->doma_risp, animale);*/
				strcpy(nodo->doma_risp, mdl);
				free(mdl);
				nodo->pieno=1;
				nodo->foglia=1;
				ftree=fopen("albero", "rb+");
				fseek (ftree, indice*sizeof(struct tree), SEEK_SET);
				fwrite(nodo, sizeof(struct tree), 1, ftree);
				fclose(ftree);
				//procedura di fine partita(gioca,menu,esci)
				free(nodo);
				fine_partita();
		}
		else{//ovvero non abbiamo ancora posto 20 domande
			free(nodo);
			lista_risposte(indice);//per ricordare all'utente quali domande sono già state fatte così non ne iserisce una uguale
			printf("\n");
			struct tree *nodo=malloc(sizeof(struct tree));
			ftree=fopen("albero", "rb+");
			fseek (ftree, indice*sizeof(struct tree), SEEK_SET);
			fread(nodo, sizeof(struct tree), 1, ftree);//per riportare il nodo conta al valore giusto
			fclose(ftree);
			free(nodo);
			sostituzione_nodo(indice, nodo->conta/*contatore*/);
			//procedura di fine partita(gioca,menu,esci)
			fine_partita();
		}

		}
	else{//nodo pieno
		if (nodo->foglia==1){//ci troviamo in una foglia
			printf("____________________\n\nPROVO AD INDOVINARE!\n____________________\n\n");
			printf ("Stavi pensando a %s?\nRispondi con SI o NO: ", nodo->doma_risp);
			si_no=richiesta_si_no();
			if(si_no==1){//INDOVINATO
				printf("____________________\n\nHO VINTO! HO INDOVINATO!\n____________________\n\n");
				free(nodo);
				//procedura di fine partita(gioca,menu,esci)
				fine_partita();
			}
			else{//NON INDOVINATO
				printf("____________________\n\nOH NO! NON HO INDOVINATO!\n____________________\n\n");
				if(nodo->conta==20){//ovvero siamo oltre la 20esima domanda, cioè siamo nell'ultimo livello dell'albero dove sono presenti solo foglie
					//volendo si può chiedere a quale animale pensasse ma ai fini del software è un dato che andrebbe perso
					free(nodo);
					//procedura di fine partita(gioca,menu,esci)//non si richiede nient'altro perchè si possono fare massimo 20 domande
					fine_partita();
				}
				else{//non si sono ancora poste 20 domande
					free(nodo);
					lista_risposte(indice);//per ricordare all'utente quali domande sono già state fatte così non ne iserisce una uguale
					printf("\n");
					struct tree *nodo=malloc(sizeof(struct tree));
					ftree=fopen("albero", "rb+");
					fseek (ftree, indice*sizeof(struct tree), SEEK_SET);
					fread(nodo, sizeof(struct tree), 1, ftree);//per riportare il nodo conta al valore giusto
					fclose(ftree);
					free(nodo);
					sostituzione_nodo(indice, nodo->conta/*contatore*/);
					fine_partita();}
				}
			}
		else{//ci troviamo in un nodo interno
			//system("cls");
			printf("____________________\n\nDOMANDA %d\nRICORDA: inserisci SI o NO per rispondere o uno degli altri 7 comandi\nSe non li ricordi inserisci COMANDI\n____________________\n\n", nodo->conta+1);
			free(nodo);
			lista_risposte(indice);
			struct tree *nodo=malloc(sizeof(struct tree));
			ftree=fopen("albero", "rb+");
			fseek (ftree, indice*sizeof(struct tree), SEEK_SET);
			fread(nodo, sizeof(struct tree), 1, ftree);
			fclose(ftree);
			printf("\nDOMANDA: %s\nInserisci la risposta o il comando: ", nodo->doma_risp);
			comando=richiesta_comando(indice);
			if(comando==0){//risposta NO
				system("cls");
				free(nodo);
				analisi_nodo(nodo->sx);
			}
			else if(comando==1){//risposta SI
				system("cls");
				free(nodo);
				analisi_nodo(nodo->dx);
			}
			else if(comando==2){//risposta COMANDI
				free(nodo);
				schermata_di_spiegazione (0, indice);
			}
			else if(comando==3){//risposta INDIETRO
				system("cls");
				free(nodo);
				analisi_nodo(nodo->prec);
			}
			else if(comando==4){//risposta SALVA
				system("cls");
				free(nodo);
				crea_salvataggio(indice, 0);
			}
			else if(comando==5){//risposta MENU
				printf("Sei sicuro di voler tornare al menù? ");
				si_no=richiesta_si_no();
				if(si_no==1){
					printf("Vuoi salvare prima di tornare al menù? ");
					si_no=richiesta_si_no();
					if(si_no==0){
						system("cls");
						free(nodo);
						menu_principale();
					}
					if(si_no==1){
						system("cls");
						free(nodo);
						crea_salvataggio(indice, 1);
					}
				}
				else {
					system("cls");
					free(nodo);
					analisi_nodo(indice);}
			}
			else if(comando==6){//risposta ESCI
				printf("Sei sicuro di voler uscire? ");
				si_no=richiesta_si_no();
				if(si_no==1){
					printf("Vuoi salvare prima di uscire? ");
					si_no=richiesta_si_no();
					if(si_no==0){
						system("cls");
						free(nodo);
						sequenza_di_chiusura();
					}
					if(si_no==1){
						system("cls");
						free(nodo);
						crea_salvataggio(indice, 2);
					}
				}
				else{
					system("cls");
					free(nodo);
					analisi_nodo(indice);}
			}
		}
	}
}

char *richiesta_stringa(){
	/*INFO
	 1.a
	 	 PARAMETRI DI INPUT:
	 	 	niente
	 1.b
	 	 PARAMETRI DI OUTPUT:
	 	 	 restituisce:
	 	 	 il puntatore ad una stringa se tutto va bene
	 	 	 "ERRORE 002: parola immessa non valida\nReinseriscila per favore: " se la stringa è vuota o formata solo da spazi e tabulazioni
	 2
	 	 DESCRIZIONE:
	 	 	acquisice una stringa, controlla se la stringa è vuota o composta solo da spazi e tabulazioni, se non lo è restituisce il suo puntatore
	 2.a
	 	 DETTAGLI IMPLEMENTATIVI SALIENTI:
	 	 	 controlla la stringa con strspc
	 2.b
	 	 CASI LIMITE:
	 	 	 nessuno
	 2.c
	 	 INPUT:
	 	 	 risp="    	"
	 	 OUTPUT:
	 		 ERRORE 002: parola immessa non valida\nReinseriscila per favore:
	 	 INPUT:
	 	 	 risp="cavolo"
	 	 SITUAZIONE FINALE:
	 	 	return risp
	 3
	 	 AUTORI:
	 	 	 Francesco Laghetti
	 4
	 	 REVISIONE:
	 	 	 versione 1.0 18/05/2017
	 */

	char *risp=malloc(51);
	char tmp[52];//stringa dove effettuerò il controllo

	fgets(tmp, 52, stdin);//salva i primi 51 caratteri inseriti in tmp
	if(tmp[strlen(tmp)-1]=='\n'){//se l'ultimo carattere è il \n lo fa diventare \0. devo eliminare il \n perchè  se fossero stati inseriti esattamente 50 caratteri si avrebbe l'array [50char][\n][\0] e si avrebbe strlen(tmp)>50 anche se non è vero
		tmp[strlen(tmp)-1]='\0';
	}
	if(strlen(tmp)>50){//potrei scrivere semplicemente else perchè se \n è presente in tmp vuol dire che ci sono al massimo 5o caratteri
		printf("ERRORE 004: parola immessa troppo lunga\nReinseriscila per favore: ");
		while(getchar()!='\n');//assorbe tutti i caratteri rimasti nel buffer fino a quando non incontra \n
		risp=richiesta_stringa();
		return risp;
	}
	strcpy(risp, tmp);
	if(strspc(risp)!=0){//la stringa inserita è vuota o composta solo da spazi e tabbature
		printf("ERRORE 002: parola immessa non valida\nReinseriscila per favore: ");
		risp=richiesta_stringa();
		return risp;
	}

	return risp;
}

bool richiesta_si_no(){
	/*INFO
	 1.a
	 	 PARAMETRI DI INPUT:
	 	 	 nessuno
	 1.b
	 	 PARAMETRI DI OUTPUT:
	 	 	 restituisce:
	 	 	 1 se la risposta è si o SI
	 	 	 0 se la risposta è no o NO
	 	 	 ERRORE 002: parola immessa non valida\nReinseriscila per favore: in tutti gli altri casi e ricomincia la funzione
	 2
	 	 DESCRIZIONE:
	 	 	 controlla se la risposta inserita è si (o SI) oppure no (o NO)
	 2.a
	 	 DETTAGLI IMPLEMENTATIVI SALIENTI:
	 	 	 nessuno
	 2.b
	 	 CASI LIMITE:
	 	 	 nessuno
	 2.c
	 	 INPUT:
	 	 	si_no="cavolo"
	 	 OUTPUT
	 	 	ERRORE 002: parola immessa non valida\nReinseriscila per favore:
	 	 INPUT
	 	 	si_no="SI"
	 	 SITUAZIONE FINALE:
	 	 	return 1
	 3
	 	 AUTORI:
	 	 	 Francesco Laghetti
	 4
	 	 REVISIONE:
	 	 	 versione 1.0 18/05/2017
	 */
	char tmp[4];
	bool comando;

	fgets(tmp, 4, stdin);//salva i primi 8 caratteri inseriti in tmp
	if(tmp[strlen(tmp)-1]=='\n'){//se l'ultimo carattere è il \n lo fa diventare \0. devo eliminare il \n perchè  se fossero stati inseriti esattamente 50 caratteri si avrebbe l'array [50char][\n][\0] e si avrebbe strlen(tmp)>50 anche se non è vero
		tmp[strlen(tmp)-1]='\0';
	}
	if(strlen(tmp)>2){//potrei scrivere semplicemente else perchè se \n è presente in tmp vuol dire che ci sono al massimo 5o caratteri
		while(getchar()!='\n');//assorbe tutti i caratteri rimasti nel buffer fino a quando non incontra \n
		printf("ERRORE 001: comando immesso non valido\nReinseriscilo per favore: ");
		comando=richiesta_si_no();
		return comando;
	}
	if(strspc(tmp)!=0){//la stringa inserita è vuota o composta solo da spazi e tabbature
		printf("ERRORE 001: comando immesso non valido\nReinseriscilo per favore: ");
		comando=richiesta_si_no();
		return comando;
	}
	if(!strcmp(tmp, "NO") || !strcmp(tmp, "no")) return 0;
	if(!strcmp(tmp, "SI") || !strcmp(tmp, "si")) return 1;
	printf("ERRORE 002: parola immessa non valida\nReinseriscila per favore: ");
	comando=richiesta_si_no();
	return comando;
}

void nodo_vuoto(int indice_nodo, int indice_prec, int contatore){
	/*INFO
		 1.a
		 	 PARAMETRI DI INPUT:
		 	 	 puntatore a file
		 	 	 puntatore a struct tree
		 	 	 indice del nodo che si vuole creare vuoto
		 	 	 indice della domanda a cui si è arrivati (ftell(ftree)/sizeof(struct tree))
		 	 	 nodo->conta
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
		 	 	 nessuno
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

				(ftree, nodo, nodo->sx, ftell(ftree)/sizeof(struct tree)-1, nodo->conta)
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
	struct tree *nodo=malloc(sizeof(struct tree));

	FILE *ftree;

	nodo->pieno=0;
	nodo->foglia=0;
	nodo->conta=contatore+1;
	nodo->prec=indice_prec;
	nodo->sx=-500;
	nodo->dx=-500;
	strcpy(nodo->doma_risp, "");
	ftree=fopen("albero", "rb+");
	fseek (ftree, indice_nodo*sizeof(struct tree), SEEK_SET);
	fwrite(nodo, sizeof(struct tree), 1, ftree);
	fclose(ftree);
	free(nodo);
}

void sostituzione_nodo(int indice, int contatore){

	struct tree *nodo=malloc(sizeof(struct tree));
	FILE *ftree;
	char *mdl;
	char animale[51];
	int ind_sx, ind_dx;
	bool si_no;

	printf ("A quale animale avevi pensato (MAX 50 caratteri)? ");
	mdl=richiesta_stringa();
	strcpy(animale, mdl);
	free(mdl);
	/*strcpy(nodo->doma_risp, animale);//si potrebbe fare direttamente domarisp, mdl*/
	/*nodo->pieno=1;
	nodo->foglia=1;
	fwrite(nodo, sizeof(struct tree), 1, ftree);*/
	printf ("Quale domanda mi avrebbe aiutato ad identificarlo (MAX 50 caratteri)? ");
	mdl=punto_interrogativo(richiesta_stringa());
	strcpy(nodo->doma_risp, mdl);
	free(mdl);
	printf ("A questa domanda avresti risposto SI o NO? ");
	si_no=richiesta_si_no();
	ftree=fopen("albero", "rb+");
	fseek (ftree, 0, SEEK_END);//punta alla fine del file e quindi al suo primo indice libero
	nodo->sx=ftell(ftree)/sizeof(struct tree);
	nodo->dx=ftell(ftree)/sizeof(struct tree)+1;
	fclose(ftree);
	ind_sx=nodo->sx;//li salvo qui perchè poi verrano sovrascritti quando scriverò i nodi sx e dx
	ind_dx=nodo->dx;
	nodo->foglia=0;
	nodo->pieno=1;
	ftree=fopen("albero", "rb+");
	fseek (ftree, indice*sizeof(struct tree), SEEK_SET);//punta di nuovo all'indice del nodo iniziale
	fwrite(nodo, sizeof(struct tree), 1, ftree);
	fclose(ftree);
	free(nodo);
	if (si_no==0){//risposta NO
		sottonodi(ind_sx, ind_dx, animale, indice, contatore);}
	if (si_no==1){//risposta SI
		sottonodi(ind_dx, ind_sx, animale, indice, contatore);}
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

	struct tree *nodo=malloc(sizeof(struct tree));
	FILE *ftree;
	int i, contatore, posizione;

	printf("Domande precedenti:\n");
	posizione=indice;//ftell(ftree)/sizeof(struct tree);
	ftree=fopen("albero", "rb+");
	fseek(ftree, indice*sizeof(struct tree), SEEK_SET);
	fread(nodo, sizeof(struct tree), 1, ftree);
	fclose(ftree);
	contatore=nodo->conta;
	for (i=contatore-1; i>=0; i--){
		ftree=fopen("albero", "rb+");
		fseek(ftree, nodo->prec*sizeof(struct tree), SEEK_SET);
		fread(nodo, sizeof(struct tree), 1, ftree);
		printf("%d) %s: ",i+1,nodo->doma_risp);
		if(nodo->sx==posizione){
			printf("NO\n");
		}
		else if(nodo->dx==posizione){
			printf("SI\n");
		}
		posizione=ftell(ftree)/sizeof(struct tree)-1;//-1 perchè dopo fread il puntatore si sposta nel successivo indice
		fclose(ftree);
	}
	free(nodo);
}

int richiesta_comando(int indice){
	/*INFO
	 1.a
	 	 PARAMETRI DI INPUT:
	 	 	 nessuno
	 1.b
	 	 PARAMETRI DI OUTPUT:
	 	 	 restituisce:
	 	 	 0 se la risposta è no o NO
	 	 	 1 se la risposta è si o SI
	 	 	 2 se la risposta è comandi o COMANDI
	 	 	 3 se la risposta è indietro o INDIETRO
	 	 	 4 se la risposta è salva o SALVA
	 	 	 5 se la risposta è menu o MENU
	 	 	 6 se la risposta è esci o ESCI
	 	 	 ERRORE 002: parola immessa non valida\nReinseriscila per favore: in tutti gli altri casi e ricomincia la funzione
	 2
	 	 DESCRIZIONE:
	 	 	 controlla se la risposta inserita è si (o SI) oppure no (o NO)
	 2.a
	 	 DETTAGLI IMPLEMENTATIVI SALIENTI:
	 	 	 nessuno
	 2.b
	 	 CASI LIMITE:
	 	 	 nessuno
	 2.c
	 	 INPUT:
	 	 	si_no="cavolo"
	 	 OUTPUT
	 	 	ERRORE 002: parola immessa non valida\nReinseriscila per favore:
	 	 INPUT
	 	 	si_no="SI"
	 	 SITUAZIONE FINALE:
	 	 	return 1
	 3
	 	 AUTORI:
	 	 	 Francesco Laghetti
	 4
	 	 REVISIONE:
	 	 	 versione 1.0 18/05/2017
	 */
	char tmp[10];
	int comando;

	fgets(tmp, 10, stdin);//salva i primi 8 caratteri inseriti in tmp
	if(tmp[strlen(tmp)-1]=='\n'){//se l'ultimo carattere è il \n lo fa diventare \0. devo eliminare il \n perchè  se fossero stati inseriti esattamente 50 caratteri si avrebbe l'array [50char][\n][\0] e si avrebbe strlen(tmp)>50 anche se non è vero
		tmp[strlen(tmp)-1]='\0';
	}
	if(strlen(tmp)>8){//potrei scrivere semplicemente else perchè se \n è presente in tmp vuol dire che ci sono al massimo 5o caratteri
		while(getchar()!='\n');//assorbe tutti i caratteri rimasti nel buffer fino a quando non incontra \n
		printf("ERRORE 001: comando immesso non valido\nReinseriscilo per favore: ");
		comando=richiesta_comando(indice);
		return comando;
	}
	if(strspc(tmp)!=0){//la stringa inserita è vuota o composta solo da spazi e tabbature
		printf("ERRORE 001: comando immesso non valido\nReinseriscilo per favore: ");
		comando=richiesta_comando(indice);
		return comando;
	}


	if(!strcmp(tmp, "NO") || !strcmp(tmp, "no")) return 0;
	if(!strcmp(tmp, "SI") || !strcmp(tmp, "si")) return 1;
	if(!strcmp(tmp, "COMANDI") || !strcmp(tmp, "comandi")) return 2;
	if(!strcmp(tmp, "INDIETRO") || !strcmp(tmp, "indietro")) {
		if (indice==0){
			printf("ERRORE 003: impossibile tornare indietro alla prima domanda.\nInserisci un altro comando per favore: ");
			comando=richiesta_comando(indice);
			return comando;
		}
		else return 3;
	}
	if(!strcmp(tmp, "SALVA") || !strcmp(tmp, "salva")) return 4;
	if(!strcmp(tmp, "MENU") || !strcmp(tmp, "menu")) return 5;
	if(!strcmp(tmp, "ESCI") || !strcmp(tmp, "esci")) return 6;
	printf("ERRORE 001: comando immesso non valido\nReinseriscilo per favore: ");
	comando=richiesta_comando(indice);
	return comando;
}

void fine_partita(){

		int scelta;

		char tmp[3];//stringa dove effettuerò il controllo

		printf ("\n__________________________________________\n\nGRAZIE PER AVER GIOCATO!\n__________________________________________\n\nCosa vuoi fare ora?\n1)...NUOVA PARTITA\n2)...MENU PRINCIPALE\n3)...ESCI\n\nImmetti il numero corrispondente alla scelta che vuoi fare: ");
		fgets(tmp, 3, stdin);//salva i primi 2 caratteri inseriti in tmp
		if(tmp[strlen(tmp)-1]=='\n'){//se l'ultimo carattere è il \n lo fa diventare \0. devo eliminare il \n perchè  se fossero stati inseriti esattamente 50 caratteri si avrebbe l'array [50char][\n][\0] e si avrebbe strlen(tmp)>50 anche se non è vero
			tmp[strlen(tmp)-1]='\0';
		}
		if(strlen(tmp)>1){//potrei scrivere semplicemente else perchè se \n è presente in tmp vuol dire che ci sono al massimo 5o caratteri
			while(getchar()!='\n');//assorbe tutti i caratteri rimasti nel buffer fino a quando non incontra \n
			system("cls");
			printf ("__________________________________________\n\nErrore 001: comando immesso non valido.\n__________________________________________\n");
			fine_partita();
		}
		if(strspc(tmp)!=0){//la stringa inserita è vuota o composta solo da spazi e tabbature
			system("cls");
			printf ("__________________________________________\n\nErrore 001: comando immesso non valido.\n__________________________________________\n");
			fine_partita();
		}
		scelta=atoi(tmp);
		switch(scelta){
			case 1:
				system("cls");
				analisi_nodo(0);
				break;
			case 2:
				system("cls");
				menu_principale();
				break;
			case 3:
				system("cls");
				sequenza_di_chiusura();
				break;
			default:
				system("cls");
				printf ("__________________________________________\n\nErrore 001: comando immesso non valido.\n");
				fine_partita();}

}

/*void fine_partita(struct tree *nodo){
	bool si_no;
	printf ("Vuoi giocare ancora? ");
					si_no=richiesta_si_no();
					if (si_no==0){
						exit(1);
					}
					else if(si_no==1){
						analisi_nodo(nodo, 0);
					}
}*/

void sottonodi(int leaf, int empty, char* animale, int indice, int contatore){

	struct tree *nodo=malloc(sizeof(struct tree));
	FILE *ftree;

	nodo->prec=indice;
	nodo->sx=-500;
	nodo->dx=-500;
	nodo->pieno=1;
	nodo->foglia=1;
	nodo->conta=contatore+1;
	strcpy(nodo->doma_risp, animale);
	ftree=fopen("albero", "rb+");
	fseek (ftree, leaf*sizeof(struct tree), SEEK_SET); //si sposta sul nodo indicato da leaf ovvero il sottoalbero dove metterò la risposta
	fwrite(nodo, sizeof(struct tree), 1, ftree);
	fclose(ftree);
	nodo_vuoto(empty, indice, contatore);//si sposta sul nodo indicato da empty ovvero il sottoalbero che sarà vuoto

}

void lista_salvataggi (){

	struct salvataggio *salva=malloc(sizeof(struct salvataggio));
	FILE *fsave;
	int i;

	fsave=fopen("salvataggi", "rb+");
	for (i=0; i<10; i++){
	fseek(fsave, i*sizeof(struct salvataggio), SEEK_SET);
	//printf("prima di fread siamo all'indice %ld\n", ftell(fsave));
	fread(salva, sizeof(struct salvataggio), 1, fsave);
	//printf("dopo fread siamo all'indice %ld\n", ftell(fsave));
	//printf ("salva->pieno è %d e salva->risposta è %s\n", salva->pieno, salva->risposta);

	printf ("%c)...",ftell(fsave)/sizeof(struct salvataggio)+64);


	if (salva->pieno==0)
		printf ("SLOT_VUOTO\n");
	else
		printf ("%s\n",salva->risposta);}
	fclose(fsave);
	free(salva);

}

void crea_salvataggio(int indice, int provenienza){

	//struct tree *nodo=malloc(sizeof(struct tree));

	FILE *fsave;
	int n;
	char tmp[10];
	bool si_no;

	printf ("_________________________\n\nSCHERMATA DI SALVATAGGIO\n_________________________\n\n");
	lista_salvataggi ();
	struct salvataggio *salva=malloc(sizeof(struct salvataggio));
	printf ("\nscrivi INDIETRO per tornare indietro o scegli il salvataggio dove vuoi salvare con la lettera corrispondente: ");
	fgets(tmp, 10, stdin);//salva i primi 9 caratteri inseriti in tmp
	if(tmp[strlen(tmp)-1]=='\n'){//se l'ultimo carattere è il \n lo fa diventare \0. devo eliminare il \n perchè  se fossero stati inseriti esattamente 50 caratteri si avrebbe l'array [50char][\n][\0] e si avrebbe strlen(tmp)>50 anche se non è vero
		tmp[strlen(tmp)-1]='\0';
	}
	if(strlen(tmp)>8){//potrei scrivere semplicemente else perchè se \n è presente in tmp vuol dire che ci sono al massimo 5o caratteri
		while(getchar()!='\n');//assorbe tutti i caratteri rimasti nel buffer fino a quando non incontra \n
		system("cls");
		printf ("__________________________________________\n\nErrore 001: comando immesso non valido.\n__________________________________________\n");
		free(salva);
		crea_salvataggio(indice, provenienza);
	}
	if(strspc(tmp)!=0){//la stringa inserita è vuota o composta solo da spazi e tabbature
		system("cls");
		printf ("__________________________________________\n\nErrore 001: comando immesso non valido.\n__________________________________________\n");
		free(salva);
		crea_salvataggio(indice, provenienza);
	}
	if (strlen(tmp)==1){//inserisce un solo carattere
		//printf("strlen(tmp) è %d\n", strlen(tmp));
		//printf ("hai inserito %s\n", tmp);
		n = *tmp;
		//printf ("n è %d", n);
		if (n>=65 && n<=74){//codice ASCII per le lettere da A a J
			n=n-64;
		}
		else if (n>=97 && n<=106){//codice ASCII per le lettere da a a j
			n=n-96;
		}
		else {
			//system("cls");
			printf ("_________________________\n\nERRORE 001: COMANDO INSERITO NON VALIDO\n_________________________\n\n");
			free(salva);
			crea_salvataggio(indice, provenienza);
	}}
	else{//inserisce più di un carattere
		if(!strcmp(tmp, "INDIETRO") || !strcmp(tmp, "indietro")){
			if (provenienza==0){// da domanda
				system("cls");
				free(salva);
				analisi_nodo(indice);
			}
			if (provenienza==1){//da finale partita
				system("cls");
				free(salva);
				menu_principale();
			}
			if (provenienza==2){//da finale partita
				free(salva);
				sequenza_di_chiusura();
			}
		}
		else{
			system("cls");
			printf ("_________________________\n\nERRORE 001: COMANDO INSERITO NON VALIDO\n_________________________\n\n");
			free(salva);
			crea_salvataggio(indice, provenienza);}

	}
	//printf("che ho trasformato con strtol in %ld\n", n); // stampa 1 o 0
	/*fsave=fopen("salvataggi", "rb+");
	fseek(fsave, (n-1)*sizeof(struct salvataggio), SEEK_SET);
	fread(salva, sizeof(struct salvataggio), 1, fsave);
	close(fsave);*/
	/*if (salva->pieno==1){
		printf ("vuoi sovrascrivere il salvataggio presente nello slot %c? (Y/N)\n", n+64);
		a=getch();
		if (a==78){
			exit(1);
		}}*/

	//controllo sovrascrittura
	fsave=fopen("salvataggi", "rb+");
	fseek(fsave, (n-1)*sizeof(struct salvataggio), SEEK_SET);
	fread(salva, sizeof(struct salvataggio), 1, fsave);
	fclose(fsave);
	if(salva->pieno==1){//salvataggio occupato
		printf("\nil salvataggio selezionato è già occupato. Vuoi sovrascriverlo? ");
		si_no=richiesta_si_no();
		if(si_no==0){//NO alla sovrascrittura
			system("cls");
			free(salva);
			crea_salvataggio(indice, provenienza);
		}
	}

	printf("inserisci l'animale che avevi pensato: ");
	//COSTRETTO AD INSERIRE DOPPIO GETS PERCHè IL PRIMO NON LO VEDE. è SUCCESSO DOPO CHE HO INSERITO GETCHAR AL POST DI GETS PER tmp
	//gets(salva->risposta);
	gets(salva->risposta);
	//printf ("l'animale è %s\n", salva->risposta);
	salva->pieno=1;
	//printf ("salva->pieno è %d e salva->risposta è %s\n", salva->pieno, salva->risposta);
	salva->ind_nod=indice;
	fsave=fopen("salvataggi", "rb+");
	fseek(fsave, (n-1)*sizeof(struct salvataggio), SEEK_SET);
	//printf("prima di fwrite siamo all'indice %ld\n", ftell(fsave));
	fwrite(salva, sizeof(struct salvataggio), 1, fsave);
	//fseek(fsave, (n+1)*sizeof(struct salvataggio), SEEK_SET);
	//printf("dopo fwrite siamo all'indice %ld\n", ftell(fsave));
	//fseek(fsave, (n-1)*sizeof(struct salvataggio), SEEK_SET);
	//printf("prima di fread siamo all'indice %ld\n", ftell(fsave));
	//fread(salva, sizeof(struct salvataggio), 1, fsave);
	//printf("dopo fread siamo all'indice %ld\n", ftell(fsave));
	//printf ("ha letto che salva->pieno è %d e salva->risposta è %s\n", salva->pieno, salva->risposta);

	fclose(fsave);
	system("cls");
	printf ("_________________________\n\nsalvataggio avvenuto correttamente nello slot %c\n_________________________\n\n", n+64);
	//lista_salvataggi();
	//system("PAUSE");
	//getchar ();//elimina il \n lasciato da systemPAUSE
	free(salva);
	crea_salvataggio(indice, provenienza);
	system("PAUSE");
}

void elimina_salvataggio(){


	FILE *fsave;
	char a;
	int n;
	bool si_no;
	char tmp[10];

	printf ("_________________________\n\nSCHERMATA DI ELIMINAZIONE DEI SALVATAGGI\n_________________________\n\n");
	lista_salvataggi();
	struct salvataggio *salva=malloc(sizeof(struct salvataggio));
	printf ("\nscrivi INDIETRO per tornare indietro o scegli il salvataggio che vuoi cancellare con la lettera corrispondente: ");
	fgets(tmp, 10, stdin);//salva i primi 9 caratteri inseriti in tmp
	if(tmp[strlen(tmp)-1]=='\n'){//se l'ultimo carattere è il \n lo fa diventare \0. devo eliminare il \n perchè  se fossero stati inseriti esattamente 50 caratteri si avrebbe l'array [50char][\n][\0] e si avrebbe strlen(tmp)>50 anche se non è vero
		tmp[strlen(tmp)-1]='\0';
	}
	if(strlen(tmp)>8){//potrei scrivere semplicemente else perchè se \n è presente in tmp vuol dire che ci sono al massimo 5o caratteri
		while(getchar()!='\n');//assorbe tutti i caratteri rimasti nel buffer fino a quando non incontra \n
		system("cls");
		printf ("__________________________________________\n\nErrore 001: comando immesso non valido.\n__________________________________________\n");
		free(salva);
		elimina_salvataggio();
	}
	if(strspc(tmp)!=0){//la stringa inserita è vuota o composta solo da spazi e tabbature
		system("cls");
		printf ("__________________________________________\n\nErrore 001: comando immesso non valido.\n__________________________________________\n");
		free(salva);
		elimina_salvataggio();
	}
		if (strlen(tmp)==1){//inserisce un solo carattere
			//printf("strlen(tmp) è %d\n", strlen(tmp));
			//printf ("hai inserito %s\n", tmp);
			n = *tmp;
			//printf ("n è %d", n);
			if (n>=65 && n<=74){//codice ASCII per le lettere da A a J
				n=n-64;
			}
			else if (n>=97 && n<=106){//codice ASCII per le lettere da a a j
				n=n-96;
			}
			else {
				system("cls");
				printf ("_________________________\n\nERRORE 001: COMANDO INSERITO NON VALIDO\n_________________________\n\n");
				free(salva);
				elimina_salvataggio();
		}}
		else{//inserisce più di un carattere
			if(!strcmp(tmp, "INDIETRO") || !strcmp(tmp, "indietro")){
				system("cls");
				free(salva);
				menu_principale();}
			else{
				system("cls");
				printf ("_________________________\n\nERRORE 001: COMANDO INSERITO NON VALIDO\n_________________________\n\n");
				free(salva);
				elimina_salvataggio();

		}}
	//printf("che ho trasformato con strtol in %ld\n", n); // stampa 1 o 0
	/*fsave=fopen("salvataggi", "rb+");
	fseek(fsave, (n-1)*sizeof(struct salvataggio), SEEK_SET);
	fread(salva, sizeof(struct salvataggio), 1, fsave);
	close(fsave);*/
	/*if (salva->pieno==1){
		printf ("vuoi sovrascrivere il salvataggio presente nello slot %c? (Y/N)\n", n+64);
		a=getch();
		if (a==78){
			exit(1);
		}}*/

		fsave=fopen("salvataggi", "rb+");
		fseek(fsave, (n-1)*sizeof(struct salvataggio), SEEK_SET);
		fread(salva, sizeof(struct salvataggio), 1, fsave);
		fclose(fsave);
		if(salva->pieno==1){//salvataggio occupato
			printf("\nSei sicuro di voler eliminare il salvataggio nello slot %c? ", n+64);
			si_no=richiesta_si_no();
			if(si_no==0){//NO alla sovrascrittura
				system("cls");
				free(salva);
				elimina_salvataggio();
			}
			else{
				salva->pieno=0;
				//printf ("salva->pieno è %d e salva->risposta è %s\n", salva->pieno, salva->risposta);
				fsave=fopen("salvataggi", "rb+");
				fseek(fsave, (n-1)*sizeof(struct salvataggio), SEEK_SET);
				//printf("prima di fwrite siamo all'indice %ld\n", ftell(fsave));
				fwrite(salva, sizeof(struct salvataggio), 1, fsave);
						//fseek(fsave, (n+1)*sizeof(struct salvataggio), SEEK_SET);
						//printf("dopo fwrite siamo all'indice %ld\n", ftell(fsave));
				fseek(fsave, (n-1)*sizeof(struct salvataggio), SEEK_SET);
						//printf("prima di fread siamo all'indice %ld\n", ftell(fsave));
				fread(salva, sizeof(struct salvataggio), 1, fsave);
						//printf("dopo fread siamo all'indice %ld\n", ftell(fsave));
						//printf ("ha letto che salva->pieno è %d e salva->risposta è %s\n", salva->pieno, salva->risposta);

				fclose(fsave);
				system("cls");
				printf ("_________________________\n\neliminazione del salvataggio avvenuta correttamente nello slot %c\n_________________________\n\n", n+64);
						//lista_salvataggi();
						//system("PAUSE");
						//getchar ();//elimina il \n lasciato da systemPAUSE
				free(salva);
				elimina_salvataggio();
				system("PAUSE");
			}
		}
		else{//salvataggio vuoto
			system("cls");
			printf ("_________________________\n\nnon c'è alcun salvataggio da eliminare nello slot %c\n_________________________\n\n", n+64);
			free(salva);
			elimina_salvataggio();
		}


}

void menu_principale (){

	int scelta;
	char tmp[3];//stringa dove effettuerò il controllo

	printf ("_________________________\n\nBENVENUTO NEL GIOCO DELLE 20 DOMANDE\n_________________________\n\n1)...NUOVA PARTITA\n2)...CARICA PARTITA\n3)...ELIMINA SALVATAGGIO\n4)...REGOLE E COMANDI\n5)...ESCI\n\nImmetti il numero corrispondente alla scelta che vuoi fare: ");
	fgets(tmp, 3, stdin);//salva i primi 2 caratteri inseriti in tmp
	if(tmp[strlen(tmp)-1]=='\n'){//se l'ultimo carattere è il \n lo fa diventare \0. devo eliminare il \n perchè  se fossero stati inseriti esattamente 50 caratteri si avrebbe l'array [50char][\n][\0] e si avrebbe strlen(tmp)>50 anche se non è vero
		tmp[strlen(tmp)-1]='\0';
	}
	if(strlen(tmp)>1){//potrei scrivere semplicemente else perchè se \n è presente in tmp vuol dire che ci sono al massimo 5o caratteri
		while(getchar()!='\n');//assorbe tutti i caratteri rimasti nel buffer fino a quando non incontra \n
		system("cls");
		printf ("__________________________________________\n\nErrore 001: comando immesso non valido.\n__________________________________________\n");
		menu_principale();
	}
	if(strspc(tmp)!=0){//la stringa inserita è vuota o composta solo da spazi e tabbature
		system("cls");
		printf ("__________________________________________\n\nErrore 001: comando immesso non valido.\n__________________________________________\n");
		menu_principale();
	}
	scelta=atoi(tmp);
	switch(scelta){
		case 1:
			system("cls");
			analisi_nodo(0);
			break;
		case 2:
			system("cls");
			carica_salvataggio();
			break;
		case 3:
			system("cls");
			elimina_salvataggio();
			break;
		case 4:
			system("cls");
			schermata_di_spiegazione(1, -500);
			break;
		case 5:
			system("cls");
			sequenza_di_chiusura();
			break;
		default:
			system("cls");
			printf ("__________________________________________\n\nErrore 001: comando immesso non valido.\n");
			menu_principale();}

	/*scelta_menu=getchar();//prima c'era solo getch() senza il getchar() nella riga dopo
	getchar();
		if (scelta_menu==49){
		  	system("cls");
			analisi_nodo(nodo, salva, 0);}
		else if (scelta_menu==50){
			system("cls");
			carica_salvataggio(salva, nodo);}
		else if (scelta_menu==51){
			system("cls");
			elimina_salvataggio(salva, nodo);}
		else if (scelta_menu==52){
			system("cls");
			schermata_di_spiegazione(nodo, salva, 1, -500);}
		else if (scelta_menu==53){
			system("cls");
			sequenza_di_chiusura();}
		else {system("cls");
			printf ("__________________________________________\n\nErrore 001: comando immesso non valido.\n");
			menu_principale(nodo, salva);}*/

}

void schermata_di_spiegazione (int provenienza, int indice){

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
	getch();
	if (provenienza == 1){
		system("cls");
		menu_principale();}
	else
		system("cls");
		analisi_nodo(indice);
}

void sequenza_di_chiusura(){

	printf("__________________________________________\n\nGrazie per aver giocato. Spero tu ti sia divertito. Alla prossima!\n__________________________________________");
	getch();
	exit(1); //termina il programma
}

void carica_salvataggio(){


	FILE *fsave;
	int n;
	char tmp[10];//se usavo *tmp non funzionava
	printf ("_________________________\n\nSCHERMATA DI CARICAMENTO DEI SALVATAGGI\n_________________________\n\n");
	lista_salvataggi ();

	struct salvataggio *salva=malloc(sizeof(struct salvataggio));

	printf ("\nscrivi INDIETRO per tornare indietro o scegli il salvataggio che vuoi caricare con la lettera corrispondente: ");
	fgets(tmp, 10, stdin);//salva i primi 9 caratteri inseriti in tmp
	if(tmp[strlen(tmp)-1]=='\n'){//se l'ultimo carattere è il \n lo fa diventare \0. devo eliminare il \n perchè  se fossero stati inseriti esattamente 50 caratteri si avrebbe l'array [50char][\n][\0] e si avrebbe strlen(tmp)>50 anche se non è vero
		tmp[strlen(tmp)-1]='\0';
	}
	if(strlen(tmp)>8){//potrei scrivere semplicemente else perchè se \n è presente in tmp vuol dire che ci sono al massimo 5o caratteri
		while(getchar()!='\n');//assorbe tutti i caratteri rimasti nel buffer fino a quando non incontra \n
		system("cls");
		printf ("__________________________________________\n\nErrore 001: comando immesso non valido.\n__________________________________________\n");
		free(salva);
		carica_salvataggio();
	}
	if(strspc(tmp)!=0){//la stringa inserita è vuota o composta solo da spazi e tabbature
		system("cls");
		printf ("__________________________________________\n\nErrore 001: comando immesso non valido.\n__________________________________________\n");
		free(salva);
		carica_salvataggio();
	}
	if (strlen(tmp)==1){//inserisce un solo carattere
		//printf("strlen(tmp) è %d\n", strlen(tmp));
		//printf ("hai inserito %s\n", tmp);
		n = *tmp;
		//printf ("n è %d", n);
		if (n>=65 && n<=74){//codice ASCII per le lettere da A a J
			n=n-64;
		}
		else if (n>=97 && n<=106){//codice ASCII per le lettere da a a j
			n=n-96;
		}
		else {
			//system("cls");
			printf ("_________________________\n\nERRORE 001: COMANDO INSERITO NON VALIDO\n_________________________\n\n");
			free(salva);
			carica_salvataggio();
	}}
	else{//inserisce più di un carattere
		if(!strcmp(tmp, "INDIETRO") || !strcmp(tmp, "indietro")){
			system("cls");
			free(salva);
			menu_principale();
		}
		else{
			system("cls");
			printf ("_________________________\n\nERRORE 001: COMANDO INSERITO NON VALIDO\n_________________________\n\n");
			free(salva);
			carica_salvataggio();}

	}
	//printf("che ho trasformato con strtol in %ld\n", n); // stampa 1 o 0
	/*fsave=fopen("salvataggi", "rb+");
	fseek(fsave, (n-1)*sizeof(struct salvataggio), SEEK_SET);
	fread(salva, sizeof(struct salvataggio), 1, fsave);
	close(fsave);*/
	/*if (salva->pieno==1){
		printf ("vuoi sovrascrivere il salvataggio presente nello slot %c? (Y/N)\n", n+64);
		a=getch();
		if (a==78){
			exit(1);
		}}*/






	fsave=fopen("salvataggi", "rb+");
	fseek(fsave, (n-1)*sizeof(struct salvataggio), SEEK_SET);
	//printf("prima di fwrite siamo all'indice %ld\n", ftell(fsave));
	fread(salva, sizeof(struct salvataggio), 1, fsave);
	//fseek(fsave, (n+1)*sizeof(struct salvataggio), SEEK_SET);
	//printf("dopo fwrite siamo all'indice %ld\n", ftell(fsave));
	//fseek(fsave, (n-1)*sizeof(struct salvataggio), SEEK_SET);
	//printf("prima di fread siamo all'indice %ld\n", ftell(fsave));
	//fread(salva, sizeof(struct salvataggio), 1, fsave);
	//printf("dopo fread siamo all'indice %ld\n", ftell(fsave));
	//printf ("ha letto che salva->pieno è %d e salva->risposta è %s\n", salva->pieno, salva->risposta);

	fclose(fsave);
	if(salva->pieno==0){
		system("cls");
		printf("_________________________\n\nIl salvataggio scelto è vuoto. Selezionane un altro o torna INDIETRO.\n_________________________\n\n");
		free(salva);
		carica_salvataggio();
	}
	system("cls");
	printf ("_________________________\n\nHai deciso di caricare il salvataggio dello slot %c\nL'animale che avevi pensato per questa partita era %s\n_________________________\n\n", n+64, salva->risposta);
	//lista_salvataggi();
	//system("PAUSE");
	//getchar ();//elimina il \n lasciato da systemPAUSE
	free(salva);
	analisi_nodo(salva->ind_nod);
	//system("PAUSE");
}

char* punto_interrogativo(char *str){

	int c;
	bool punto_int=0;

	c=(int) strlen(str);
	//printf("TT BN. strlen = %d\n", c);
	for(c=(int) strlen(str)-1; c>=0; c--){
		//printf("carattere %d\n", c);
		if(punto_int==0){
			//printf("punto_int==0\n");
			//printf("|%c|", str[c]);
			if(isspace(str[c])!=0){
				//printf("spazio\n");
				str[c]=0;//'\0'
			}
			else if(str[c]=='?'){//è il primo ? che incontra
				//printf("punto interrogativo\n");
				punto_int=1;
			}
			else{
				//printf("carattere normale\n");
				str[c+1]=63;//se non ha incontrato ? fino ad allora lo mette al carattere prima
				str[c+2]=0;//'\0'
				c=-1;//esce per condizione del for
			}}
		else if(punto_int==1){
			//printf("punto_int==1\n");
			//printf("|%c|", str[c]);
			if(isspace(str[c])!=0){
				//printf("spazio\n");
				str[c]=63;//se ha già incontrato ? allora lo mette al posto dello spazio
				str[c+1]=0;//'\0'
			}
			else if(str[c]=='?'){
				//printf("punto interrogativo\n");
				str[c+1]=0;//'\0'
			}
			else{
				//printf("carattere normale\n");
				c=-1;//esce per condizione del for
			}
		}
	}
	//printf("%s", str);
	return str;
}



