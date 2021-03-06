#ifndef TERRAIN_H
#define TERRAIN_H

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "glm/gtc/type_ptr.hpp"   // glm::value_ptr
#include <vector>

#include "gl/shaders/CS123Shader.h"
#include "camera/Camera.h"
#include "openglshape.h"

class Terrain {
public:
    Terrain();

    void init();
    void render(Camera *camera);
    void draw();
    void drawTop();
    void drawElse();
    glm::vec3 getPosition(float row, float col);
    glm::vec3 getNormal(float row, float col);

private:
    void genSides();
    void loadPhongShader();
    float randValue(int row, int col);
    bool ifRock(glm::vec3 nor);

    std::unique_ptr<OpenGLShape> m_top;
    std::unique_ptr<OpenGLShape> m_front;
    std::unique_ptr<OpenGLShape> m_back;
    std::unique_ptr<OpenGLShape> m_left;
    std::unique_ptr<OpenGLShape> m_right;
    std::unique_ptr<OpenGLShape> m_bot;
    const float m_numRows, m_numCols;
    const float waterLevel = -0.4f;

    std::unique_ptr<CS123::GL::CS123Shader> m_phongShader;

};

#endif // TERRAIN_H
