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

