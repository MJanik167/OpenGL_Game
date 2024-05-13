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

#include "vendor/imGui/imgui.h"
#include "vendor/imGui/imgui_impl_glfw.h"
#include "vendor/imGui/imgui_impl_opengl3.h"

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
    window = glfwCreateWindow(960, 540, "Gaming", NULL, NULL);
    // error check
    if (window == NULL)
    {
        cout << "Failed to open GLFW window" << endl;
        return -1;
    }
    // wprowadzenie okna do aktualnego contextu
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    // załadowanie gladLoadera i przy okazji error check
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float positions[] = {
        100.0f, 100.0f, 0.0f, 0.0f, // 0
        200.0f, 100.0f, 1.0f, 0.0f, // 1
        200.0f, 200.0f, 1.0f, 1.0f, // 2
        100.0f, 200.0f, 0.0f, 1.0f  // 3
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
    //  x,x,y,y,z,z
    glm::mat4 projectionMatrix = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
    glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));

    Shader shader("../resources/shaders/default.shader");
    shader.bind();

    Texture texture("../resources/textures/2.png");
    texture.bind();
    shader.setUniform1i("u_Texture", 0);

    va.unbind();
    vb.unbind();
    ib.unbind();
    shader.unbind();

    Renderer renderer;

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGuiIO &io = ImGui::GetIO();
    ImGui_ImplOpenGL3_Init("#version 130");
    ImGui::StyleColorsDark();

    glm::vec3 translation(200, 200, 0);
    /*loop until user closes the window*/
    while (!glfwWindowShouldClose(window))
    {
        renderer.clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), translation);
        glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

        shader.bind();
        shader.setUniformMat4f("u_MVP", mvp);

        renderer.draw(va, ib, shader);

        ImGui::Begin("Controls");

        ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 960.0f);

        ImGui::End();

        ImGui::Begin("PERFORMANCE");

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers*/
        glfwSwapBuffers(window);

        /*Poll for and process events*/
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext(nullptr); // usun nullptr w najblizszej przyszlosci

    glfwTerminate();
    return 0;
}