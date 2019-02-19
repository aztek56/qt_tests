#version 150
in vec4 vertex;
in vec3 normal;
in vec3 color;
out vec3 vColor;
out vec3 vert;
out vec3 vertNormal;
uniform mat4 worldToView ;
uniform mat4 modelToWorld;
uniform mat3 normalMatrix;

void main() {
   vColor = color;
   vert = vertex.xyz;
   vertNormal = normalMatrix * normal;
   gl_Position = worldToView  * modelToWorld * vertex;
}