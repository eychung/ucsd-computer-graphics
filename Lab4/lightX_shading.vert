varying vec4 diffuse0, diffuse1, ambient0, ambient1;
varying vec3 lightDir0, lightDir1, halfVector0, halfVector1;
varying float dist0, dist1;

varying vec4 ambientGlobal;
varying vec3 normal;


void main()
{
	vec4 ecPos;
	vec3 aux;
	
	/* first transform the normal into eye space and normalize the result */
	normal = normalize(gl_NormalMatrix * gl_Normal);
	
	/* now normalize the light's direction. Note that according to the
	OpenGL specification, the light is stored in eye space. Also since 
	we're talking about a directional light, the position field is actually 
	direction */
	ecPos = gl_ModelViewMatrix * gl_Vertex;
	aux = vec3(gl_LightSource[0].position-ecPos);
	lightDir0 = normalize(aux);
	
	/* compute the distance to the light source to a varying variable*/
	dist0 = length(aux);

	/* Normalize the halfVector to pass it to the fragment shader */
	halfVector0 = normalize(gl_LightSource[0].halfVector.xyz);
	
	/* Compute the diffuse, ambient and globalAmbient terms */
	diffuse0 = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
	ambient0 = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	ambientGlobal = gl_LightModel.ambient * gl_FrontMaterial.ambient;
		
	//gl_Position = ftransform();


	/* these are the new lines of code to compute the light's direction */
	aux = vec3(gl_LightSource[1].position-ecPos);
	lightDir1 = normalize(aux);
	dist1 = length(aux);
	halfVector1 = normalize(gl_LightSource[1].halfVector.xyz);

	/* Compute the diffuse, ambient and globalAmbient terms */
	diffuse1 = gl_FrontMaterial.diffuse * gl_LightSource[1].diffuse;
	/* The ambient terms have been separated since one of them */

	/* suffers attenuation */
	ambient1 = gl_FrontMaterial.ambient * gl_LightSource[1].ambient;

	gl_Position = ftransform();
}
