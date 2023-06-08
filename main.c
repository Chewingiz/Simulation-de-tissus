#include "main.h"

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

Vector3 * mycalloc(int taille_tableau_poids) {
    Vector3 * a;
    a = calloc(taille_tableau_poids,sizeof(Vector3));
    if(a == NULL) {
        printf("Erreur calloc\n");
        exit(1);
    }
    return a;
}

Vector3 * mymalloc(int taille_tableau_poids) {
    Vector3 * a;
    a = malloc(sizeof(Vector3)*taille_tableau_poids);
    if(a == NULL) {
        printf("Erreur malloc\n");
        exit(1);
    }
    return a;
}

void calculer_forces_totale_maj_vitesses(Poids* tableau_poids, int taille_tableau_poids, int** tableau_ressorts, int taille_tableau_ressorts,float longueur_ressort_repos, float viscosite, float rayon, Vector3 autres_forces, float k) {
    int i;
    float force_pesanteur;
    Vector3* force_totales;
    Vector3 force_resist, force_tmp;
    force_totales = mycalloc(taille_tableau_poids); 

    tables_forces_ressorts(tableau_poids, tableau_ressorts, taille_tableau_ressorts, k, longueur_ressort_repos, force_totales);
    /*for (i = 0;i < taille_tableau_poids;i++){
        printf("\n forces tab =[%f,%f,%f]\n",force_totales[i].x,force_totales[i].y,force_totales[i].z );
    }*/
    force_pesanteur = -tableau_poids[0].masse * g;
    for (i = 0;i < taille_tableau_poids;i++){
        force_resist = f_resist(viscosite, rayon, tableau_poids[i].vitesse_instantanee);// vitesse précédente
        //printf("\n resist =[%f,%f,%f]\n",force_resist.x,force_resist.y,force_resist.z );
        //force_pesanteur = f_pesanteur(tableau_poids[i].masse);// La masse est constante donc je ne le fait qu'une fois(en cas de masses différentes, le faire dans la boucle)
        force_resist.z += force_pesanteur;
        //printf("\n resist2 =[%f,%f,%f]\n",force_resist.x,force_resist.y,force_resist.z );
        //printf("\n force_pesanteur =[%f]\n",force_pesanteur );
        force_tmp = add(force_resist, autres_forces);
        force_totales[i] = add(force_tmp, force_totales[i]);
        //Mise à jours des vitesses pour ne pas àvoir à iterer une seconde fois sur la liste
        maj_vitesse(&tableau_poids[i], force_totales[i]);
    }
    free(force_totales);
}
