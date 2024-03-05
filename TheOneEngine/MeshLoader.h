#ifndef __MESH_LOADER_H__
#define __MESH_LOADER_H__
#pragma once

#include "Defs.h"
#include "Graphic.h"
#include "VertexArray.h"

#include <assimp/scene.h>
#include <vector>
#include <array>
#include <memory>
#include <string>

class GameObject;
class Texture;

class MeshLoader
{
public:

	MeshLoader();
	//MeshLoader(MeshLoader&& b) noexcept;
	virtual ~MeshLoader();

	std::vector<MeshBufferedData> LoadMesh(const std::string& path);
	std::vector<std::shared_ptr<Texture>> LoadTexture(const std::string& path, std::shared_ptr<GameObject> containerGO = nullptr);


	void serializeMeshData(const MeshData& data, const std::string& filename);
	MeshData deserializeMeshData(const std::string& filename);

	const MeshBufferedData GetBufferData() const { return meshBuffData; }
	const MeshData GetMeshData() const { return meshData; }

private:

	MeshData meshData;
	MeshBufferedData meshBuffData;
};

#endif // !__MESH_LOADER_H__