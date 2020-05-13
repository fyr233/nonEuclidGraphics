#version 330 core

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform bool useTexture;

uniform sampler2D AlbedoTexture;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform mat3 G;

uniform Material material;

in vec2 TexCoord;
in vec3 SRnormal;
in vec3 WorldPos;
// Ouput data
out vec3 color;

vec3 G_normalize(vec3 v)
{
	return v / sqrt(dot(v, G * v));
}

void main()
{
	//将来再加光源、材质
	// 环境光
	//float ambientStrength = 0.1;
	vec3 ambient = material.ambient * lightColor;

	// 漫反射
	vec3 lightDir = G_normalize(lightPos - WorldPos);
	float diff = max(dot(SRnormal, G * lightDir), 0.0);
	vec3 diffuse = diff * lightColor * material.diffuse;

	// 高光
	//float shininess =33;
	//float specularStrength = 0.3;
	vec3 viewDir = G_normalize(viewPos - WorldPos);
	//vec3 reflectDir = reflect(-lightDir, SRnormal);
	vec3 reflectDir = 2 * dot(lightDir, G * SRnormal) * G_normalize(SRnormal) - lightDir;
	float spec = pow(max(dot(viewDir, G * reflectDir), 0.0), material.shininess);
	vec3 specular = material.specular * spec * lightColor;
	
	// Output color
	vec3 objColor = texture(AlbedoTexture, TexCoord).rgb;
	color = useTexture ? objColor * (ambient + diffuse + specular) : ambient + diffuse + specular;
}
