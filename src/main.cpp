#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>

#include <headers/VertexBuffer.h>
#include <headers/IndexBuffer.h>
#include <headers/VertexArray.h>

// #include <filesystem>
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

struct shaderSource
{
    string VertexSource;
    string FragmentSource;
};

static shaderSource parseShader(const string &filepath)
{
    // std::string path = filepath;
    // for (const auto &entry : std::filesystem::directory_iterator(path))
    //     std::cout << entry.path() << std::endl;

    ifstream stream;
    stream.open(filepath);

    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    string line;
    ShaderType type = ShaderType::NONE;
    stringstream ss[2];
    while (getline(stream, line))
    {
        if (line.find("#shader") != string::npos) // jeżeli nie znajdzie nic w danym stringu to zwraca jego koniec dlatego ma nie równać się npos(końcowej pozycji)
        {
            if (line.find("vertex") != string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return {ss[(int)ShaderType::VERTEX].str(), ss[(int)ShaderType::FRAGMENT].str()};
}

static unsigned int compileShader(unsigned int type, const string &source)
{
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        cout << " Failed to compile shader! " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << endl;
        cout << message << endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int createShader(const string &vertexShader, const string &fragmentShader)
{
    /* unsigned int to ten sam typ co GLuint, po prostu kwestia preferencji czy używa się typów GLowych czy tych z C++*/
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{

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

    /*tworzenie shadera*/
    shaderSource source = parseShader("../resources/shaders/default.shader");
    unsigned int shader = createShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);

    /*tworzenie vertex array*/
    /*bind buffer, czyli przypisuje te wartości do następnego buffera który zostanie wygenerowany w oknie, można je nadpisać kolejnym bindem*/

    const int size = 8;
    float positions[size] = {
        -0.5f, -0.5f, // 0
        -0.5f, 0.5f,  // 1
        0.5f, 0.5f,   // 2
        0.5f, -0.5f   // 3
    };

    unsigned int indexes[] = {
        0, 1, 2,
        2, 3, 0};

    /* Bind our Vertex Array Object as the current used object */

    VertexArray va;
    VertexBuffer vb(positions, size * sizeof(float));
    IndexBuffer ib(indexes, 6);
    VertexBufferLayout layout;
    layout.push<float>(2);
    va.addBuffer(vb, layout);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    int location = glGetUniformLocation(shader, "u_color");
    float red = 0.0f;
    float inc = 0.05f;
    /*loop until user closes the window*/
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform4f(location, red, 0.5f, 0.3f, 1.0f);

        va.bind();
        ib.bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
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

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}