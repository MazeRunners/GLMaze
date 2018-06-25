#version 330 core

uniform vec3 viewPos; 
uniform vec3 lightPos;
uniform sampler2D clothTexture;

in vec4 fragLightSpacePos;
in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

out vec4 FragColor;

vec3 phongLighting(float c_ambient, float c_diffuse, float c_specular, float shininess, vec3 color) {
    vec3 ambient = c_ambient * color;

    vec3 norm = normalize(normal);
    vec3 lightDirection = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = c_diffuse * diff * color;

    vec3 viewDirection = normalize(viewPos - fragPos);
    vec3 reflectDirection = reflect(-lightDirection, norm);  
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), shininess);
    vec3 specular = c_specular * spec * color; 
    
    vec3 result =  ambient + (1.0 - 0.0) * (diffuse + specular);

    return result;
}


void main() {
    float c_ambient = 0.3;
    float c_diffuse = 0.3;
    float c_specular = 0.3;
    float shininess = 1.0;

    vec3 color = texture(clothTexture, texCoord).rgb;
    vec3 result = phongLighting(c_ambient, c_diffuse, c_specular, shininess, color);
	FragColor = vec4(result, 1.0);
}