#version 330 core

struct Material {
	sampler2D diffuseTexture;
	sampler2D specularTexture;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 normal;
in vec3 fragmentPosition;
in vec2 texturePosition;

uniform Material uMaterial;
uniform Light uLight;
uniform vec3 uViewPosition;

out vec4 fragmentColor;

void main() {
	//ambient component
	vec3 ambient = uLight.ambient * texture(uMaterial.diffuseTexture, texturePosition).rgb;
	
	//diffuse component
	vec3 surfaceNormal = normalize(normal);
	vec3 lightDirection = normalize(uLight.position - fragmentPosition);
	float diffuseStrength = max(dot(surfaceNormal, lightDirection), 0.0);
	vec3 diffuse = uLight.diffuse * diffuseStrength * texture(uMaterial.diffuseTexture, texturePosition).rgb;

	//specular component
	vec3 viewDirection = normalize(uViewPosition - fragmentPosition);
	vec3 reflectedLightDirection = reflect(-lightDirection, surfaceNormal);
	float specularStrength = pow(max(dot(viewDirection, reflectedLightDirection), 0.0), uMaterial.shininess);
	vec3 specular = uLight.specular * specularStrength * texture(uMaterial.specularTexture, texturePosition).rgb;

	//end color
	vec3 result = ambient + diffuse + specular;
	fragmentColor = vec4(result, 1.0);
}