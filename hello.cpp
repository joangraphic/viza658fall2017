#include <iostream>
#include <fstream>
#include <string>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
//reads shaders from disk, compiles and links them, checks for errors 
#include <shader_s.h>
//image-loading library
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//try to add this to github
//window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0,0,width,height);
}
//input:"esc" key
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

//settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(){
	//initilization and configuration: question, how to check and update graphic driver
	if(!glfwInit()){
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_ANY_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	//create glfw window
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH,SCR_HEIGHT,"LearnGL",NULL,NULL);
	if (window == NULL){
		std::cout<<"Failed to create GLFW window"<<std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//initialize glad
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout<<"Failed to initialize GLAD"<<std::endl;
		return -1;
	}
	//build and compile shader
	Shader shader0("vertexshader.glsl","fragshader.glsl");
	
	//set vertices for the triangle
	/*float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, // right 
         0.0f,  0.5f, 0.0f  // top   
    };*/ 
	//set vertices for the rectangle
	float vertices[] = {
		//positions       //colors         //texture coordinate
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};
	//create vertex buffer and vertex array objects
    unsigned int VBO, VAO, EBO; 
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);
	//bind VBO to GL_ARRAY_BUFFER, copy previous defined the vertex data into the buffer's memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//specify the way to interpret vertex data
    //position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
	//texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
	
	// load and create a texture --- build texture class***
    unsigned int texture0, texture1;
    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_2D, texture0); 
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    //flip the y-axis during image loading
	stbi_set_flip_vertically_on_load(true);  
	int width, height, nrChannels;
    unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

	glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1); 
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);//awesomeface.png
    if (data)
    {
        //data type is of GL_RGBA: add transparency
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
	
	shader0.use();
	shader0.setInt("texture0", 0);
	shader0.setInt("texture1", 1);

	// keep EBO bound while VAO is active
    // unbind VBO: the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object
    //glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // unbind VAO: other VAO calls won't accidentally modify this VAO (generally not necessary)
    //glBindVertexArray(0); 

	//game loop
	while(!glfwWindowShouldClose(window)){
		//Input from keyboard: check if "escape" key is pressed or not
		processInput(window);
		//rendering code
		//clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// bind Texture on cooresponding texture units
        glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);
		
		//activate the shader
		shader0.use();

		//update the uniform color
		float timeValue = glfwGetTime();
		float greenValue = sin(timeValue)/2.0f + 0.5f;
		shader0.setFloat("ourColor",greenValue);

        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();//check event
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	/*
	//query # of vertex attributes
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout<<"Max nr of vertex attributes supported: "<<nrAttributes<<std::endl;
	*/
	//std::cout<<"Hello"<<std::endl;
	//std::cin.ignore();
	return 0;
}
