#include <stdlib.h>
#include <stdio.h>
#include "lanceur.h"
#include "stat.h"

int main(int argc, char const *argv[])
{
	/* On regarde le nombre d'arguments qui sont passés lorsque le programme est lancé. Par défaut il y a au moins 1 argument 
	(le nom du programme). S'il n'y a qu'un seul argument, c'est donc que l'utilisateur souhaite simplement lancer un screenSaver */
	if (argc == 1)
	{
		lanceur();
	}

	/* Nous sommes ici dans le cas où il n'y a pas qu'un seul argument or comme il ne peut pas y en avaoir moins de 1, c'est qu'il y en a
	plus. On regarde donc si le 2eme correspond à "-stats". Si c'est le cas on envoi les statistiques, sinon rien ne se passe. */
	else if (strcmp(argv[1], "-stats") == 0)
	{
		statistique();
	}

	return 0;
}