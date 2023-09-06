#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <learnopengl/shader_s.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION 
#include <stb_image.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;

double posX1, posY1, esc1, posX2, posY2, esc2 = 0;
float rot1, rot2 = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "KENNY PINCHAO: 1726380098", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------

    Shader ourShader("shaders/shader_exercise7.vs", "shaders/shader_exercise7.fs"); // you can name your shader files however you like

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        //FIGURA 1
        //POS                           //COlOLORES
        // TA
         -0.6f,-0.5f,  0.0f,           1.0f, 0.0f, 0.0f,    //0      
         -0.3f,-0.5f,  0.0f,           0.0f, 0.5f, 0.0f,    //1        
        -0.45f,-0.65f, 0.0f,           0.0f, 0.0f, 0.8f,    //2

        // TB        
         -0.4f, -0.5f, 0.0f,           1.0f, 0.0f, 0.0f,    //3
        -0.04f, -0.5f, 0.0f,           0.2f, 1.0f, 0.0f,    //4
        -0.04f,-0.16f, 0.0f,           0.2f, 0.0f, 1.0f,    //5

        // TC
        -0.04f,-0.02f, 0.0f,           1.0f, 0.0f, 0.0f,    //6
        -0.04f, -0.5f, 0.0f,           0.4f, 1.0f, 0.0f,    //7
         0.46f, -0.5f, 0.0f,           0.0f, 0.0f, 1.0f,    //8

         // TD
         0.26f, -0.5f,  0.0f,           1.0f, 0.0f, 0.0f,   //9
         0.36f, -0.82f, 0.0f,           0.6f, 1.0f, 7.0f,   //10
         0.46f, -0.61f, 0.0f,           0.0f, 0.0f, 1.0f,   //11

         // TE
         0.08f, 0.53f, 0.0f,            1.0f, 0.0f, 1.0f,   //12
        -0.25f,0.195f, 0.0f,            0.0f, 1.0f, 0.0f,   //13
         0.08f,-0.14f, 0.0f,            1.0f, 0.0f, 1.0f,   //14

         // CUADRADO
         0.2f,  0.72f, 0.0f,            1.0f, 0.0f, 0.0f,   //15
         0.0f,  0.84f, 0.0f,            0.0f, 1.0f, 0.8f,   //16
        -0.12f, 0.64f, 0.0f,            0.4f, 0.5f, 1.0f,   //17
         0.08f, 0.53f, 0.0f,            0.1f, 0.1f, 0.0f,   //18

         // ROMBO
        0.71f, 0.78f, 0.0f,             1.0f, 0.0f, 0.1f,   //19
         0.4f, 0.85f, 0.0f,             0.7f, 1.0f, 0.9f,   //20
         0.2f, 0.72f, 0.0f,             0.0f, 0.0f, 1.0f,   //21
         0.5f, 0.65f, 0.0f,             0.0f, 1.0f, 0.9f    //22
    };

    unsigned int indices[] = {
       0,  1,  2,
       5,  4,  3,
       6,  7,  8,
       9, 10, 11,
      12, 13, 14,
      15, 16, 17,
      15, 17, 18,
      19, 20, 21,
      19, 21, 22
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    // ---------------------------TEXTURAS---------------------------
    unsigned int texture1, texture2;


    // ---------------------------TEXTURA1---------------------------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    int width, height, nrChannels;
    unsigned char* data = stbi_load("textures/malla.jpg", &width, &height, &nrChannels, 0);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // note that we set the container wrapping method to GL_CLAMP_TO_EDGE
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // set texture filtering to nearest neighbor to clearly see the texels/pixels
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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

    // ---------------------------TEXTURA2---------------------------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    data = stbi_load("textures/cuadricula.jpg", &width, &height, &nrChannels, 0);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // note that we set the container wrapping method to GL_CLAMP_TO_EDGE
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // set texture filtering to nearest neighbor to clearly see the texels/pixels
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    //// color attribute
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    //glEnableVertexAttribArray(2);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);

    //USAMOS LOS SHADERS
    ourShader.use();
    // render loop
    // -----------
    esc1 = 1, esc2 = 1;
    GLuint offsetTA = 0 * sizeof(unsigned int);
    GLuint offsetTB = 3 * sizeof(unsigned int);
    GLuint offsetTC = 6 * sizeof(unsigned int);
    GLuint offsetTD = 9 * sizeof(unsigned int);
    GLuint offsetTE = 12 * sizeof(unsigned int);
    GLuint offsetC = 15 * sizeof(unsigned int);
    GLuint offsetR = 21 * sizeof(unsigned int);

    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.85f, 0.85f, 0.85f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render the triangle
        // ESPACIOS
       
        
        ourShader.use();
        glBindVertexArray(VAO);

        //SETEAMOS UN UNIFORM CON UN VALOR VARIANTE DE ACUERDO EL TIEMPO PASADO
        float timeValue = glfwGetTime();
        float value1 = (sin(timeValue * 2) / 2.0f) + 0.5f;

        int vertexColorLocation = glGetUniformLocation(ourShader.ID, "cambioColor1");
        glUniform1f(vertexColorLocation, value1);

        //---------------------------CONFIGURACION PARA LOS TRANSFORMS FIGURA1-------------------------------//
        ourShader.use();

        glm::mat4 transform = glm::mat4(1.0f); 
        transform = glm::translate(transform, glm::vec3(posX1, posY1, 0.0f));
        transform = glm::scale(transform, glm::vec3(esc1, esc1, esc1));
        transform = glm::rotate(transform, glm::radians(rot1), glm::vec3(0.0, 0.0, 1.0));
               
        unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        //DIBUJAMOS LAS FIGURAS CON UN EBO
        // GRUPO 1

        // BIND TEXTURES GRUPO1
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);


        // GRUPO 2
        // BIND TEXTURES GRUPO2
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)offsetTE);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)offsetC);

        //---------------------------CONFIGURACION PARA LOS TRANSFORMS FIGURA2-------------------------------//
        
        transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(posX2, posY2, 0.0f));
        transform = glm::scale(transform, glm::vec3(esc2, esc2, esc2));
        transform = glm::rotate(transform, glm::radians(rot2), glm::vec3(0.0, 0.0, 1.0));

        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));



        //DIBUJAMOS LAS FIGURAS CON UN EBO
        // GRUPO 1

        // BIND TEXTURES GRUPO1
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);


        // GRUPO 2
        // BIND TEXTURES GRUPO2
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)offsetTE);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)offsetC);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(1, &texture1);
    glDeleteTextures(1, &texture2);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    //OBJ1
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        posY1 += 0.0005;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        posY1 -= 0.0005;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        posX1 -= 0.0005;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        posX1 += 0.0005;

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) //rotar antihorario
        rot1 += 0.05f;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) //rotar horario
        rot1 -= 0.05f;

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        esc1 += 0.0005;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        esc1 -= 0.0005;

    //OBJ2
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        posY2 += 0.0005;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        posY2 -= 0.0005;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        posX2 -= 0.0005;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        posX2 += 0.0005;

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) //rotar antihorario
        rot2 += 0.05f;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) //rotar horario
        rot2 -= 0.05f;

    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        esc2 += 0.0005;
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        esc2 -= 0.0005;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
