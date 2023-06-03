#include "forces.h"

void maj_positions(Poids *tableau, int taille_x){
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



/* Calcul de la force totale */
//Vector3 * calculer_
/*Autres forces = autres forces qui s'appliquent sur le tissus (vent)*/

void calculer_forces_totale_maj_vitesses(Poids* tableau_poids, int taille_tableau_poids, int tableau_ressorts[][2], int taille_tableau_ressorts,float longueur_ressort_repos, float viscosite, float rayon, Vector3 autres_forces, float k) {
    int i;
    float force_pesanteur;
    Vector3* force_totales;
    Vector3 force_resist, force_tmp;
    force_totales = malloc(sizeof(Vector3)*taille_tableau_poids); // créé dehors pour ne pas oublier de free ou mettre à jours les vitesses ici pour ne pas avoir à iterer deux fois sur la liste 

    tables_forces_ressorts(tableau_poids, tableau_ressorts, taille_tableau_ressorts, k, longueur_ressort_repos, force_totales);
    force_pesanteur = -tableau_poids[0].masse * g;
    for (i = 0;i < taille_tableau_poids;i++){
        force_resist = f_resist(viscosite, rayon, tableau_poids[i].vitesse_instantanee);// vitesse précédente
        //force_pesanteur = f_pesanteur(tableau_poids[i].masse);// La masse est constante donc je ne le fait qu'une fois(en cas de masses différentes, le faire dans la boucle)
        force_resist.z += force_pesanteur;
        force_tmp = add_sub(force_resist, autres_forces, 1);
        force_totales[i] = add_sub(force_tmp, force_totales[i], 1);

        //Mise à jours des vitesses pour ne pas àvoir à iterer une seconde fois sur la liste
        maj_vitesse(&tableau_poids[i], force_totales[i]);
    }
    free(force_totales);
}
/*p1->p2*/
/*Vector3 deformation(Poids p1, Poids p2){
    Vector3 def;
    def.x = p2.position.x - p1.position.x;
    def.y = p2.position.y - p1.position.y;
    def.z = p2.position.z - p1.position.z;
    
    return def;
}*/

int main() {
    /*Poids * tableau;
    Vector3 * tableau_position_poids_fixes;// de taille taille_tableau_index_poids_fixes
    int * tableau_index_poids_fixes;
    int taille_tableau, i, taille_tableau_index_poids_fixes;
    float temps_total_simulation = 10.5, t ;*/
    
    /*Pour concerver les positions initial des poids qui seront fixes durant la simulation*/
    /*for(i=0; i<taille_tableau_index_poids_fixes; i++){
        if(tableau_index_poids_fixes[i]<taille_tableau){
            tableau_index_poids_fixes[i] = tableau[i].position;
        }else{
            printf("Erreur de modèle : le poids fixes à la position %d n'existe pas dans le modèle.", i);
        }
    }

    int x = 5;*/

    /*Boucle principale*/
    //for(t = 0; t <= temps_total_simulation; t+=DT){
        // maj vitesses
        //calculer_forces_totale_maj_vitesses(Poids* tableau_poids, int taille_tableau_poids, int** tableau_ressorts, int taille_tableau_ressorts,float longueur_ressort_repos, float viscosite, float rayon, Vector3 autres_forces, float k);

        // maj positions
        //maj_positions(tableau, taille_tableau);

        //réinitialisation des positions des points fixes.
        /*for(i=0; i<taille_tableau_index_poids_fixes; i++){
            tableau[tableau_index_poids_fixes[i]].position = tableau_position_poids_fixe[i];
        }*/
    
        // affichage
    //}

    //int y = 5;
    //Ressorts R;
    //R.longueur = 3;
    //Poids P;
    
    /*tableau = init_tableau_exemple(x , 2.3) ;
    afficher_positions_tableau(tableau, x);
    printf("\n");

    afficher_vitesse_tableau(tableau, x);
    //tableau = poids(x, y, R.longueur, P); 
    maj_vitesses(tableau, x);
    printf("\n");
    afficher_vitesse_tableau(tableau, x);*/

    Poids * tableau;
    int x = 9;
    Ressorts R;
    R.longueur = 3;
    //int tableau_ressorts[12];
    int taille_tableau_ressorts = 12;
    //tableau_ressorts = (int*)malloc(taille_tableau_ressorts * sizeof(int));
    int tableau_ressorts[12][2] = {{0, 1}, {1, 2}, 
                                {3, 4}, {4, 5}, 
                                {6, 7}, {7, 8}, 
                                {0, 3}, {3, 6}, 
                                {1, 4}, {4, 7}, 
                                {2, 5}, {5, 8}};
    int k = 15;
    Vector3 vent;
    vent.x = 0;
    vent.y = 0;
    vent.z = 0;
    tableau = init_tableau_exemple(x, R.longueur);
    /*afficher_positions_tableau(tableau, x);
    printf("\n\n");
    calculer_forces_totale_maj_vitesses(tableau, x, tableau_ressorts, taille_tableau_ressorts, R.longueur, 5, 3, vent, k);
    afficher_positions_tableau(tableau, x);
    printf("\n\n");
    calculer_forces_totale_maj_vitesses(tableau, x, tableau_ressorts, taille_tableau_ressorts, R.longueur, 5, 3, vent, k);
    afficher_positions_tableau(tableau, x);
    printf("\n\n");*/
    //afficher_positions_tableau(tableau, x);
    /*printf("\n\n");
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            tableau_ressorts[i][j] = 1;
            printf("%d", tableau_ressorts[i][j]);
        }
        printf("\n");
    }*/
    afficher_positions_tableau(tableau, x);
    calculer_forces_totale_maj_vitesses(tableau, x, tableau_ressorts, taille_tableau_ressorts, R.longueur, 10, 3, vent, k);
    /*for(int i = 0; i < taille_tableau_ressorts; i++) {
        for(int j = 0; j < 2; j++) {
            printf("%d  ", tableau_ressorts[i][j]);
        }
        printf("\n");
    }*/
    printf("\n\n");
    afficher_vitesse_tableau(tableau, x);
    calculer_forces_totale_maj_vitesses(tableau, x, tableau_ressorts, taille_tableau_ressorts, R.longueur, 10, 3, vent, k);
    printf("\n\n");
    afficher_positions_tableau(tableau, x);
    printf("\n\n");
    afficher_vitesse_tableau(tableau, x);
    printf("\n\n");
    return 0;
}
