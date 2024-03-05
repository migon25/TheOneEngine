#ifndef __MESH_H__
#define __MESH_H__
#pragma once

#include "Defs.h"
#include "MeshLoader.h"
#include "Component.h"
#include "Texture.h"
#include "Graphic.h"
#include "Material.h"

#include <vector>
#include <memory>
#include <string>

class GameObject;

struct MeshVertex
{
    vec3f position;
    vec2f texCoords;
};

struct MeshData
{
    std::string meshName;

    BufferLayout format;

    std::vector<MeshVertex> vertexData;
    std::vector<unsigned int> indexData;

    uint numFaces;

    std::vector<vec3f> meshVerts;
    std::vector<vec3f> meshNorms;
    std::vector<vec3f> meshFaceCenters;
    std::vector<vec3f> meshFaceNorms;

    std::string texturePath;
    uint materialIndex;
};

class Mesh : public Component
{
public:

    Mesh(std::shared_ptr<GameObject> containerGO);
    virtual ~Mesh();

    void DrawComponent();

    json SaveComponent();
    void LoadComponent(const json& meshJSON);

    void LoadMesh(const std::string& path);
    void LoadTexture(const std::string& path);

private:

    void DrawVertexNormals();
    void DrawFaceNormals();
    //void DrawWireframe();
    void GenBufferData();

    void serializeMeshData(const std::string& filename);
    void deserializeMeshData(const std::string& filename);

public:
    MeshData meshData;

    bool active;
    bool drawWireframe;
    bool drawNormalsVerts;
    bool drawNormalsFaces;
    bool drawAABB;
    bool drawOBB;
    bool drawChecker;
   
    int normalLineWidth;
    float normalLineLength;

    std::string path;

private:

    MeshLoader* meshLoader;

    std::shared_ptr<Material> material;
    std::shared_ptr<Texture> texture;


    std::shared_ptr<VertexArray> meshVAO;
    std::shared_ptr<VertexBuffer> meshVBO;
    std::shared_ptr<IndexBuffer> meshIBO;
};

#endif // !__MESH_H__
