#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>
#include <dirent.h>

// Prototypes des fonctions
void lanceur();
void screenSaver1(char heure[], FILE* fichier);
void screenSaver2(char heure[], FILE* fichier);
void screenSaver3(char heure[], FILE* fichier);