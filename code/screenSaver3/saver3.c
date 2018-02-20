#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include "saver3.h"

// fonction qui sert à détecter la pression sur le clavier
int kbhit() 
{
    struct timeval tv = { 0, 0 }; 
    fd_set readfds; 
  
    FD_ZERO(&readfds); 
    FD_SET(STDIN_FILENO, &readfds); 
  
    return select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) == 1; 
}

// fonction charge l'image de l'avionPassif dans l'avionActif (pour changer la direction de l'avion)
void changeAvion(picture* avionActif, picture* avionPassif)
{
	int i = 0;

	// parcourt toutes les valeurs du tableau et affecte la valeur du taleau passif à l'actif
	for (i = 0 ; i < (avionPassif->hauteur) * (avionPassif->largeur) ; i++)
	{
		avionActif->donnee[i] = avionPassif->donnee[i];
	}
}

// gère l'affichage des différents avions actifs
void avionAffichage(picture avionActif[])
{
	int xDebut[4] = {0}, xFin[4] = {6,6,6,6}, yDebut[4] = {0}, yFin[4] = {6,6,6,6};
	int etatAvionActif[4] = {1,0,0,0};
	int i =0;
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);

	// si l'ordonnée de l'image est inférieur à 4 alors l'image va être coupée
	if (avionActif[0].posy < 4)
	{
		yDebut[0] = 4 - avionActif[0].posy; // défini la 1ere ligne à afficher pour l'image active[0] (la principale)

		// défini la 1ere ligne à afficher pour l'image active[1] (l'image qui s'affiche lorsque l'image est coupé sur le côté)
		yDebut[1] = 4 - avionActif[0].posy;

		// défini la dernière ligne à afficher pour l'image active[2] (l'image qui s'affiche lorsque l'image est coupé en haut ou bas)
		yFin[2] = 4 - avionActif[0].posy;
		etatAvionActif[2] = 1; // on met le niveau d'activité de cette image à 1
		avionActif[2].posx = avionActif[0].posx;
		avionActif[2].posy = w.ws_row + 4 - ((4 - avionActif[0].posy) );

		// défini la dernière ligne à afficher pour l'image active[3]
		// (l'image qui s'affiche lorsque l'image est coupé en haut ou bas et sur le côté)
		yFin[3] = 4 - avionActif[0].posy;
		avionActif[3].posy = w.ws_row + 4 - ((4 - avionActif[0].posy) );
	}
	
	// si l'abscisse de l'image est inférieur à 7 alors l'image va être coupée
	// on fait alors comme précédemment
	if (avionActif[0].posx < 7)
	{
		xDebut[0] = 7 - avionActif[0].posx; 

		xFin[1] = 7 - avionActif[0].posx;
		etatAvionActif[1] = 1; // on met le niveau d'activité de cette image à 1
		avionActif[1].posx = w.ws_col + 7 - ((7 - avionActif[0].posx) * 2);
		avionActif[1].posy = avionActif[0].posy;

		xDebut[2] = 7 - avionActif[0].posx;

		xFin[3] = 7 - avionActif[0].posx;
		avionActif[3].posx = w.ws_col + 7 - ((7 - avionActif[0].posx) * 2);
	}
	
	// Dans le cas où l'image est coupée en abscisse et en ordonnée
	if ((avionActif[0].posx < 7) && (avionActif[0].posy < 4))
	{
		etatAvionActif[3] = 1; // on met le niveau d'activité de cette image à 1
	}


	for (i = 0 ; i < 4 ; i++) // on affiche les images dont le niveau d'activité est à 1
	{
		if (etatAvionActif[i] == 1)
		{
			afficherImage(avionActif[i], xDebut[i], xFin[i], yDebut[i], yFin[i]);
		}
	}
}

// fonction qui change la position de l'image de l'avion
void avionAvance(picture* avionActif, int direction)
{
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);

	switch (direction) // en fonction de la direction, on incrémente ou décrémente l'abscisse/ordonnée de l'image
	{ // Si la position de l'image devient hors cadre, alors on envoi l'image de l'autre côté de la console
		case 0:
			avionActif->posy--;
			if (avionActif->posy < -2)
			{
				avionActif->posy = w.ws_row - 2;
			}
			break;
		case 2:
			avionActif->posy++;
			if (avionActif->posy > (w.ws_row - 2))
			{
				avionActif->posy = -1;
			}
			break;
		case 1:
			avionActif->posx++;
			if (avionActif->posx > (w.ws_col - 5))
			{
				avionActif->posx = 1;
			}
			break;
		case 3:
			avionActif->posx--;
			if (avionActif->posx < 1)
			{
				avionActif->posx = (w.ws_col - 5);
			}
			break;
	}
}


int changeDirection(int direction, int* continuer)
{
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w); // on récupère les dimensions de la console
	int tampon = 0;

	system("stty cbreak -echo"); // console mode pas besoin de confirmer

	gotoxy(w.ws_col, w.ws_row);

	if (kbhit()) // s'il y a pression d'une touche
	{
		tampon = getchar(); // on récupère le code de la touche et on regarde si c'en est une qui nous intéresse
	
		if (tampon == 32) // bouton entrer
			{
			*continuer = 0; // on met la variable continuer à 0, on sortira de la boucle
			}
			
		else if (tampon == 27) // les flèches on 3 codes, on regarde la 1ère
		{
			if (getchar() == 91) // 2eme code des touches
			{
				tampon = getchar();

				if (tampon == 65) //fleche haut
				{
					direction = 0;
				}

				else if (tampon == 66) //fleche bas
				{
					direction = 2;
				}
				
				else if (tampon == 67) //fleche droite
				{
					direction = 1;
				}

				else if (tampon == 68) //fleche gauche
				{
					direction = 3;
				}
			}
		}
	}

	
	system("stty cooked echo"); // on repasse la console en mode "à confirmer"
	return direction;
}