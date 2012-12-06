varying vec4 diffuse0, diffuse1, ambient0, ambient1;
varying vec3 lightDir0, lightDir1, halfVector0, halfVector1;
varying float dist0, dist1;

varying vec4 ambientGlobal;
varying vec3 normal;


void main()
{
	vec3 n,halfV,viewV,ldir;
	float NdotL,NdotHV;
	vec4 color = ambientGlobal;
	float att;
	
	/* a fragment shader can't write a verying variable, hence we need
	a new variable to store the normalized interpolated normal */
	n = normalize(normal);
	
	/* compute the dot product between normal and ldir */
	NdotL = max(dot(n,normalize(lightDir0)),0.0);

	if (NdotL > 0.0) {
		att = 1.0 / (gl_LightSource[0].constantAttenuation +
				gl_LightSource[0].linearAttenuation * dist0 +
				gl_LightSource[0].quadraticAttenuation * dist0 * dist0);
		color += att * (diffuse0 * NdotL + ambient0);
		
		halfV = normalize(halfVector0);
		NdotHV = max(dot(n,halfV),0.0);
		color += att * gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(NdotHV,gl_FrontMaterial.shininess);
	}


	float spotEffect;
	
	/* a fragment shader can't write a verying variable, hence we need
	a new variable to store the normalized interpolated normal */
	n = normalize(normal);
	
	/* compute the dot product between normal and ldir */
	NdotL = max(dot(n,normalize(lightDir1)),0.0);

	if (NdotL > 0.0) {
	
		spotEffect = dot(normalize(gl_LightSource[1].spotDirection), normalize(-lightDir1));
		if (spotEffect > gl_LightSource[1].spotCosCutoff) {
			spotEffect = pow(spotEffect, gl_LightSource[1].spotExponent);
			att = spotEffect / (gl_LightSource[1].constantAttenuation +
					gl_LightSource[1].linearAttenuation * dist1 +
					gl_LightSource[1].quadraticAttenuation * dist1 * dist1);
				
			color += att * (diffuse1 * NdotL + ambient1);
		
			
			halfV = normalize(halfVector1);
			NdotHV = max(dot(n,halfV),0.0);
			color += att * gl_FrontMaterial.specular * gl_LightSource[1].specular * pow(NdotHV,gl_FrontMaterial.shininess);
		}
	}

	color[0] = min(1.0,color[0]);
	color[1] = min(1.0,color[1]);
	color[2] = min(1.0,color[2]);

	gl_FragColor = color;
}
