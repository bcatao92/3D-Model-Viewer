#version 420 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in vec3 cameraPos;


uniform vec3 ambient_light;

uniform int light_num;
uniform vec3 light_positions[128];
uniform vec3 light_colors[128];

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

float ambientStrength = 0.1;
float diffuseStrength = 0.7;
float specularStrength = 0.2;
float atenuationFactor = 0.8;
float specularFactor = 32;

float getDifuseCosine(vec3 lightPos){
    vec3 lightDirection = normalize(lightPos-FragPos);
    vec3 normalizedNormal = normalize(Normal);
    float cos = max(dot(lightDirection, normalizedNormal), 0.0);
    return cos;
}

float getSpecularCosine(vec3 lightPos){
    vec3 lightDirection = normalize(lightPos-FragPos);
    vec3 cameraDirection = normalize(cameraPos-FragPos);
    vec3 normalizedNormal = normalize(Normal);
    vec3 reflexionDirection = reflect(-lightDirection, normalizedNormal);
    float cos = max(dot(cameraDirection, reflexionDirection), 0.0);
    return cos;
}

vec4 calculateLighting(vec4 materialColor){
    //Ambient Lighting
    vec4 ambientLighting = materialColor*vec4(ambient_light,1.0)*ambientStrength;
    vec4 result = ambientLighting;
    for(int i = 0; i < light_num; i++){
        vec3 lightPosition = light_positions[i];
        vec4 lightColor = vec4(light_colors[i], 1.0);
        //Difuse reflexion
        float diffuseCosine = getDifuseCosine(lightPosition);
        vec4 diffuse = atenuationFactor*diffuseStrength*diffuseCosine*lightColor*materialColor;
        //Difuse reflexion
        float specularCosine = getSpecularCosine(lightPosition);
        specularCosine = pow(specularCosine, specularFactor);
        vec4 specular = atenuationFactor*specularStrength*specularCosine*lightColor*vec4(1.0);
        result += diffuse+specular;
    }
    return result;
}


void main()
{    
    vec4 material = texture(texture_diffuse1, TexCoords);
    FragColor = calculateLighting(material);
    //FragColor = FragColor*vec4(light_colors[0], 1.f);
    //FragColor = FragColor*vec4(light_colors[1],1.0f);
}

