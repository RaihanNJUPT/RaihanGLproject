#pragma once
#include <GL/glew.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using CheckStatus = void(*)(GLuint, GLenum, GLint*);
using ReadLog = void(*)(GLuint, GLsizei, GLsizei*, GLchar*);

class Shader {
public:
    Shader(const char* vertexPath, const char* fragmentPath) {
        shaderProgram = glCreateProgram();
        CompileShaderFromFile(vertexPath, GL_VERTEX_SHADER);
        CompileShaderFromFile(fragmentPath, GL_FRAGMENT_SHADER);
        LinkShaderProgram();
    };

    ~Shader() { glDeleteProgram(shaderProgram); };

    void UseProgram() { glUseProgram(shaderProgram); };
	void SetInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
	}
    void SetFloat(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
    }

private:
    GLuint shaderProgram;
    void CompileShaderFromFile(const char* Path, GLenum type) {
        std::string code;
        std::fstream shaderFile;
        shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            shaderFile.open(Path);
            std::stringstream shaderStream;
            shaderStream << shaderFile.rdbuf();
            code = shaderStream.str();
        }
        catch (std::ifstream::failure& e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n"
                << e.what() << std::endl;
        }
        const GLchar* shaderCode = code.c_str();
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &shaderCode, NULL);
        glCompileShader(shader);

        CheckAndLog(glGetShaderiv, glGetShaderInfoLog, shader, GL_COMPILE_STATUS);

        /*
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
          GLchar infoLog[1024];
          glGetShaderInfoLog(shader, 1024, NULL, infoLog);
          std::cout << "ERROR::SHADER::COMPILATION\n" << infoLog << std::endl;
          exit(-1);
        } */
        glAttachShader(shaderProgram, shader);
        glDeleteShader(shader);
    }
    void LinkShaderProgram() {
        glLinkProgram(shaderProgram);
        CheckAndLog(glGetProgramiv, glGetProgramInfoLog, shaderProgram, GL_LINK_STATUS);

        /*
        glLinkProgram(shaderProgram);
        GLint success;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
          GLchar infoLog[1024];
          glGetProgramInfoLog(shaderProgram, 1024, NULL, infoLog);
          std::cout << "ERROR::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
          exit(-1);*/
#ifdef _WIN64
        glValidateProgram(shaderProgram);
        CheckAndLog(glGetProgramiv, glGetProgramInfoLog, shaderProgram, GL_VALIDATE_STATUS);

        /*
        glValidateProgram(shaderProgram);
        glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
        if (!success) {
          glGetProgramInfoLog(shaderProgram, 1024, NULL, infoLog);
          std::cout << "ERROR::PROGRAM::VALIDATE_FAILED\n"
                    << infoLog << std::endl;
          exit(-1);
        }*/
#endif // _WIN64
    }
    void CheckAndLog(CheckStatus Check, ReadLog Log, GLuint obj, GLenum status) {
        {
            std::string type = "PROGRAM";
            if (status == GL_COMPILE_STATUS) {
                GLint shaderType;
                glGetShaderiv(obj, GL_SHADER_TYPE, &shaderType);
                type = "SHADER";

                if (shaderType == GL_VERTEX_SHADER)
                {
                    type = "VERTEX::SHADER";
                }
                else if (shaderType == GL_FRAGMENT_SHADER)
                {
                    type = "FRAGMENT::SHADER";
                }
            }
            //HOMEWORK-2
            //Assign type with VERTEX::SHADER or FRAGMENT::SHADER 

            GLint success;
            Check(obj, status, &success);
            if (!success) {
                GLchar infoLog[1024];
                Log(obj, 1024, NULL, infoLog);
                std::cout << "ERROR::" << type << "::FAILED\n" << infoLog << std::endl;
                exit(-1);
            }
        }
    };
};