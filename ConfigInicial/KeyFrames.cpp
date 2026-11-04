// Previo 12
// Calderon Monroy Emily
// 03 de Noviembre del 2026
// 318041267

#include <iostream>
#include <cmath>
#include <cstdlib>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Modelos y shaders
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Prototipos
void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();
void SaveFrame();
void ResetElements();
void Interpolation();
void CreateSitWaveAnimation();

// Ventana
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camara
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = HEIGHT / 2.0f;
bool keys[1024] = { false };
bool firstMouse = true;

// Iluminacion
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
glm::vec3 Light1(0.0f);
bool active = false;

glm::vec3 pointLightPositions[] = {
    glm::vec3(0.0f, 2.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f)
};

// Cubo utilizado para visualizar la fuente de luz
float vertices[] = {
    -0.5f,-0.5f,-0.5f, 0,0,-1,  0.5f,-0.5f,-0.5f, 0,0,-1,  0.5f,0.5f,-0.5f, 0,0,-1,
     0.5f,0.5f,-0.5f, 0,0,-1, -0.5f,0.5f,-0.5f, 0,0,-1, -0.5f,-0.5f,-0.5f, 0,0,-1,
    -0.5f,-0.5f, 0.5f, 0,0,1,   0.5f,-0.5f, 0.5f, 0,0,1,   0.5f,0.5f,0.5f, 0,0,1,
     0.5f,0.5f,0.5f, 0,0,1,  -0.5f,0.5f,0.5f, 0,0,1,  -0.5f,-0.5f,0.5f, 0,0,1,
    -0.5f,0.5f,0.5f, -1,0,0, -0.5f,0.5f,-0.5f, -1,0,0, -0.5f,-0.5f,-0.5f, -1,0,0,
    -0.5f,-0.5f,-0.5f, -1,0,0, -0.5f,-0.5f,0.5f, -1,0,0, -0.5f,0.5f,0.5f, -1,0,0,
     0.5f,0.5f,0.5f, 1,0,0,  0.5f,0.5f,-0.5f, 1,0,0,  0.5f,-0.5f,-0.5f, 1,0,0,
     0.5f,-0.5f,-0.5f, 1,0,0, 0.5f,-0.5f,0.5f, 1,0,0, 0.5f,0.5f,0.5f, 1,0,0,
    -0.5f,-0.5f,-0.5f, 0,-1,0, 0.5f,-0.5f,-0.5f, 0,-1,0, 0.5f,-0.5f,0.5f, 0,-1,0,
     0.5f,-0.5f,0.5f, 0,-1,0, -0.5f,-0.5f,0.5f, 0,-1,0, -0.5f,-0.5f,-0.5f, 0,-1,0,
    -0.5f,0.5f,-0.5f, 0,1,0, 0.5f,0.5f,-0.5f, 0,1,0, 0.5f,0.5f,0.5f, 0,1,0,
     0.5f,0.5f,0.5f, 0,1,0, -0.5f,0.5f,0.5f, 0,1,0, -0.5f,0.5f,-0.5f, 0,1,0
};

// Variables de transformacion del perro
float dogPosX = 0.0f;
float dogPosY = 0.0f;
float dogPosZ = 0.0f;
float rotDog = 0.0f;

float head = 0.0f;
float tail = 0.0f;

float frontLeftLeg = 0.0f;
float frontRightLeg = 0.0f;
float backLeftLeg = 0.0f;
float backRightLeg = 0.0f;

float bodyTilt = 0.0f;
float bodyOffsetY = 0.0f;

float rotBall = 0.0f;

// Sistema de Keyframes
#define MAX_FRAMES 12
int i_max_steps = 190;
int i_curr_steps = 0;

struct FRAME {
    float dogPosX, dogPosY, dogPosZ;
    float rotDog;
    float bodyTilt, bodyOffsetY;
    float head, tail;
    float frontLeftLeg, frontRightLeg;
    float backLeftLeg, backRightLeg;

    float incX, incY, incZ;
    float rotDogInc;
    float bodyTiltInc, bodyOffsetYInc;
    float headInc, tailInc;
    float frontLeftInc, frontRightInc;
    float backLeftInc, backRightInc;
};

FRAME KeyFrame[MAX_FRAMES] = {};
int FrameIndex = 0;
bool play = false;
int playIndex = 0;

// Tiempo
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Guarda la postura actual en un fotograma
void SaveFrame()
{
    if (FrameIndex >= MAX_FRAMES) {
        std::cout << "Se alcanzo el maximo de Keyframes.\n";
        return;
    }

    FRAME& f = KeyFrame[FrameIndex];

    f.dogPosX = dogPosX;
    f.dogPosY = dogPosY;
    f.dogPosZ = dogPosZ;
    f.rotDog = rotDog;

    f.bodyTilt = bodyTilt;
    f.bodyOffsetY = bodyOffsetY;
    f.head = head;
    f.tail = tail;

    f.frontLeftLeg = frontLeftLeg;
    f.frontRightLeg = frontRightLeg;
    f.backLeftLeg = backLeftLeg;
    f.backRightLeg = backRightLeg;

    std::cout << "Keyframe guardado: " << FrameIndex << std::endl;
    FrameIndex++;
}

// Recupera la postura del primer fotograma
void ResetElements()
{
    if (FrameIndex == 0) return;

    const FRAME& f = KeyFrame[0];

    dogPosX = f.dogPosX;
    dogPosY = f.dogPosY;
    dogPosZ = f.dogPosZ;
    rotDog = f.rotDog;

    bodyTilt = f.bodyTilt;
    bodyOffsetY = f.bodyOffsetY;
    head = f.head;
    tail = f.tail;

    frontLeftLeg = f.frontLeftLeg;
    frontRightLeg = f.frontRightLeg;
    backLeftLeg = f.backLeftLeg;
    backRightLeg = f.backRightLeg;
}

// Calcula los incrementos entre dos fotogramas consecutivos
void Interpolation()
{
    if (playIndex < 0 || playIndex >= FrameIndex - 1) return;

    const FRAME& a = KeyFrame[playIndex];
    const FRAME& b = KeyFrame[playIndex + 1];
    FRAME& f = KeyFrame[playIndex];

    const float steps = static_cast<float>(i_max_steps);

    f.incX = (b.dogPosX - a.dogPosX) / steps;
    f.incY = (b.dogPosY - a.dogPosY) / steps;
    f.incZ = (b.dogPosZ - a.dogPosZ) / steps;
    f.rotDogInc = (b.rotDog - a.rotDog) / steps;

    f.bodyTiltInc = (b.bodyTilt - a.bodyTilt) / steps;
    f.bodyOffsetYInc = (b.bodyOffsetY - a.bodyOffsetY) / steps;
    f.headInc = (b.head - a.head) / steps;
    f.tailInc = (b.tail - a.tail) / steps;

    f.frontLeftInc = (b.frontLeftLeg - a.frontLeftLeg) / steps;
    f.frontRightInc = (b.frontRightLeg - a.frontRightLeg) / steps;
    f.backLeftInc = (b.backLeftLeg - a.backLeftLeg) / steps;
    f.backRightInc = (b.backRightLeg - a.backRightLeg) / steps;
}

// Crea una secuencia inicial de ejemplo: de pie, sentarse y saludar.
// Los angulos pueden necesitar ajuste segun los pivotes/orientacion de los OBJ.
void CreateSitWaveAnimation()
{
    FrameIndex = 0;

    // Pose 1: de pie
    dogPosX = 0.0f; dogPosY = 0.0f; dogPosZ = 0.0f;
    rotDog = 0.0f;
    bodyTilt = 0.0f; bodyOffsetY = 0.0f;
    head = 0.0f; tail = 0.0f;
    frontLeftLeg = 0.0f; frontRightLeg = 0.0f;
    backLeftLeg = 0.0f; backRightLeg = 0.0f;
    SaveFrame();

    // Pose 2: comienza a sentarse
    bodyOffsetY = -0.025f;
    backLeftLeg = 28.0f;
    backRightLeg = 28.0f;
    frontLeftLeg = -8.0f;
    frontRightLeg = -8.0f;
    tail = 8.0f;
    SaveFrame();

    // Pose 3: postura sentada
    bodyOffsetY = -0.055f;
    bodyTilt = -5.0f;
    backLeftLeg = 62.0f;
    backRightLeg = 62.0f;
    frontLeftLeg = 0.0f;
    frontRightLeg = 0.0f;
    head = -5.0f;
    tail = -8.0f;
    SaveFrame();

    // Pose 4: levanta la pata delantera derecha
    frontRightLeg = -58.0f;
    head = 4.0f;
    tail = 12.0f;
    SaveFrame();

    // Pose 5: primer movimiento del saludo
    frontRightLeg = -78.0f;
    tail = -12.0f;
    SaveFrame();

    // Pose 6: segundo movimiento del saludo
    frontRightLeg = -48.0f;
    tail = 12.0f;
    SaveFrame();

    // Pose 7: regresa a la posicion de saludo
    frontRightLeg = -72.0f;
    tail = -8.0f;
    SaveFrame();

    // Pose 8: mantiene la pata levantada
    frontRightLeg = -58.0f;
    tail = 0.0f;
    SaveFrame();

    // Dejar el modelo en la pose inicial
    ResetElements();

    std::cout << "Secuencia de sentado y saludo creada con "
        << FrameIndex << " Keyframes." << std::endl;
}

int main()
{
    if (!glfwInit()) {
        std::cout << "No se pudo inicializar GLFW." << std::endl;
        return EXIT_FAILURE;
    }

    GLFWwindow* window = glfwCreateWindow(
        WIDTH, HEIGHT, "Previo 12 - Emily Calderon", nullptr, nullptr
    );

    if (window == nullptr) {
        std::cout << "No se pudo crear la ventana GLFW." << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "No se pudo inicializar GLEW." << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Shaders
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

    // Modelos OBJ
    Model DogBody((char*)"Models/DogBody.obj");
    Model HeadDog((char*)"Models/HeadDog.obj");
    Model DogTail((char*)"Models/TailDog.obj");
    Model F_RightLeg((char*)"Models/F_RightLegDog.obj");
    Model F_LeftLeg((char*)"Models/F_LeftLegDog.obj");
    Model B_RightLeg((char*)"Models/B_RightLegDog.obj");
    Model B_LeftLeg((char*)"Models/B_LeftLegDog.obj");
    Model Piso((char*)"Models/piso.obj");
    Model Ball((char*)"Models/ball.obj");

    // Crear la secuencia de animacion solicitada
    CreateSitWaveAnimation();

    // VAO y VBO para visualizar la fuente de luz
    GLuint VBO = 0, VAO = 0;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
        6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Unidades de textura
    lightingShader.Use();
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

    glm::mat4 projection = glm::perspective(
        camera.GetZoom(),
        (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT,
        0.1f, 100.0f
    );

    // Ciclo principal
    while (!glfwWindowShouldClose(window)) {
        GLfloat currentFrame = (GLfloat)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();
        Animation();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        lightingShader.Use();

        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);

        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(viewPosLoc,
            camera.GetPosition().x,
            camera.GetPosition().y,
            camera.GetPosition().z
        );

        // Luz direccional
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"),
            -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"),
            0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"),
            0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"),
            0.3f, 0.3f, 0.3f);

        // Luz puntual
        glm::vec3 lightColor(
            fabs(sin(glfwGetTime() * Light1.x)),
            fabs(sin(glfwGetTime() * Light1.y)),
            fabs(sin(glfwGetTime() * Light1.z))
        );

        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"),
            pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"),
            lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"),
            lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"),
            1.0f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);

        // Luz tipo reflector
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"),
            camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"),
            camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"),
            0.2f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"),
            0.2f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"),
            0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"),
            glm::cos(glm::radians(12.0f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"),
            glm::cos(glm::radians(18.0f)));

        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

        glm::mat4 view = camera.GetViewMatrix();
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 model(1.0f);
        glm::mat4 modelTemp(1.0f);

        // Piso
        model = glm::mat4(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
        Piso.Draw(lightingShader);

        // Transformacion raiz: posicion y orientacion general del perro.
        // bodyOffsetY desplaza todo el conjunto para bajar/subir el perro.
        modelTemp = glm::mat4(1.0f);
        modelTemp = glm::translate(modelTemp,
            glm::vec3(dogPosX, dogPosY + bodyOffsetY, dogPosZ));
        modelTemp = glm::rotate(modelTemp, glm::radians(rotDog),
            glm::vec3(0.0f, 1.0f, 0.0f));

        // Cuerpo: inclinacion local
        model = modelTemp;
        model = glm::rotate(model, glm::radians(bodyTilt),
            glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        DogBody.Draw(lightingShader);

        // Cabeza
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f));
        model = glm::rotate(model, glm::radians(head),
            glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        HeadDog.Draw(lightingShader);

        // Cola
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.0f, 0.026f, -0.288f));
        model = glm::rotate(model, glm::radians(tail),
            glm::vec3(0.0f, 0.0f, -1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        DogTail.Draw(lightingShader);

        // Pata delantera izquierda
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.112f, -0.044f, 0.074f));
        model = glm::rotate(model, glm::radians(frontLeftLeg),
            glm::vec3(-1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        F_LeftLeg.Draw(lightingShader);

        // Pata delantera derecha
        model = modelTemp;
        model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f));
        model = glm::rotate(model, glm::radians(frontRightLeg),
            glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        F_RightLeg.Draw(lightingShader);

        // Pata trasera izquierda
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.082f, -0.046f, -0.218f));
        model = glm::rotate(model, glm::radians(backLeftLeg),
            glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        B_LeftLeg.Draw(lightingShader);

        // Pata trasera derecha
        model = modelTemp;
        model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f));
        model = glm::rotate(model, glm::radians(backRightLeg),
            glm::vec3(-1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        B_RightLeg.Draw(lightingShader);

        // Pelota (se conserva del proyecto original)
        model = glm::mat4(1.0f);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
        model = glm::rotate(model, glm::radians(rotBall),
            glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Ball.Draw(lightingShader);
        glDisable(GL_BLEND);

        // Visualizacion de la fuente de luz
        lampShader.Use();

        modelLoc = glGetUniformLocation(lampShader.Program, "model");
        viewLoc = glGetUniformLocation(lampShader.Program, "view");
        projLoc = glGetUniformLocation(lampShader.Program, "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        model = glm::mat4(1.0f);
        model = glm::translate(model, pointLightPositions[0]);
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

// Controles de teclado
void DoMovement()
{
    // Movimiento general del perro
    if (keys[GLFW_KEY_2]) rotDog += 1.0f;
    if (keys[GLFW_KEY_3]) rotDog -= 1.0f;
    if (keys[GLFW_KEY_H]) dogPosZ += 0.01f;
    if (keys[GLFW_KEY_Y]) dogPosZ -= 0.01f;
    if (keys[GLFW_KEY_G]) dogPosX -= 0.01f;
    if (keys[GLFW_KEY_J]) dogPosX += 0.01f;

    // Cabeza
    if (keys[GLFW_KEY_4]) head += 1.0f;
    if (keys[GLFW_KEY_5]) head -= 1.0f;

    // Pata delantera izquierda
    if (keys[GLFW_KEY_6]) frontLeftLeg += 1.0f;
    if (keys[GLFW_KEY_7]) frontLeftLeg -= 1.0f;

    // Pata delantera derecha
    if (keys[GLFW_KEY_8]) frontRightLeg += 1.0f;
    if (keys[GLFW_KEY_9]) frontRightLeg -= 1.0f;

    // Pata trasera izquierda
    if (keys[GLFW_KEY_C]) backLeftLeg += 1.0f;
    if (keys[GLFW_KEY_V]) backLeftLeg -= 1.0f;

    // Pata trasera derecha
    if (keys[GLFW_KEY_B]) backRightLeg += 1.0f;
    if (keys[GLFW_KEY_N]) backRightLeg -= 1.0f;

    // Cola
    if (keys[GLFW_KEY_COMMA]) tail += 1.0f;
    if (keys[GLFW_KEY_PERIOD]) tail -= 1.0f;

    // Inclinacion del cuerpo
    if (keys[GLFW_KEY_MINUS]) bodyTilt += 1.0f;
    if (keys[GLFW_KEY_EQUAL]) bodyTilt -= 1.0f;

    // Altura general del perro
    if (keys[GLFW_KEY_I]) bodyOffsetY += 0.001f;
    if (keys[GLFW_KEY_O]) bodyOffsetY -= 0.001f;

    // Camara
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// Callback de teclado
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }

    // Guardar pose manual con K
    if (key == GLFW_KEY_K && action == GLFW_PRESS) {
        if (!play) SaveFrame();
    }

    // Reproducir o pausar con L
    if (key == GLFW_KEY_L && action == GLFW_PRESS) {
        if (!play && FrameIndex > 1) {
            ResetElements();
            playIndex = 0;
            i_curr_steps = 0;
            Interpolation();
            play = true;
            std::cout << "Reproduciendo Keyframes..." << std::endl;
        }
        else if (play) {
            play = false;
            std::cout << "Animacion pausada." << std::endl;
        }
    }

    // Recrear la secuencia automatica con R
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        play = false;
        CreateSitWaveAnimation();
    }

    // Iluminacion con ESPACIO
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        active = !active;
        Light1 = active ? glm::vec3(0.2f, 0.8f, 1.0f) : glm::vec3(0.0f);
    }
}

// Actualiza todos los componentes durante la animacion
void Animation()
{
    if (!play || FrameIndex < 2) return;

    if (i_curr_steps >= i_max_steps) {
        playIndex++;

        if (playIndex >= FrameIndex - 1) {
            std::cout << "Termina animacion." << std::endl;
            play = false;
            playIndex = 0;
            return;
        }

        i_curr_steps = 0;
        Interpolation();
    }
    else {
        FRAME& f = KeyFrame[playIndex];

        dogPosX += f.incX;
        dogPosY += f.incY;
        dogPosZ += f.incZ;
        rotDog += f.rotDogInc;

        bodyTilt += f.bodyTiltInc;
        bodyOffsetY += f.bodyOffsetYInc;
        head += f.headInc;
        tail += f.tailInc;

        frontLeftLeg += f.frontLeftInc;
        frontRightLeg += f.frontRightInc;
        backLeftLeg += f.backLeftInc;
        backRightLeg += f.backRightInc;

        i_curr_steps++;
    }
}

// Movimiento del mouse para la camara
void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse) {
        lastX = (GLfloat)xPos;
        lastY = (GLfloat)yPos;
        firstMouse = false;
    }

    GLfloat xOffset = (GLfloat)xPos - lastX;
    GLfloat yOffset = lastY - (GLfloat)yPos;

    lastX = (GLfloat)xPos;
    lastY = (GLfloat)yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}
