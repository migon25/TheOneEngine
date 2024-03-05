#pragma once

#include "Uniform.h"
#include "glm/glm.hpp"


#define SHADER_TEST
class Material;
struct UniformField
{
	std::string name;
	UniformType type;
	uint32_t location;
};

class Shader
{
public:
	enum State {
		ToCompile,
		Compiled,
		Error
	};

	Shader();
	Shader(const std::string& path);

	~Shader();

	static void CreateDefault(const char* path);

	void Init(const char* filename);
	void Compile(const char* filename);
	void CompileFiles(const char* vertexShaderSource, const char* fragmentShaderSource, bool hasGS, std::string* geometryShaderSourceStr, bool& retflag);

	uint32_t getID() { return ProgramID; }
	void Bind();
	void UnBind();
	void Delete();

	std::string* getFileData(const char* file);

	//Old shader functions required for 2D renderer
	unsigned int getUniformLocation(const char* uniform_name);

	template<class T> void setUniform(unsigned int uniform_id, T value);

	void SetInt(const std::string& name, int value);
	void SetUInt(const std::string& name, unsigned int value);
	void SetFloat(const std::string& name, float value);
	void SetFloat2(const std::string& name, const glm::vec2& value);
	void SetFloat3(const std::string& name, const glm::vec3& value);
	void SetFloat4(const std::string& name, const glm::vec4& value);
	void SetMat4(const std::string& name, const glm::mat4& value);



	inline std::vector<UniformField>& getUniforms() { return uniforms; }
	// Dynamic shader functions
	void addUniform(const char* name, const UniformType type);
	void deleteUniform(const char* name);
	void setUniformType(const char* name, const UniformType type);
	void setUniformName(const char* oldName, const char* newName);
	void SetMVP(const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj);
	void SetViewProj(const glm::mat4& view, const glm::mat4& proj);
	void SetModel(const glm::mat4& model);
	UniformField* getUniform(const char* name);

	inline const char* getPath() { return path.c_str(); }
	inline void setPath(const char* path) { path = path; }

	inline void AddRef(Material* ref)
	{
		matRefs.emplace_back(ref);
	}
	void DeleteRef(Material* ref)
	{
		for (size_t i = 0; i < matRefs.size(); i++)
		{
			if (matRefs[i] == ref)
			{
				matRefs.erase(matRefs.begin() + i);
				break;
			}
		}
	}
	std::vector<Material*>& getRefs() { return matRefs; }

	State getState() const { return compileState; }
protected:
	uint32_t ProgramID;
	State compileState;
	std::vector<UniformField> uniforms;

	std::vector<Material*> matRefs;

	std::string path;


	uint32_t modelMatrixID;
	uint32_t viewMatrixID;
	uint32_t projectionMatrixID;
};