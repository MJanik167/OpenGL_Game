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
#include <headers/Renderer.h>
#define STB_IMAGE_IMPLEMENTATION
#include "vendor/stb_image/stb_image.h"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include <headers/Texture.h>

using namespace std;

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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float positions[] = {
        -0.5f, -0.5f, 0.0f, 0.0f, // 0
        0.5f, -0.5f, 1.0f, 0.0f,  // 1
        0.5f, 0.5f, 1.0f, 1.0f,   // 2
        -0.5f, 0.5f, 0.0f, 1.0f   // 3
    };

    unsigned int indexes[] = {
        0, 1, 2,
        2, 3, 0};

    VertexArray va;
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));

    VertexBufferLayout layout;
    layout.push_float(2);
    layout.push_float(2);
    va.addBuffer(vb, layout);

    IndexBuffer ib(indexes, 6);

    // to tak jakbym ustawiał sobie granice okna
    // teraz lewy kraniec będzie na -2.0 prawy na 2.0 itd, ma znaczanie przy określaniu positions
    glm::mat4 projMatrix = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

    Shader shader("../resources/shaders/default.shader");
    shader.bind();
    shader.setUniformMat4f("u_MVP", projMatrix);

    float red = 0.0f;
    float inc = 0.05f;

    Texture texture("../resources/textures/2.png");
    texture.bind();
    shader.setUniform1i("u_Texture", 0);

    va.unbind();
    vb.unbind();
    ib.unbind();
    shader.unbind();

    Renderer renderer;

    /*loop until user closes the window*/
    while (!glfwWindowShouldClose(window))
    {
        renderer.clear();

        shader.bind();
        shader.setUniform4f("u_color", red, 0.4f, 0.7f, 1.0f);

        renderer.draw(va, ib, shader);

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