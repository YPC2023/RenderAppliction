#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec2 aTexCoords;
layout (location = 4) in vec3 aTangent;
layout (location = 5) in vec3 aBitangent;

## if hasColor | default(false)
out vec4 Color;
## endif 
## if hasNormal | default(false)
out vec3 Normal;
## endif 
## if hasTexCoords | default(false)
out vec2 TexCoords;
## endif 
## if hasTangent | default(false)
out vec3 Tangent;
## endif 
## if hasBitangent | default(false)
out vec3 Bitangent;
## endif 

## if hasMVP | default(false)
uniform mat4 model;
## if hasMVP_UBO | default(false)
layout (std140) uniform MVPMatrix {
    {# // ◊¢ Õ Õ∂”∞æÿ’Û #}
    mat4 projection;    
    {# // ◊¢ Õ  ”Õºæÿ’Û #}
    mat4 view;          
};
{# // ◊¢ Õ hasMVP_UBO #}
## else 
uniform mat4 view;
uniform mat4 projection;
{# // hasMVP_UBO #}
## endif 
{# // hasMVP #}
## endif 


void main()
{
## if hasMVP | default(false)
    gl_Position = projection * view * model * vec4(aPos, 1.0);
## endif
## if hasColor | default(false)
        Color = aColor;
## endif 
## if hasNormal | default(false)
        Normal = aNormal;
## endif 
## if hasTexCoords | default(false)
        TexCoords = aTexCoords; 
## endif 
## if hasTangent | default(false)
        Tangent = aTangent;
## endif  
## if hasBitangent | default(false)
        Bitangent = aBitangent;
## endif 
}