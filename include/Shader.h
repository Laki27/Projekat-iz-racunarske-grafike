//
// Created by lazar on 4.2.22..
//

#ifndef PROJEKAT_SHADER_H
#define PROJEKAT_SHADER_H


#include <glm/glm.hpp>

class Shader {

public:
    unsigned m_shaderProgramId;
    Shader(const char* vertexShaderSourcePath, const char* fragmentShaderSourcePath);
    void use();
    void deleteProgram();
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
    void setUniform1I(const std::string &namevarName, int v0);
    void setFloat(const std::string &name, float value) const;
};

#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Shader.h"

std::string readFileContents(const char* path){
    std::ifstream in(path);
    std::stringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

Shader::Shader(const char* vertexShaderSourcePath, const char* fragmentShaderSourcePath){
    std::string vertexShaderSourceString= readFileContents(vertexShaderSourcePath);
    int vertexShaderId=glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderSource = vertexShaderSourceString.c_str();
    glShaderSource(vertexShaderId, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShaderId);
    int success;
    char infoLog[512]={0};
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShaderId, 512, nullptr, infoLog);
        std::cout<<"Error VShader\n" << infoLog << '\n';
    }

    int fragmentShaderId=glCreateShader(GL_FRAGMENT_SHADER);
    std::string fragmentShaderSourceString = readFileContents(fragmentShaderSourcePath);
    const char* fragmentShaderSource = fragmentShaderSourceString.c_str();
    glShaderSource(fragmentShaderId, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShaderId);

    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShaderId, 512, nullptr, infoLog);
        std::cout << "Error Fshader\n" << infoLog << '\n';
    }

    int shaderProgram=glCreateProgram();
    glAttachShader(shaderProgram, vertexShaderId);
    glAttachShader(shaderProgram, fragmentShaderId);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Err PRShader\n" << infoLog << '\n';
    }
    m_shaderProgramId=shaderProgram;
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
}
void Shader::use(){
    glUseProgram(m_shaderProgramId);
}

void Shader::setUniform1I(const std::string &varName, int v0) {
    int varId=glGetUniformLocation(m_shaderProgramId, varName.c_str());
    glUniform1i(varId, v0);
}
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgramId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(m_shaderProgramId, name.c_str()), 1, &value[0]);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(m_shaderProgramId, name.c_str()), value);
}



void Shader::deleteProgram(){
    glDeleteProgram(m_shaderProgramId);
}




#endif //PROJEKAT_SHADER_H
