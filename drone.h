#pragma once

#include "components/simple_scene.h"
#include "camera.h"


namespace m1
{
    class DroneDelivery : public gfxc::SimpleScene
    {
     public:
         DroneDelivery();
        ~DroneDelivery();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix) override;
        void Render2(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, implemented::Camera2* cam, const glm::mat4& projection);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void NewPackage();
        void NewDest();
        void CloudsInit();
        void Clouds(float deltaTimeSeconds);
        bool checkCollision(const glm::vec3& position);
        bool checkPos(const glm::vec3& pos1, const glm::vec3& size1, const glm::vec3& pos2, const glm::vec3& size2);
        bool Pickup();
        bool ReachDest();
        glm::vec3 getRandomPos(float minX, float maxX, float minZ, float maxZ);
        glm::vec3 Target() const;
        float GetAngle(const glm::vec3& currentPos, const glm::vec3& targetPos) const;
        Mesh* CreateArrow(const glm::vec3& size);
        Mesh* CreateCloud(const glm::vec3& size);
        Mesh* CreatePlatform(const glm::vec3& size);
        Mesh* CreatePackage(const glm::vec3& size);
        Mesh* CreateCircle(float radius, int segments);
        Mesh* CreateCuboid(const std::string& name, glm::vec3 corner, float lengthX, float lengthY, float lengthZ, glm::vec3 color);
        Mesh* CreateDrone();
        Mesh* CreateTerrain(int size, float step);
        Mesh* CreateTree(const glm::vec3& size);
        Mesh* CreateBuilding(const glm::vec3& size);


     protected:
        implemented::Camera2 *camera;
        implemented::Camera2 *camera2;
        glm::mat4 minimapProjection;
        glm::mat4 projectionMatrix;
       
        glm::vec3 dronePos = glm::vec3(0, 2, 0);
        glm::vec3 packagePos;
        glm::vec3 destination;

        float rotation = 0;
        float minimapSize;
        float camAngleX = 0, camAngleY = 0, camDistance = 5;
        float platformY, packageSize, destinationRadius;
        float collisionCount = 0;
        bool package, first = true;
        bool gameOver = false;
        int score = 0;
        int lives = 5;

        struct Cloud {
            glm::vec3 pos;
            bool moveRight;
        };

        std::vector<Cloud> clouds;
        std::vector<std::pair<glm::vec3, glm::vec3>> obstacles;

        
        float fov, aspect = window->props.aspectRatio, znear, zfar, bottom, top, left1, right1;
    };
}   // namespace m1
