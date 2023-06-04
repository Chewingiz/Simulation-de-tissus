#include "forces.h"

void maj_positions(Poids *tableau, int taille_x);
void maj_vitesse(Poids* poids, Vector3 force);
void maj_vitesses(Poids * tableau, int taille_x);
void calculer_forces_totale_maj_vitesses(Poids* tableau_poids, int taille_tableau_poids, int tableau_ressorts[][2], int taille_tableau_ressorts,float longueur_ressort_repos, float viscosite, float rayon, Vector3 autres_forces, float k);
