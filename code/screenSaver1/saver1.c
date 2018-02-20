#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "image.h"
#include "saver1.h"

int getk()
{
	int c;

	system("stty cbreak -echo"); // mode saisi à pas confirmer
	c = getchar(); // on attend la pression d'une touche
	system("stty cooked echo"); // mode saisi à confirmer

	system("clear");

	return c;
}