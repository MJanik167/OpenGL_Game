#include <headers/Shader.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <glad/glad.h>
#include "vendor/glm/glm.hpp"

using namespace std;

Shader::Shader(const string &filepath) : m_FilePath(filepath), m_RendererID(0)
{
  shaderSource source = parseShader(filepath);
  m_RendererID = createShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
  glDeleteProgram(m_RendererID);
}

shaderSource Shader::parseShader(const string &filepath)
{
  // string path = filepath;
  // for (const auto  &entry : std::filesystem::directory_iterator(path))
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

void Shader::bind() const
{
  glUseProgram(m_RendererID);
}

void Shader::unbind()
{
  glUseProgram(0);
}

unsigned int Shader::compileShader(unsigned int type, const string &source)
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

unsigned int Shader::createShader(const string &vertexShader, const string &fragmentShader)
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

void Shader::setUniform1i(const std::string &name, int v0)
{
  glUniform1i(getUniformLocation(name), v0);
}
void Shader::setUniform1f(const std::string &name, float v0)
{
  glUniform1f(getUniformLocation(name), v0);
}

void Shader::setUniform4f(const string &name, float v0, float v1, float v2, float v3)
{
  glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
};

void Shader::setUniformMat4f(const std::string &name, const glm::mat4 &matrix)
{
  glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

int Shader::getUniformLocation(const string &name)
{
  if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    return m_UniformLocationCache[name];

  int location = glGetUniformLocation(m_RendererID, name.c_str());
  if (location == -1)
    std::cout << "WARNING: unfiorm " << name << " doesn't exist" << std::endl;

  m_UniformLocationCache[name] = location;
  return location;
}
