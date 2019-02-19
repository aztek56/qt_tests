#version 150
in highp vec3 vColor;
in highp vec3 vert;
in highp vec3 vertNormal;
out highp vec4 fragColor;
uniform highp vec3 lightPos;

void main() {
   highp vec3 L = normalize(lightPos - vert);
   highp float NL = max(dot(normalize(vertNormal), L), 0.0);
   highp vec3 col = clamp(vColor * 0.2 + vColor * 0.8 * NL, 0.0, 1.0);
   fragColor = vec4(col, 1.0);
}
