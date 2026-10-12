//Practica 08
//Calderon Monroy Emily
//07 de Octubre 2026
//318041267

// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"
// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();


// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;


// Light attributes
glm::vec3 lightPos(0.5f, 0.5f, 2.5f);
float movelightPos = 0.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
float rot = 0.0f;
bool activanim = false;

int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica  08- Emily Calderon", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
    Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");



    // Load models
    Model red_dog((char*)"Models/RedDog.obj");
    Model arbol((char*)"Models/arbol.obj");
    Model gorrito((char*)"Models/gorro_fiesta.obj");
    Model globos((char*)"Models/ramo_globos.obj");
    Model guirnalda((char*)"Models/guirnalda_banderines.obj");
    Model moño((char*)"Models/mono_perrito.obj");
    Model pastel((char*)"Models/pastel_cumpleanos.obj");
    Model pasto((char*)"Models/pasto.obj");
    Model regalo((char*)"Models/regalo_1.obj");
    Model regalo2((char*)"Models/regalo_2.obj");
    Model regalo3((char*)"Models/regalo_3.obj");
    Model regalo4((char*)"Models/regalo_4.obj");
    Model sol((char*)"Models/sol.obj");
    Model luna((char*)"Models/luna.obj");
     
    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    float vertices[] = {
      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    // First, set the container's VAO (and VBO)
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Load textures
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

    image = stbi_load("Models/Texture_albedo.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(image);


    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();
        float radio = 5.0f;
        glm::vec3 centro(0.0f, 0.0f, -1.0f);
        float angulo = glm::radians(movelightPos);

        glm::vec3 orbitPos;
        orbitPos.x = centro.x + radio * cos(angulo);
        orbitPos.y = centro.y + radio * sin(angulo);   // órbita vertical (sol que sube y baja)
        orbitPos.z = centro.z;

        glm::vec3 lunaPos(centro.x - radio * cos(angulo),
            centro.y - radio * sin(angulo),
            centro.z);

        // Intensidad: el sol se apaga al bajar, la luna al subir
        float intSol = glm::clamp(sin(angulo) + 0.3f, 0.0f, 1.0f);
        float intLuna = glm::clamp(-sin(angulo) + 0.3f, 0.0f, 1.0f);
       

        // Clear the colorbuffer
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        lightingShader.Use();
        GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(lightPosLoc, orbitPos.x, orbitPos.y, orbitPos.z);
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


        // Set lights properties
     
       // Sol (cálido)
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.position"), orbitPos.x, orbitPos.y, orbitPos.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.30f * intSol, 0.22f * intSol, 0.12f * intSol);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 1.00f * intSol, 0.75f * intSol, 0.40f * intSol);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 1.00f * intSol, 0.85f * intSol, 0.60f * intSol);

        // Luna (fría)
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.position"), lunaPos.x, lunaPos.y, lunaPos.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.ambient"), 0.08f * intLuna, 0.10f * intLuna, 0.20f * intLuna);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.diffuse"), 0.30f * intLuna, 0.45f * intLuna, 0.95f * intLuna);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.specular"), 0.40f * intLuna, 0.50f * intLuna, 0.90f * intLuna);

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection)); 
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Set material properties
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.9f, 0.9f, 0.9f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.9f, 0.4f, 0.4f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);


        // Draw the loaded model
        glm::mat4 model= glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
		red_dog.Draw(lightingShader);
        glDrawArrays(GL_TRIANGLES, 0, 36);
      
        //Gorrito cumpleañero
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 0.4f, 0.8f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        gorrito.Draw(lightingShader);

        //Moño cumpleañero
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.9f));
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        moño.Draw(lightingShader);

        //Pastel
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 1.5f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        pastel.Draw(lightingShader);

        //Arbol
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-3.0f, -1.2f, 1.0f));
        model = glm::scale(model, glm::vec3(0.7f, 0.9f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arbol.Draw(lightingShader);

        //Arbol2
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(3.0f, -1.2f, 1.0f));
        model = glm::scale(model, glm::vec3(0.7f, 0.9f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arbol.Draw(lightingShader);

        //Arbol3
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(4.5f, -1.2f, 0.0f));
        model = glm::scale(model, glm::vec3(0.7f, 0.9f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arbol.Draw(lightingShader);

        //Arbol4
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-4.5f, -1.2f, 0.0f));
        model = glm::scale(model, glm::vec3(0.7f, 0.9f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arbol.Draw(lightingShader);

        //Arbol5
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(4.0f, -1.2f, -0.9f));
        model = glm::scale(model, glm::vec3(0.7f, 0.9f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arbol.Draw(lightingShader);

        //Arbol6
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-4.0f, -1.2f, -0.9f));
        model = glm::scale(model, glm::vec3(0.7f, 0.9f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arbol.Draw(lightingShader);

        //Arbol7
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-1.1f, -1.2f, -3.0f));
        model = glm::scale(model, glm::vec3(0.7f, 0.9f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arbol.Draw(lightingShader);

        //Arbol8
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(1.1f, -1.2f, -3.0f));
        model = glm::scale(model, glm::vec3(0.7f, 0.9f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arbol.Draw(lightingShader);

        //Globos1
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(2.0f, -1.0f, -1.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        globos.Draw(lightingShader);

        //Globos2
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-2.0f, -1.0f, -1.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        globos.Draw(lightingShader);

        //Guirnalda1
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -2.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        guirnalda.Draw(lightingShader);

        //Guirnalda2
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(2.0f, 1.0f, -2.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        guirnalda.Draw(lightingShader);

        //Pasto
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, -1.2f, -1.0f));
        model = glm::scale(model, glm::vec3(2.5f, 1.0f, 2.5f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        pasto.Draw(lightingShader);

        //Regalo1  Morado Izquierda
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-2.0f, -0.65f, -1.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        regalo.Draw(lightingShader);

        //Regalo2 Rosa Izquierda
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-1.5f, -1.0f, -1.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        regalo2.Draw(lightingShader);

        //Regalo2  Derecha
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(1.5f, -0.9f, 1.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.5f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        regalo2.Draw(lightingShader);

        //Regalo2  Derecha2
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(1.5f, -0.6f, -0.5f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        regalo2.Draw(lightingShader);

        //Regalo3 Azul Izquierda
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-1.2f, -1.2f, 0.7f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.5f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        regalo3.Draw(lightingShader);

        //Regalo3 Derecha
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(1.5f, -1.0f, -0.5f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        regalo3.Draw(lightingShader);

        //Regalo4 Amarillo Izquierda
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-1.8f, -1.0f, -0.4f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        regalo4.Draw(lightingShader);

        //Regalo4 Derecha
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(2.2f, -0.8f, -0.9f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        regalo4.Draw(lightingShader);

        glBindVertexArray(0); 




        lampshader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(
            centro.x - radio * cos(angulo),
            centro.y - radio * sin(angulo),
            centro.z));
        model = glm::scale(model, glm::vec3(1.5f));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        luna.Draw(lampshader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, orbitPos);
        model = glm::scale(model, glm::vec3(1.5f));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        sol.Draw(lampshader);
      


        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (activanim)
    {
        if (rot > -90.0f)
            rot -= 0.1f;
    }

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }

    if (keys[GLFW_KEY_O])
    {
        movelightPos += 100.0f * deltaTime;   // 100 grados por segundo
    }

    if (keys[GLFW_KEY_L])
    {
        movelightPos -= 100.0f * deltaTime;
    }

    if (movelightPos >= 360.0f) movelightPos -= 360.0f;
    if (movelightPos < 0.0f)    movelightPos += 360.0f;
    


}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}


