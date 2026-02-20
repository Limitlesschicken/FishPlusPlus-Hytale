/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "glad/glad.h"

#include "../Util/Util.h"
#include "../Math/Color.h"



class Shader {
public:
	Shader(std::string vertexCode, std::string fragmentCode) {
        id = 0;

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        uint32_t vertex;
        uint32_t fragment;
        int success;
        char infoLog[512];

        // vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        // print compile errors if any
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
        };

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        // print compile errors if any
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
        };


            // shader Program
        id = glCreateProgram();
        glAttachShader(id, vertex);
        glAttachShader(id, fragment);
        glLinkProgram(id);
        // print linking errors if any
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(id, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
        }

        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
	}
	void bind() {
		glUseProgram(id);
	}

	void set(const std::string name, bool value) {
		glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
	}
	void set(const std::string name, int value) {
		glUniform1i(glGetUniformLocation(id, name.c_str()), value);
	}
	void set(const std::string name, float value) {
		glUniform1f(glGetUniformLocation(id, name.c_str()), value);
	}
    void set(const std::string name, Matrix4x4 value) {
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &value.m[0][0]);
    }
    void set(const std::string name, Vector3 value) {
        glUniform3f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z);
    }
    void set(const std::string name, Color value) {
        glUniform3f(glGetUniformLocation(id, name.c_str()), value.r, value.g, value.b);
    }
private:
	uint32_t id;
};