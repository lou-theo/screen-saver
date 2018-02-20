#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <string.h>
#include "image.h"
#include <sys/mman.h>
#include <sys/types.h>

// fonction servant à déplacer le curseur sur la console
void gotoxy(int x,int y)
{
	if (x < 0)
	{
		x = 0;
		//printf("erreur x\n");
	}
	if (y < 0)
	{
		y = 0;
		//printf("erreur y\n");
	}

    printf("%c[%d;%df",0x1B,y,x);
}

// fonction qui sert à diriger vers la bonne fonction pour initialiser la structure
void chargerImage(char nom[], picture* image)
{
	char copie[50], last[50];

	strcpy(copie, nom);
	strtok(copie, ".");
	strcpy(last, strtok(NULL, ".")); // on récupère l'extension du fichier

	// en fonction de l'extension on redirige soit vers la fonction qui charge les pbm soit les ppm
	if (strcmp(last, "pbm") == 0)
	{
		chargerPBM(nom, image);
	}
	else if (strcmp(last, "ppm") == 0)
	{
		chargerPPM(nom, image);
	}
}

// fonction qui sert à initialiser la structure pour un pbm
void chargerPBM(char nom[], picture* image)
{
	FILE* fichier = fopen(nom, "r"); // on ouvre l'image
	int caractereActuel = 0, compteur = 0;

	if (fichier == NULL) // si l'image n'a pas été ouverte, on quitte le programme
	{
		exit(EXIT_FAILURE);
	}

	// on récupère le format puis le format de l'image (hauteur/largeur)
	fscanf(fichier, "%s", image->format);
	fgetc(fichier);
	fscanf(fichier, "%d", &image->largeur);
	fgetc(fichier);
	fscanf(fichier, "%d", &image->hauteur);

	// on alloue dynamiquement un tableau en fonction du format de l'image
	//image->donnee = mmap(NULL, image->hauteur * image->largeur * sizeof(int*), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

	do // on parcourt tous les caractères de l'image, en fonction du numéro du bit on met la couleur en noir ou en blanc
	{
		caractereActuel = fgetc(fichier);
		if (caractereActuel == 48)
		{
			image->donnee2[compteur] = 0;
			compteur++;
		}
		else if (caractereActuel == 49)
		{
			image->donnee2[compteur] = 15;
			compteur++;
		}
	}while(caractereActuel != EOF); // quand il n'y a plus rien dans le fichier, on sort de la boucle

	fclose(fichier); // on ferme ensuite l'image
}

// fonction qui sert à initialiser la structure pour un ppm
void chargerPPM(char nom[], picture* image)
{
	FILE* fichier = fopen(nom, "r"); // on ouvre l'image
	int nombreCouleur = 0, pixelR = 0, pixelG = 0, pixelB = 0, i = 0;

	if (fichier == NULL) // si l'ouverture a échouée, on quitte
	{
		exit(EXIT_FAILURE);
	}

	// on récupère le format de l'image ainsi que ses dimensions et le nombre de niveau de couleur
	fscanf(fichier, "%s", image->format);
	fgetc(fichier);
	fscanf(fichier, "%d", &image->largeur);
	fgetc(fichier);
	fscanf(fichier, "%d", &image->hauteur);
	fgetc(fichier);
	fscanf(fichier, "%d", &nombreCouleur);

	//image->donnee = malloc(image->hauteur * image->largeur * sizeof(int)); // on alloue dynamiquement en fonction des dimensions

	// on parcourt l'image et on récupère le RGB de chaque pixel
	for (i = 0 ; i < image->hauteur * image->largeur ; i++)
	{
		fgetc(fichier);
		fscanf(fichier, "%d", &pixelR);
		fgetc(fichier);
		fscanf(fichier, "%d", &pixelG);
		fgetc(fichier);
		fscanf(fichier, "%d", &pixelB);

		// on envoie le code RGB à une fonction qui va essayer de trouver la couleur la plus approchante
		image->donnee2[i] = findCouleur((pixelR * 255) / nombreCouleur, (pixelG * 255) / nombreCouleur, (pixelB * 255) / nombreCouleur);
	}

	fclose(fichier); // on ferme l'image
}

// fonction qui cherche la couleur la plus proche
int findCouleur(int pixelR, int pixelG, int pixelB)
{
	int couleurMin = 0, ecartCouleurMin = 0, i = 0;
	int ecartCouleur[16] = {0};

	// on calcul l'écart entre la couleur envoyée et les différentes couleurs affichées par linux
	ecartCouleur[0] = ((46 - pixelR) * (46 - pixelR)) + ((52 - pixelG) * (52 - pixelG)) + ((54 - pixelB) * (54 - pixelB));
	ecartCouleur[1] = ((204 - pixelR) * (204 - pixelR)) + ((0 - pixelG) * (0 - pixelG)) + ((0 - pixelB) * (0 - pixelB));
	ecartCouleur[2] = ((78 - pixelR) * (78 - pixelR)) + ((154 - pixelG) * (154 - pixelG)) + ((6 - pixelB) * (6 - pixelB));
	ecartCouleur[3] = ((196 - pixelR) * (196 - pixelR)) + ((160 - pixelG) * (160 - pixelG)) + ((0 - pixelB) * (0 - pixelB));
	ecartCouleur[4] = ((52 - pixelR) * (52 - pixelR)) + ((101 - pixelG) * (101 - pixelG)) + ((164 - pixelB) * (164 - pixelB));
	ecartCouleur[5] = ((117 - pixelR) * (117 - pixelR)) + ((80 - pixelG) * (80 - pixelG)) + ((123 - pixelB) * (123 - pixelB));
	ecartCouleur[6] = ((6 - pixelR) * (6 - pixelR)) + ((152 - pixelG) * (152 - pixelG)) + ((154 - pixelB) * (154 - pixelB));
	ecartCouleur[7] = ((211 - pixelR) * (211 - pixelR)) + ((215 - pixelG) * (215 - pixelG)) + ((207 - pixelB) * (207 - pixelB));
	ecartCouleur[8] = ((85 - pixelR) * (85 - pixelR)) + ((87 - pixelG) * (87 - pixelG)) + ((83 - pixelB) * (83 - pixelB));
	ecartCouleur[9] = ((239 - pixelR) * (239 - pixelR)) + ((41 - pixelG) * (41 - pixelG)) + ((41 - pixelB) * (41 - pixelB));
	ecartCouleur[10] = ((138 - pixelR) * (138 - pixelR)) + ((226 - pixelG) * (226 - pixelG)) + ((52 - pixelB) * (52 - pixelB));
	ecartCouleur[11] = ((252 - pixelR) * (252 - pixelR)) + ((233 - pixelG) * (233 - pixelG)) + ((79 - pixelB) * (79 - pixelB));
	ecartCouleur[12] = ((114 - pixelR) * (114 - pixelR)) + ((159 - pixelG) * (159 - pixelG)) + ((207 - pixelB) * (207 - pixelB));
	ecartCouleur[13] = ((173 - pixelR) * (173 - pixelR)) + ((127 - pixelG) * (127 - pixelG)) + ((168 - pixelB) * (168 - pixelB));
	ecartCouleur[14] = ((52 - pixelR) * (52 - pixelR)) + ((226 - pixelG) * (226 - pixelG)) + ((226 - pixelB) * (226 - pixelB));
	ecartCouleur[15] = ((238 - pixelR) * (238 - pixelR)) + ((238 - pixelG) * (238 - pixelG)) + ((236 - pixelB) * (236 - pixelB));

	ecartCouleurMin = ecartCouleur[0];

	for (i = 0 ; i < 16 ; i++) // on regarde quelle est la couleur pour laquelle il y a le moins de différence
	{
		if (ecartCouleur[i] < ecartCouleurMin)
		{
			ecartCouleurMin = ecartCouleur[i];
			couleurMin = i;
		}
	}

	return couleurMin; // on retourne le numéro de la couleur corespondante
}

// fonction qui gère l'affichage des images. On peut rogner l'image à afficher en fonction de xDebut/xFin/yDebut/yFin
void afficherImage(picture image, int xDebut, int xFin, int yDebut, int yFin)
{
	int i = 0, j = 0;

	// on parcourt chacun des bits de l'image
	for (i = yDebut ; i < yFin ; i++)
	{
		// le goto sert à mettre l'image au centre de la position demandée
		gotoxy((image.posx - image.largeur) + xDebut, (image.posy - (image.hauteur / 2) + i));

		for (j = xDebut ; j < xFin ; j++)
		{
			// selon le numéro récupéré, on affiche une couleur différente
			switch (image.donnee[(i * image.largeur) + j])
			{
				case 0:
					printf(ANSI_COLOR_BLACK "  " ANSI_COLOR_RESET "");
					break;
				case 1:
					printf(ANSI_COLOR_RED "%c%c" ANSI_COLOR_RESET "", 219, 219);
					break;
				case 2:
					printf(ANSI_COLOR_GREEN "%c%c" ANSI_COLOR_RESET "", 219, 219);
					break;
				case 3:
					printf(ANSI_COLOR_BROWN "%c%c" ANSI_COLOR_RESET "", 219, 219);
					break;
				case 4:
					printf(ANSI_COLOR_BLUE "%c%c" ANSI_COLOR_RESET "", 219, 219);
					break;
				case 5:
					printf(ANSI_COLOR_MAGENTA "%c%c" ANSI_COLOR_RESET "", 219, 219);
					break;
				case 6:
					printf(ANSI_COLOR_CYAN "%c%c" ANSI_COLOR_RESET "", 219, 219);
					break;
				case 7:
					printf(ANSI_COLOR_GRAY "%c%c" ANSI_COLOR_RESET "", 219, 219);
					break;
				case 8:
					printf(ANSI_COLOR_DARK_GRAY "%c%c" ANSI_COLOR_RESET "", 219, 219);
					break;
				case 9:
					printf(ANSI_COLOR_LIGHT_RED "%c%c" ANSI_COLOR_RESET "", 219, 219);
					break;
				case 10:
					printf(ANSI_COLOR_LIGHT_GREEN "%c%c" ANSI_COLOR_RESET "", 219, 219);
					break;
				case 11:
					printf(ANSI_COLOR_YELLOW "%c%c" ANSI_COLOR_RESET "", 219, 219);
					break;
				case 12:
					printf(ANSI_COLOR_LIGHT_BLUE "%c%c" ANSI_COLOR_RESET "", 219, 219);
					break;
				case 13:
					printf(ANSI_COLOR_LIGHT_MAGENTA "%c%c" ANSI_COLOR_RESET "", 219, 219);
					break;
				case 14:
					printf(ANSI_COLOR_LIGHT_CYAN "%c%c" ANSI_COLOR_RESET "", 219, 219);
					break;
				case 15:
					printf(ANSI_COLOR_WHITE "%c%c" ANSI_COLOR_RESET "", 219, 219);
					break;
			}
		}
	} 
}