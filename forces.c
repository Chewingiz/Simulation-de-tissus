#include <stdio.h>
#include <math.h>
#include "structs.h"

/*#define g 9.800908285 // constante gravitationnelle

typedef struct {
    float x;
    float y;
    float z;
} Vector3;*/

/*       Forces intérieures      */
//Ressort
/* Calcul de la force de rappel en Newton 
k : la constante de rappel en Newton par mètre (N/m),
x : le d ́eformation en mètre (m).
*/

Vector3 calculs_ressort(Vector3 Pa, Vector3 Pb, float k, float longueur_ressort_repos) {
    Vector3 direction;
    Vector3 direction_normalisee;
    Vector3 force;

    direction.x = Pb.x - Pa.x;
    direction.y = Pb.y - Pa.y;
    direction.z = Pb.z - Pa.z;

    float distance = sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z); // distance euclidienne 

    direction_normalisee.x = direction.x / distance;
    direction_normalisee.y = direction.y / distance;
    direction_normalisee.z = direction.z / distance;


	float kld = k * (longueur_ressort_repos - distance);
    force.x = (direction_normalisee.x / distance) * kld;
    force.y = (direction_normalisee.y / distance) * kld;
    force.z = (direction_normalisee.z / distance) * kld;

    return force;
}


Vector3 add(Vector3 a, Vector3 b){
    Vector3 res;
    res.x = a.x + b.x;
    res.y = a.y + b.y;
    res.z = a.z + b.z;
    return res;
}
Vector3 sub(Vector3 a, Vector3 b){
    Vector3 res;
    res.x = a.x - b.x;
    res.y = a.y - b.y;
    res.z = a.z - b.z;
    return res;
}

void tables_forces_ressorts(Poids* tableau_poids, int** tableau_ressorts, int taille_tableau_ressorts, float k, float longueur_ressort_repos, Vector3* tableau_forces_total_appliquer_sur_les_points_R) {
    int i;
    int* ressort;
    Vector3 resultat_calcul_force, nouvelle_force_P1, nouvelle_force_P2;
    for(i = 0; i < taille_tableau_ressorts; i++) {
        ressort = tableau_ressorts[i];
        resultat_calcul_force = calculs_ressort(tableau_poids[ressort[0]].position, tableau_poids[ressort[1]].position, k, longueur_ressort_repos);
        nouvelle_force_P1 = add(tableau_forces_total_appliquer_sur_les_points_R[ressort[0]], resultat_calcul_force);
        nouvelle_force_P2 = sub(tableau_forces_total_appliquer_sur_les_points_R[ressort[1]], resultat_calcul_force);
        tableau_forces_total_appliquer_sur_les_points_R[ressort[0]] = nouvelle_force_P1;
        tableau_forces_total_appliquer_sur_les_points_R[ressort[1]] = nouvelle_force_P2;
    }
}
/*
Vector3 f_rappel(float k, Vector3 deformation) {
    Vector3 force_rappel;
    force_rappel.x = -k * deformation.x;
    force_rappel.y = -k * deformation.y;
    force_rappel.z = -k * deformation.z;
    return force_rappel;
}*/

/*       Forces extérieures      */
/*Calcul de la force de frottement du fluide en Newton.
viscosite : la viscosité du fluide en Pascal seconde (P a · s),
r : rayon de la boule en m`etre (m),
v : vitesse de la boule en m`etre par seconde (m.s−1)
*/
Vector3 f_resist(float viscosite, float rayon, Vector3 vitesse) {
    Vector3 force_resist;
    float constante = -6 * M_PI * viscosite * rayon;
    force_resist.x = constante * vitesse.x;
    force_resist.y = constante * vitesse.y;
    force_resist.z = constante * vitesse.z;
    return force_resist;
}


/* Calcul de la force de pesanteur 
m : masse de l'objet
*/
Vector3 f_pesanteur(float masse) {
    Vector3 force_pesanteur;
    force_pesanteur.x = 0.0;
    force_pesanteur.y = 0.0;
    force_pesanteur.z = -masse * g;
    return force_pesanteur;
}

/* Calcul de la force totale */
//Vector3 * calculer_
/*Autres forces = autres forces qui s'appliquent sur le tissus (vent)*/

void calculer_forces_totale(Poids* tableau_poids, int taille_tableau_poids, int** tableau_ressorts, int taille_tableau_ressorts,float longueur_ressort_repos, float viscosite, float rayon, Vector3 autres_forces, float k) {
    int i;
    float force_pesanteur,;
    Vector3* force_totales;
    Vector3 force_resist, force_tmp;
    force_totales = malloc(sizeof(Vector3)*taille_tableau_poids); // créé dehors pour ne pas oublier de free ou mettre à jours les vitesses ici pour ne pas avoir à iterer deux fois sur la liste 

    tables_forces_ressorts(tableau_poids, tableau_ressorts, taille_tableau_ressorts, k, longueur_ressort_repos, force_totales);
    //Calcul de la force + application résistance
    force_pesanteur = -tableau_poids[i].masse * g;
    for (i = 0;i < taille_tableau_poids;i++){
        force_resist = f_resist(viscosite, rayon, tableau_poids[i].vitesse_instantanee);// vitesse précédente
        //force_pesanteur = f_pesanteur(tableau_poids[i].masse);// La masse est constante donc je ne le fait qu'une fois(en cas de masses différentes, le faire dans la boucle)
        force_resist.z += force_pesanteur;
        force_tmp = add(force_resist + autres_forces);
        force_totales[i] = add(force_tmp, force_totales[i]);
    }
    free(force_totales);
    //return force_totales;
}
/*ne pas oublier de tester si les ressorts relie des poids existant au chargement du  model (voir si chaque points sont < à taille tableau poids)
tester si la liste de poids n'est pas vide */

int main(void) {
    // Test
    /*float k = 10.0;                 // Constante de rappel
    Vector3 deformation = {1.0, 2.0, 3.0};    // Déformation en x, y, z
    float viscosite = 0.8;          // Viscosité du fluide
    float rayon = 0.1;              // Rayon de la boule
    Vector3 vitesse = {2.0, 1.0, 3.0};        // Vitesse en x, y, z
    float masse = 1.5;              // Masse de l'objet
	Vector3 vent = {1.0, 0.0, 0.0};  */
	
    /*Vector3 force_totale = calculer_force_totale(k, deformation, viscosite, rayon, vitesse, masse, vent);

    printf("Force totale : [%f, %f, %f]\n", force_totale.x, force_totale.y, force_totale.z);*/

    return 0;
}

