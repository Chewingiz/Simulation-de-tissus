#include <stdio.h>
#include <math.h>
#include "forces.h"

/*       Forces intérieures      */
//Ressort
/* Calcul de la force de rappel en Newton 
k : la constante de rappel en Newton par mètre (N/m),
*/

Vector3 calculs_ressort(Vector3 Pa, Vector3 Pb, float k, float longueur_ressort_repos) {
    //printf("\n K calcul ressorts = %f\n",k);
    Vector3 direction;
    Vector3 direction_normalisee;
    Vector3 force;

    direction.x = Pb.x - Pa.x;
    direction.y = Pb.y - Pa.y;
    direction.z = Pb.z - Pa.z;

    float distance = sqrt(fabs(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z)); // distance euclidienne 
    
    if (distance == 0) {
        //printf("\nDIV PAR 0\n");
        force.x = 0.0;
        force.y = 0.0;
        force.z = 0.0;
    } else {
        direction_normalisee.x = direction.x / distance;
        direction_normalisee.y = direction.y / distance;
        direction_normalisee.z = direction.z / distance;

        float kld = k * (distance - longueur_ressort_repos);
        force.x = direction_normalisee.x * kld;
        force.y = direction_normalisee.y * kld;
        force.z = direction_normalisee.z * kld;
    }

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
    //printf("taille_tableau_ressorts %d                      ",taille_tableau_ressorts);
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
//force_totales;

/* Ne pas oublierde vérifier si les ressorts relient des poids existants lors du chargement du modèle (vérifie si chaque point est inférieur à la taille du tableau des poids).
Vérifier également si la liste des poids n'est pas vide. */