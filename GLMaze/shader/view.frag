#version 330 core
out vec4 fragColor;  

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;


in vec4 fragLightSpacePos;

uniform sampler2D shadowMap;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform vec3 viewPos; 
uniform vec3 lightPos;

float depthCompare(vec4 fragLightSpacePos)
{
    vec3 coords = fragLightSpacePos.xyz / fragLightSpacePos.w;
    vec3 scaledCoords = coords * 0.5 + 0.5;
   
    float depthFromLight = texture(shadowMap, scaledCoords.xy).r; 
    float depthFromCamera = scaledCoords.z;
   
    float isInShadow = (depthFromCamera - 0.005)> depthFromLight ? 1.0 : 0.0;

    return isInShadow;
}

vec3 phongLighting(float c_ambient, float c_diffuse, float c_specular, float shininess, vec3 color)
{
    vec3 ambient = c_ambient * color;

	vec3 norm = normalize(normal);
    vec3 lightDirection = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = c_diffuse * diff * color;

    vec3 viewDirection = normalize(viewPos - fragPos);
    vec3 reflectDirection = reflect(-lightDirection, norm);  
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), shininess);
    vec3 specular = c_specular * spec * color; 

	float isInShadow = depthCompare(fragLightSpacePos);
    vec3 result =  ambient + (1.0 - isInShadow) * (diffuse + specular);

    return result;
}

void main() 
{
	float c_ambient = 0.3;
	float c_diffuse = 0.8;
	float c_specular = 0.8;
	float shininess = 1.0;

	vec3 color = texture(texture_diffuse1, texCoord).rgb;

    vec3 result = phongLighting(c_ambient, c_diffuse, c_specular, shininess, color);
    fragColor = vec4(result, 1.0);
}