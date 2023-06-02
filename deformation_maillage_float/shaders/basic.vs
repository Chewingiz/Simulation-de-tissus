#version 330

layout (location = 0) in vec3 vsiPosition;
layout (location = 1) in vec3 vsiNormal;
layout (location = 2) in vec2 vsiTexCoord;

out vec2 vsoTexCoord;
out vec3 vsoNormal;
out vec4 vsoModPos;
out float vsoIL;

uniform mat4 projectionMatrix, viewMatrix, modelMatrix;

uniform vec4 Lp;
uniform float phase;
uniform sampler2D textureSampler;




void main(void) {
  mat4 N = transpose(inverse(viewMatrix * modelMatrix));
  vec4 Lpv = viewMatrix * Lp;
  vec3 textureColor = texture(textureSampler, vsiTexCoord).rgb;

  vec3 p = vec3(textureColor.x, textureColor.y, textureColor.z);

  vsoModPos = viewMatrix * modelMatrix * vec4(p, 1.0);

  vec3 Ld = normalize(vsoModPos - Lpv).xyz;
  vsoNormal = normalize((N * vec4(vsiNormal, 0.0)).xyz);
  vsoIL = clamp(dot(-Ld, vsoNormal), 0.0, 1.0);
  gl_Position = projectionMatrix * vsoModPos;
  
  vsoTexCoord = vsiTexCoord;
}







/*!\file basic.vs
 *
 * \brief vertex shader basique un sommet possédant les attributs :
 * coordonnées spaciales 3D, couleur et coordonnées de texture. De
 * plus, une matrice de projection, une matrice vue et une matrice de
 * modélisation sont envoyées par GL4Dummies, nous les réceptionnons
 * sous la forme de "uniform mat4" et nous les utilisons.
 * \author Farès BELHADJ, amsi@up8.edu
 * \date February 6 2023
 */

/* Vertex shader, directement en relatrion avec le code-CPU */
/* Version GLSL 3.30 */
/*#version 330

layout (location = 0) in vec3 vsiPosition;
layout (location = 1) in vec3 vsiNormal;
layout (location = 2) in vec2 vsiTexCoord;


out vec2 vsoTexCoord;
out vec3 vsoNormal;
out vec4 vsoModPos;
out float vsoIL;

uniform mat4 projectionMatrix, viewMatrix, modelMatrix;


uniform vec4 Lp;

uniform float phase;

void main(void) {

  mat4 N = transpose(inverse(viewMatrix * modelMatrix));
  vsoNormal = normalize((N * vec4(vsiNormal, 0.0)).xyz);
  vec4 Lpv = viewMatrix * Lp;
  
  float dist = length(vsiPosition.xz), freq = 10.0, amplitude = 0.1 * (sqrt(2.0) - dist);
  float y = amplitude * cos(phase + freq * dist);
  vec3 p = vec3(vsiPosition.x, y, vsiPosition.z);
  
  vsoModPos = viewMatrix * modelMatrix * vec4(p, 1.0);
  vec3 Ld = normalize(vsoModPos - Lpv).xyz;
  
  vsoIL = clamp(dot(-Ld, vsoNormal), 0.0, 1.0);
  gl_Position = projectionMatrix * vsoModPos;
  
  vsoTexCoord = vec2(vsiTexCoord.x, 1.0 - vsiTexCoord.y);
}
*/
/*vide*/
