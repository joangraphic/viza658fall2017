#version 330 core
layout (location = 0) in vec3 aPos; //the position variable has "vertex attribute position" 0
//layout (location = 1) in vec3 aColor; //the color variable has "vertex attribute position" 1
layout (location = 1) in vec2 aTexCoord; //the texture coordinate variable has "vertex attribute position" 2

//out vec4 vertexColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
   gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
   //vertexColor = vec4(aColor, 1.0);//vec4(0.5, 0.0, 0.0, 1.0);
   TexCoord = aTexCoord;
}