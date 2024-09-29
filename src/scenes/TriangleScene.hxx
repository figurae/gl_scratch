#pragma once

#include <vector>
#include "Scene.hxx"
#include "graphics/ShaderProgram.hxx"
#include "graphics/VertexArray.hxx"

class TriangleScene : public Scene {
public:
    TriangleScene();
    ~TriangleScene() override;
    void update(float dt) override;
    void draw() override;

private:
    std::vector<VertexArray> m_vaos;
    ShaderProgram m_shader_program;
};
