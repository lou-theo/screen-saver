#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include "stat.h"

int menu()
{
	int fin = 1, tampon = 0, choix = 0;

	system("clear");
	system("stty cbreak -echo"); // on met la console en mode "pas besoin de confirmer avec entrer"

	// On fait une boucle qui ne se termine que lorsqu'on appuie sur entrer, la boucle affiche le menu
	while(fin)
	{
		if (choix == 0)
		{
			printf("--> ");
		}
		printf("Screensaver\n");
		if (choix == 1)
		{
			printf("--> ");
		}
		printf("Nom des images\n");
		if (choix == 2)
		{
			printf("--> ");
		}
		printf("Taille de l'heure\n");
		if (choix == 3)
		{
			printf("--> ");
		}
		printf("Historique\n");
		if (choix == 4)
		{
			printf("--> ");
		}
		printf("Quitter\n");

		tampon = getchar(); // On capture le numéro de la touche pressée
		if (tampon == 10) // dans le cas du bouton "entrer"
			{
			fin = 0; // on quitte la boucle
			}
		else if (tampon == 27) // les flèches directionnelles ont 3 numéros, on vérifie donc les 3
		{
			if (getchar() == 91)
			{
				tampon = getchar();
				if (tampon == 66) //fleche bas
				{
					choix++;
					if (choix == 5)
					{
						choix = 0;
					}
				}
				else if (tampon == 65) //fleche haut
				{
					choix--;
					if (choix == -1)
					{
						choix = 4;
					}		
				}
			}
		}
		system("clear");
	}

	system("stty cooked echo"); // on repasse la console en mode "confirmer avec entrer"
	system("clear");
	return choix; // on retourne le numéro du menu
}

void statistique()
{
	FILE* fichier = fopen("historique.txt", "r"); // on ouvre l'historique en mode lecture

	if (fichier == NULL) // si l'ouverture échoue, on quitte le programme
	{
		printf("Auncun historique disponible !\n");
		exit(EXIT_FAILURE);
	}

	switch(menu()) // selon le choix fait au menu, on lance une fonction différente
	{
		case 0:
			optionScreenSaver(fichier);
			break;
		case 1:
			optionImage(fichier);
			break;
		case 2:
			optionTailleHeure(fichier);
			break;
		case 3:
			optionHistorique(fichier);
			break;
	}

}

// dans le cas du choix numéro 0
void optionScreenSaver(FILE* fichier)
{
	char chaine[500] = "";
	int screensaver1 = 0, screensaver2 = 0, screensaver3 = 0;

	printf("\tVous avez choisis screensaver\n\n"); 			

	// On parcourt le fichier phrase par phrase et on regarde le 21ème de chaque phrase qui correspond au numéro du screenSaver lancé
	while (fgets(chaine, 500, fichier) != NULL)
	{			
       if  (atoi(&chaine[20]) == 1 )
	        {
	         screensaver1++;
	        }
       if  (atoi(&chaine[20]) == 2 )
		{
		 screensaver2++;
		}
       if  (atoi(&chaine[20]) == 3 )
		{
		 screensaver3++;
		}
	}
	
	// On affiche le résultat
	printf(" le statique est apparus %d fois\n" , screensaver1);			
	printf(" le dynamique est apparus %d fois\n" , screensaver2);
	printf(" l'interactif est apparus %d fois\n" , screensaver3);
}

// dans le cas du choix numéro 1
void optionImage(FILE* fichier)
{	
	char **nomImage; // tableau dynamique de chaines de caractères
	int *nombreNomImage; // tableau dynamique de nombres
	int nombreImage = 0, i = 0;
	char chemin[150] = "", tampon[40] = "", nomFichier[200] = "", chaine[500];
	DIR* rep = NULL;
	struct dirent *contenuDossier = NULL;

	// On récupère le chemin des images, s'il n'est pas défini on met en local
	strcpy(chemin,getenv ("EXIASAVER1_PBM"));
	if (strcmp(chemin,"") == 0)
	{
		strcpy(chemin,".");
	}

	rep = opendir(chemin); // on ouvre le dossier des images, s'il n'existe pas on quitte
	if (rep == NULL)
	{
		printf("Erreur de chemin\n");
		exit(EXIT_FAILURE);
	}	

	// on parcourt le dossier pour compter le nombre d'images
	while ((contenuDossier = readdir(rep)) != NULL)
	{
		strcpy(nomFichier,contenuDossier->d_name);
		if ((strtok(nomFichier,".")!=NULL) && (strtok(NULL,".") != NULL))
		{
			strcpy(nomFichier,contenuDossier->d_name);
			strtok(nomFichier,".");
			strcpy(tampon, strtok(NULL,"."));
			if ((strcmp(tampon,"pbm")==0) || strcmp(tampon,"ppm")==0)
			{
				nombreImage ++;
			}
		}
	}
	closedir(rep); // on ferme le dossier


	nombreNomImage = malloc(nombreImage * sizeof(int)); // on alloue dynamiquement un tableau d'int en fontion du nombre d'images
	for (i = 0; i < nombreImage; i++) // On initialise chaque valeur à 0
	{
		nombreNomImage[i] = 0;
	}
	// on alloue dynamiquement le tableau de chaine de caractère (chaque case de ce tableau contient un pointeur de char)
	nomImage = (char**)malloc(nombreImage * sizeof(char*));
	// on alloue dynamiquement chaque chaine de caractère, 40 caractères par chaine
	for(i = 0 ; i < nombreImage ; i++)
	{
		nomImage[i]=(char*)malloc(40 * sizeof(char));
	}


	i = 0;
	rep = opendir(chemin); // on ouvre le dossier
	while ((contenuDossier = readdir(rep)) != NULL) // on parcout le dossier
	{
		strcpy(nomFichier,contenuDossier->d_name);
		if ((strtok(nomFichier,".")!=NULL) && (strtok(NULL,".") != NULL))
		{
			strcpy(nomFichier,contenuDossier->d_name);
			strtok(nomFichier,".");
			strcpy(tampon, strtok(NULL,"."));
			if ((strcmp(tampon,"pbm")==0) || strcmp(tampon,"ppm")==0)
			{
				strcpy(nomImage[i], contenuDossier->d_name); // on récupère le nom de chaque image dans le tableau
				i++;
			}
		}
	}
	closedir(rep);



	while (fgets(chaine, 500, fichier) != NULL)  // on parcourt chaque phrase de l'historique
	{			
		strtok(chaine, ";");
		strtok(NULL, ";");
		strcpy(tampon, strtok(NULL, ";"));
		strcpy(tampon, strtok(tampon, "\n"));

		// on regarde le nom de l'image récupérée dans l'historique et on le compare à chacun des noms du dossier
		for (i = 0; i < nombreImage; i++) 
		{
			if (strcmp(tampon, nomImage[i]) == 0) // si le nom est égal alors on incrémente la valeur du tableau correspondante
			{
				nombreNomImage[i]++;
			}
		}
	}

	// on affiche les résultats
	printf("\tVous avez choisis nom des images\n\n");

	if (nombreImage == 0)
	{
		printf("Il n'y a pas d'image dans le dossier defini par la variable d'environnement !\n");
	}

	for (i = 0; i < nombreImage; i++)
	{
		printf("L'image %s est apparue %d fois\n", nomImage[i], nombreNomImage[i]);
	}

	// on libère l'espace mémoire
	for(i = 0 ; i < nombreImage ; i++)
	{
		free(nomImage[i]);
	}
	free(nomImage);
	free(nombreNomImage);
}

// choix numéro 2
void optionTailleHeure(FILE* fichier)
{
	int tailleheure1 = 0, tailleheure2 = 0;
	char chaine[500] = "";

	printf("\tvous avez choisis taille de l'heure\n\n");

	// on procède comme dans le choix 0
	// on parcourt chaque phrase et on regarde le 23ème caractère, significatif de la taille de l'heur
	while (fgets(chaine, 500, fichier) != NULL)
	{			
	    if  (atoi(&chaine[22]) == 5 )
		    {
		     tailleheure1++;
		    }
            if  (atoi(&chaine[22]) == 9)	     
		    {
		     tailleheure2++;
		    }						
	}
	printf("taille heure 1 (5x3) : %d\n",tailleheure1);
	printf("taille heure 2 (9x5) : %d\n",tailleheure2);
}

// choix numéro 3
void optionHistorique(FILE* fichier)
{
	char caractereActuel = 0;

	printf("\tVous avez choisis l'historique\n\n");

	do // on parcourt chaque caractère de l'historique puis on l'affiche
    {
	caractereActuel = fgetc(fichier);
	printf("%c", caractereActuel);
	} while (caractereActuel != EOF);
}