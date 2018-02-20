#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include "saver2.h"

// fonction qui détecte la pression d'une touche
int kbhit() 
{
    struct timeval tv = { 0, 0 }; 
    fd_set readfds; 
  
    FD_ZERO(&readfds); 
    FD_SET(STDIN_FILENO, &readfds); 
  
    return select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) == 1; 
}

// fonction qui crée une grandeImage à partir des différentes images qui sont des chiffres
void fusionImage(picture* grandeImage, picture chiffres[], int heure, int minute, int seconde)
{
	int compteur = 0, i =0, j = 0;

	free(grandeImage->donnee); // on libère l'ancienne allocation dynamique
	// on réinitialise alors les différentes infos de la grandeImage : hauteur, largeur, tableau avec allocation
	grandeImage->largeur = (chiffres[0].largeur * 6) + 2 * 3 + 3;
	grandeImage->hauteur = chiffres[0].hauteur;
	grandeImage->donnee = malloc(grandeImage->hauteur * grandeImage->largeur * sizeof(int));

	// on parcourt la grande image ligne par ligne
	for (i = 0 ; i < grandeImage->hauteur ; i++)
	{
		// on parcourt ensuite chacun des bits des petites images en fonction du numéro de la ligne de la grande
		// entre chaque petite image, on met des espaces
		for (j = 0 ; j < chiffres[0].largeur ; j++)
		{
			grandeImage->donnee[compteur] = chiffres[heure/10].donnee[(i * chiffres[0].largeur) + j];
			compteur++;
		}

		grandeImage->donnee[compteur] = 0;
		compteur++;

		for (j = 0 ; j < chiffres[0].largeur ; j++)
		{
			grandeImage->donnee[compteur] = chiffres[heure%10].donnee[(i * chiffres[0].largeur) + j];
			compteur++;
		}

		grandeImage->donnee[compteur] = 0;
		compteur++;
		if (i == ((2 * grandeImage->hauteur) /3))
		{
			grandeImage->donnee[compteur] = 15;
		}
		else if (i == ((grandeImage->hauteur) /3))
		{
			grandeImage->donnee[compteur] = 15;
		}
		else
		{
			grandeImage->donnee[compteur] = 0;
		}
		compteur++;
		grandeImage->donnee[compteur] = 0;
		compteur++;



		for (j = 0 ; j < chiffres[0].largeur ; j++)
		{
			grandeImage->donnee[compteur] = chiffres[minute/10].donnee[(i * chiffres[0].largeur) + j];
			compteur++;
		}

		grandeImage->donnee[compteur] = 0;
		compteur++;

		for (j = 0 ; j < chiffres[0].largeur ; j++)
		{
			grandeImage->donnee[compteur] = chiffres[minute%10].donnee[(i * chiffres[0].largeur) + j];
			compteur++;
		}

		grandeImage->donnee[compteur] = 0;
		compteur++;
		if (i == ((2 * grandeImage->hauteur) /3))
		{
			grandeImage->donnee[compteur] = 15;
		}
		else if (i == ((grandeImage->hauteur) /3))
		{
			grandeImage->donnee[compteur] = 15;
		}
		else
		{
			grandeImage->donnee[compteur] = 0;
		}
		compteur++;
		grandeImage->donnee[compteur] = 0;
		compteur++;


		for (j = 0 ; j < chiffres[0].largeur ; j++)
		{
			grandeImage->donnee[compteur] = chiffres[seconde/10].donnee[(i * chiffres[0].largeur) + j];
			compteur++;
		}

		grandeImage->donnee[compteur] = 0;
		compteur++;

		for (j = 0 ; j < chiffres[0].largeur ; j++)
		{
			grandeImage->donnee[compteur] = chiffres[seconde%10].donnee[(i * chiffres[0].largeur) + j];
			compteur++;
		}
	}
}

// fonction qui sert à transformer les images 5x3 en 9x5
picture agrandirImage(picture image)
{
	int i = 0, j = 0;
	picture grandeImage;
	// on définit la grande image
	grandeImage.hauteur = 9;
	grandeImage.largeur = 5;
	grandeImage.donnee = malloc(grandeImage.hauteur * grandeImage.largeur * sizeof(int));

	// on initialise chaque case du tableau à 0
	for (i = 0 ; i < grandeImage.hauteur * grandeImage.largeur ; i++)
	{
		grandeImage.donnee[i] = 0;
	}

	// pour chaque "barre" de la petite image, on regarde si elle est "active", si c'est le cas, on l'active sur la grande image
	if ((image.donnee[0 * image.largeur + 1]) == 15)
	{
		for (i = 0 ; i < 5 ; i++)
		{
			grandeImage.donnee[0 * grandeImage.largeur + i] = 15;
		}
	}
	if ((image.donnee[2 * image.largeur + 1]) == 15)
	{
		for (i = 0 ; i < 5 ; i++)
		{
			grandeImage.donnee[4 * grandeImage.largeur + i] = 15;
		}
	}
	if ((image.donnee[4 * image.largeur + 1]) == 15)
	{
		for (i = 0 ; i < 5 ; i++)
		{
			grandeImage.donnee[8 * grandeImage.largeur + i] = 15;
		}
	}

	if ((image.donnee[1 * image.largeur + 0]) == 15)
	{
		for (i = 0 ; i < 5 ; i++)
		{
			grandeImage.donnee[i * grandeImage.largeur + 0] = 15;
		}
	}
	if ((image.donnee[1 * image.largeur + 2]) == 15)
	{
		for (i = 0 ; i < 5 ; i++)
		{
			grandeImage.donnee[i * grandeImage.largeur + 4] = 15;
		}
	}
	if ((image.donnee[3 * image.largeur + 0]) == 15)
	{
		for (i = 0 ; i < 5 ; i++)
		{
			grandeImage.donnee[(5 + i) * grandeImage.largeur + 0] = 15;
		}
	}
	if ((image.donnee[3 * image.largeur + 2]) == 15)
	{
		for (i = 0 ; i < 5 ; i++)
		{
			grandeImage.donnee[(5 + i) * grandeImage.largeur + 4] = 15;
		}
	}

	return grandeImage;
}