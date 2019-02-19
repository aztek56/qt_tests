attribute vec4 vertex;
attribute vec3 normal;
attribute vec3 color;
varying vec3 vColor;
varying vec3 vert;
varying vec3 vertNormal;
uniform mat4 worldToView ;
uniform mat4 modelToWorld;
uniform mat3 normalMatrix;

void main() {
   vColor = color;
   vert = vertex.xyz;
   vertNormal = normalMatrix * normal;
   gl_Position = worldToView  * modelToWorld * vertex;
}
