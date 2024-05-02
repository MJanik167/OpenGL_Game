#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>
#include <filesystem>

#include <headers/VertexBuffer.h>
#include <headers/IndexBuffer.h>
#include <headers/VertexArray.h>
#include <headers/Shader.h>

using namespace std;

#define ASSERT(x) \
    if (!(x))     \
        __debugbreak();

#define GLCall(x)    \
    glClearErrors(); \
    x;               \
    ASSERT(glLogCall(#x, __FILE__, __LINE__))

static void glClearErrors()
{
    while (glGetError() != GL_NO_ERROR)
        ;
}

static bool glLogCall(const char *function, const char *file, int line)
{
    while (GLenum error = glGetError())
    {
        cout << "[OpenGL Error] (" << error << "):" << function << " " << file << ": " << line << endl;
        return false;
    }
    return true;
}

int main(void)
{

    // string path = "../resources/shaders/default.shader";
    // for (const auto &entry : std::filesystem::directory_iterator(path))
    //     std::cout << entry.path() << std::endl;

    // Inicjalizuje GLFW przy okazji sprawdza czy faktycznie się udała
    if (!glfwInit())
    {
        cout << "Failed to initialize GLFW" << endl;
        return -1;
    }

    // glfwWindowHint:Sets the specified window hint to the desired value.
    // GLFW_SAMPLES: Framebuffer MSAA samples (cokowliek to znaczy????)
    glfwWindowHint(GLFW_SAMPLES, 4);
    // Specyfikacja wersji, jako że używam 3.3 to 3 i 3 XD
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Specyfikacja profilu na profil CORE
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Tworzenie nowego okna
    GLFWwindow *window;
    window = glfwCreateWindow(640, 480, "Gaming", NULL, NULL);
    // error check
    if (window == NULL)
    {
        cout << "Failed to open GLFW window" << endl;
        return -1;
    }
    // wprowadzenie okna do aktualnego contextu
    glfwMakeContextCurrent(window);
    glfwSwapInterval(2);
    // załadowanie gladLoadera i przy okazji error check
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    const int size = 8;
    float positions[size] = {
        -0.5f, -0.5f, // 0
        -0.5f, 0.5f,  // 1
        0.5f, 0.5f,   // 2
        0.5f, -0.5f   // 3
    };

    unsigned int indexes[] = {
        0, 1, 2,
        0, 1, 3,
        3, 2, 1};

    VertexArray va;
    VertexBuffer vb(positions, size * sizeof(float));
    IndexBuffer ib(indexes, 9);
    VertexBufferLayout layout;
    layout.push_float(2);
    va.addBuffer(vb, layout);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    Shader shader("../resources/shaders/default.shader");
    shader.bind();

    float red = 0.0f;
    float inc = 0.05f;
    shader.setUniform4f("u_color", red, 0.4f, 0.7f, 1.0f);
    /*loop until user closes the window*/
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        va.bind();
        ib.bind();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        shader.setUniform4f("u_color", red, 0.4f, 0.7f, 1.0f);

        if (red > 1.0f || red < 0.0f)
        {
            inc = -inc;
        }

        red += inc;
        /* Swap front and back buffers*/
        glfwSwapBuffers(window);

        /*Poll for and process events*/
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}