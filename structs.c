#include "structs.h"

/*longueure ressort*/
Poids* init_tableau_exemple(int x, float longueur) {
    Poids* tableau_poids = malloc(sizeof(Poids)*x);

    for (int i = 0; i < x; i++) {
        Vector3 vitesse;
        vitesse.x = 0.0; vitesse.y = 0.0; vitesse.z = 0.0;

        tableau_poids[i].position.x = i * longueur;
        tableau_poids[i].position.y = i * longueur;
        tableau_poids[i].position.z = 0;
            
        tableau_poids[i].vitesse_instantanee = vitesse ;
        tableau_poids[i].masse = 0.5 ;
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

