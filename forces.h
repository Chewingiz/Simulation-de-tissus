#include <stdio.h>
#include <math.h>
#include "structs.h"

Vector3 add(Vector3 a, Vector3 b);
Vector3 sub(Vector3 a, Vector3 b);

Vector3 calculs_ressort(Vector3 Pa, Vector3 Pb, float k, float longueur_ressort_repos);
void tables_forces_ressorts(Poids* tableau_poids, int** tableau_ressorts, int taille_tableau_ressorts, float k, float longueur_ressort_repos, Vector3* tableau_forces_total_appliquer_sur_les_points_R);
Vector3 f_resist(float viscosite, float rayon, Vector3 vitesse);
Vector3 f_pesanteur(float masse);
