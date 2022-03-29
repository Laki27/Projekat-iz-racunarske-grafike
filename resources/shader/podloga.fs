#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos0;
    vec3 TangentLightPos1;
    vec3 TangentLightPos2;
    vec3 TangentLightPos3;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

struct DirLight{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;

uniform DirLight direkciono;
uniform vec3 viewPos;
uniform PointLight na_sceni[4];

uniform vec3 lightPos0;
uniform vec3 lightPos1;
uniform vec3 lightPos2;
uniform vec3 lightPos3;

vec3 CalcDirLight(DirLight direkciono, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight na_sceni, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 tlp, vec3 pozicija_svetla);

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{
    const float minLayers=8;
    const float maxLayers=32;
    float numLayers=mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));

    float layerDepth=1.0/numLayers;
    float currentLayerDepth=0.0;

    vec2 p=viewDir.xy/viewDir.z*0.001;
    vec2 deltaTexCoords=p/numLayers;

    vec2 currentTexCoords=texCoords;
    float currentDepthMapValue=texture(specularMap, currentTexCoords).r;

    while(currentLayerDepth<currentDepthMapValue){
        currentTexCoords-=deltaTexCoords;
        currentDepthMapValue = texture(specularMap, currentTexCoords).r;
        currentLayerDepth+=layerDepth;
    }

    vec2 prevTexCoords=currentTexCoords+deltaTexCoords;
    float afterDepth=currentDepthMapValue-currentLayerDepth;
    float beforeDepth=texture(specularMap, prevTexCoords).r-currentLayerDepth+layerDepth;
    float weight=afterDepth/(afterDepth-beforeDepth);
    vec2 finalTexCoords=prevTexCoords*weight+currentTexCoords*(1.0-weight);
    return finalTexCoords;
}

void main()
{
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec2 texCoords = ParallaxMapping(fs_in.TexCoords,  viewDir);
    if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
        discard;
    vec3 normal = texture(normalMap, texCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    vec3 result = CalcDirLight(direkciono, normal, viewDir);

    result+=CalcPointLight(na_sceni[0], normal, fs_in.FragPos, viewDir, fs_in.TangentLightPos0, lightPos0);
    result+=CalcPointLight(na_sceni[1], normal, fs_in.FragPos, viewDir, fs_in.TangentLightPos1, lightPos1);
    result+=CalcPointLight(na_sceni[2], normal, fs_in.FragPos, viewDir, fs_in.TangentLightPos2, lightPos2);
    result+=CalcPointLight(na_sceni[3], normal, fs_in.FragPos, viewDir, fs_in.TangentLightPos3, lightPos3);
    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight direkciono, vec3 normal, vec3 viewDir){
        vec3 lightDir = normalize(-direkciono.direction);
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0f);
        vec3 ambient = direkciono.ambient * vec3(texture(diffuseMap, fs_in.TexCoords));
        vec3 diffuse = direkciono.diffuse * diff * vec3(texture(diffuseMap, fs_in.TexCoords));
        vec3 specular = direkciono.specular * spec * vec3(texture(specularMap, fs_in.TexCoords));
        return (vec3(0.6)*ambient + vec3(0.8)*diffuse + specular*1.8);
}

vec3 CalcPointLight(PointLight na_sceni, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 tlp, vec3 pozicija_svetla){
        vec3 lightDir=normalize(tlp-fs_in.TangentFragPos);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 halfwayDir=normalize(lightDir+viewDir);
        float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0f);
        float distance = length(pozicija_svetla - fragPos);
        float attenuation = 1.0 / (1.0f + 0.4f * distance + 0.057f * (distance * distance));
        vec3 ambient = na_sceni.ambient * vec3(texture(diffuseMap, fs_in.TexCoords));
        vec3 diffuse = na_sceni.diffuse * diff * vec3(texture(diffuseMap, fs_in.TexCoords));
        vec3 specular = na_sceni.specular * spec * vec3(texture(specularMap, fs_in.TexCoords));
        ambient *= attenuation;
        diffuse *= attenuation;
        specular*=attenuation;
        return vec3(ambient+diffuse+specular*15);
}

