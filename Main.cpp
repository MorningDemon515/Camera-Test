
#define SDL_MAIN_HANDLED
#include "Main.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <windows.h>

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event event;
bool run = true;

GLuint VAO,VBO,texture;

ShaderFromFile* shader;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, FPS);

void GameCode();

int main()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow(
        " ",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WIDTH,HEIGHT,
        SDL_WINDOW_SHOWN
    );

    renderer = SDL_CreateRenderer(window,3,SDL_RENDERER_ACCELERATED);

    SDL_RendererInfo info = {};
    int count = SDL_GetNumRenderDrivers();
    for (int i = 0; i < count; ++i) {
        SDL_GetRenderDriverInfo(i,&info);
        const char* name = info.name;
        printf("Render driver: %d: %s\n", i, name);
    }

    SDL_SetRelativeMouseMode(SDL_TRUE);

    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (run)
    {
        float currentFrame = SDL_GetTicks() / 1000.0f;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

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

                camera.processMouseMovement(event.motion.xrel, -event.motion.yrel);

         }

        if(event.type == SDL_KEYDOWN)
        {
            if(event.key.keysym.sym == SDLK_ESCAPE)
            {
                run = false;
            }

            if(event.key.keysym.sym == SDLK_p)
            {
                camera.switchMode(FPS);
            }

            if(event.key.keysym.sym == SDLK_f)
            {
                camera.switchMode(FLIGHT);
            }
        }

        const Uint8* state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_W] )
        {
            camera.processKeyboard(static_cast<SDL_Scancode>(SDL_SCANCODE_W), deltaTime);
        }

        if(state[SDL_SCANCODE_S])
        {
            camera.processKeyboard(static_cast<SDL_Scancode>(SDL_SCANCODE_S), deltaTime);
        }

        if(state[SDL_SCANCODE_A] )
        {
            camera.processKeyboard(static_cast<SDL_Scancode>(SDL_SCANCODE_A), deltaTime);
        }

        if(state[SDL_SCANCODE_D])
        {
            camera.processKeyboard(static_cast<SDL_Scancode>(SDL_SCANCODE_D), deltaTime);
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
        
        GameCode();
        RenderText(L"W, A, S, D移动", (float)(800/2  - 100), (float)(720 - 200), 1.0f, glm::vec3(1.0, 0.0f, 0.0f));
        SDL_RenderPresent(renderer);
    }
    
    glDeleteBuffers(1,&texture);
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);

    delete shader;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void GameCode()
{
    shader= new ShaderFromFile("vertex.glsl","fragment.glsl");
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
     
    shader->Use();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

    model = glm::rotate(model, glm::radians(60.0f), glm::vec3(1.0f, 1.0f, 0.0f));
    //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
   // projection = glm::perspective(glm::radians(45.0f), (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);
    
    GLuint modelloc = shader->GetShaderSourceUniform("model");
    GLuint viewloc = shader->GetShaderSourceUniform("view");
    GLuint proloc = shader->GetShaderSourceUniform("projection");

    glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewloc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(proloc, 1, GL_FALSE, &projection[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, 36);

}