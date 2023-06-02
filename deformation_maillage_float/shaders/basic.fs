#version 330

in float vsoIL; /* Gouraud */
in vec3 vsoNormal;
in vec4 vsoModPos;

in  vec2 vsoTexCoord;
out vec4 fragColor;

uniform sampler2D myTex;
uniform mat4 viewMatrix;
uniform vec4 Lp;
uniform vec4 couleur;

void main(void) {
  const vec3 Vu = vec3(0.0, 0.0, -1.0);
  vec4 Lpv = viewMatrix * Lp;
  vec3 Ld = normalize(vsoModPos - Lpv).xyz;
  vec3 R = normalize(reflect(Ld, vsoNormal));
  float ispec = pow(clamp(dot(-Vu, R), 0.0, 1.0), 20.0);
  float phongIL = clamp(dot(-Ld, vsoNormal), 0.0, 1.0);
  //fragColor = vsoIL * texture(myTex, vsoTexCoord) + ispec * vec4(1.0);
  fragColor = /*vsoIL * vec4(1.0) */+ ispec * vec4(1.0)+ couleur;
}
