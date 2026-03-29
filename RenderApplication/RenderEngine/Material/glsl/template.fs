#version 330 core
out vec4 FragColor;

## if hasColor | default(false)
in vec4 Color;
## endif 
## if hasNormal | default(false)
in vec3 Normal;
## endif 
## if hasTexCoords | default(false)
in vec2 TexCoords;
## endif 
## if hasTangent | default(false)
in vec3 Tangent;
## endif 
## if hasBitangent | default(false)
in vec3 Bitangent;
## endif 
## if hasSelected | default(false)
uniform bool selectedID;
## endif

## if renderID | default(false)
uniform vec4 objectID;
## endif 

## if hasTexCoords | default(false)
uniform sampler2D texture_diffuse1;
## endif 

void main()
{
## if hasTexCoords | default(false)
        FragColor = texture(texture_diffuse1, TexCoords);
## endif 
## if hasColor | default(false)
        FragColor = Color;
## endif 
## if hasSelected | default(false)
    if (selectedID) {
        FragColor += 0.5;
    }
## endif
## if renderID | default(false)
        FragColor = vec4(objectID.x, objectID.y, objectID.z, objectID.w);
## endif 
}