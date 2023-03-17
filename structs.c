#include <stdio.h>
#include <stdlib.h>

typedef float A;

typedef struct {
    A x;
    A y;
    A z;
} Vector3;

typedef struct {
    Vector3 position;
    A masse;
    Vector3 vitesse_instantane;  
} Poids;

typedef struct {
    A force_rappel;
    A constante_rappel;
    int longueur;
} Ressorts;

typedef struct {
    A delta_t_;
    Vector3 gravite;
    int nmb_forces;
    A *FE;
} Env;

A ** poids(int x, int y, int longueur, Poids P) {
    A ** tableau_poids;
    tableau_poids = malloc(sizeof(int *) * x);
    for(int i = 0; i < x; i++) {
        tableau_poids[i] = malloc(sizeof(int) * y);
    }

    for(int i = 0; i < x; i++) {
        for(int j = 0; j < y; j++) {
            P.position.x = i * longueur;
            P.position.y = j * longueur;
            P.position.z = 0;
            tableau_poids[i][j] = P.position.x, P.position.y, P.position.z;
        }
    }
    return tableau_poids;
}

void main() {
    int x = 5;
    int y = 5;
    Ressorts R;
    R.longueur = 3;
    Poids P;
    A ** tableau;
    tableau = poids(x, y, R.longueur, P);
    for(int i = 0; i < x; i++) {
        for(int j = 0; j < y; j++) {
            printf("%f ", tableau[i][j]);
        }
        printf("\n");
    }
}