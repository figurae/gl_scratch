#pragma once

class Scene {
public:
    virtual ~Scene() = 0;
    virtual void update(float dt) = 0;
    virtual void draw() = 0;
};

inline Scene::~Scene() {}
