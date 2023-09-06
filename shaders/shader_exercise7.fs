#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;

uniform float cambioColor1;

uniform sampler2D texture1;

void main()
{
    FragColor = mix(texture(texture1, texCoord), vec4(ourColor, 1.0), cambioColor1);
    //FragColor = vec4(ourColor,1.0);
}