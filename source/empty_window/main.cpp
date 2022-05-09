#include <gl/gl3w.h>
#include <SDL.h>
#include <SDL_syswm.h>
#include <cassert>
#include <regex>

// wglCreateContextAttribsARB

const char vertShader[] = R"(#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;
out vec3 frag_color;
void main() {
    frag_color = color;
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
})";

const char fragShader[] = R"(#version 330 core
in vec3 frag_color;
out vec4 FragColor;
void main() {
    FragColor = vec4(frag_color, 1.0);
})";

#ifdef _WIN32
typedef HWND NativeWindow;
#else
#ifdef __ANDROID__
#endif
#endif

// IApplication* delegate = nullptr;
bool SDLQuit = false;

SDL_Window* window = nullptr;

void SDL_EventProc(const SDL_Event& _event);

extern "C" int main(int argc, char** argv)
{

    SDL_Event event;
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
    // SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
#ifdef __ANDROID__
    std::string assetRoot = SDL_AndroidGetExternalStoragePath();
#else
    const char* basePath = SDL_GetBasePath();
    std::string assetRoot = basePath;
    assetRoot.append("../../"); // remove "Windows/x64"
#endif
    window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    struct SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    if (-1 == SDL_GetWindowWMInfo(window, &wmInfo)) {
        return -1;
    }
    assert(wmInfo.subsystem == SDL_SYSWM_WINDOWS);
    auto ogl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, ogl_context);
    SDL_GL_SetSwapInterval(1);
    if (gl3wInit()) {
        fprintf(stderr, "failed to initialize OpenGL\n");
        return -1;
    }
    if (!gl3wIsSupported(4, 4)) {
        fprintf(stderr, "OpenGL 3.2 not supported\n");
        return -1;
    }
    printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

    //創建program
    uint32_t shaderProgram = glCreateProgram();
    //創建頂點著色器
    uint32_t vs = glCreateShader(GL_VERTEX_SHADER);
    //創建片段著色器
    uint32_t fs = glCreateShader(GL_FRAGMENT_SHADER);
    auto p = (char const*)vertShader;
     //定義著色器代碼來源
    glShaderSource(vs, 1, &p, nullptr);
    //編譯頂點著色器
    glCompileShader(vs);

    p = (char const*)fragShader;
    //定義片段著色器代碼來源
    glShaderSource(fs, 1, &p, nullptr);
    //編譯片段著色器
    glCompileShader(fs);
    //將編譯後的著色器綁到
    program上
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);
 
    //定義頂點結構體
    struct pos_t {
        float x, y, z;
    };
    //定義顔色結構體
    struct color_t {
        float r, g, b;
    };
   //頂點數組
    pos_t pos_arr[] = { {0, 1, 1}, {-1, -1, 1}, {1, -1, 1} };
    //顔色數組
    color_t color_arr[] = { {1.0, 0.0, 0}, {0, 1, 0}, {0, 0, 1} };
    //頂點索引
    uint32_t indices[] = {0, 1, 2};
    //創建3個buffer,分別存放頂點位置信息、顔色信息、頂點索引，glGenBuffers 還沒有真正創建
    uint32_t posVBO = 0;
    uint32_t colorVBO = 0;
    uint32_t ibo = 0;
    glGenBuffers(1, &posVBO);
    glGenBuffers(1, &colorVBO);
    glGenBuffers(1, &ibo);
    //buffer 類型
    enum class VBOBindPoint {
        Position = 0,
        Color = 1
    };
    //屬性類型
    enum class VABAttriPoint {
        Position = 0,
        Color = 1
    };
    //指定posVBO buffer類型
    glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    //真正創建buffer，并且將頂點位置信息，讀入posVBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(pos_arr), pos_arr, GL_STATIC_DRAW);
    //指定colorVBO buffer類型
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    //真正創建buffer,并且將頂點顔色信息，讀入colorVBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(color_arr), color_arr, GL_STATIC_DRAW);
   //指定ibo buffer類型
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    //真正創建buffer,并且將頂點索引信息，讀入ibo
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    uint32_t vertLayout = 0;
    //創建頂點佈局
    glGenVertexArrays(1, &vertLayout);
    //指定將要設置的佈局
    glBindVertexArray(vertLayout);
    //設置第一個屬性，可用
    glEnableVertexAttribArray((uint32_t)(VABAttriPoint::Position)); // enable attr 0
    //該屬性的格式（3 個float大小）
    glVertexAttribFormat((uint32_t)(VABAttriPoint::Position), 3, GL_FLOAT, GL_FALSE, 0);
    //指定該屬性從哪個buffer獲取（從posVBO）
    glVertexAttribBinding((uint32_t)(VABAttriPoint::Position), (uint32_t)(VBOBindPoint::Position));
    //指定
    glVertexBindingDivisor((uint32_t)(VABAttriPoint::Position), 0);
    //設置第二個屬性可用
    glEnableVertexAttribArray((uint32_t)(VABAttriPoint::Color)); // enable attr 0
    //該屬性的格式（3個float大小）
    glVertexAttribFormat((uint32_t)(VABAttriPoint::Color), 3, GL_FLOAT, GL_FALSE, 0);
    //指定該屬性從哪個buffer獲取（從colorVBO）
    glVertexAttribBinding((uint32_t)(VABAttriPoint::Color), (uint32_t)(VBOBindPoint::Color));
    //
    glVertexBindingDivisor((uint32_t)(VABAttriPoint::Color), 0);
    //避免再次修改佈局信息
    glBindVertexArray(0);
    
    while (!SDLQuit) {
        while (SDL_PollEvent(&event)) {
            SDL_EventProc(event);
        }
        //清屏
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(1, 1, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        //用哪個佈局信息繪製
        glBindVertexArray(vertLayout);01
        //指定頂點位置信息
        glBindVertexBuffer((uint32_t)(VBOBindPoint::Position), posVBO, 0, 12);
        //指定頂點顔色信息
        glBindVertexBuffer((uint32_t)(VBOBindPoint::Color), colorVBO, 0, 12);
        //指定頂點索引
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        //指定繪製程序
        glUseProgram(shaderProgram);
        //開始繪製
        glDrawElements(GL_TRIANGLE_STRIP, 3, GL_UNSIGNED_INT, nullptr);
        //
        SDL_GL_SwapWindow(window);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void SDL_EventProc(const SDL_Event& _event)
{
    switch (_event.type) {
    case SDL_QUIT: {
        SDLQuit = true;
        break;
    }
    case SDL_WINDOWEVENT: {
        switch (_event.window.event) {
        case SDL_WINDOWEVENT_SIZE_CHANGED: {
            break;
        }
        case SDL_WINDOWEVENT_MINIMIZED:
        case SDL_WINDOWEVENT_HIDDEN: {
            break;
        }
        case SDL_WINDOWEVENT_RESTORED: {
            int w, h;
            SDL_GetWindowSize(window, &w, &h);
            break;
        }
        }
        break;
    }
    case SDL_KEYDOWN: {
        break;
    }
    case SDL_MOUSEBUTTONDOWN: {
        if (_event.button.button) {
        }
        break;
    }
    case SDL_MOUSEBUTTONUP: {
        break;
    }
    case SDL_MOUSEMOTION: {
        break;
    }
    }
    return;
}