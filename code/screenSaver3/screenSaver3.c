#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "saver3.h"

int main(int argc, char const *argv[])
{
	picture avionActif[4], avionPassif[4];
	char nom[500], pos[50];
	int position[2];
	int i = 0, directionAvion = 1, continuer = 1;
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);

	// on récupère les coordonnées de départ
	strcpy(pos, argv[1]);
	position[0] = atoi(strtok(pos, "x"));
	position[1] = atoi(strtok(NULL, "x"));

	// on initialise les images des différents avions (les différentes images serviront pour le passage des coins)
	for (i = 0 ; i < 4 ; i++)
	{
		avionActif[i].posy = position[1];
		avionActif[i].posx = position[0];
		avionActif[i].hauteur = 6;
		avionActif[i].largeur = 6;
		avionActif[i].donnee = malloc(avionActif[i].largeur * avionActif[i].hauteur * sizeof(int));
	}

	// on charge les 4 images d'avions, chacune ayant une direction différente
	for (i = 0 ; i < 4 ; i++)
	{
		if (getenv("EXIASAVER3_PBM") == 0)
		{
			sprintf(nom, "avion%d.pbm", i);
		}
		else
		{
			sprintf(nom, "%s/avion%d.pbm",getenv("EXIASAVER3_PBM"), i);
		}

		chargerImage(nom, &avionPassif[i]);
		avionPassif[i].posy = w.ws_row / 2;
		avionPassif[i].posx = w.ws_col / 2;
	}
//#########################################################################
	
	do // boucle infini, jusqu'à ce qu'on appui sur espace, on sort alors de la boucle
	{
		//on récupère la direction sur laquelle appui l'utilisateur
		directionAvion = changeDirection(directionAvion, &continuer);
		system("clear");

		for (i = 0 ; i < 4 ; i++) // on actualise les avions actifs en fonction de la direction
		{
			changeAvion(&avionActif[i], &avionPassif[directionAvion]);
		}

		avionAvance(&avionActif[0], directionAvion); // on change les coordonnées des images actives de l'avion

		avionAffichage(avionActif); // fait l'affichage des différents avions actifs (gère notamment passage des coins)
		fflush(stdout); // force l'actualisation

		usleep(200000); // en nano seconde
	}while(continuer);

	
//############################################################################################
	system("clear");

	// on libère les tableaux dynamiques
	for (i = 0 ; i < 4 ; i++)
		{
			free(avionPassif[i].donnee);
		}
	for (i = 0 ; i < 4 ; i++)
		{
			free(avionActif[i].donnee);
		}
	return 0;
}