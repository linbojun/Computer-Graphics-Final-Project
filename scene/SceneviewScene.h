#ifndef SCENEVIEWSCENE_H
#define SCENEVIEWSCENE_H

#include "shapes/Shape.h"
#include "shapes/ConeShape.h"
#include "shapes/CubeShape.h"
#include "shapes/CylinderShape.h"
#include "shapes/SphereShape.h"
#include "lib/CS123SceneData.h"
#include "lsystem/MeshGenerator.h"
#include "camera/Camera.h"
#include "terrain/terrain.h"

#include <string>
#include <QTimer>

#include <memory>

#define dt 0.0167

struct Snow {
    glm::vec3 pos;
    glm::vec3 velocity;
    glm::vec3 gravity;
    bool flag;
    int start;
};


namespace CS123 { namespace GL {

    class Shader;
    class CS123Shader;
    class Texture2D;
}}

/**
 *
 * @class SceneviewScene
 *
 * A complex scene consisting of multiple objects. Students will implement this class in the
 * Sceneview assignment.
 *
 * Here you will implement your scene graph. The structure is up to you - feel free to create new
 * classes and data structures as you see fit. We are providing this SceneviewScene class for you
 * to use as a stencil if you like.
 *
 * Keep in mind that you'll also be rendering entire scenes in the next two assignments, Intersect
 * and Ray. The difference between this assignment and those that follow is here, we are using
 * OpenGL to do the rendering. In Intersect and Ray, you will be responsible for that.
 */
class SceneviewScene{
public:
    SceneviewScene();
    virtual ~SceneviewScene();

    virtual void render(Camera * camera, int time, int mili);


    // Use this method to set an internal selection, based on the (x, y) position of the mouse
    // pointer.  This will be used during the "modeler" lab, so don't worry about it for now.
    void setSelection(int x, int y);

protected:

private:

    void loadPhongShader();
    bool updateSnow(Snow& sw);


    void setSceneUniforms(Camera * camera);
    void setMatrixUniforms(CS123::GL::Shader *shader);
    void setLights();
    void renderGeometry();

    void genTrees();
    void paintTrees();
    void paintTree(glm::vec4 place, glm::vec4 dir, std::string lTree, float scale, float angle);

    CS123ScenePrimitive getBranch();
    CS123ScenePrimitive getLeave();
    CS123ScenePrimitive getSnow();


    std::unique_ptr<CS123::GL::CS123Shader> m_phongShader;
    std::unique_ptr<CS123::GL::Shader> m_wireframeShader;
    std::unique_ptr<CS123::GL::Shader> m_normalsShader;
    std::unique_ptr<CS123::GL::Shader> m_normalsArrowShader;

    std::unique_ptr<Terrain> m_terrain;

    std::unique_ptr<CubeShape> m_cube;
    std::unique_ptr<ConeShape> m_cone;
    std::unique_ptr<CylinderShape> m_cylinder;
    std::unique_ptr<SphereShape> m_sphere;

//     const float m_fps = 60.0f;
//     QTimer m_timer;
//     int m_increment;

     std::vector<CS123ScenePrimitive> m_primitives;
     std::vector<glm::mat4> m_trans;

     CS123SceneLightData m_testLight;
     std::vector<std::unique_ptr<MeshGenerator>> m_trees;
     std::vector<glm::vec3> m_trees_loc;

     std::vector<struct Snow> m_snow;

     int m_time;
     int total = 1800;
     float y_start = 5.0;
     float terminal_speed = -1.f;
     int tree_number = 200;

};

#endif // SCENEVIEWSCENE_H
