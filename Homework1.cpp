


#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include"Shader.h"
#include"SOIL2/soil2.h"



//vertex specification
GLuint VAO, VBO, EBO;
static void CreateVertexBuffer()
{
    /*
                        //postition                 //color
    GLfloat vertices[]= {  -0.5f, -0.5f, -0.0f,       1.0f, 0.0f, 0.0f,
                            0.0f, 0.5f, 0.0f,         0.0f, 1.0f, 0.0f,
                            0.5f, -0.5f, 0.0f,        0.0f, 0.0f, 1.0f };
    */
    GLfloat vertices[] = {
        //position          //color     //uv coords
        -0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,      0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f,     1.0f, 0.0f, 0.0f,     0.0f, 2.0f,
        0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,     2.0f, 0.0f,
        0.5f, 0.5f, 0.0f,      1.0f, 0.0f, 0.0f,     2.0f, 2.0f };
    unsigned int indices[] = {
        0,2,1,
        1,2,3
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
        8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
        8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}
GLuint texture0 ;
static void LoadTexture(char const* path)

{
    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_2D, texture0 );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height;
	unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGBA);
    if (image == nullptr)
    {
        std::cout << "Failed to load texture: " << path << std::endl;
        std::cout << SOIL_last_result() << std::endl;
        return;
    }
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0); 

}
static void RenderScreen(Shader& shader)
{
    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture0);

    //glUseProgram(shaderProgram);
    shader.UseProgram();
	shader.SetInt("Texture0", 0);
    glBindVertexArray(VAO);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

}

static void SetViewport(GLFWwindow* window) {
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    glViewport(0, 0, screenWidth, screenHeight);

}



int main(void)
{


    /* Initialize the library */
    if (!glfwInit())return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _APPLE_
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif



    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(640, 480, "F24040230", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);  

    if (GLEW_OK != glewInit()) {

        std::cout << "Failed to init glew! " << std::endl;
        glfwTerminate();
        return -1;

    }
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

    CreateVertexBuffer();

    // GLuint ShaderProgram = glCreateProgram();
   //  CreateShaderProgram(ShaderProgram);
    Shader shader = Shader("res/shaders/core.vs", "res/shaders/core.fs");
	LoadTexture("res/image/tex.png");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        SetViewport(window);
        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Animation speed: 8 frames per second
        int currentFrame = (int)(glfwGetTime() * 8.0) % 14;

        // Send current frame to vertex shader
        shader.UseProgram();
        shader.SetInt("currentFrame", currentFrame);

        // Render animation
        RenderScreen(shader);
        /* Swap front and back buffers */ 
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
} 