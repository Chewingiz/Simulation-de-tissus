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

void calculer_forces_totale_maj_vitesses(Poids* tableau_poids, int taille_tableau_poids, int tableau_ressorts[][2], int taille_tableau_ressorts,float longueur_ressort_repos, float viscosite, float rayon, Vector3 autres_forces, float k) {
    int i;
    float force_pesanteur;
    Vector3* force_totales;
    Vector3 force_resist, force_tmp;
    force_totales = mycalloc(taille_tableau_poids); 

    tables_forces_ressorts(tableau_poids, tableau_ressorts, taille_tableau_ressorts, k, longueur_ressort_repos, force_totales);
    for (i = 0;i < taille_tableau_poids;i++){
        printf("\n forces tab =[%f,%f,%f]\n",force_totales[i].x,force_totales[i].y,force_totales[i].z );
    }
    force_pesanteur = -tableau_poids[0].masse * g;
    for (i = 0;i < taille_tableau_poids;i++){
        force_resist = f_resist(viscosite, rayon, tableau_poids[i].vitesse_instantanee);// vitesse précédente
        printf("\n resist =[%f,%f,%f]\n",force_resist.x,force_resist.y,force_resist.z );
        //force_pesanteur = f_pesanteur(tableau_poids[i].masse);// La masse est constante donc je ne le fait qu'une fois(en cas de masses différentes, le faire dans la boucle)
        force_resist.z += force_pesanteur;
        printf("\n resist2 =[%f,%f,%f]\n",force_resist.x,force_resist.y,force_resist.z );
        printf("\n force_pesanteur =[%f]\n",force_pesanteur );
        force_tmp = add(force_resist, autres_forces);
        force_totales[i] = add(force_tmp, force_totales[i]);
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
/*
int main() {
    Poids * tableau;
    Vector3 * tableau_position_poids_fixes;// de taille taille_tableau_index_poids_fixes
    int * tableau_index_poids_fixes;
    int taille_tableau, i, taille_tableau_index_poids_fixes;
    float temps_total_simulation = 10.5, t ;*/



/*

int main() {
    // Exemple de données de test
    int taille_tableau_poids = 4;
    Poids tableau_poids[taille_tableau_poids];
    int tableau_ressorts[4][2] = {{0, 1},{0, 2},{1, 3},{2, 3}};
    int taille_tableau_ressorts = 4;
    float longueur_ressort_repos = 1.0;
    float viscosite = 0.5;
    float rayon = 0.1;
    Vector3 autres_forces = {0.0, 0.0, 0.0};
    float k = 100.0;

    // Initialisation des valeurs des poids
    tableau_poids[0].position.x = 0.0;
    tableau_poids[0].position.y = 0.0;
    tableau_poids[0].position.z = 1.0;
    tableau_poids[0].masse = 1.0;
    tableau_poids[0].vitesse_instantanee.x = 0.0;
    tableau_poids[0].vitesse_instantanee.y = 0.0;
    tableau_poids[0].vitesse_instantanee.z = 0.0;

    tableau_poids[1].position.x = 1.0;
    tableau_poids[1].position.y = 0.0;
    tableau_poids[1].position.z = 1.0;
    tableau_poids[1].masse = 0.5;
    tableau_poids[1].vitesse_instantanee.x = 0.0;
    tableau_poids[1].vitesse_instantanee.y = 0.0;
    tableau_poids[1].vitesse_instantanee.z = 0.0;

    tableau_poids[2].position.x = 0.0;
    tableau_poids[2].position.y = 0.0;
    tableau_poids[2].position.z = 0.0;
    tableau_poids[2].masse = 0.5;
    tableau_poids[2].vitesse_instantanee.x = 0.0;
    tableau_poids[2].vitesse_instantanee.y = 0.0;
    tableau_poids[2].vitesse_instantanee.z = 0.0;

    tableau_poids[3].position.x = 1.0;
    tableau_poids[3].position.y = 0.0;
    tableau_poids[3].position.z = 0.0;
    tableau_poids[3].masse = 0.5;
    tableau_poids[3].vitesse_instantanee.x = 0.0;
    tableau_poids[3].vitesse_instantanee.y = 0.0;
    tableau_poids[3].vitesse_instantanee.z = 0.0;

    // Appel de la fonction calculer_forces_totale_maj_vitesses
    for(int i=0 ; i < 100;i++){

    calculer_forces_totale_maj_vitesses(tableau_poids, taille_tableau_poids, tableau_ressorts, taille_tableau_ressorts, longueur_ressort_repos, viscosite, rayon, autres_forces, k);

    // Affichage des résultats après l'appel de la fonction
    for (int i = 0; i < taille_tableau_poids; i++) {
        printf("Poids %d : vitesse = (%f, %f, %f)\n", i, tableau_poids[i].vitesse_instantanee.x, tableau_poids[i].vitesse_instantanee.y, tableau_poids[i].vitesse_instantanee.z);
    }
    maj_positions(tableau_poids, 2);


    tableau_poids[0].position.x = 0.0;
    tableau_poids[0].position.y = 0.0;
    tableau_poids[0].position.z = 1.0;
    tableau_poids[1].vitesse_instantanee.x = 0.0;
    afficher_positions_tableau(tableau_poids, 4);
    //afficher_positions_tableau(tableau_poids, 2);
    }


   /* Vector3 pa = {0.0, 0.0, 0.0};
    Vector3 pb = {1.0, 1.0, 1.0};
    float k = 1.0;
    float length_rest = 0.5;
    Vector3 spring_force = calculs_ressort(pa, pb, k, length_rest);
    printf("Spring force: (%f, %f, %f) \n", spring_force.x, spring_force.y, spring_force.z);

    // Test the vector addition function
    Vector3 a = {1.0, 2.0, 3.0};
    Vector3 b = {4.0, 5.0, 6.0};
    Vector3 sum = add(a, b);
    printf("Vector sum: (%f, %f, %f)\n", sum.x, sum.y, sum.z);

    // Test the vector subtraction function
    Vector3 c = {3.0, 2.0, 1.0};
    Vector3 d = {1.0, 2.0, 3.0};
    Vector3 diff = sub(c, d);
    printf("Vector difference: (%f, %f, %f)\n", diff.x, diff.y, diff.z);

    // Test the forces on the springs using a table of weights
    Poids weights[3];
    weights[0].position.x = 0.0;
    weights[0].position.y = 0.0;
    weights[0].position.z = 0.0;
    weights[1].position.x = 0.0;
    weights[1].position.y = 1.0;
    weights[1].position.z = 0.0;
    weights[2].position.x = 0.0;
    weights[2].position.y = 2.0;
    weights[2].position.z = 0.0;


    int springs[][2] = {{0, 1},{1, 2}};
    int num_springs = 2;

    Vector3 total_forces[3] = {{0.0, 0.0, 0.0},{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}};

    tables_forces_ressorts(weights, springs, num_springs, k, length_rest, total_forces);

    printf("Total forces on weight 0: (%f, %f, %f)\n", total_forces[0].x, total_forces[0].y, total_forces[0].z);
    printf("Total forces on weight 1: (%f, %f, %f)\n", total_forces[1].x, total_forces[1].y, total_forces[1].z);
    printf("Total forces on weight 2: (%f, %f, %f)\n", total_forces[2].x, total_forces[2].y, total_forces[2].z);

    // Test the fluid friction force
    float viscosity = 0.1;
    float radius = 0.5;
    Vector3 velocity = {1.0, 2.0, 3.0};
    Vector3 friction_force = f_resist(viscosity, radius, velocity);
    printf("Fluid friction force: (%f, %f, %f)\n", friction_force.x, friction_force.y, friction_force.z);

    // Test the gravitational force
    float mass = 1.0;
    Vector3 gravity_force = f_pesanteur(mass);
    printf("Gravitational force: (%f, %f, %f)\n", gravity_force.x, gravity_force.y, gravity_force.z);
*/
   /* Poids * tableau;
    int taille_tableau = 9,i;
    Ressorts R;
    R.longueur = 3;
    float t;
    //int tableau_ressorts[12];
    int taille_tableau_ressorts = 12;
    //tableau_ressorts = (int*)malloc(taille_tableau_ressorts * sizeof(int));
    int tableau_ressorts[12][2] = {{0, 1}, {1, 2}, 
                                {3, 4}, {4, 5}, 
                                {6, 7}, {7, 8}, 
                                {0, 3}, {3, 6}, 
                                {1, 4}, {4, 7}, 
                                {2, 5}, {5, 8}};
    int k = 100;
    Vector3 vent;
    vent.x = 0;
    vent.y = 0;
    vent.z = 0;
    tableau = init_tableau_exemple(taille_tableau, R.longueur);
    float temps_total_simulation = 1;// en secondes

    int taille_tableau_index_poids_fixes = 2;
    int tableau_index_poids_fixes[2]= {0,1};
    Vector3 * tableau_position_poids_fixes;// de taille taille_tableau_index_poids_fixes
    tableau_position_poids_fixes = mymalloc(taille_tableau_index_poids_fixes);
    for(i = 0;i<taille_tableau_index_poids_fixes;i++){ // recuperation de toutes les positions des poids fixes
        if(tableau_index_poids_fixes[i]<taille_tableau){
            tableau_position_poids_fixes[i] = tableau[tableau_index_poids_fixes[i]].position;
        }else{
            printf("Erreur de modèle : le poids fixes à la position %d n'existe pas dans le modèle.", i);
        }

    }*/

    //int x = 5;

    /*Boucle principale*/
   // for(t = 0; t <= temps_total_simulation; t+=DT){

   /* }
    printf("Positions initiale :\n");
    afficher_positions_tableau(tableau, taille_tableau);
    printf("\n\n");
    afficher_positions_tableau(tableau, taille_tableau);
    printf("\n");
    for(t = 0; t <= temps_total_simulation; t+=DT){

        // maj vitesses
        printf("Vitesses :\n");
        calculer_forces_totale_maj_vitesses(tableau, taille_tableau, tableau_ressorts, taille_tableau_ressorts, R.longueur, 0.5, 0.1, vent, k);
        afficher_vitesse_tableau(tableau, taille_tableau);
        printf("\n");

        // maj positions
        //maj_positions(tableau, taille_tableau);

        printf("Position avant reinit :\n");
        afficher_positions_tableau(tableau, taille_tableau);
        printf("\n");

        //réinitialisation des positions des points fixes.
        for(i=0; i<taille_tableau_index_poids_fixes; i++){
            printf("OK\n");
            tableau[tableau_index_poids_fixes[i]].position = tableau_position_poids_fixes[i];
            printf("%f\n",tableau[tableau_index_poids_fixes[i]].position.x);
        }

        printf("Position finale :\n");
        afficher_positions_tableau(tableau, taille_tableau);
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
    afficher_vitesse_tableau(tableau, x);
    return 0;
}

        printf("\n\n");
    }

    free(tableau_position_poids_fixes);
    free(tableau);
    return 0;
}*/

