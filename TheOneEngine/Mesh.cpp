#include "Mesh.h"
#include "GameObject.h"
#include "Transform.h"
#include "nlohmann/json.hpp"

#include "Log.h"

#include <span>
#include <vector>
#include <array>
#include <cstdio>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <assimp/scene.h>

namespace fs = std::filesystem;
using namespace std;
using namespace nlohmann::literals;

Mesh::Mesh(std::shared_ptr<GameObject> containerGO) : Component(containerGO, ComponentType::Mesh)
{
    active = true;
    drawNormalsFaces = false;
    drawNormalsVerts = false;
    drawAABB = true;
    drawChecker = false;

    normalLineWidth = 1;
    normalLineLength = 0.1f;
}

Mesh::~Mesh()
{
}


// Draw
void Mesh::DrawComponent()
{
    //TODO: change this to retained mode, add a shader to the material and make it work
    glLineWidth(1);
    glColor4ub(255, 255, 255, 255); 

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vertex_buffer_id);
    glEnableClientState(GL_VERTEX_ARRAY);

    std::shared_ptr<GameObject> containerGO = GetContainerGO();
    glPushMatrix();
    glMultMatrixd(&containerGO.get()->GetComponent<Transform>()->CalculateWorldTransform()[0].x);


    if (active)
    {
        if (drawWireframe)
        {
            glDisable(GL_TEXTURE_2D);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glEnable(GL_TEXTURE_2D);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        if (mesh.indexs_buffer_id)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.indexs_buffer_id);
            glDrawElements(GL_TRIANGLES, mesh.numIndexs, GL_UNSIGNED_INT, nullptr);
        }
        else
            glDrawArrays(GL_TRIANGLES, 0, mesh.numVerts);

        if (drawNormalsVerts) DrawVertexNormals();
        if (drawNormalsFaces) DrawFaceNormals();


        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisable(GL_TEXTURE_2D);
    }
    
    GLenum error = glGetError();

    glPopMatrix();
}

void Mesh::DrawVertexNormals()
{
    if (meshData.meshVerts.empty() || meshData.meshNorms.empty()) return;

    glLineWidth(normalLineWidth);
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);

    for (int i = 0; i < meshData.meshVerts.size(); i++)
    {
        glVertex3f(meshData.meshVerts[i].x, meshData.meshVerts[i].y, meshData.meshVerts[i].z);
        glVertex3f(meshData.meshVerts[i].x + meshData.meshNorms[i].x * normalLineLength,
            meshData.meshVerts[i].y + meshData.meshNorms[i].y * normalLineLength,
            meshData.meshVerts[i].z + meshData.meshNorms[i].z * normalLineLength);
    }

    glColor3f(1.0f, 1.0f, 0.0f);
    glEnd();
}

void Mesh::DrawFaceNormals() 
{
    if (meshData.meshFaceCenters.empty() || meshData.meshFaceNorms.empty()) return;

    glLineWidth(normalLineWidth);
    glColor3f(1.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);

    for (int i = 0; i < meshData.meshFaceCenters.size(); i++)
    {
        glm::vec3 endPoint = meshData.meshFaceCenters[i] + normalLineLength * meshData.meshFaceNorms[i];
        glVertex3f(meshData.meshFaceCenters[i].x, meshData.meshFaceCenters[i].y, meshData.meshFaceCenters[i].z);
        glVertex3f(endPoint.x, endPoint.y, endPoint.z);
    }

    glColor3f(0.0f, 1.0f, 1.0f);
    glEnd();
}



void Mesh::GenBufferData()
{
    meshVAO = std::make_shared<VertexArray>();

    if (glGetError() != 0)
    {
        LOG(LogType::LOG_ERROR, "Check error %s", glewGetErrorString(glGetError()));
    }
    meshVBO = std::make_shared<VertexBuffer>(meshData.vertexData.data(), meshData.vertexData.size() * sizeof(MeshVertex));

    if (glGetError() != 0)
    {
        LOG(LogType::LOG_ERROR, "Check error {0}", glewGetErrorString(glGetError()));
    }

    meshVBO->SetLayout({
        { ShaderDataType::Float3, "aPos"           },
        { ShaderDataType::Float3, "aColor"         },
        { ShaderDataType::Float2, "aTex"           }
        });
    meshVAO->AddVertexBuffer(meshVBO);

    std::shared_ptr<IndexBuffer> meshIB = std::make_shared<IndexBuffer>(meshData.indexData.data(), meshData.indexData.size());


    if (glGetError() != 0)
    {
        LOG(LogType::LOG_ERROR, "Check error %s", glewGetErrorString(glGetError()));
    }

    meshVAO->SetIndexBuffer(meshIB);


    if (glGetError() != 0)
    {
        LOG(LogType::LOG_ERROR, "Check error %s", glewGetErrorString(glGetError()));
    }


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    if (glGetError() != 0)
    {
        LOG(LogType::LOG_ERROR, "Check error %s", glewGetErrorString(glGetError()));
    }
}



// Load/Save
json Mesh::SaveComponent()
{
    json meshJSON;

    meshJSON["Name"] = name;
    meshJSON["Type"] = type;
    if (auto pGO = containerGO.lock())
    {
        meshJSON["ParentUID"] = pGO.get()->GetUID();
    }
    meshJSON["UID"] = UID;
    meshJSON["Active"] = active;
    meshJSON["DrawWireframe"] = drawWireframe;
    meshJSON["DrawAABB"] = drawAABB;
    meshJSON["DrawOBB"] = drawOBB;
    meshJSON["DrawChecker"] = drawChecker;
    meshJSON["DrawNormalsVerts"] = drawNormalsVerts;
    meshJSON["DrawNormalsFaces"] = drawNormalsFaces;
    meshJSON["Path"] = path;

    //MeshData && MeshBufferedData are already serialized in .mesh files

    return meshJSON;
}

void Mesh::LoadComponent(const json& meshJSON)
{
    // Load basic properties
    if (meshJSON.contains("UID"))
    {
        UID = meshJSON["UID"];
    }

    if (meshJSON.contains("Name"))
    {
        name = meshJSON["Name"];
    }

    // Load mesh-specific properties
    if (meshJSON.contains("Active"))
    {
        active = meshJSON["Active"];
    }

    if (meshJSON.contains("DrawWireframe"))
    {
        drawWireframe = meshJSON["DrawWireframe"];
    }

    if (meshJSON.contains("DrawAABB"))
    {
        drawAABB = meshJSON["DrawAABB"];
    }

    if (meshJSON.contains("DrawOBB"))
    {
        drawOBB = meshJSON["DrawOBB"];
    }

    if (meshJSON.contains("DrawChecker"))
    {
        drawChecker = meshJSON["DrawChecker"];
    }

    if (meshJSON.contains("DrawNormalsVerts"))
    {
        drawNormalsVerts = meshJSON["DrawNormalsVerts"];
    }

    if (meshJSON.contains("DrawNormalsFaces"))
    {
        drawNormalsFaces = meshJSON["DrawNormalsFaces"];
    }

    if (meshJSON.contains("Path"))
    {
        path = meshJSON["Path"];
    }

    // Implement additional logic to handle other mesh-specific properties as needed
    // ...

    //Reinitialize or update the mesh based on the loaded data

    if (!path.empty())
    {
        meshData = meshLoader->deserializeMeshData(path);
        meshLoader->BufferData(meshData);
        mesh = meshLoader->GetBufferData();
        meshData.meshVAO->GetVertexBuffers().size
    }

}


void Mesh::serializeMeshData(const std::string& filename)
{
    std::ofstream outFile(filename, ios::binary);

    // Write meshName size and data
    size_t meshNameSize = meshData.meshName.size();
    outFile.write(reinterpret_cast<const char*>(&meshNameSize), sizeof(size_t));
    outFile.write(meshData.meshName.c_str(), meshNameSize);

    // Write format
    outFile.write(reinterpret_cast<const char*>(&meshData.format), /*TODO*/);

    // Write vertex_data size and data
    uint numVerts = static_cast<uint>(meshData.vertexData.size());
    outFile.write(reinterpret_cast<const char*>(&numVerts), sizeof(uint));
    outFile.write(reinterpret_cast<const char*>(&meshData.vertexData[0]), numVerts * sizeof(MeshVertex));

    // Write index_data size and data
    uint numIndexs = static_cast<uint>(meshData.indexData.size());
    outFile.write(reinterpret_cast<const char*>(&numIndexs), sizeof(uint));
    outFile.write(reinterpret_cast<const char*>(&meshData.indexData[0]), numIndexs * sizeof(uint));

    // Write meshVerts size and data
    uint numMeshVerts = static_cast<uint>(meshData.meshVerts.size());
    outFile.write(reinterpret_cast<const char*>(&numMeshVerts), sizeof(uint));
    outFile.write(reinterpret_cast<const char*>(&meshData.meshVerts[0]), numMeshVerts * sizeof(vec3f));

    // Write meshNorms size and data
    uint numMeshNorms = static_cast<uint>(meshData.meshNorms.size());
    outFile.write(reinterpret_cast<const char*>(&numMeshNorms), sizeof(uint));
    outFile.write(reinterpret_cast<const char*>(&meshData.meshNorms[0]), numMeshNorms * sizeof(vec3f));

    // Write meshFaceCenters size and data
    uint numMeshFaceCenters = static_cast<uint>(meshData.meshFaceCenters.size());
    outFile.write(reinterpret_cast<const char*>(&numMeshFaceCenters), sizeof(uint));
    outFile.write(reinterpret_cast<const char*>(&meshData.meshFaceCenters[0]), numMeshFaceCenters * sizeof(vec3f));

    // Write meshFaceNorms size and data
    uint numMeshFaceNorms = static_cast<uint>(meshData.meshFaceNorms.size());
    outFile.write(reinterpret_cast<const char*>(&numMeshFaceNorms), sizeof(uint));
    outFile.write(reinterpret_cast<const char*>(&meshData.meshFaceNorms[0]), numMeshFaceNorms * sizeof(vec3f));

    LOG(LogType::LOG_OK, "-%s created", filename.data());

    outFile.close();
}

void Mesh::deserializeMeshData(const std::string& filename)
{

    std::ifstream inFile(filename, std::ios::binary);

    // Read meshName
    size_t meshNameSize = meshData.meshName.size();
    inFile.read(reinterpret_cast<char*>(&meshNameSize), sizeof(size_t));

    meshData.meshName.resize(meshNameSize);
    inFile.read(&meshData.meshName[0], meshNameSize);

    // Read format
    inFile.read(reinterpret_cast<char*>(&meshData.format), /*TODO*/);

    // Read vertex_data size and allocate memory
    uint numVerts;
    inFile.read(reinterpret_cast<char*>(&numVerts), sizeof(uint));
    meshData.vertexData.resize(numVerts);

    // Read vertex_data
    inFile.read(reinterpret_cast<char*>(&meshData.vertexData[0]), numVerts * sizeof(MeshVertex));

    // Read index_data size and allocate memory
    uint numIndexs;
    inFile.read(reinterpret_cast<char*>(&numIndexs), sizeof(uint));
    meshData.indexData.resize(numIndexs);

    // Read index_data
    inFile.read(reinterpret_cast<char*>(&meshData.indexData[0]), numIndexs * sizeof(uint));

    // Read meshVerts size and allocate memory
    uint numMeshVerts;
    inFile.read(reinterpret_cast<char*>(&numMeshVerts), sizeof(uint));
    meshData.meshVerts.resize(numMeshVerts);

    // Read meshVerts
    inFile.read(reinterpret_cast<char*>(&meshData.meshVerts[0]), numMeshVerts * sizeof(vec3f));

    // Read meshNorms size and allocate memory
    uint numMeshNorms;
    inFile.read(reinterpret_cast<char*>(&numMeshNorms), sizeof(uint));
    meshData.meshNorms.resize(numMeshNorms);

    // Read meshNorms
    inFile.read(reinterpret_cast<char*>(&meshData.meshNorms[0]), numMeshNorms * sizeof(vec3f));

    // Read meshFaceCenters size and allocate memory
    uint numMeshFaceCenters;
    inFile.read(reinterpret_cast<char*>(&numMeshFaceCenters), sizeof(uint));
    meshData.meshFaceCenters.resize(numMeshFaceCenters);

    // Read meshFaceCenters
    inFile.read(reinterpret_cast<char*>(&meshData.meshFaceCenters[0]), numMeshFaceCenters * sizeof(vec3f));

    // Read meshFaceNorms size and allocate memory
    uint numMeshFaceNorms;
    inFile.read(reinterpret_cast<char*>(&numMeshFaceNorms), sizeof(uint));
    meshData.meshFaceNorms.resize(numMeshFaceNorms);

    // Read meshFaceNorms
    inFile.read(reinterpret_cast<char*>(&meshData.meshFaceNorms[0]), numMeshFaceNorms * sizeof(vec3f));

    LOG(LogType::LOG_INFO, "-%s loaded", filename.data());
    inFile.close();
}