#include "Camera.h"

#define SDL_MAIN_HANDLED
#include <SDL.h>

int width = 800;
int height = 600;
ShaderFromFile* shader;

int main()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow(
        "Camera Test",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,height,
        SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL
    );

    SDL_GLContext glc;

    glc = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window,glc);
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

    SDL_Event event;
    bool run = true;

    // camera
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    float lastX = width / 2.0f;
    float lastY = height / 2.0f;
    bool firstMouse = true;

    // timing
    float deltaTime = 0.0f;	// time between current frame and last frame
    float lastFrame = 0.0f;

    shader = new ShaderFromFile("camera.vs","camera.fs");

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  

         0.5f,  0.5f,  0.5f, 
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f, 
         0.5f,  0.5f,  0.5f, 

        -0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f, -0.5f,  

        -0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f, -0.5f 
    };
    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (run)
    {
        SDL_PollEvent(&event);

        if(event.type == SDL_QUIT)
        {
            run = false;
        }

        if(event.type == SDL_MOUSEMOTION)
        {
                //printf("Mouse moved: xrel=%d, yrel=%d\n", event.motion.xrel, event.motion.yrel);
                
                int px = event.motion.x;
                int py = event.motion.y;
            
                float xpos = static_cast<float>(px);
                 float ypos = static_cast<float>(py);

                    if (firstMouse)
                    {
                        lastX = xpos;
                        lastY = ypos;
                        firstMouse = false;
                    }

                    float xoffset = xpos - lastX;
                    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

                    lastX = xpos;
                    lastY = ypos;

                    camera.ProcessMouseMovement(xoffset, yoffset);
         }

         if(event.type == SDL_MOUSEWHEEL)
         {
            int px = event.wheel.x;
            int py = event.wheel.y;
        
            camera.ProcessMouseScroll(static_cast<float>(py));
         }

        if(event.type == SDL_KEYDOWN)
        {
            if(event.key.keysym.sym == SDLK_ESCAPE)
            {
                run = false;
            }

            if(event.key.keysym.sym == SDLK_w)
            {
                camera.ProcessKeyboard(FORWARD, deltaTime);
            }

            if(event.key.keysym.sym == SDLK_s)
            {
                camera.ProcessKeyboard(BACKWARD, deltaTime);
            }

            if(event.key.keysym.sym == SDLK_a)
            {
                camera.ProcessKeyboard(LEFT, deltaTime);
            }

            if(event.key.keysym.sym == SDLK_d)
            {
                camera.ProcessKeyboard(RIGHT, deltaTime);
            }
        }

        float currentFrame = static_cast<float>(SDL_GetTicks64());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        shader->Use();

        glEnable(GL_DEPTH_TEST);

         // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
        GLuint proloc = shader->GetShaderSourceUniform("projection");
        glUniformMatrix4fv(proloc,1,GL_FALSE,&projection[0][0]);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        GLuint viewloc = shader->GetShaderSourceUniform("view");
        glUniformMatrix4fv(viewloc,1,GL_FALSE,&view[0][0]);

        // render boxes
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            GLuint modelloc = shader->GetShaderSourceUniform("model");
            glUniformMatrix4fv(modelloc,1,GL_FALSE,glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        SDL_GL_SwapWindow(window);


    }
    
    SDL_GL_DeleteContext(glc);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}