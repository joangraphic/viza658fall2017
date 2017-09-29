#version 330 core
out vec4 FragColor;

//in vec4 vertexColor;//get from vertex shader
in vec2 TexCoord;
//uniform vec4 ourColor;//set this color in OPEGL code

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
    FragColor = texture(texture0, TexCoord);
    //mix(texture(texture0, TexCoord), texture(texture1, TexCoord),0.2);
    //vertexColor; 
    //ourColor; //assign uniform variable
    //vec4(1.0f, 0.5f, 0.2f, 1.0f);
} 