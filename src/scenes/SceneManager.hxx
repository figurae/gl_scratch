#pragma once

#include <memory>
#include "scenes/Scene.hxx"

enum SceneName { Triangle };

namespace SceneManager {
    static std::unique_ptr<Scene> current_scene;

    void loadScene(SceneName scene_name);
    void updateScene(float dt);
    void drawScene();
}
