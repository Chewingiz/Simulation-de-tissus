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
/*longueure ressort*/
Poids* init_tableau_exemple(int x, float longueur) {
    Poids* tableau_poids = malloc(sizeof(Poids)*x);
    Vector3 vitesse;
    vitesse.x = 0.0; vitesse.y = 0.0; vitesse.z = 0.0;

    for (int i = 0; i < x; i++) {
        tableau_poids[i].position.x = i * longueur;
        tableau_poids[i].position.y = i * longueur;
        tableau_poids[i].position.z = 0;
            
        tableau_poids[i].vitesse_instantanee = vitesse ;
        tableau_poids[i].masse = 5 ;
    }
    return tableau_poids;
}

void afficher_positions_tableau(Poids * tableau, int taille_x){
    for(int i = 0; i < taille_x; i++) {
        printf("[");
        printf("x:%f, ",tableau[i].position.x );
        printf("y:%f, ",tableau[i].position.y );
        printf("z:%f,",tableau[i].position.z );
        printf("] ");
    }
}

void afficher_vitesse_tableau(Poids * tableau, int taille_x){
    for(int i = 0; i < taille_x; i++) {
        printf("[");
        printf("x:%f, ",tableau[i].vitesse_instantanee.x );
        printf("y:%f, ",tableau[i].vitesse_instantanee.y );
        printf("z:%f,",tableau[i].vitesse_instantanee.z );
        printf("] ");
    }
}

void maj_position(Poids *tableau, int taille_x){
    for(int i = 0; i < taille_x; i++) {
        tableau[i].position.x += tableau[i].vitesse_instantanee.x * DT;
        tableau[i].position.y += tableau[i].vitesse_instantanee.y * DT;
        tableau[i].position.z += tableau[i].vitesse_instantanee.z * DT;
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

void maj_vitesses(Poids * tableau, int taille_x){
    Vector3 force;
    //cal forces 
    for(int i = 0; i < taille_x; i++) {
            //Calcule des forces
        force.x = 0.0; force.y = 1.0; force.z = 0.0;
        maj_vitesse(&tableau[i], force);
    }
}
/*p1->p2*/
Vector3 deformation(Poids p1, Poids p2){
    Vector3 def;
    def.x = p2.position.x - p1.position.x;
    def.y = p2.position.y - p1.position.y;
    def.z = p2.position.z - p1.position.z;
    
    return def;
}

int main() {
    int x = 5;
    //int y = 5;
    //Ressorts R;
    //R.longueur = 3;
    //Poids P;
    Poids * tableau;
    tableau = init_tableau_exemple(x , 2.3) ;
    afficher_positions_tableau(tableau, x);
    printf("\n");

    afficher_vitesse_tableau(tableau, x);
    //tableau = poids(x, y, R.longueur, P); 
    maj_vitesses(tableau, x);
    printf("\n");
    afficher_vitesse_tableau(tableau, x);
    return 0;
}
