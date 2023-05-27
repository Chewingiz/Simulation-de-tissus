/*!\file basic.fs
 *
 * \brief fragment shader basique qui applique une couleur mélangée
 * avec la texture.
 * \author Farès BELHADJ, amsi@up8.edu
 * \date February 06 2023
 */
/* Version GLSL 3.30 */
#version 330
/* Le fragment shader est directement en relation avec les sorties du vertex shader */
in float vsoIL; /* Gouraud */
in vec3 vsoNormal;
in vec4 vsoModPos;

/* Le fragment shader est directement en relation avec les sorties du vertex shader */
in  vec2 vsoTexCoord;
/* sortie du frament shader : une couleur */
out vec4 fragColor;

uniform sampler2D myTex;
uniform mat4 viewMatrix;
uniform vec4 Lp;

void main(void) {
  const vec3 Vu = vec3(0.0, 0.0, -1.0);
  vec4 Lpv = viewMatrix * Lp;
  vec3 Ld = normalize(vsoModPos - Lpv).xyz;
  vec3 R = normalize(reflect(Ld, vsoNormal));
  float ispec = pow(clamp(dot(-Vu, R), 0.0, 1.0), 20.0);
  float phongIL = clamp(dot(-Ld, vsoNormal), 0.0, 1.0);
  //fragColor = vsoIL * texture(myTex, vsoTexCoord) + ispec * vec4(1.0);
  fragColor = vsoIL * vec4(1.0) + ispec * vec4(1.0);
}
