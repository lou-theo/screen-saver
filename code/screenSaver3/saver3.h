#include "image.h"

// prototypes des fonctions
int kbhit();
void changeAvion(picture* avionActif, picture* avionPassif);
void avionAffichage(picture avionActif[]);
void avionAvance(picture* avionActif, int direction);
int changeDirection(int direction, int* continuer);