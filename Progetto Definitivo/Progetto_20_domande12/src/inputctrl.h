/*
 * inputctrl.h
 *
 *  Created on: 08 giu 2017
 *      Author: Laghetti
 */

#ifndef INPUTCTRL_H_
#define INPUTCTRL_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>

char* richiesta_stringa();
int strspc(char*);
bool richiesta_si_no();
int richiesta_comando(int indice);
char* punto_interrogativo(char*);

#endif /* INPUTCTRL_H_ */
