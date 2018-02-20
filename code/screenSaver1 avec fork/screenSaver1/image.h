// on défini les différents codes couleur
#define ANSI_COLOR_BLACK "\033[22;30m"
#define ANSI_COLOR_RED "\033[22;31m"
#define ANSI_COLOR_GREEN "\033[22;32m"
#define ANSI_COLOR_BROWN "\033[22;33m"
#define ANSI_COLOR_BLUE "\033[22;34m"
#define ANSI_COLOR_MAGENTA "\033[22;35m"
#define ANSI_COLOR_CYAN "\033[22;36m"
#define ANSI_COLOR_GRAY "\033[22;37m"
#define ANSI_COLOR_DARK_GRAY "\033[01;30m"
#define ANSI_COLOR_LIGHT_RED "\033[01;31m"
#define ANSI_COLOR_LIGHT_GREEN "\033[01;32m"
#define ANSI_COLOR_YELLOW "\033[01;33m"
#define ANSI_COLOR_LIGHT_BLUE "\033[01;34m"
#define ANSI_COLOR_LIGHT_MAGENTA "\033[01;35m"
#define ANSI_COLOR_LIGHT_CYAN "\033[01;36m"
#define ANSI_COLOR_WHITE "\033[01;37m"
#define ANSI_COLOR_RESET "\x1b[0m"

// initialisation de la structure picture qui va stocker les images
typedef struct picture picture;
struct picture
{
	char format[3];
    int* donnee;
    int donnee2[1000];
    int largeur;
    int hauteur;
    int posx;
    int posy;
};

// prototypes des fonctions
void gotoxy(int x,int y);
void chargerImage(char nom[], picture* image);
void chargerPBM(char nom[], picture* image);
void chargerPPM(char nom[], picture* image);
int findCouleur(int pixelR, int pixelG, int pixelB);
void afficherImage(picture image, int xDebut, int xFin, int yDebut, int yFin);