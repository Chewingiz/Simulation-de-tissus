#include <stdio.h>
#include <stdlib.h>

typedef float A;
#define DT 0.1 // intervale de temps entre les images 
#define g 9.800908285 // constante gravitationnelle

typedef struct {
    A x;
    A y;
    A z;
} Vector3;

typedef struct {
    Vector3 position;
    A masse;
    Vector3 vitesse_instantanee;  
} Poids;

typedef struct {
    A force_rappel;
    A constante_rappel;
    int longueur;
} Ressorts;

typedef struct {
    A delta_t_;
    Vector3 forces_exterieures;
    A viscosite;
    //int nmb_forces;
    //A *FE;
} Env;

typedef struct {
    int taille_x;
    int taille_y;
    A rayon_poids;
    Poids* tableau;
    /*Ressorts*/
    int ** tableau_ressorts;
    int taille_tableau_ressorts;
    A k ; //constante de raideur du ressort
    A longueur_ressorts_repos;
    /*Poids fixes*/
    int * liste_index_poids_fixes;
    int nb_poids_fixes;
} Modele;

typedef struct {
    A gravite; // La masse est la même pour tout les poids
    int taille_tableau;
    Vector3 * position_poids_fixes;
} Complement_Modele; // Paramètres calculés une seule fois pour limiter le nombre de calculs


Poids* init_tableau_exemple(int x,int y, float longueur) ;
void afficher_positions_tableau(Poids * tableau, int taille_x);
void afficher_vitesse_tableau(Poids * tableau, int taille_x);