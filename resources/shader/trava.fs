#version 330 core
out vec4 FragColor;

in vec2 TexCords;
in vec3 FragPos;

uniform sampler2D trava;
uniform int noc;
uniform float p;

uniform vec3 na_sceni[4];

void main(){

        vec4 result=texture(trava, TexCords);
        float distance=0.0;
        float q=0.0f;

        if(result.a<0.1){
           discard;
        }
        if(noc==1){
            for(int i=0; i<4; i++){
                distance = length(na_sceni[i] - FragPos);
                q += 1.0 / (1.0 + 0.5 * distance + 0.01 * (distance * distance));
            }
            result=result*q+vec4(0.09, 0.05, 0.0, 0.0);
        }
        else{
            result=result*vec4(p+0.09, p+0.09, p, 1.0);
        }

        FragColor=result;
}