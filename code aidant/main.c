#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "image.h"

int main(int argc, char const *argv[])
{
	/*picture image;
	char nom[50];
	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);
	image.posy = w.ws_row / 2;
	image.posx = w.ws_col / 2;

	sprintf(nom, "%s",argv[1]);

	chargerImage(nom, &image);

	system("clear");
	afficherImage(image, 0, image.largeur, 0, image.hauteur);
	
	free(image.donnee);*/

	picture image;
	picture* fond = &image;
	char nom[50];
	struct winsize w;
	pid_t pid = fork();

    int descripteurTube[2];

    if(pipe(descripteurTube) != 0)
    {
        fprintf(stderr, "Erreur de création du tube.\n");
        return EXIT_FAILURE;
    }


	switch (pid)
	{
		case -1:
			return EXIT_FAILURE;
			break;
		case 0:
			ioctl(0, TIOCGWINSZ, &w);
			image.posy = w.ws_row / 2;
			image.posx = w.ws_col / 2;

			sprintf(nom, "%s",argv[1]);

			chargerImage(nom, &image);

			close(descripteurTube[0]);
			write(descripteurTube[1], fond, sizeof(image));

			break;
		default:
			close(descripteurTube[1]);
	        read(descripteurTube[0], fond, sizeof(image));

			wait(NULL);

			break;
	}

	printf("%d %d", fond->largeur, fond->hauteur);


	return 0;
}