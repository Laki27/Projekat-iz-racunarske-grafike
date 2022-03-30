// shder koji se koristi za podlogu
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos0;
    vec3 TangentLightPos1;
    vec3 TangentLightPos2;
    vec3 TangentLightPos3;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 lightPos0;
uniform vec3 lightPos1;
uniform vec3 lightPos2;
uniform vec3 lightPos3;

uniform vec3 viewPos;

void main()
{
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.TexCoords = aTexCoords;

    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    vec3 tmp=vec3(0.0, 0.0, 0.0);

    mat3 TBN = transpose(mat3(T, B, N));

    vs_out.TangentLightPos0 = TBN * lightPos0;
    vs_out.TangentLightPos1 = TBN * lightPos1;
    vs_out.TangentLightPos2 = TBN * lightPos2;
    vs_out.TangentLightPos3 = TBN * lightPos3;

    vs_out.TangentViewPos  = TBN * viewPos;
    vs_out.TangentFragPos  = TBN * vs_out.FragPos;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}