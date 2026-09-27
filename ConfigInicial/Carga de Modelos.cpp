//Practica 06 - Carga de Modelos
// Emily Calderon
// 25 de Septiembre 2026
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
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );


// Camera
Camera camera( glm::vec3( 0.0f, 0.0f, 3.0f ) );
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;



int main( )
{
    // Init GLFW
    glfwInit( );
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "Practica 06 - Emily Calderon", nullptr, nullptr );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    
    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    
    // Set the required callback functions
    glfwSetKeyCallback( window, KeyCallback );
    glfwSetCursorPosCallback( window, MouseCallback );
    
    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    
    // OpenGL options
    glEnable( GL_DEPTH_TEST );
    
    // Setup and compile our shaders
    Shader shader( "Shader/modelLoading.vs", "Shader/modelLoading.frag" );
    
    // Load models
    Model arbol((char*)"Models/arbol.obj");
	Model dog((char*)"Models/RedDog.obj");
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

    glm::mat4 projection = glm::perspective( camera.GetZoom( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 100.0f );
    
  
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

        // Clear the colorbuffer
        glClearColor(0.65f, 0.85f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Draw the loaded model
        glm::mat4 model(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		dog.Draw(shader);

        //Gorrito cumpleañero
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 0.4f, 0.8f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        gorrito.Draw(shader);

        //Moño cumpleañero
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.9f));
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        moño.Draw(shader);
      
        //Pastel
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        pastel.Draw(shader);

        //Arbol
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-3.0f, -1.2f, 1.0f));
        model = glm::scale(model, glm::vec3(0.7f, 0.9f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arbol.Draw(shader);
      
        //Arbol2
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(3.0f, -1.2f, 1.0f));
        model = glm::scale(model, glm::vec3(0.7f, 0.9f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arbol.Draw(shader);

        //Arbol3
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(4.5f, -1.2f, 0.0f));
        model = glm::scale(model, glm::vec3(0.7f, 0.9f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arbol.Draw(shader);

        //Arbol4
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-4.5f, -1.2f, 0.0f));
        model = glm::scale(model, glm::vec3(0.7f, 0.9f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arbol.Draw(shader);
       
        //Arbol5
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(4.0f, -1.2f, -0.9f));
        model = glm::scale(model, glm::vec3(0.7f, 0.9f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arbol.Draw(shader);

        //Arbol6
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-4.0f, -1.2f, -0.9f));
        model = glm::scale(model, glm::vec3(0.7f, 0.9f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arbol.Draw(shader);
      
        //Arbol7
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-1.1f, -1.2f, -3.0f));
        model = glm::scale(model, glm::vec3(0.7f, 0.9f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arbol.Draw(shader);

        //Arbol8
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(1.1f, -1.2f, -3.0f));
        model = glm::scale(model, glm::vec3(0.7f, 0.9f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        arbol.Draw(shader);

        //Globos1
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(2.0f, -1.0f, -1.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        globos.Draw(shader);

        //Globos2
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-2.0f, -1.0f, -1.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        globos.Draw(shader);

        //Guirnalda1
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -2.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        guirnalda.Draw(shader);

        //Guirnalda2
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(2.0f, 1.0f, -2.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        guirnalda.Draw(shader);

        //Pasto
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, -1.2f, -1.0f));
        model = glm::scale(model, glm::vec3(2.5f, 1.0f, 2.5f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        pasto.Draw(shader);

        //Regalo1  Morado Izquierda
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-2.0f, -0.65f, -1.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        regalo.Draw(shader);

        //Regalo2 Rosa Izquierda
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-1.5f, -1.0f, -1.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        regalo2.Draw(shader);

        //Regalo2  Derecha
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(1.5f, -0.9f, 1.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.5f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        regalo2.Draw(shader);

        //Regalo2  Derecha2
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(1.5f, -0.6f, -0.5f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        regalo2.Draw(shader);

        //Regalo3 Azul Izquierda
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-1.2f, -1.2f, 0.7f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.5f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        regalo3.Draw(shader);

        //Regalo3 Derecha
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(1.5f, -1.0f, -0.5f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        regalo3.Draw(shader);

        //Regalo4 Amarillo Izquierda
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-1.8f, -1.0f, -0.4f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        regalo4.Draw(shader);
        
        //Regalo4 Derecha
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(2.2f, -0.8f, -0.9f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        regalo4.Draw(shader);





        // Swap the buffers
        glfwSwapBuffers( window );
    }
    
    glfwTerminate( );
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement( )
{
    // Camera controls
    if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] )
    {
        camera.ProcessKeyboard( FORWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] )
    {
        camera.ProcessKeyboard( BACKWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] )
    {
        camera.ProcessKeyboard( LEFT, deltaTime );
    }
    
    if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] )
    {
        camera.ProcessKeyboard( RIGHT, deltaTime );
    }

   
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action )
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if ( key >= 0 && key < 1024 )
    {
        if ( action == GLFW_PRESS )
        {
            keys[key] = true;
        }
        else if ( action == GLFW_RELEASE )
        {
            keys[key] = false;
        }
    }

 

 
}

void MouseCallback( GLFWwindow *window, double xPos, double yPos )
{
    if ( firstMouse )
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement( xOffset, yOffset );
}

