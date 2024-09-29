#pragma once

#include <memory>
#include "scenes/Scene.hxx"

enum SceneName { Triangle };

class SceneManager {
public:
    void static loadScene(SceneName scene_name);
    void static updateScene(float dt);
    void static drawScene();

private:
    inline static std::unique_ptr<Scene> current_scene;
};
