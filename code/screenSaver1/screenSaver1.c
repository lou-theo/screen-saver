#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "image.h"
#include "saver1.h"

int main(int argc, char const *argv[])
{
	picture image;
	char nom[500];
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);
	image.posy = w.ws_row / 2;
	image.posx = w.ws_col / 2;

	// on récupère le chemin du dossier contenant les images, si NULL alors défini en local
	if (getenv("EXIASAVER1_PBM") == 0)
	{
		sprintf(nom, "%s",argv[1]);
	}
	else
	{
		sprintf(nom, "%s/%s",getenv("EXIASAVER1_PBM"), argv[1]);
	}

	chargerImage(nom, &image); // fonction qui sert à initialiser la variable image de type "picture"

	system("clear");
	afficherImage(image, 0, image.largeur, 0, image.hauteur); // on affiche l'image
	
	gotoxy(w.ws_col, w.ws_row); // on place le curseur dans un coin de l'écran pour pas faire moche
	getk(); // on attend la pression d'une touche

	free(image.donnee); // on libère le tableau dynamique placé dans la structure picture
	return 0;
}