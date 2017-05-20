/*
 ============================================================================
 Name        : finale01.c
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


void analisi_nodo(FILE*, struct tree*, int, int);
char *richiesta_stringa();//VERIFICATO
int strspc(char*);//VERIFICATO
bool richiesta_si_no();//VERIFICATO
void nodo_vuoto(FILE*, struct tree*, int, int, int);//VERIFICATO
void sostituzione_nodo(FILE *, struct tree*, int, int);
void lista_risposte(FILE *, struct tree*, int/*, int*/);//VERIFICATO

int main (){
	setbuf(stdout, NULL);
	struct tree *nodo=malloc(sizeof(struct tree));
	FILE *ftree;

	nodo->pieno=0;
	nodo->foglia=0;
	nodo->conta=0;
	nodo->prec=-500;
	nodo->sx=-500;
	nodo->dx=-500;
	strcpy(nodo->doma_risp, "");

	ftree=fopen("albero", "rb+");
	analisi_nodo(ftree, nodo, 0, 0);
	fclose(ftree);

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

void analisi_nodo(FILE *ftree, struct tree *nodo, int indice, int contatore){

	char *mdl;
	char animale[51];
	int i, ind_sx, ind_dx;
	bool si_no;

	fseek (ftree, indice*sizeof(struct tree), SEEK_SET);
	fread(nodo, sizeof(struct tree), 1, ftree);

	if (nodo->pieno==0){//NODO VUOTO
		printf("____________________\n\nMI HAI SCONFITTO! MI ARRENDO!\n____________________\n\n");
		if(nodo->conta==20){//ovvero siamo oltre la 20esima domanda, cioè siamo nell'ultimo livello dell'albero dove sono presenti solo foglie
				printf ("A quale animale avevi pensato (MAX 50 caratteri)? ");
				mdl=richiesta_stringa();
				strcpy(animale, mdl);
				strcpy(nodo->doma_risp, animale);
				nodo->pieno=1;
				nodo->foglia=1;
				fwrite(nodo, sizeof(struct tree), 1, ftree);
				//procedura di fine partita(gioca,menu,esci)
		}
		else{//ovvero non abbiamo ancora posto 20 domande
			sostituzione_nodo(ftree, nodo, indice, nodo->conta/*contatore*/);}
		//procedura di fine partita(gioca,menu,esci)
		}
	else{//nodo pieno
		if (nodo->foglia==1){//ci troviamo in una foglia
			printf("____________________\n\nPROVO AD INDOVINARE!\n____________________\n\n");
			printf ("Stavi pensando a %s?\nRispondi con SI o NO: ", nodo->doma_risp);
			si_no=richiesta_si_no();
			if(si_no==1){//INDOVINATO
				printf("____________________\n\nHO VINTO! HO INDOVINATO!\n____________________\n\n");
				//procedura di fine partita(gioca,menu,esci)
			}
			else{//NON INDOVINATO
				printf("____________________\n\nOH NO! NON HO INDOVINATO!\n____________________\n\n");
				if(nodo->conta==20){//ovvero siamo oltre la 20esima domanda, cioè siamo nell'ultimo livello dell'albero dove sono presenti solo foglie
					//volendo si può chiedere a quale animale pensasse ma ai fini del software è un dato che andrebbe perso
					//procedura di fine partita(gioca,menu,esci)//non si richiede nient'altro perchè si possono fare massimo 20 domande
				}
				else{//non si sono ancora poste 20 domande
					sostituzione_nodo(ftree, nodo, indice, nodo->conta/*contatore*/);}
				}
			}
		else{//ci troviamo in un nodo interno
			printf("____________________\n\nDOMANDA %d\nRICORDA: inserisci SI o NO per rispondere o uno degli altri 7 comandi\nse non li ricordi inserisci COMANDI\n____________________\n\n", nodo->conta+1);
			lista_risposte(ftree, nodo, indice);
			fseek (ftree, indice*sizeof(struct tree), SEEK_SET);
			fread(nodo, sizeof(struct tree), 1, ftree);
			printf("\nDOMANDA: %s\nInserisci la risposta o il comando: ", nodo->doma_risp);
			si_no=richiesta_si_no();
			if(si_no==0){//risposta NO
				analisi_nodo(ftree, nodo, nodo->sx, nodo->conta);
			}
			else if(si_no==1){//risposta SI
				analisi_nodo(ftree, nodo, nodo->dx, nodo->conta);
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

void nodo_vuoto(FILE *ftree, struct tree *nodo, int indice_nodo, int indice_prec, int contatore){
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
	nodo->pieno=0;
	nodo->foglia=0;
	nodo->conta=contatore+1;
	nodo->prec=indice_prec;
	nodo->sx=-500;
	nodo->dx=-500;
	strcpy(nodo->doma_risp, "");
	fseek (ftree, indice_nodo*sizeof(struct tree), SEEK_SET);
	fwrite(nodo, sizeof(struct tree), 1, ftree);
}

void sostituzione_nodo(FILE *ftree, struct tree*nodo, int indice, int contatore){
	char *mdl;
	char animale[51];
	int ind_sx, ind_dx;
	bool si_no;

	printf ("A quale animale avevi pensato (MAX 50 caratteri)? ");
	mdl=richiesta_stringa();
	strcpy(animale, mdl);
	strcpy(nodo->doma_risp, animale);//si potrebbe fare direttamente domarisp, mdl
	nodo->pieno=1;
	nodo->foglia=1;
	fwrite(nodo, sizeof(struct tree), 1, ftree);
	printf ("Quale domanda mi avrebbe aiutato ad identificarlo (MAX 50 caratteri)? ");
	mdl=richiesta_stringa();
	strcpy(nodo->doma_risp, mdl);
	printf ("A questa domanda avresti risposto SI o NO? ");
	si_no=richiesta_si_no();
	fseek (ftree, 0, SEEK_END);//punta alla fine del file e quindi al suo primo indice libero
	nodo->sx=ftell(ftree)/sizeof(struct tree);
	nodo->dx=ftell(ftree)/sizeof(struct tree)+1;
	nodo->foglia=0;
	nodo->pieno=1;
	fseek (ftree, indice*sizeof(struct tree), SEEK_SET);//punta di nuovo all'indice del nodo iniziale
	ind_sx=nodo->sx;//li salvo qui perchè poi verrano sovrascritti quando scriverò i nodi sx e dx
	ind_dx=nodo->dx;
	fwrite(nodo, sizeof(struct tree), 1, ftree);
	if (si_no==0){//risposta NO
		fseek (ftree, ind_sx*sizeof(struct tree), SEEK_SET); //si sposta sul nodo indicato da sx ovvero il sottoalbero di sinistra dove metterò la risposta
		nodo->prec=indice;
		nodo->sx=-500;
		nodo->dx=-500;
		nodo->pieno=1;
		nodo->foglia=1;
		nodo->conta=contatore+1;
		strcpy(nodo->doma_risp, animale);
		fwrite(nodo, sizeof(struct tree), 1, ftree);
		nodo_vuoto(ftree, nodo, ind_dx, indice, contatore);//si sposta sul nodo indicato da dx ovvero il sottoalbero di destra che sarà vuoto
	}
	if (si_no==1){//risposta SI
		nodo_vuoto(ftree, nodo, ind_sx, indice, contatore);//si sposta sul nodo indicato da sx ovvero il sottoalbero di sinistra che sarà vuoto
		fseek (ftree, ind_dx*sizeof(struct tree), SEEK_SET); //si sposta sul nodo indicato da dx ovvero il sottoalbero di destra dove metterò la risposta
		nodo->prec=indice;
		nodo->sx=-500;
		nodo->dx=-500;
		nodo->pieno=1;
		nodo->foglia=1;
		nodo->conta=contatore+1;
		strcpy(nodo->doma_risp, animale);
		fwrite(nodo, sizeof(struct tree), 1, ftree);
	}
}

void lista_risposte(FILE *ftree, struct tree*nodo, int indice/*, int contatore*/){
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
	int i, contatore, posizione;

	printf("Domande precedenti:\n");
	fseek(ftree, indice*sizeof(struct tree), SEEK_SET);
	posizione=indice;//ftell(ftree)/sizeof(struct tree);
	fread(nodo, sizeof(struct tree), 1, ftree);
	contatore=nodo->conta;
	for (i=contatore-1; i>=0; i--){
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
	}
}


