#include <stdio.h>
#include <stdlib.h>

typedef float A;
#define DT 0.1; // intervale de temps entre les images 

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
    Vector3 vitesse;
    vitesse.x = 0.0; vitesse.y = 0.0; vitesse.z = 0.0;

    for (int i = 0; i < x; i++) {
        tableau_poids[i] = malloc(sizeof(Poids) * y);

        for (int j = 0; j < y; j++) {
            tableau_poids[i][j].position.x = i * longueur;
            tableau_poids[i][j].position.y = j * longueur;
            tableau_poids[i][j].position.z = 0;
            
            tableau_poids[i][j].vitesse_instantanee = vitesse ;
            tableau_poids[i][j].masse = 5 ;
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

void afficher_vitesse_tableau(Poids ** tableau, int taille_x, int taille_y){
    for(int i = 0; i < taille_x; i++) {
        for(int j = 0; j < taille_y; j++) {
            printf("[");
            printf("x:%f, ",tableau[i][j].vitesse_instantanee.x );
            printf("y:%f, ",tableau[i][j].vitesse_instantanee.y );
            printf("z:%f,",tableau[i][j].vitesse_instantanee.z );
            printf("] ");
        }
    printf("\n");
    }
}

void maj_position(Poids ** tableau, int taille_x, int taille_y){
    for(int i = 0; i < taille_x; i++) {
        for(int j = 0; j < taille_y; j++) {
            tableau[i][j].position.x += tableau[i][j].vitesse_instantanee.x * DT;
            tableau[i][j].position.y += tableau[i][j].vitesse_instantanee.y * DT;
            tableau[i][j].position.z += tableau[i][j].vitesse_instantanee.z * DT;
        }
    }
}

void maj_vitesse(Poids* poids, Vector3 force) {
    Vector3 acceleration;
    acceleration.x = force.x / poids->masse;
    acceleration.y = force.y / poids->masse;
    acceleration.z = force.z / poids->masse;

    Vector3 delta_vitesse;
    delta_vitesse.x = acceleration.x * DT;
    delta_vitesse.y = acceleration.y * DT;
    delta_vitesse.z = acceleration.z * DT;

    poids->vitesse_instantanee.x += delta_vitesse.x;
    poids->vitesse_instantanee.y += delta_vitesse.y;
    poids->vitesse_instantanee.z += delta_vitesse.z;
}
void maj_vitesses(Poids ** tableau, int taille_x, int taille_y){
    Vector3 force;
    for(int i = 0; i < taille_x; i++) {
        for(int j = 0; j < taille_y; j++) {
            //Calcule des forces
            force.x = 0.0; force.y = 1.0; force.z = 0.0;
            maj_vitesse(&tableau[i][j], force);
        }
    }
}

int main() {
    int x = 5;
    int y = 5;
    //Ressorts R;
    //R.longueur = 3;
    //Poids P;
    Poids ** tableau;
    tableau = init_tableau_exemple(x , y, 2.3 ) ;
    afficher_positions_tableau(tableau, x , y);
    printf("\n");

    afficher_vitesse_tableau(tableau, x , y);
    //tableau = poids(x, y, R.longueur, P); 
    maj_vitesses(tableau, x , y);
    printf("\n");
    afficher_vitesse_tableau(tableau, x , y);
    return 0;
}
