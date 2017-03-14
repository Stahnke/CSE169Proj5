#version 330 core

//Structs
struct DirLight {
    vec3 direction;

	vec3 diffuse;
	vec3 specular;
	vec3 ambient;

};  

struct Material {
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    float shininess;
}; 

//Inputs  
in vec3 Normal;
in vec3 FragPos;

//Outputs
out vec4 color;

//Uniforms
uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight1;
uniform DirLight dirLight2;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result1;
	vec3 result2;
	vec3 finalResult;

	result1 = CalcDirLight(dirLight1, norm, viewDir);
	result2 = CalcDirLight(dirLight2, norm, viewDir);

	finalResult = result1 + result2;

	color = vec4(finalResult, 1.0f);
} 

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	//Direction
	vec3 lightDir = normalize(-light.direction);
	
	//diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * material.diffuse * light.diffuse;
	
	//specular
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess * 128.0f);
	vec3 specular = spec * material.specular * light.specular;
	
	//ambient
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * material.ambient * light.ambient;

    return (diffuse + ambient);
}