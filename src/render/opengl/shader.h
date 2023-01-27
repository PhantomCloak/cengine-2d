#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

std::string get_file_contents(const char* filename);

class Shader
{
public:
	// Reference ID of the Shader Program
	GLuint ID;
	// Constructor that build the Shader Program from 2 different shaders
  Shader() = default;
	Shader(const char* vertexFile, const char* fragmentFile);

	// Activates the Shader Program
	void Activate();
	void Deactivate();
	// Deletes the Shader Program
	void Delete();
  bool compiledSuccessfully;
private:
	// Checks if the different Shaders have compiled properly
	bool compileErrors(unsigned int shader, const std::string type);
};


#endif
