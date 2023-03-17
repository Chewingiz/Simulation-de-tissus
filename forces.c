#include "math.h"
#include <stdio.h>

#define g 9.800908285 // constante gravitatonelle

/*       Forces intérieures      */
//Ressort
/* Calcul de la force de rappel en Newton 
k : la constante de rappel en Newton par mètre (N/m),
x : le d ́eformation en mètre (m).
*/
float f_rappel ( float k, float x ){
 return -k * x;
}

/*       Forces extérieures      */
/*Calcul de la force de frottement du fluide en Newton.
viscosite : la viscosité du fluide en Pascal seconde (P a · s),
r : rayon de la boule en m`etre (m),
v : vitesse de la boule en m`etre par seconde (m.s−1)
*/
float f_resist(float viscosite, float r, float v){
    printf("%f",M_PI);
    return -6 * M_PI * viscosite * v ;
}

/* Calcul de la force de pesanteur 
m : masse de l'objet
*/
float f_pesanteur (float m){
    return m * g;
}

void main(){
    //f_resist();
}