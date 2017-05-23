/*
 ============================================================================
 Name        : finale02.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>



struct tree{
	char doma_risp[51];
	int prec;
	int sx;
	int dx;
	int conta; //conta il numero della domanda per sapere se è arrivato a 20;
	bool foglia;//if 0 nodo, if 1 foglia
	bool pieno;//if 0 vuoto, if 1 pieno
};

struct salvataggio{
	char risposta[50];
	bool pieno;
	int ind_nod;
};

void analisi_nodo(struct tree*, struct salvataggio*, int);
char *richiesta_stringa();//VERIFICATO
int strspc(char*);//VERIFICATO
bool richiesta_si_no();//VERIFICATO
void nodo_vuoto(struct tree*, int, int, int);//VERIFICATO
void sostituzione_nodo(struct tree*, int, int);
void lista_risposte(struct tree*, int/*, int*/);//VERIFICATO
int richiesta_comando(int indice);
void fine_partita(struct tree*, struct salvataggio*);
void sottonodi(struct tree*, int, int, char*, int, int);
void lista_salvataggi (struct salvataggio*);
void elimina_salvataggio(struct salvataggio*, struct tree*);
void crea_salvataggio(struct salvataggio*, struct tree*, int, int);
void menu_principale (struct tree*, struct salvataggio*);
void schermata_di_spiegazione (struct tree*, struct salvataggio*, int, int);
void sequenza_di_chiusura();
void carica_salvataggio(struct salvataggio*, struct tree*);

int main (){
	setbuf(stdout, NULL);
	struct tree *nodo=malloc(sizeof(struct tree));
	struct salvataggio *salva=malloc(sizeof(struct salvataggio));

	FILE *ftree;
	/*int i;
	salva->pieno=1;
	salva->ind_nod=20;
	strcpy(salva->risposta, "cavallo");
	fsave=fopen("salvataggi", "rb+");
	for (i=0; i<10; i++){
		fseek(fsave, i*sizeof(struct salvataggio), SEEK_SET);
		fwrite(salva, sizeof(struct salvataggio), 1, fsave);
		fseek(fsave, i*sizeof(struct salvataggio), SEEK_SET);
		fread(salva, sizeof(struct salvataggio), 1, fsave);
		printf("%d)%d...%s...%d\n",ftell(fsave)/sizeof(struct salvataggio)-1, salva->pieno, salva->risposta, salva->ind_nod);
	}
	system("PAUSE");*/

	ftree=fopen("albero", "ab");
	fclose(ftree);

	menu_principale(nodo, salva);
}

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

void analisi_nodo(struct tree *nodo, struct salvataggio *salva, int indice){


	char *mdl;
	char animale[51];
	int i, ind_sx, ind_dx, comando;
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
				nodo->pieno=1;
				nodo->foglia=1;
				ftree=fopen("albero", "rb+");
				fseek (ftree, indice*sizeof(struct tree), SEEK_SET);
				fwrite(nodo, sizeof(struct tree), 1, ftree);
				fclose(ftree);
				//procedura di fine partita(gioca,menu,esci)
				fine_partita(nodo, salva);
		}
		else{//ovvero non abbiamo ancora posto 20 domande
			lista_risposte(nodo, indice);//per ricordare all'utente quali domande sono già state fatte così non ne iserisce una uguale
			printf("\n");
			ftree=fopen("albero", "rb+");
			fseek (ftree, indice*sizeof(struct tree), SEEK_SET);
			fread(nodo, sizeof(struct tree), 1, ftree);//per riportare il nodo conta al valore giusto
			fclose(ftree);
			sostituzione_nodo(nodo, indice, nodo->conta/*contatore*/);}
		//procedura di fine partita(gioca,menu,esci)
		fine_partita(nodo, salva);
		}
	else{//nodo pieno
		if (nodo->foglia==1){//ci troviamo in una foglia
			printf("____________________\n\nPROVO AD INDOVINARE!\n____________________\n\n");
			printf ("Stavi pensando a %s?\nRispondi con SI o NO: ", nodo->doma_risp);
			si_no=richiesta_si_no();
			if(si_no==1){//INDOVINATO
				printf("____________________\n\nHO VINTO! HO INDOVINATO!\n____________________\n\n");
				//procedura di fine partita(gioca,menu,esci)
				fine_partita(nodo, salva);
			}
			else{//NON INDOVINATO
				printf("____________________\n\nOH NO! NON HO INDOVINATO!\n____________________\n\n");
				if(nodo->conta==20){//ovvero siamo oltre la 20esima domanda, cioè siamo nell'ultimo livello dell'albero dove sono presenti solo foglie
					//volendo si può chiedere a quale animale pensasse ma ai fini del software è un dato che andrebbe perso
					//procedura di fine partita(gioca,menu,esci)//non si richiede nient'altro perchè si possono fare massimo 20 domande
					fine_partita(nodo, salva);
				}
				else{//non si sono ancora poste 20 domande
					lista_risposte(nodo, indice);//per ricordare all'utente quali domande sono già state fatte così non ne iserisce una uguale
					printf("\n");
					ftree=fopen("albero", "rb+");
					fseek (ftree, indice*sizeof(struct tree), SEEK_SET);
					fread(nodo, sizeof(struct tree), 1, ftree);//per riportare il nodo conta al valore giusto
					fclose(ftree);
					sostituzione_nodo(nodo, indice, nodo->conta/*contatore*/);
					fine_partita(nodo, salva);}
				}
			}
		else{//ci troviamo in un nodo interno
			//system("cls");
			printf("____________________\n\nDOMANDA %d\nRICORDA: inserisci SI o NO per rispondere o uno degli altri 7 comandi\nSe non li ricordi inserisci COMANDI\n____________________\n\n", nodo->conta+1);
			lista_risposte(nodo, indice);
			ftree=fopen("albero", "rb+");
			fseek (ftree, indice*sizeof(struct tree), SEEK_SET);
			fread(nodo, sizeof(struct tree), 1, ftree);
			fclose(ftree);
			printf("\nDOMANDA: %s\nInserisci la risposta o il comando: ", nodo->doma_risp);
			comando=richiesta_comando(indice);
			if(comando==0){//risposta NO
				system("cls");
				analisi_nodo(nodo, salva, nodo->sx);
			}
			else if(comando==1){//risposta SI
				system("cls");
				analisi_nodo(nodo, salva, nodo->dx);
			}
			else if(comando==2){//risposta COMANDI
				schermata_di_spiegazione (nodo, salva, 0, indice);
			}
			else if(comando==3){//risposta INDIETRO
				system("cls");
				analisi_nodo(nodo, salva, nodo->prec);
			}
			else if(comando==4){//risposta SALVA
				system("cls");
				crea_salvataggio(salva, nodo, indice, 0);
			}
			else if(comando==5){//risposta MENU
				printf("Sei sicuro di voler tornare al menù? ");
				si_no=richiesta_si_no();
				if(si_no==1){
					printf("Vuoi salvare prima di tornare al menù? ");
					si_no=richiesta_si_no();
					if(si_no==0){
						menu_principale(nodo, salva);
					}
					if(si_no==1){
						system("cls");
						crea_salvataggio(salva, nodo, indice, 1);
					}
				}
				else {
					analisi_nodo(nodo, salva, indice);}
			}
			else if(comando==6){//risposta ESCI
				printf("Sei sicuro di voler uscire? ");
				si_no=richiesta_si_no();
				if(si_no==1){
					printf("Vuoi salvare prima di uscire? ");
					si_no=richiesta_si_no();
					if(si_no==0){
						sequenza_di_chiusura();
					}
					if(si_no==1){
						system("cls");
						crea_salvataggio(salva, nodo, indice, 2);
					}
				}
				else{
					analisi_nodo(nodo, salva, indice);}
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

	gets(risp);
	if(strspc(risp)!=0){
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
	char si_no[3];
	bool y_n;

	gets(si_no);
	if(!strcmp(si_no, "NO") || !strcmp(si_no, "no")) return 0;
	if(!strcmp(si_no, "SI") || !strcmp(si_no, "si")) return 1;
	printf("ERRORE 002: parola immessa non valida\nReinseriscila per favore: ");
	y_n=richiesta_si_no();
	return y_n;
}

void nodo_vuoto(struct tree *nodo, int indice_nodo, int indice_prec, int contatore){
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
}

void sostituzione_nodo(struct tree *nodo, int indice, int contatore){
	FILE *ftree;
	char *mdl;
	char animale[51];
	int ind_sx, ind_dx;
	bool si_no;

	printf ("A quale animale avevi pensato (MAX 50 caratteri)? ");
	mdl=richiesta_stringa();
	strcpy(animale, mdl);
	/*strcpy(nodo->doma_risp, animale);//si potrebbe fare direttamente domarisp, mdl*/
	/*nodo->pieno=1;
	nodo->foglia=1;
	fwrite(nodo, sizeof(struct tree), 1, ftree);*/
	printf ("Quale domanda mi avrebbe aiutato ad identificarlo (MAX 50 caratteri)? ");
	mdl=richiesta_stringa();
	strcpy(nodo->doma_risp, mdl);
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
	if (si_no==0){//risposta NO
		sottonodi(nodo, ind_sx, ind_dx, animale, indice, contatore);}
	if (si_no==1){//risposta SI
		sottonodi(nodo, ind_dx, ind_sx, animale, indice, contatore);}
}

void lista_risposte(struct tree *nodo, int indice/*, int contatore*/){
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
	char si_no[9];
	int comando;

	gets(si_no);
	if(!strcmp(si_no, "NO") || !strcmp(si_no, "no")) return 0;
	if(!strcmp(si_no, "SI") || !strcmp(si_no, "si")) return 1;
	if(!strcmp(si_no, "COMANDI") || !strcmp(si_no, "comandi")) return 2;
	if(!strcmp(si_no, "INDIETRO") || !strcmp(si_no, "indietro")) {
		if (indice==0){
			printf("ERRORE 003: impossibile tornare indietro alla prima domanda.\nInserisci un altro comando per favore: ");
			comando=richiesta_comando(indice);
			return comando;
		}
		else return 3;
	}
	if(!strcmp(si_no, "SALVA") || !strcmp(si_no, "salva")) return 4;
	if(!strcmp(si_no, "MENU") || !strcmp(si_no, "menu")) return 5;
	if(!strcmp(si_no, "ESCI") || !strcmp(si_no, "esci")) return 6;
	printf("ERRORE 002: parola immessa non valida\nReinseriscila per favore: ");
	comando=richiesta_comando(indice);
	return comando;
}

void fine_partita(struct tree *nodo, struct salvataggio *salva){

		char scelta_menu;

		//system("cls"); da vedere se mettere perchè elimina il messaggio di resoconto della partita
		printf ("\n__________________________________________\n\nGRAZIE PER AVER GIOCATO!\n__________________________________________\n\nCosa vuoi fare ora?\n1)...NUOVA PARTITA\n2)...MENU PRINCIPALE\n3)...ESCI\n\nImmetti il numero corrispondente alla scelta che vuoi fare: ");
		scelta_menu=getchar();//forse devo inserire getchar per il \n
		getchar();
			if (scelta_menu==49){//scelta 1 nuova partita
			  	system("cls");
			  	analisi_nodo(nodo, salva, 0);}
			else if (scelta_menu==50){//scelta 2 menu
				system("cls");
				menu_principale(nodo,salva);}
			else if (scelta_menu==51){//scelta 3 esci
				system("cls");
				sequenza_di_chiusura();}
			else {system("cls");
				printf ("__________________________________________\n\nErrore 001: comando immesso non valido.\n");
				fine_partita(nodo, salva);}


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

void sottonodi(struct tree *nodo, int leaf, int empty, char* animale, int indice, int contatore){
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
	nodo_vuoto(nodo, empty, indice, contatore);//si sposta sul nodo indicato da empty ovvero il sottoalbero che sarà vuoto

}

void lista_salvataggi (struct salvataggio *salva){

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

}

void crea_salvataggio(struct salvataggio *salva, struct tree *nodo, int indice, int provenienza){

	FILE *fsave;
	int n;
	char tmp[9];
	bool si_no;
	printf ("_________________________\n\nSCHERMATA DI SALVATAGGIO\n_________________________\n\n");
	lista_salvataggi (salva);
	printf ("\nscrivi INDIETRO per tornare indietro o scegli il salvataggio dove vuoi salvare con la lettera corrispondente: ");
	gets(tmp); //elimina il \n lasciato da getchar
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
			crea_salvataggio(salva, nodo, indice, provenienza);
	}}
	else{//inserisce più di un carattere
		if(!strcmp(tmp, "INDIETRO") || !strcmp(tmp, "indietro")){
			if (provenienza==0){// da domanda
				system("cls");
				analisi_nodo(nodo, salva, indice);
			}
			if (provenienza==1){//da finale partita
				menu_principale(nodo, salva);
			}
			if (provenienza==2){//da finale partita
				sequenza_di_chiusura();
			}
		}
		else{
			system("cls");
			printf ("_________________________\n\nERRORE 001: COMANDO INSERITO NON VALIDO\n_________________________\n\n");
			crea_salvataggio(salva, nodo, indice, provenienza);}

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
			crea_salvataggio(salva, nodo, indice, provenienza);
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
	crea_salvataggio(salva, nodo, indice, provenienza);
	system("PAUSE");
}

void elimina_salvataggio(struct salvataggio *salva, struct tree *nodo){
	FILE *fsave;

	char a;
	int n;
	bool si_no;
	char tmp[9];

	printf ("_________________________\n\nSCHERMATA DI ELIMINAZIONE DEI SALVATAGGI\n_________________________\n\n");
	lista_salvataggi(salva);
	printf ("\nscrivi INDIETRO per tornare indietro o scegli il salvataggio che vuoi cancellare con la lettera corrispondente: ");
	gets(tmp); //elimina il \n lasciato da getchar
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
				elimina_salvataggio(salva, nodo);
		}}
		else{//inserisce più di un carattere
			if(!strcmp(tmp, "INDIETRO") || !strcmp(tmp, "indietro")){
				menu_principale(nodo, salva);}
			else{
				system("cls");
				printf ("_________________________\n\nERRORE 001: COMANDO INSERITO NON VALIDO\n_________________________\n\n");
				elimina_salvataggio(salva, nodo);

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
				elimina_salvataggio(salva, nodo);
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
				elimina_salvataggio(salva, nodo);
				system("PAUSE");
			}
		}
		else{//salvataggio vuoto
			system("cls");
			printf ("_________________________\n\nnon c'è alcun salvataggio da eliminare nello slot %c\n_________________________\n\n", n+64);
			elimina_salvataggio(salva, nodo);
		}


}

void menu_principale (struct tree *nodo, struct salvataggio *salva){


	char scelta_menu;

	system("cls");
	printf ("_________________________\n\nBENVENUTO NEL GIOCO DELLE 20 DOMANDE\n_________________________\n\n1)...NUOVA PARTITA\n2)...CARICA PARTITA\n3)...ELIMINA SALVATAGGIO\n4)...REGOLE E COMANDI\n5)...ESCI\n\nImmetti il numero corrispondente alla scelta che vuoi fare: ");
	scelta_menu=getchar();//prima c'era solo getch() senza il getchar() nella riga dopo
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
			menu_principale(nodo, salva);}

}

void schermata_di_spiegazione (struct tree* nodo, struct salvataggio *salva, int provenienza, int indice){

	system("cls");
	printf("__________________________________________\n\nREGOLE E COMANDI\n\nQuesto gioco si pone l’obbiettivo di sfidare il giocatore nel riuscire ad indovinare un qualsiasi animale da lui pensato in massimo 20 domande.\n"
			"Per cominciare non devi far altro che selezionare NUOVA PARTITA  oppure su CARICA PARTITA se vuoi riprendere una partita lasciata a metà.\n"
			"NOTA BENE: ricorda l’animale pensato per tutta la durata della partita!\n\n"
			"Durante la partita, avrai a disposizione ben 7 comandi:\n"
			"-SI: per rispondere affermativamente alla domanda che ti viene posta.\n"
			"-NO: per rispondere negativamente alla domanda che ti viene posta.\n"
			"-COMANDI: per ritornare in questa schermata.\n"
			"-BACK: per tornare alla domanda precedente ed eventualmente dare una risposta diversa (NON puoi usare questo comando alla prima domanda!).\n"
			"-SALVA: ti permetterà di salvare la partita in corso. "
			"Ti verrà chiesto di inserire il nome dell’animale a cui stai pensando così potrai ricordartelo quando caricherai il salvataggio.\n"
			"-MENU: ti fa tornare al menù principale.\n"
			"-EXIT: ti fa uscire dal gioco.\n\n"
			"NOTA BENE: i comandi potrai scriverli solo tutto in maiuscolo o tutto in minuscolo!\n\n"
			"Premi un tasto qualsiasi per uscire da questa schermata.\n");
	getch();
	if (provenienza == 1){
		system("cls");
		menu_principale(nodo, salva);}
	else
		system("cls");
		analisi_nodo(nodo, salva, indice);
}

void sequenza_di_chiusura(){

	printf("__________________________________________\n\nGrazie per aver giocato. Spero tu ti sia divertito. Alla prossima!\n__________________________________________");
	getch();
	exit(1); //termina il programma
}

void carica_salvataggio(struct salvataggio *salva, struct tree *nodo){

	FILE *fsave;
	int n;
	char tmp[9];//se usavo *tmp non funzionava
	printf ("_________________________\n\nSCHERMATA DI CARICAMENTO DEI SALVATAGGI\n_________________________\n\n");
	lista_salvataggi (salva);
	printf ("\nscrivi INDIETRO per tornare indietro o scegli il salvataggio che vuoi caricare con la lettera corrispondente: ");
	gets(tmp); //elimina il \n lasciato da getchar
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
			carica_salvataggio(salva, nodo);
	}}
	else{//inserisce più di un carattere
		if(!strcmp(tmp, "INDIETRO") || !strcmp(tmp, "indietro")){
			menu_principale(nodo, salva);
		}
		else{
			system("cls");
			printf ("_________________________\n\nERRORE 001: COMANDO INSERITO NON VALIDO\n_________________________\n\n");
			carica_salvataggio(salva, nodo);}

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
		carica_salvataggio(salva, nodo);
	}
	system("cls");
	printf ("_________________________\n\nHai deciso di caricare il salvataggio dello slot %c\nL'animale che avevi pensato per questa partita era %s\n_________________________\n\n", n+64, salva->risposta);
	//lista_salvataggi();
	//system("PAUSE");
	//getchar ();//elimina il \n lasciato da systemPAUSE
	analisi_nodo(nodo, salva, salva->ind_nod);
	system("PAUSE");
}
