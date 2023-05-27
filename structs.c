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
/*
char ** poids(int x, int y, int longueur, Poids P) {
    char ** tableau_poids;
    tableau_poids = malloc(sizeof(char *) * x);
    for(int i = 0; i < x; i++) {
        tableau_poids[i] = malloc(sizeof(char) * y);
    }

    for(int i = 0; i < x; i++) {
        for(int j = 0; j < y; j++) {
            P.position.x = i * longueur;
            P.position.y = j * longueur;
            P.position.z = 0;
            tableau_poids[i][j] = '.';
            printf("%s   ", &tableau_poids[i][j]);
        }
        printf("\n\n");
    }
    return tableau_poids;
}*/

Poids** init_tableau_exemple(int x, int y, float longueur) {
    Poids** tableau_poids = malloc(sizeof(Poids*) * x);

    for (int i = 0; i < x; i++) {
        tableau_poids[i] = malloc(sizeof(Poids) * y);

        for (int j = 0; j < y; j++) {
            tableau_poids[i][j].position.x = i * longueur;
            tableau_poids[i][j].position.y = j * longueur;
            tableau_poids[i][j].position.z = 0;
        }
    }
    return tableau_poids;
}

void afficher_positions_tableau(Poids ** tableau, int taille_x, int taille_y){
    for(int i = 0; i < taille_x; i++) {
        for(int j = 0; j < taille_y; j++) {
            printf("[");
            printf("x:%f, ",tableau[i][j].position.x );
            printf("y:%f, ",tableau[i][j].position.y );
            printf("z:%f,",tableau[i][j].position.z );
            printf("] ");
        }
    printf("\n");
    }
}

int main() {
    int x = 5;
    int y = 5;
    Ressorts R;
    R.longueur = 3;
    Poids P;
    Poids ** tableau;
    tableau = init_tableau_exemple(x , y, 2.3 ) ;
    afficher_positions_tableau(tableau, x , y);
    //tableau = poids(x, y, R.longueur, P);  
    return 0;
}
