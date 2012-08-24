varying vec2 texCoord;
varying vec3 normal;
varying vec4 pos;
varying vec3 lightVec;
varying vec3 viewVec;
varying vec3 vert;

void main(void) {
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	texCoord = gl_MultiTexCoord0.xy;
	pos = gl_Position;
	normal = normalize(gl_NormalMatrix * gl_Normal);
	vert = vec3(gl_ModelViewMatrix * gl_Vertex);
	lightVec = vec3(normalize(gl_LightSource[0].position.xyz - vert));
	viewVec = -vec3(vert);
}
