#ifndef PLAYGROUND_SCENE_H
#define PLAYGROUND_SCENE_H

#include "Renderer.h"
#include "Camera.h"
#include <memory>

class Scene {
public:
    virtual void setup() = 0;
    virtual void draw() = 0;
    virtual std::shared_ptr<Camera> getCamera(float width, float height) = 0;
    virtual void showSceneControlsWindow() {};
};

#endif //PLAYGROUND_SCENE_H
