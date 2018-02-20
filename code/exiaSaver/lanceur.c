#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include "lanceur.h"

// Fonction lancé par exiaSaver quand il n'y a qu'un seul argument
void lanceur()
{
	time_t secondes;
	struct tm date;
	const int MAX = 3, MIN = 1;
	int screenSaver;
	char heure[30] = "";
	
	FILE* fichier = fopen("historique.txt", "a"); // On ouvre l'historique en mode ajout

	if (fichier == NULL) // Si l'ouverture de l'historique a échoué, on quitte le programme
	{
		exit(EXIT_FAILURE);
	}
	
	srand(time(NULL)); // On initialise l'aléatoire

	time(&secondes); // On récupères le nombre de seconde écoulé depuis le 1er janvier 1970
	date = *localtime(&secondes); // On converti ces secondes en date

	// On met la date en forme dans une chaine de caractère
	sprintf(heure,"%d/%d/%d %02d:%02d:%02d",date.tm_mday, date.tm_mon+1, date.tm_year+1900, date.tm_hour, date.tm_min, date.tm_sec);

	screenSaver = (rand()%(MAX-MIN+1)) + MIN; // Aléatoire pour savoir quel screenSaver lancer

	switch (screenSaver) // On lance le screenSaver correspondant
	{
		case 1:
			screenSaver1(heure, fichier);
			break;
		case 2:
			screenSaver2(heure, fichier);
			break;
		case 3:
			screenSaver3(heure, fichier);
			break;
	}
}

// Fonction lancé pour le screenSaver1
void screenSaver1(char heure[], FILE* fichier)
{
	char chemin[150] = "", nomFichier[200] = "", tampon[40] = "", cheminEXE[200] = "";
	int nombreImage = 0, imageChoisie = 0, compteur = 0, MIN = 1;
	DIR* rep = NULL; // On initialise un pointeur de dossier
	struct dirent *contenuDossier = NULL; // pointeur de contenu du dossier

	strcpy(chemin,getenv ("EXIASAVER1_PBM")); // On récupère le chemin l'adresse des images dans la variable d'env
	if (strcmp(chemin,"") == 0) // Si la variable n'est pas défini ou égale à NULL, on met le chemin en mode local
	{
		strcpy(chemin,".");
	}

	
	rep = opendir(chemin); // On ouvre le dossier contenant les images
	if (rep == NULL) // Si le dossier ne s'ouvre pas, on quitte le programme
	{
		printf("Erreur de chemin\n");
		exit(EXIT_FAILURE);
	}	
	
	while ((contenuDossier = readdir(rep)) != NULL) // On parcourt tous les éléments du dossier
	{
		strcpy(nomFichier,contenuDossier->d_name); // On récupère le nom de l'élément actuellement parcouru
		if ((strtok(nomFichier,".")!=NULL) && (strtok(NULL,".") != NULL)) // On regarde si l'élement est de forme "nom.extension"
		{ // Si c'est le cas, on regarde quelle est l'extension
			strcpy(nomFichier,contenuDossier->d_name);
			strtok(nomFichier,".");
			strcpy(tampon, strtok(NULL,"."));
			if ((strcmp(tampon,"pbm")==0) || strcmp(tampon,"ppm")==0)
			{
				nombreImage ++; // On compte ainsi le nombre d'images dans le dossier
			}
		}
	}

	closedir(rep); // on referme le dossier

	if (nombreImage == 0) // s'il n'y a pas d'images dans le dossier, on quitte le programme
	{
		printf("Il n'y a pas d'images dans le dossier defini par la variable d'environnement\n");
		exit(EXIT_FAILURE);
	}
	
	imageChoisie = (rand()%(nombreImage - MIN + 1)) + MIN; // on fait un aléa entre 1 et le nombre d'image trouvées
	
	rep = opendir(chemin); // on réouvre le dossier
	while (compteur < imageChoisie) // on parcourt les éléments jusqu'à la Nième, N étant le numéro de l'aléa
	{
		contenuDossier = readdir(rep);
		strcpy(nomFichier,contenuDossier->d_name);
		if ((strtok(nomFichier,".")!=NULL) && (strtok(NULL,".") != NULL))
		{
			strcpy(nomFichier,contenuDossier->d_name);
			strtok(nomFichier,".");
			strcpy(tampon, strtok(NULL,"."));
			if ((strcmp(tampon,"pbm")==0) || strcmp(tampon,"ppm")==0)
			{
				compteur ++;
			}
		}
	}

	fprintf(fichier,"%s;1;%s\n", heure, contenuDossier->d_name); // on écrit dans l'historique

	if (strcmp(getenv("EXIASAVER_HOME"), "") == 0) // On récupère le chemin dans lequel se trouve les exécutables
	{
		sprintf(cheminEXE, "screenSaver1");
	}
	else
	{
		sprintf(cheminEXE, "%s/screenSaver1", getenv("EXIASAVER_HOME"));
	}

	char *arguments[] = { "screenSaver1", contenuDossier->d_name, NULL }; // on défini les arguments qu'on va envoyer à screenSaver1
	execv(cheminEXE, arguments); // On lance sreenSaver1
}

void screenSaver2(char heure[], FILE* fichier)
{
	char cheminEXE[200] = "";
	char tailleChiffre[20] = "";

	strcpy(tailleChiffre, getenv ("EXIASAVER2_TAILLE")); // On récupère la taille des chiffres dans la var d'env

	// On vérifie si la taille récupérée est valable, sinon on la met à "5x3"
	if (strcmp(tailleChiffre, "5x3") != 0 && strcmp(tailleChiffre, "9x5") != 0)
	{
		strcpy(tailleChiffre, "5x3");
	}

	fprintf(fichier,"%s;2;%s\n", heure, tailleChiffre); // on écrit l'historique


	if (strcmp(getenv("EXIASAVER_HOME"), "") == 0) // On récupère le chemin dans lequel se trouve les exécutables
	{
		sprintf(cheminEXE, "screenSaver2");
	}
	else
	{
		sprintf(cheminEXE, "%s/screenSaver2", getenv("EXIASAVER_HOME"));
	}

	char *arguments[] = { "screenSaver2", NULL }; // on défini les arguments qu'on va envoyer à screenSaver2
	execv(cheminEXE, arguments); // On lance screenSaver2
}

void screenSaver3(char heure[], FILE* fichier)
{
	char cheminEXE[200] = "", coordonnees[10] = "";
	int largeur = 0, hauteur = 0, MIN = 1;
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w); // on récupère la taille de la console

	// On fait un aléatoire pour les coordonnées de l'écran
	hauteur = (rand()%(w.ws_row-MIN+1)) + MIN;
	largeur = (rand()%(w.ws_col-MIN+1)) + MIN;

	fprintf(fichier,"%s;3;%dx%d\n", heure, largeur, hauteur); // On écrit l'historique

	if (strcmp(getenv("EXIASAVER_HOME"), "") == 0) // On récupère le chemin des exécutables
	{
		sprintf(cheminEXE, "screenSaver3");
	}
	else
	{
		sprintf(cheminEXE, "%s/screenSaver3", getenv("EXIASAVER_HOME"));
	}

	sprintf(coordonnees, "%dx%d", largeur, hauteur); // on met les coordonnées sous forme de texte

	char *arguments[] = { "screenSaver1", coordonnees, NULL }; // on défini les arguments
	execv(cheminEXE, arguments); // on lance screenSaver3
}