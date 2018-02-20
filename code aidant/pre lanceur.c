#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <string.h>
#include <dirent.h>

int main(int argc, char const *argv[])
{

	time_t secondes;
	struct tm instant;
	const int MAX = 3, MIN = 1;
	int screenSaver;
	char heure[30] = "", chemin[150] = "", nomFichier[200] = "", *tampon, cheminEXE[200] = "";
	int nombreImage = 0, imageChoisie = 0, compteur = 0;
	
	FILE* fichier = fopen("historique.txt", "a");
	DIR* rep = NULL;	
	struct dirent *contenuDossier = NULL;

	if (fichier == NULL)
	{
		return EXIT_FAILURE;
	}
	
	srand(time(NULL));

	time(&secondes);
	instant=*localtime(&secondes);

	sprintf(heure,"%d/%d/%d %02d:%02d:%02d",instant.tm_mday, instant.tm_mon+1, instant.tm_year+1900, instant.tm_hour, instant.tm_min, instant.tm_sec);

	screenSaver = (rand()%(MAX-MIN+1)) + MIN;

	// ##################################################################

	//system("clear");
	
	if (screenSaver == 1)
	{
		strcpy(chemin,getenv ("EXIASAVER1_PBM"));
		if (strcmp(chemin,"") == 0)
		{
			strcpy(chemin,".");
		}

		
		rep = opendir(chemin);	
		if (rep == NULL)
		{
			printf("Erreur de chemin\n");
			return EXIT_FAILURE;
		}	
		
		while ((contenuDossier = readdir(rep)) != NULL)
		{
			strcpy(nomFichier,contenuDossier->d_name);
			if (strtok(nomFichier,".")!=NULL)
			{
				if (strtok(NULL,".") != NULL)
				{
					strcpy(nomFichier,contenuDossier->d_name);
					tampon=strtok(nomFichier,".");
					tampon=strtok(NULL,".");
					if ((strcmp(tampon,"pbm")==0) || strcmp(tampon,"ppm")==0)
					{
						nombreImage ++;
					}
				}
			}
		}

		closedir(rep);

		if (nombreImage == 0)
		{
			printf("Il n'y a pas d'images dans le dossier defini par la variable d'environnement\n");
			return EXIT_FAILURE;
		}
		
		imageChoisie = (rand()%(nombreImage-MIN+1)) + MIN;
		
		rep = opendir(chemin);
		while (compteur < imageChoisie)
		{
			contenuDossier = readdir(rep);
			strcpy(nomFichier,contenuDossier->d_name);
			if (strtok(nomFichier,".")!=NULL)
			{
				if (strtok(NULL,".") != NULL)
				{
					strcpy(nomFichier,contenuDossier->d_name);
					tampon=strtok(nomFichier,".");
					tampon=strtok(NULL,".");
					if ((strcmp(tampon,"pbm")==0) || strcmp(tampon,"ppm")==0)
					{
						compteur ++;
					}
				}
			}
		}

		fprintf(fichier,"%s;1;%s\n", heure, contenuDossier->d_name);

		if (strcmp(getenv("EXIASAVER_HOME"), "") == 0)
		{
			sprintf(cheminEXE, "./screenSaver1 %s", contenuDossier->d_name);
		}
		else
		{
			sprintf(cheminEXE, "./%s/screenSaver1 %s", getenv("EXIASAVER_HOME"), contenuDossier->d_name);
		}
		system(cheminEXE);
		
	}


	else if(screenSaver == 2)
	{
		char tailleChiffre[20] = "";

		strcpy(tailleChiffre, getenv ("EXIASAVER2_TAILLE"));

		if (strcmp(tailleChiffre, "5x3") != 0 && strcmp(tailleChiffre, "9x5") != 0)
		{
			strcpy(tailleChiffre, "5x3");
		}

		fprintf(fichier,"%s;2;%s\n", heure, tailleChiffre);


		if (strcmp(getenv("EXIASAVER_HOME"), "") == 0)
		{
			sprintf(cheminEXE, "./screenSaver2");
		}
		else
		{
			sprintf(cheminEXE, "./%s/screenSaver2", getenv("EXIASAVER_HOME"));
		}
		system(cheminEXE);
	}


	else if(screenSaver == 3)
	{
		int largeur = 0, hauteur = 0;
		struct winsize w;
		ioctl(0, TIOCGWINSZ, &w);

		hauteur = (rand()%(w.ws_row-MIN+1)) + MIN;
		largeur = (rand()%(w.ws_col-MIN+1)) + MIN;

		fprintf(fichier,"%s;3;%dx%d\n", heure, largeur, hauteur);

		if (strcmp(getenv("EXIASAVER_HOME"), "") == 0)
		{
			sprintf(cheminEXE, "./screenSaver3 %dx%d", largeur, hauteur);
		}
		else
		{
			sprintf(cheminEXE, "./%s/screenSaver3 %dx%d", getenv("EXIASAVER_HOME"), largeur, hauteur);
		}
		system(cheminEXE);
	}

	fclose(fichier);
	return 0;
}