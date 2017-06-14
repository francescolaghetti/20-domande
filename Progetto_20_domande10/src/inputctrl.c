/*
 * inputctrl.c
 *
 *  Created on: 08 giu 2017
 *      Author: Laghetti
 */

#include "inputctrl.h"

char *richiesta_stringa(){
	/*INFO
	 1.a
	 	 PARAMETRI DI INPUT:
	 	 	niente
	 1.b
	 	 PARAMETRI DI OUTPUT:
	 	 	 restituisce:
	 	 	 	 il puntatore ad una stringa se tutto va bene
	 	 	 stampa:
	 	 	 	 "ERRORE 005: memoria per allocazione non sufficiente" se non c'è abbastanza memoria per allocare la stringa
	 	 	 	 "ERRORE 004: frase immessa troppo lunga\nReinseriscila per favore: " se la frase immessa supera i 50 caratteri
	 	 	 	 "ERRORE 002: parola immessa non valida\nReinseriscila per favore: " se la stringa è vuota o formata solo da spazi e tabulazioni
	 2
	 	 DESCRIZIONE:
	 	 	controlla che ci sia abbastanza memoria per allocare la stringa;
	 	 	acquisice una stringa con fgets;
	 	 	se il penultimo carattere acquisito è '\n' lo sostituisce con '\0';
	 	 	controlla che la stringa inserita non superi i 50 caratteri e nel caso li superi libera lo stack dai caratteri avanzati;
	 	 	controlla se la stringa è vuota o composta solo da spazi e tabulazioni;
	 	 	elimina gli spazi superflui prima e dopo il contenuto della frase;
	 	 	se tutto va bene restituisce il puntatore alla stringa
	 2.a
	 	 DETTAGLI IMPLEMENTATIVI SALIENTI:
	 	 	 alloca dinamicamente una stringa con malloc;
	 	 	 acquisisce 52 caratteri con fgets: 50 per la stringa che interessa, 1 per lo '\n' che viene acquisito da fgets, 1 che serve a capire se sono stati superati i 50 caratteri o ad inserire lo '\0';
	 	 	 controlla la stringa con strspc per sapere se è vuota o composta solo da spazi e tabbature
	 2.b
	 	 CASI LIMITE:
	 	 	 non c'è abbastanza spazio per l'allocazione dinamica;
	 	 	 la stringa inserita è costituita da più di 50 caratteri;
	 	 	 la stringa inserita è vuota;
	 	 	 la stringa inserita è formata solo da spazi e/o tabbature;

	 2.c
	   1)INPUT:
	 	 	 risp="123456789012345678901234567890123456789012345678901"
	 	 OUTPUT:
	 		 ERRORE 004: frase immessa troppo lunga\nReinseriscila per favore:

	   2)INPUT:
	 	 	 risp="    	"
	 	 OUTPUT:
	 		 ERRORE 002: parola immessa non valida\nReinseriscila per favore:

	   3)INPUT:
	 	 	 risp=""
	 	 OUTPUT:
	 		 ERRORE 002: parola immessa non valida\nReinseriscila per favore:

	   4)INPUT:
	 	 	 risp="cavolo"
	 	 SITUAZIONE FINALE:
	 	 	return risp
	 3
	 	 AUTORI:
	 	 	 Francesco Laghetti
	 4
	 	 REVISIONE:
	 	 	 versione 1.3 08/06/2017
	 */

	char *risp=malloc(51);
	if(risp==NULL){//controllo sulla memoria disponibile per allocare
		system("cls");
	    printf("____________________\n\nERRORE 005: memoria per allocazione non sufficiente\n____________________\n\n");
	    free(risp);
	    getch();
	    exit(1);
	}

	char tmp[52];//stringa dove effettuerò il controllo
	int n=0;
	int i, c;
	bool spz=0;

	fgets(tmp, 52, stdin);//salva i primi 52 caratteri inseriti in tmp
	if(tmp[strlen(tmp)-1]=='\n'){//se l'ultimo carattere è il \n lo fa diventare \0. devo eliminare il \n perchè  se fossero stati inseriti esattamente 50 caratteri si avrebbe l'array [50char][\n][\0] e si avrebbe strlen(tmp)>50 anche se non è vero
		tmp[strlen(tmp)-1]='\0';
	}

	if(strlen(tmp)>50){//potrei scrivere semplicemente else perchè se \n è presente in tmp vuol dire che ci sono al massimo 5o caratteri
		printf("ERRORE 004: frase immessa troppo lunga\nReinseriscila per favore: ");
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

	for (i=0; spz==0; i++){//calcola quanti spazi ci sono prima del primo carattere
		if(isspace(risp[i])!=0){
			n++;
		}
		else{
			spz=1;
		}
	}

	for (i=0; i<strlen(risp)-n; i++){//copia la stringa di tanti spazi indietro quanti sono gli spazi calcolati precedentemente
		risp[i]=risp[i+n];
	}
	risp[strlen(risp)-n]='\0';//pone un '\0' alla fine della stringa copiata

	c=strlen(risp);

	for (i=c-1; i>=0; i--){//sostituisce tutti gli spazi che trova partendo dall'ultimo elemento dell'array con '\0' fino a quando non incontra un carattere
		if(isspace(risp[i])!=0){
			risp[i]='\0';
		}
		else {
			i=-1;
		}
	}

	return risp;
}

int strspc(char *str){
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
	 	 	 stampa:
	 	 	 	 "ERRORE 001: comando immesso non valido\nReinseriscilo per favore: " in tutti gli altri casi e ricomincia la funzione
	 2
	 	 DESCRIZIONE:
	 	 	 acquisice una stringa con fgets;
	 	 	 se il penultimo carattere acquisito è '\n' lo sostituisce con '\0';
	 	 	 controlla che la stringa inserita non superi i 2 caratteri e nel caso li superi libera lo stack dai caratteri avanzati;
	 	 	 controlla che la stringa inserita non sia vuota o composta solo da spazi e tabbature;
	 	 	 controlla che la risposta inserita sia "si", "SI", "no" o "NO"
	 2.a
	 	 DETTAGLI IMPLEMENTATIVI SALIENTI:
	 	 	 acquisisce 4 caratteri con fgets: 2 per la stringa che interessa, 1 per lo '\n' che viene acquisito da fgets, 1 che serve a capire se sono stati superati i 2 caratteri o ad inserire lo '\0';
	 	 	 controlla la stringa con strspc per sapere se è vuota o composta solo da spazi e tabbature
	 2.b
	 	 CASI LIMITE:
	 	 	 la stringa inserita è costituita da più di 2 caratteri;
	 	 	 la stringa inserita è vuota;
	 	 	 la stringa inserita è formata solo da spazi e/o tabbature;
	 	 	 la stringa inserita non è "si" o "SI" o "no" o "NO"
	 2.c
	   1)INPUT:
	 	 	si_no="cavolo"
	 	 OUTPUT
	 	 	ERRORE 002: parola immessa non valida\nReinseriscila per favore:

	   2)INPUT
	 	 	si_no="SI"
	 	 SITUAZIONE FINALE:
	 	 	return 1

	   3)INPUT
	 	 	si_no="no"
	 	 SITUAZIONE FINALE:
	 	 	return 0
	 3
	 	 AUTORI:
	 	 	 Francesco Laghetti
	 4
	 	 REVISIONE:
	 	 	 versione 1.1 08/06/2017
	 */
	char tmp[4];
	bool comando;

	fgets(tmp, 4, stdin);//salva i primi 2 caratteri inseriti in tmp
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
	printf("ERRORE 001: comando immesso non valido\nReinseriscilo per favore: ");
	comando=richiesta_si_no();
	return comando;
}

int richiesta_comando(int indice){
	/*INFO
	 1.a
	 	 PARAMETRI DI INPUT:
	 	 	 intero indice che indica la domanda a cui si è arrivati
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
	 	 	 stampa:
	 	 	 "ERRORE 003: impossibile tornare indietro alla prima domanda.\nInserisci un altro comando per favore: " se si scrive il comando indietro (o INDIETRO) ma indice=0 (ovvero si è alla prima domanda del gioco ergo non si può tornare indietro)
	 	 	 "ERRORE 001: comando immesso non valido\nReinseriscilo per favore: " in tutti gli altri casi e ricomincia la funzione
	 2
	 	 DESCRIZIONE:
	 	 	acquisice una stringa;
	 	 	controlla che la stringa inserita non superi gli 8 caratteri e nel caso li superi libera lo stack dai caratteri avanzati;
	 	 	controlla che la stringa inserita sia vuota o composta solo da spazi e tabulazioni;
	 	 	controlla che la stringa inserita sia "si", "SI", "no", "NO", "indietro", "INDIETRO", "salva", "SALVA", "menu", "MENU", "comandi", "COMANDI", "esci" o "ESCI"
	 2.a
	 	 DETTAGLI IMPLEMENTATIVI SALIENTI:
	 	 	 acquisisce 10 caratteri con fgets: 8 per la stringa che interessa, 1 per lo '\n' che viene acquisito da fgets, 1 che serve a capire se sono stati superati gli 8 caratteri o ad inserire lo '\0';
	 	 	 controlla la stringa con strspc per sapere se è vuota o composta solo da spazi e tabbature
	 2.b
	 	 CASI LIMITE:
	 	 	 la stringa inserita è costituita da più di 8 caratteri;
	 	 	 la stringa inserita è vuota;
	 	 	 la stringa inserita è formata solo da spazi e/o tabbature;
	 	 	 la stringa inserita non è "si", "SI","no", "NO", "indietro", "INDIETRO", "salva", "SALVA", "menu", "MENU", "comandi", "COMANDI", "esci" o "ESCI"
	 2.c
	   1)INPUT:
	 	 	si_no="cavolo"
	 	 OUTPUT
	 	 	ERRORE 001: comando immesso non valido\nReinseriscilo per favore:

	   2)INPUT
	 	 	si_no="SI"
	 	 SITUAZIONE FINALE:
	 	 	return 1

	   3)INPUT
	   	 	si_no="indietro"
	   	    indice=0
	   	 OUTPUT
	   	    ERRORE 003: impossibile tornare indietro alla prima domanda.\nInserisci un altro comando per favore:
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


char* punto_interrogativo(char *str){
	/*INFO
	 1.a
	 	 PARAMETRI DI INPUT:
	 	 	puntatore ad array di caratteri
	 1.b
	 	 PARAMETRI DI OUTPUT:
	 	 	 restituisce:
	 	 	 	 il puntatore all'array di caratteri
	 2
	 	 DESCRIZIONE:
	 	 	analizza la stringa partendo dall'ultimo carattere:
	 	 		se è uno spazio o un '?' lo sostituisce con '\0'
	 	 		se è un carattere sostituisce il carattere successivo con un '?' e il carattere ancora successivo con '\0'
	 	 	se tutto va bene restituisce il puntatore alla stringa
	 2.a
	 	 DETTAGLI IMPLEMENTATIVI SALIENTI:
	 	 	 controlla se la stringa è vuota o composta solo da spazi e tabulazioni con la funzione isspace()
	 2.b
	 	 CASI LIMITE:
	 	 	 la stringa è vuota o formata solo da spazi e restituisce una stringa vuota (infatti bisognerebbe prima fare un controllo sulla stringa in modo tale da non accettarne una vuota o composta solo da spazi)

	 2.c
	   	 INPUT:
	 	 	 str="ha 4 zampe  ????   ???  "
	 	 SITUAZIONE FINALE:
	 	 	 return str="ha 4 zampe?"
	 3
	 	 AUTORI:
	 	 	 Francesco Laghetti
	 4
	 	 REVISIONE:
	 	 	 versione 1.0 23/05/2017
	 */


	int c;

	for(c=strlen(str)-1; c>=0; c--){//parte dall'ultimo elemento dell'array
		if(isspace(str[c])!=0 || str[c]=='?'){//sostituisce tutti gli spazi e i '?' con '\0'
			str[c]='\0';
		}
		else{//se incontra qualcosa di diverso da spazi o '?' sostituisce l'elemento successivo con un '?' e l'elemento ancora successivo con '\0'
			str[c+1]='?';
			str[c+2]='\0';
			c=-1;
		}
	}

	return str;
}
