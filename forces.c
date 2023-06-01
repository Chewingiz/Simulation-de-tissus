#include <stdio.h>
#include <math.h>
#define g 9.800908285 // constante gravitationnelle

typedef struct {
    float x;
    float y;
    float z;
} Vector3;

/*       Forces intérieures      */
//Ressort
/* Calcul de la force de rappel en Newton 
k : la constante de rappel en Newton par mètre (N/m),
x : le d ́eformation en mètre (m).
*/
Vector3 f_rappel(float k, Vector3 deformation) {
    Vector3 force_rappel;
    force_rappel.x = -k * deformation.x;
    force_rappel.y = -k * deformation.y;
    force_rappel.z = -k * deformation.z;
    return force_rappel;
}

/*       Forces extérieures      */
/*Calcul de la force de frottement du fluide en Newton.
viscosite : la viscosité du fluide en Pascal seconde (P a · s),
r : rayon de la boule en m`etre (m),
v : vitesse de la boule en m`etre par seconde (m.s−1)
*/
Vector3 f_resist(float viscosite, float rayon, Vector3 vitesse) {
    Vector3 force_resist;
    force_resist.x = -6 * M_PI * viscosite * rayon * vitesse.x;
    force_resist.y = -6 * M_PI * viscosite * rayon * vitesse.y;
    force_resist.z = -6 * M_PI * viscosite * rayon * vitesse.z;
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
/*Vector3 calculer_force_totale(Vector3 * tableau_f_rappel, float viscosite, float rayon, Vector3 vitesse, float masse, Vector3 autres_forces) {
    Vector3 force_totale;

   // Vector3 force_rappel = f_rappel(k, deformation);
    Vector3 force_pesanteur = f_pesanteur(masse);
    
    //Calcul de la force + application résistance
    Vector3 force_resist = f_resist(viscosite, rayon, vitesse);// vitesse précédente

    force_totale.x = force_rappel.x + force_resist.x + force_pesanteur.x + autres_forces.x;
    force_totale.y = force_rappel.y + force_resist.y + force_pesanteur.y + autres_forces.y;
    force_totale.z = force_rappel.z + force_resist.z + force_pesanteur.z + autres_forces.z ;

    return force_totale;
}

*/

//calculs ressort 
/* tableau_poids // tableau ressort (avec l'indice des deux relier)// 

Longueure ressort au repos*/

//Vector3 * calculs_ressorts
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

/*
calcul tables de ressort 

tableau vec3 toutes les forces appliquée à un poid par les ressorts
entré [tableau des poids, tableau des ressorts, longeur ressort au repos et K]

créé le tableau de taille (taille liste des poids) initialiser tout les vec à (0,0,0) // pour le retour
iterer sur le tableau des ressorts, faire les calcules entre les deux points avec calculs_ressort et ajouter le resultat au niveua de l'index des deux points.

*/

int main(void) {
    // Test
    float k = 10.0;                 // Constante de rappel
    Vector3 deformation = {1.0, 2.0, 3.0};    // Déformation en x, y, z
    float viscosite = 0.8;          // Viscosité du fluide
    float rayon = 0.1;              // Rayon de la boule
    Vector3 vitesse = {2.0, 1.0, 3.0};        // Vitesse en x, y, z
    float masse = 1.5;              // Masse de l'objet
	Vector3 vent = {1.0, 0.0, 0.0};  
	
    /*Vector3 force_totale = calculer_force_totale(k, deformation, viscosite, rayon, vitesse, masse, vent);

    printf("Force totale : [%f, %f, %f]\n", force_totale.x, force_totale.y, force_totale.z);*/

    return 0;
}

