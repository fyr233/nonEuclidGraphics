#version 330 core

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct AreaLight
{
    vec3 position;
	vec3 radiance;
	vec3 normal;
	float area;
};

uniform bool useTexture;

uniform vec3 backgroundColor;
uniform float zFar;
uniform sampler2D AlbedoTexture;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

#define MAXNUM_POINT_LIGHT 10
uniform int numAreaLights;
uniform AreaLight areaLights[MAXNUM_POINT_LIGHT];

uniform mat3 G;

uniform Material material;

in vec2 TexCoord;
in vec3 SRnormal;
in vec3 WorldPos;
// Ouput data
out vec3 fragColor;

float G_dot(vec3 v1, vec3 v2)
{
	return dot(v1, G*v2);
}
float G_norm2(vec3 v)
{
	return dot(v, G*v);
}
vec3 G_normalize(vec3 v)
{
	return v / sqrt(G_norm2(v));
}

vec3 CalcAreaLight(AreaLight light,  vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = G_normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(G_dot(SRnormal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = G_normalize(2 * G_dot(lightDir, SRnormal) * G_normalize(SRnormal) - lightDir);
    float spec = pow(max(G_dot(viewDir, reflectDir), 0.0), material.shininess);
    // 衰减
    float dist2 = G_norm2(light.position - fragPos);
    float attenuation = abs(G_dot(light.normal, lightDir)) * light.area / (light.area + dist2);    
    // 合并结果
    vec3 ambient = material.ambient * light.radiance;
    vec3 diffuse  = diff * material.diffuse * light.radiance;
    vec3 specular = spec * light.radiance;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return ambient+diffuse+specular;
}

void main()
{
	//将来再加光源、材质
	// 环境光
	//float ambientStrength = 0.1;
	//vec3 ambient = material.ambient * vec3(0.1,0.1,0.1);

	// 漫反射
	vec3 lightDir = G_normalize(lightPos - WorldPos);
	float diff = max(dot(SRnormal, G * lightDir), 0.0);
	vec3 diffuse = diff * lightColor * material.diffuse;

	// 高光
	//float shininess =33;
	//float specularStrength = 0.3;
	float viewDistance = pow(G_norm2(viewPos - WorldPos), 0.8);
	vec3 viewDir = G_normalize(viewPos - WorldPos);
	//vec3 reflectDir = reflect(-lightDir, SRnormal);
	
	
	// Output color
	vec3 objColor = texture(AlbedoTexture, TexCoord).rgb;
	vec3 color = vec3(0);//useTexture ? objColor * ambient: ambient;
	for(int i = 0; i < numAreaLights; i++)
		color += useTexture ? objColor * CalcAreaLight(areaLights[i], WorldPos, viewDir) : CalcAreaLight(areaLights[i], WorldPos, viewDir);
	//float zfar2 = pow(16, 0.8);
	float zfar2 = pow(zFar / 2, 1.6f);
	float decay = exp(- viewDistance / zfar2);
	color = color * decay + backgroundColor * (1-decay);
	fragColor = color;
}
