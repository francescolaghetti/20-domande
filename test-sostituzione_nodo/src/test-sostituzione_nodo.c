/*
 ============================================================================
 Name        : test-sostituzione_nodo.c
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


//void analisi_nodo(FILE*, struct tree*, int, int);
char *richiesta_stringa();//VERIFICATO
int strspc(char*);//VERIFICATO
bool richiesta_si_no();//VERIFICATO
void nodo_vuoto(FILE*, struct tree*, int, int, int);//VERIFICATO
void sostituzione_nodo(FILE *, struct tree*, int, int);
//void lista_risposte(FILE *, struct tree*, int/*, int*/);//VERIFICATO

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
	strcpy(nodo->doma_risp, "SI o NO?");

	/*ftree=fopen("albero", "ab");
	fclose(ftree);*/

	ftree=fopen("albero", "rb+");
	fseek(ftree, 0*sizeof(struct tree), SEEK_SET);
	fwrite(nodo, sizeof(struct tree), 1, ftree);
	fseek(ftree, 0*sizeof(struct tree), SEEK_SET);
	fread(nodo, sizeof(struct tree), 1, ftree);
	printf("INDICE %d\ndx=%d\nsx=%d\nprec=%d\nconta=%d\nfoglia=%d\npieno=%d\ndoma_risp=%s\n\n", ftell(ftree)/sizeof(struct tree)-1, nodo->dx, nodo->sx, nodo->prec, nodo->conta, nodo->foglia, nodo->pieno, nodo->doma_risp);
	sostituzione_nodo(ftree, nodo, ftell(ftree)/sizeof(struct tree)-1, nodo->conta);
	fseek(ftree, 0*sizeof(struct tree), SEEK_SET);
	fread(nodo, sizeof(struct tree), 1, ftree);
	printf("INDICE %d\ndx=%d\nsx=%d\nprec=%d\nconta=%d\nfoglia=%d\npieno=%d\ndoma_risp=%s\n\n", ftell(ftree)/sizeof(struct tree)-1, nodo->dx, nodo->sx, nodo->prec, nodo->conta, nodo->foglia, nodo->pieno, nodo->doma_risp);
	fseek(ftree, nodo->sx*sizeof(struct tree), SEEK_SET);
	fread(nodo, sizeof(struct tree), 1, ftree);
	printf("INDICE %d\ndx=%d\nsx=%d\nprec=%d\nconta=%d\nfoglia=%d\npieno=%d\ndoma_risp=%s\n\n", ftell(ftree)/sizeof(struct tree)-1, nodo->dx, nodo->sx, nodo->prec, nodo->conta, nodo->foglia, nodo->pieno, nodo->doma_risp);
	fseek(ftree, nodo->dx*sizeof(struct tree), SEEK_SET);
	fread(nodo, sizeof(struct tree), 1, ftree);
	printf("INDICE %d\ndx=%d\nsx=%d\nprec=%d\nconta=%d\nfoglia=%d\npieno=%d\ndoma_risp=%s\n\n", ftell(ftree)/sizeof(struct tree)-1, nodo->dx, nodo->sx, nodo->prec, nodo->conta, nodo->foglia, nodo->pieno, nodo->doma_risp);
	fclose(ftree);

}

void sostituzione_nodo(FILE *ftree, struct tree*nodo, int indice, int contatore){
	char *mdl;
	char animale[51];
	int ind_sx, ind_dx;
	bool si_no;

	printf ("A quale animale avevi pensato (MAX 50 caratteri)? ");
	mdl=richiesta_stringa();
	strcpy(animale, mdl);//in animale salvo l'animale a cui aveva pensato l'utente
	printf("avevi pensato a %s\n", animale);
	//strcpy(nodo->doma_risp, animale);//si potrebbe fare direttamente domarisp, mdl
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
	printf("ind_sx = %d\nind_dx = %d\n", ind_sx, ind_dx);
	if (si_no==0){//risposta NO
		printf("RISPOSTA NO e l'animale a cui avevi pensato è %s\n", animale);
		printf("ind_sx = %d\nind_dx = %d\n", ind_sx, ind_dx);
		fseek (ftree, ind_sx*sizeof(struct tree), SEEK_SET); //si sposta sul nodo indicato da sx ovvero il sottoalbero di sinistra dove metterò la risposta
		nodo->prec=indice;
		nodo->sx=-500;
		nodo->dx=-500;
		nodo->pieno=1;
		nodo->foglia=1;
		nodo->conta=contatore+1;
		strcpy(nodo->doma_risp, animale);
		fwrite(nodo, sizeof(struct tree), 1, ftree);
		printf("ind_sx = %d\nind_dx = %d\n", ind_sx, ind_dx);
		nodo_vuoto(ftree, nodo, ind_dx, indice, contatore);//si sposta sul nodo indicato da dx ovvero il sottoalbero di destra che sarà vuoto
	}
	if (si_no==1){//risposta SI
		printf("RISPOSTA SI e l'animale a cui avevi pensato è %s\n", animale);
		printf("ind_sx = %d\nind_dx = %d\n", ind_sx, ind_dx);
		nodo_vuoto(ftree, nodo, ind_sx, indice, contatore);//si sposta sul nodo indicato da sx ovvero il sottoalbero di sinistra che sarà vuoto
		printf("ind_sx = %d\nind_dx = %d\n", ind_sx, ind_dx);
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
