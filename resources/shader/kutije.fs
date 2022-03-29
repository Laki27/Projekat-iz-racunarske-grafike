#version 330 core
out vec4 FragColor;

in vec2 TexCords;
in vec3 FragPos;
in vec3 Normal;

struct Material {
 sampler2D diffuse;
 sampler2D specular;
};

struct DirLight{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 viewPos;
uniform Material material;
uniform DirLight direkciono;
uniform PointLight na_sceni[4];

vec3 CalcDirLight(DirLight direkciono, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight na_sceni, vec3 normal, vec3 fragPos, vec3 viewDir);

void main(){

        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 norm = normalize(Normal);
        vec3 result = CalcDirLight(direkciono, norm, viewDir);

        for(int i=0; i<4; i++){
            result += CalcPointLight(na_sceni[i], norm, FragPos, viewDir);
        }
        FragColor = vec4(result, 1.0);

}

vec3 CalcDirLight(DirLight direkciono, vec3 normal, vec3 viewDir){
        vec3 lightDir = normalize(-direkciono.direction);
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 halfwayDir=normalize(lightDir+viewDir);
        float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0f);
        vec3 ambient = direkciono.ambient * vec3(texture(material.diffuse, TexCords));
        vec3 diffuse = direkciono.diffuse * diff * vec3(texture(material.diffuse, TexCords));
        vec3 specular = direkciono.specular * spec * vec3(texture(material.specular, TexCords));
        return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight na_sceni, vec3 normal, vec3 fragPos, vec3 viewDir){
        vec3 lightDir = normalize(na_sceni.position - fragPos);
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 halfwayDir=normalize(lightDir+viewDir);
        float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0f);
        float distance = length(na_sceni.position - fragPos);
        float attenuation = 1.0 / (1.0f + 0.4f * distance + 0.0057f * (distance * distance));
        vec3 ambient = na_sceni.ambient * vec3(texture(material.diffuse, TexCords));
        vec3 diffuse = na_sceni.diffuse * diff * vec3(texture(material.diffuse, TexCords));
        vec3 specular = na_sceni.specular * spec * vec3(texture(material.specular, TexCords));
        ambient *= attenuation;
        diffuse *= attenuation;
        specular*=3*attenuation;
        return (ambient + diffuse + specular);
}

