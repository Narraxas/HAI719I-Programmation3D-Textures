#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec2 uv0;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 o_positionWorld;
out vec3 o_normalWorld;
out vec2 o_uv0;
out mat3 TBN;

void main() {
  mat3 normalMatrix = mat3(transpose(inverse(model)));
  vec4 positionWorld = model * vec4(position, 1.0);
  // Calcul de la matrice TBN
    // Transformation de l'espace tangeante en espace monde
    // T: nws = N * n
    // B: tws = N * t
    // N: bws = nws x tws
  vec3 T = normalize(normalMatrix * tangent);
  vec3 N = normalize(normalMatrix * normal);

  T = normalize(T - dot(T,N) * N);
  vec3 B = cross(N, T);

  // --------------- out ---------------
  o_positionWorld = positionWorld.xyz;
  o_normalWorld = normalMatrix * normal;

  TBN = transpose(mat3(T, B, N));
  o_uv0 = vec2(uv0.x, -uv0.y);
  gl_Position = projection * view * positionWorld;
}
