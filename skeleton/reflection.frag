uniform sampler2D texture;
uniform samplerCube texCube;

varying vec2 texCoord;

varying vec3 lightVec;
varying vec3 viewVec;
varying vec3 normal;

void main(void) {

	vec4 texel = texture2D(texture, texCoord).rgba;


	if (texel.a == 0.0) discard;
	
	vec3 norm = normalize(normal);
	vec3 L = normalize (lightVec);
	vec3 V = normalize (viewVec);
	vec3 halfAngle = normalize (L + V);

	float NdotL = dot(L, norm);
	float NdotH = clamp(dot(halfAngle, norm), 0.0, 1.0);

	vec4 amb = gl_LightSource[0].ambient;
	vec4 diff = gl_LightSource[0].diffuse;
	vec4 spec = gl_LightSource[0].specular;

	vec4 ka = gl_FrontMaterial.ambient;
	vec4 kd = gl_FrontMaterial.diffuse;
	vec4 ks = gl_FrontMaterial.specular;

	float diffuse = 0.5 * NdotL + 0.5;
	float specular = pow(NdotH, 64.0);
	
	vec4 result = ka*amb + kd*diff*(dot(norm,L)) + ks*specular;

	vec3 cf = specular * kd.rgb + ka.rgb;
	
	vec3 ct = texel.rgb;
	vec4 finalColor = vec4(ct * cf, texel.a * diffuse);
	
	vec4 color = texture2D(texture, texCoord).rgba;
	
	const float LOG2 = 1.442695;
	float z = gl_FragCoord.z / gl_FragCoord.w;
	float fogFactor = exp2( -gl_Fog.density * 
				   gl_Fog.density * 
				   z * 
				   z * 
				   LOG2 );
	fogFactor = clamp(fogFactor, 0.0, 1.0);
	

	//gl_FragColor = mix(gl_Fog.color, vec4(ct * cf, texel.a * diffuse), 0.3);
//	gl_FragColor = mix(gl_Fog.color, color, fogFactor );
	gl_FragColor = mix(gl_Fog.color, texture2D(texture, V.rg).rgba, fogFactor );
	//gl_FragColor = texel;

}
