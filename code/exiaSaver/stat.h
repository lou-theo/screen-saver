#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>
#include <dirent.h>

// prototype des fonctions
int menu();
void statistique();
void optionScreenSaver(FILE* fichier);
void optionImage(FILE* fichier);
void optionTailleHeure(FILE* fichier);
void optionHistorique(FILE* fichier);