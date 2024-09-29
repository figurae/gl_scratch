#include <memory>
#include "SceneManager.hxx"
#include "TriangleScene.hxx"

void SceneManager::loadScene(SceneName scene_name) {
    switch (scene_name) {
    case SceneName::Triangle: {
        current_scene = std::make_unique<TriangleScene>(TriangleScene());
    }
    }
}

void SceneManager::updateScene(float dt) { current_scene->update(dt); }

void SceneManager::drawScene() { current_scene->draw(); }
