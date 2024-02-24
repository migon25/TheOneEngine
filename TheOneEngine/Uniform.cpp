#include "Uniform.h"
#include <GL/glew.h>

#include <glm/glm.hpp>

//Sampler2D x : OpenGL id y: ResourceId to retrieve the path when saving
void Uniform::sendToShader(uint16_t shaderProgram, int& textureId)
{
	if (uniformID == NULL)
		uniformID = glGetUniformLocation(shaderProgram, name.c_str());

	switch (type)
	{
	case UniformType::Bool:
		glUniform1i(uniformID, *static_cast<bool*>(data));
		break;
	case UniformType::Int:
		glUniform1i(uniformID, *static_cast<int*>(data));
		break;
	case UniformType::Uint:
		glUniform1ui(uniformID, *static_cast<unsigned int*>(data));
		break;
	case UniformType::Float:
		glUniform1f(uniformID, *static_cast<float*>(data));
		break;
	case UniformType::fVec2:
		glUniform2fv(uniformID, 1, static_cast<float*>(data));
		break;
	case UniformType::fVec3:
		glUniform3fv(uniformID, 1, static_cast<float*>(data));
		break;
	case UniformType::fVec4:
		glUniform4fv(uniformID, 1, static_cast<float*>(data));
		break;
	case UniformType::Mat2:
		glUniformMatrix2fv(uniformID, 1, GL_FALSE, static_cast<float*>(data));
		break;
	case UniformType::Mat3:
		glUniformMatrix3fv(uniformID, 1, GL_FALSE, static_cast<float*>(data));
		break;
	case UniformType::Mat4:
		glUniformMatrix4fv(uniformID, 1, GL_FALSE, static_cast<float*>(data));
		break;
	case UniformType::Sampler2D:
	{
		glActiveTexture(textureId++);
		glBindTexture(GL_TEXTURE_2D, ((glm::ivec2*)data)->x);
	}break;
	default:
		break;
	}
}

void Uniform::setEmptyData()
{
	switch (type)
	{
	case UniformType::Bool:
		setData(false, type);
		break;
	case UniformType::Int:
		setData(0, type);
		break;
	case UniformType::Uint:
		setData(0, type);
		break;
	case UniformType::Float:
		setData(0.0f, type);
		break;
	case UniformType::fVec2:
		setData(glm::vec2(), type);
		break;
	case UniformType::fVec3:
		setData(glm::vec3(), type);
		break;
	case UniformType::fVec4:
		setData(glm::vec4(), type);
		break;
	case UniformType::Mat2:
		setData(glm::mat2(), type);
		break;
	case UniformType::Mat3:
		setData(glm::mat3(), type);
		break;
	case UniformType::Mat4:
		setData(glm::mat4(), type);
		break;
	case UniformType::Sampler2D:
		setData(glm::ivec2(), type);
		break;
	default:
		break;
	}
}