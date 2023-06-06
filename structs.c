#include "structs.h"

/*longueure ressort*/
Poids* init_tableau_exemple(int x, int y, float longueur) {
    Poids* tableau_poids = malloc(sizeof(Poids) * x * y);

    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            Vector3 vitesse;
            vitesse.x = 0.0; vitesse.y = 0.0; vitesse.z = 0.0;

            tableau_poids[i * y + j].position.x = i * longueur;
            tableau_poids[i * y + j].position.y = j * longueur;
            tableau_poids[i * y + j].position.z = 1;

            tableau_poids[i * y + j].vitesse_instantanee = vitesse;
            tableau_poids[i * y + j].masse = 0.5;
        }
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

