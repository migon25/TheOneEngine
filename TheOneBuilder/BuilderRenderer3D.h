#ifndef __BUILDER_RENDERER_3D_H__
#define __BUILDER_RENDERER_3D_H__
#pragma once

#include "BuilderModule.h"

#include <memory>

class EngineCore;
class GameObject;

class BuilderRenderer3D : public BuilderModule
{
public:
    BuilderRenderer3D(BuilderApp* app);

    virtual ~BuilderRenderer3D();

    bool Awake();
    bool Start();

    bool PreUpdate();
    bool Update(double dt);
    bool PostUpdate();

    bool CleanUp();

    void CameraInput(double dt);

public:

    double dtSum = 0;

    std::shared_ptr<GameObject> sceneCamera;
};

#endif // !__BUILDER_RENDERER_3D_H__