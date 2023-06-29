#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec4 aNormal;


out vec3 ourColor;
out vec2 TexCoord;
out vec4 FragPos;
out vec4 Normal;

uniform mat4 u_model;
uniform mat4 u_viewProjection;

void main()
{
    gl_Position = u_viewProjection * u_model * vec4(aPos, 1.0);
    ourColor = aColor;
    TexCoord = aTexCoord;
    FragPos = vec4(u_model * vec4(aPos, 1.0));
    Normal = aNormal;
}

#shader fragment
#version 330 core

out vec4 FragColor;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;

    float shine;
};

struct DirLight
{
    vec4 direction;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct PointLight
{
    vec4 position;

    float constant;
    float linear;
    float quadratic;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct SpotLight
{
    vec4 position;
    vec4 direction;

    float cutOff;
    float outerCutOff;
    
    float constant;
    float linear;
    float quadratic;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

};


#define POINT_LIGHTS 3

in vec3 ourColor;
in vec2 TexCoord;
in vec4 FragPos;
in vec4 Normal;

uniform sampler2D ourTexture;

uniform vec4 u_color;
uniform vec4 u_viewPos;

uniform Material u_material;
uniform DirLight u_dirLight;
uniform PointLight u_pointLights[POINT_LIGHTS];
uniform SpotLight u_spotLight;

//Functions 
vec4 CalculateDirLight(DirLight light, vec4 normal, vec4 viewDir);
vec4 CalculatePointLight(PointLight light, vec4 normal, vec4 fragPos, vec4 viewDir);
vec4 CalculateSpotLight(SpotLight light, vec4 normal, vec4 fragPos, vec4 viewDir);

void main()
{


    vec4 norm = normalize(Normal);
    vec4 viewDir = normalize(u_viewPos - FragPos);
    //phase 1 : Direction Light
    vec4 result = CalculateDirLight(u_dirLight, norm, viewDir);
    //phase 2 : Point Light
    for (int i = 0; i < POINT_LIGHTS; i++)
    {
        result += CalculatePointLight(u_pointLights[i], norm, FragPos, viewDir);
    }
    //phase 3 : Spot Light
    result += CalculateSpotLight(u_spotLight, norm, FragPos, viewDir);

    FragColor = vec4(result * 1.0);


}

//Calculating colour when using direction light
vec4 CalculateDirLight(DirLight light, vec4 normal, vec4 viewDir)
{
    vec4 lightDir = normalize(-light.direction);

    //diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    //specular shading
    vec4 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shine);

    //combining results
    vec4 ambient = light.ambient * vec4(texture(u_material.diffuse, TexCoord));
    vec4 diffuse = light.diffuse * diff * vec4(texture(u_material.diffuse, TexCoord));
    vec4 specular = light.specular * spec * vec4(texture(u_material.specular, TexCoord));

    return (ambient + diffuse + specular);
 
}

//Calculating colour when using a point light
vec4 CalculatePointLight(PointLight light, vec4 normal, vec4 fragPos, vec4 viewDir)
{
    vec4 lightDir = normalize(light.position - fragPos);

    //diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    //specular shading
    vec4 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shine);
    //attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
        light.quadratic * (distance * distance));
    //combine results
    vec4 ambient = light.ambient * vec4(texture(u_material.diffuse, TexCoord));
    vec4 diffuse = light.diffuse * diff * vec4(texture(u_material.diffuse, TexCoord));
    vec4 specular = light.specular * spec * vec4(texture(u_material.specular, TexCoord));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

//Calculating colour when using a spot light
vec4 CalculateSpotLight(SpotLight light, vec4 normal, vec4 fragPos, vec4 viewDir)
{
    vec4 lightDir = normalize(light.position - fragPos);
    //diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    //specular shading
    vec4 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shine);
    //attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
        light.quadratic * (distance * distance));
    //spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    //combine results
    vec4 ambient = light.ambient * vec4(texture(u_material.diffuse, TexCoord));
    vec4 diffuse = light.diffuse * diff * vec4(texture(u_material.diffuse, TexCoord));
    vec4 specular = light.specular * spec * vec4(texture(u_material.specular, TexCoord));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);

}

