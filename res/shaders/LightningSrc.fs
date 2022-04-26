#version 330 core
struct Light {
vec3 position;
vec3 ambient;
vec3 diffuse;
vec3 specular;
};


out vec4 FragColor;

uniform Light light;

void main()
{
    vec3 result = (light.ambient + light.diffuse) * 2;
    FragColor = vec4(result, 1.0) ;
}