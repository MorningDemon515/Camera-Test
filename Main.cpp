
#define SDL_MAIN_HANDLED
#include "Main.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <windows.h>

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event event;
bool run = true;

GLuint VAO,VBO,texture,lightVAO;

ShaderFromFile* shader;
ShaderFromFile* lightshader;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, FPS);

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

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

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
       // glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        GameCode();
        RenderText(L"真正的立方体，真正的3D世界", (float)(800/2  - 100), (float)(720 - 200), 1.0f, glm::vec3(1.0, 0.0f, 0.0f));
        SDL_RenderPresent(renderer);

    }
    
    glDeleteBuffers(1,&texture);
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteVertexArrays(1,&lightVAO);

    delete shader;
    delete lightshader;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void GameCode()
{
    shader= new ShaderFromFile("light.vs","light.fs");
    lightshader = new ShaderFromFile("vertex.glsl","fragment.glsl");

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

    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //light
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // 只需要绑定VBO不用再次设置VBO的数据，因为箱子的VBO数据中已经包含了正确的立方体顶点数据
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 设置灯立方体的顶点属性（对我们的灯来说仅仅只有位置数据）
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    shader->Use();
    glUniform3f(shader->GetShaderSourceUniform("objectColor"), 1.0f, 0.5f, 0.31f);
    glUniform3f(shader->GetShaderSourceUniform("lightColor"),1.0f, 1.0f, 1.0f);
    glUniform3fv(shader->GetShaderSourceUniform("lightPos"),1,&lightPos[0]);
    glUniform3fv(shader->GetShaderSourceUniform("viewPos"), 1,&glm::value_ptr(camera.cp)[0]);

     // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.getViewMatrix();

    glUniformMatrix4fv(shader->GetShaderSourceUniform("projection"), 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(shader->GetShaderSourceUniform("view"), 1, GL_FALSE, &view[0][0]);

    // world transformation
    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(shader->GetShaderSourceUniform("model"), 1, GL_FALSE, &model[0][0]);

    // render the cube
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    


    lightshader->Use();
    glUniformMatrix4fv(lightshader->GetShaderSourceUniform("projection"), 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(lightshader->GetShaderSourceUniform("view"), 1, GL_FALSE, &view[0][0]);
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
    glUniformMatrix4fv(lightshader->GetShaderSourceUniform("model"), 1, GL_FALSE, &model[0][0]);

    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

}