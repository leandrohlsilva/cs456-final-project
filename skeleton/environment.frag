uniform sampler2D texture;

varying vec2 texCoord;


void main(void) {
	
	vec4 color = texture2D(texture, texCoord).rgba;

	if (color.a == 0.0) discard;
	
	gl_FragColor = color;
	
}
