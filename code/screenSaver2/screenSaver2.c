#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include "saver2.h"

int main(int argc, char const *argv[])
{
	picture image[10], grandeImage;
	char nom[500];
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w); // on récupère les dimensions de la console

	time_t secondes; // nombre de secondes depuis 1er janvier 1970
	struct tm instant; // conversion des secondes en date
	int i=0;

	system("stty cbreak -echo"); // passage de la console en mode "pas de confirmation de saisi"

	int N=atoi(getenv ("EXIASAVER2_SLEEP")); // on récupère dans la variable d'env le nombre de seconde entre chaque actualisation
	if (N<2 || 10<N) // si ce nombre n'est pas compris entre 2 et 10 alors on le met à 10
	{ 	
		N=10;
	}

	// on récupère les images des chiffres 0 à 9 et on les stock dans un tableau
	for (i = 0 ; i < 10 ; i++)
	{
		if (getenv("EXIASAVER2_PBM") == 0) // on prend le chemin des images, s'il est vide, on met le chemin en local
		{
			sprintf(nom, "%d.pbm", i);
		}
		else
		{
			sprintf(nom, "%s/%d.pbm",getenv("EXIASAVER2_PBM"), i);
		}

		chargerImage(nom, &image[i]); // on charge l'image

		// on récupère la taille des images dans la variable d'env, si la taille est égale à "9x5" alors on agrandi l'image
		if (strcmp(getenv("EXIASAVER2_TAILLE"), "9x5") == 0)
		{
			image[i] = agrandirImage(image[i]);
		}
		
		// on défini les positions pour mettre les images au milieu de l'éran au cas où mais elles n'ont pas vocation à être affichée
		image[i].posy = w.ws_row / 2;
		image[i].posx = w.ws_col / 2;
	}

	grandeImage.donnee = malloc(sizeof(int)); // on initialise le tableau dynamique de la grande image

	do // boucle presque infini (brisée si on appuie sur une touche)
	{
		system("clear");

		time(&secondes); // on récupère de nouveau l'heure pour actualiser nos variables
		instant=*localtime(&secondes);

		// on défini la grandeImage qui est une fusion des différentes petites images représentant les chiffres
		fusionImage(&grandeImage, image, instant.tm_hour, instant.tm_min, instant.tm_sec);
		ioctl(0, TIOCGWINSZ, &w); // on récupère les dimensions de la fenêtre
		grandeImage.posy = w.ws_row / 2; // on défini la pos de la grande image pour qu'elle soit au centre
		grandeImage.posx = w.ws_col / 2;
		afficherImage(grandeImage, 0, grandeImage.largeur, 0, grandeImage.hauteur); // on affiche l'image

		printf("\n\n");
		for (i = 0 ; i < (w.ws_col - 47 - N) / 2 ; i++) // on met des espaces pour centrer la phrase
		{
			printf(" ");
		}
		printf("Cet ecran sera actualise dans quelques secondes");
		
		i = 0;
		while (i<N) // pour chaque seconde, on rajoute 1 point et on attend une seconde
		{	
			printf(".");
			fflush(stdout);
			sleep(1);
			i++;
			if (kbhit()) // on regarde si une touche est pressée, si c'est le cas, on casse la boucle
			{
				break;
			}
		}
	} while(!kbhit()); // tant qu'auncune touche n'est pressée, la boucle continue

	getchar(); // on vide le buffer du clavier
	system("clear");
	
	system("stty cooked echo"); // on repasse la console en mode "à confirmer"

	// on libère les allocations dynamique
	for (i = 0 ; i < 10 ; i++)
	{
		free(image[i].donnee);
	}
	return 0;
}