#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "image.h"
#include "saver1.h"

#include <sys/mman.h>
#include <sys/types.h>

int main(int argc, char const *argv[])
{
	//picture image;
	char nom[500];
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);

	if (getenv("EXIASAVER1_PBM") == 0)
	{
		sprintf(nom, "%s",argv[1]);
	}
	else
	{
		sprintf(nom, "%s/%s",getenv("EXIASAVER1_PBM"), argv[1]);
	}

	// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

	// on fait une place dans la mémoire partagée
	picture* image = mmap(NULL, sizeof(picture*), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

	if (fork() == 0) {
        chargerImage(nom, image);
       //image->donnee = mmap(NULL, image->hauteur * image->largeur * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	//chargerImage(nom, image);
        exit(EXIT_SUCCESS);
    } else {
        wait(NULL);
    }
    
    // on repasse les valeur qui sont dans donnee[2] (le tableau statique, pas beau) dans donnee[1] (le beau, le dynamique)
    image->donnee = malloc(image->hauteur * image->largeur * sizeof(int));
    for (int i = 0 ; i < image->hauteur * image->largeur ; i++)
    {
    	image->donnee[i] = image->donnee2[i];
    }
    

    image->posy = w.ws_row / 2;
	image->posx = w.ws_col / 2;
	system("clear");
    afficherImage(*image, 0, image->largeur, 0, image->hauteur);

	

	// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

	// on récupère le chemin du dossier contenant les images, si NULL alors défini en local
	

	//chargerImage(nom, &image); // fonction qui sert à initialiser la variable image de type "picture"

	//afficherImage(*image, 0, image->largeur, 0, image->hauteur); // on affiche l'image
	
	gotoxy(w.ws_col, w.ws_row); // on place le curseur dans un coin de l'écran pour pas faire moche
	getk(); // on attend la pression d'une touche

	free(image->donnee); // on libère le tableau dynamique placé dans la structure picture
	return 0;
}