#include "pch.h"

#include "Shader.h"

Shader::Shader(const std::string& filepath)
    : m_filePath(filepath), m_rendererID(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_rendererID = CreateShader(source.vertexSource, source.fragmentSource);

    GLCall(glUseProgram(m_rendererID));
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_rendererID));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_rendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_uniformLocation.find(name) != m_uniformLocation.end())
        return m_uniformLocation[name];

    GLCall(int location = glGetUniformLocation(m_rendererID, name.c_str()));
    if (location == -1)
        std::cout << "No active uniform variable with name " << name << " found" << std::endl;

    m_uniformLocation[name] = location;

    return location;

}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform3f(const std::string& name, float f0, float f1, float f2)
{
    GLCall(glUniform3f(GetUniformLocation(name), f0, f1, f2));
}


void Shader::SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3)
{
    GLCall(glUniform4f(GetUniformLocation(name), f0, f1, f2, f3));
}

void Shader::SetUniform4x4(const std::string& name, glm::mat4 viewProj)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(viewProj)));
}

void Shader::SetMatrixUniform4(const std::string& name, Matrix4 viewProj)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, viewProj.GetAsFloatPtr()));
}

struct ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{

    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss[2];
    ShaderType type = NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    struct ShaderProgramSource sps = { ss[0].str(), ss[1].str() };
    return sps;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    GLCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    // Error handling
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    std::cout << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader compile status: " << result << std::endl;
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout
            << "Failed to compile "
            << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << "shader"
            << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    // create a shader program
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));

    GLCall(glLinkProgram(program));

    GLint programLinked;

    GLCall(glGetProgramiv(program, GL_LINK_STATUS, &programLinked));
    std::cout << "Program link status: " << programLinked << std::endl;
    if (programLinked != GL_TRUE)
    {
        GLsizei logLength = 0;
        GLchar message[1024];
        GLCall(glGetProgramInfoLog(program, 1024, &logLength, message));
        std::cout << "Failed to link program" << std::endl;
        std::cout << message << std::endl;
    }

    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}