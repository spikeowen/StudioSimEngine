#pragma once
#include "BasicIncludes.h"

enum ShaderType
{
    NONE = -1, VERTEX = 0, FRAGMENT = 1
};

struct ShaderProgramSource
{
    std::string vertexSource;
    std::string fragmentSource;
};

class Shader
{
private:
    unsigned int m_rendererID;
    std::string m_filePath;
    std::unordered_map<std::string, int> m_uniformLocation;

public:
    Shader(const std::string& filepath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    // Set uniforms
    void SetUniform1i(const std::string& name, int value);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform3f(const std::string& name, float f0, float f1, float f2);
    void SetUniform4f(const std::string& name, float f0, float f1, float f2, float f3);
    void SetUniform4x4(const std::string& name, glm::mat4 viewProj);
    void SetMatrixUniform4(const std::string& name, Matrix4 viewProj);
private:
    int GetUniformLocation(const std::string& name);
    struct ShaderProgramSource ParseShader(const std::string& filepath);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

};