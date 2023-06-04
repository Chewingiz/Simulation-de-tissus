#include <stdio.h>
#include <stdlib.h>

typedef float A;
#define DT 0.001 // intervale de temps entre les images 
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
    float longueur;
} Ressorts;

typedef struct {
    A delta_t_;
    Vector3 gravite;
    int nmb_forces;
    A *FE;
} Env;

Poids* init_tableau_exemple(int x, float longueur) ;
void afficher_positions_tableau(Poids * tableau, int taille_x);
void afficher_vitesse_tableau(Poids * tableau, int taille_x);
