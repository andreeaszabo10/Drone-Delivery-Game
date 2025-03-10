#include "lab_m1/tema2/tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include <cmath>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
    fov = RADIANS(60);
    znear = 0.01f;
    zfar = 200.0f;

    left1 = -1.0f;
    right1 = 1.0f;
    bottom = -1.0f;
    top = 1.0f;

    minimapSize = 0.4f;
}


Tema2::~Tema2()
{
}

Mesh* Tema2::CreateCloud(const glm::vec3& size) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    glm::vec3 cloudColor(1.0f, 1.0f, 1.0f);

    const int stacks = 20;
    const int sectors = 20;
    float radius = size.x / 2;

    std::vector<glm::vec3> sphereCenters = {
        glm::vec3(0, 0, 0),
        glm::vec3(radius, 0, 0),
        glm::vec3(-radius, 0, 0),
        glm::vec3(0, radius / 2, radius / 2)
    };

    int vertexCount = 0;

    for (const auto& center : sphereCenters) {
        for (int i = 0; i <= stacks; ++i) {
            float angle = M_PI * i / stacks;
        
            for (int j = 0; j <= sectors; ++j) {
                float angle1 = 2 * M_PI * j / sectors;
                float x = sin(angle) * cos(angle1);
                float y = cos(angle);
                float z = sin(angle) * sin(angle1);

                vertices.push_back(VertexFormat(center + glm::vec3(x * radius, y * radius, z * radius), cloudColor));
            }
        }

        for (int i = 0; i < stacks; ++i) {
            for (int j = 0; j < sectors; ++j) {
                int a = i * (sectors + 1) + j;
                int b = a + sectors + 1;

                if (i != 0) {
                    indices.push_back(a + vertexCount);
                    indices.push_back(b + vertexCount);
                    indices.push_back(a + 1 + vertexCount);
                }

                if (i != stacks - 1) {
                    indices.push_back(a + 1 + vertexCount);
                    indices.push_back(b + vertexCount);
                    indices.push_back(b + 1 + vertexCount);
                }
            }
        }
        vertexCount += (stacks + 1) * (sectors + 1);
    }

    Mesh* cloud = new Mesh("cloud");
    cloud->InitFromData(vertices, indices);
    return cloud;
}

void Tema2::CloudsInit() {
    clouds.clear();

    for (int i = 0; i < 20; i++) {
        Cloud cloud;

        cloud.pos = glm::vec3(
            -25.0f + static_cast<float>(rand()) / RAND_MAX * 50.0f,
            10.0f + static_cast<float>(rand()) / RAND_MAX * 5.0f,
            -25.0f + static_cast<float>(rand()) / RAND_MAX * 50.0f
        );

        cloud.moveRight = (rand() % 2) == 0;
        clouds.push_back(cloud);
    }
}

void Tema2::Clouds(float deltaTimeSeconds) {
    for (auto& cloud : clouds) {
        
        if (cloud.moveRight) {
            cloud.pos.x += deltaTimeSeconds;
            if (cloud.pos.x > 25) {
                cloud.moveRight = false;
            }
        }
        else {
            cloud.pos.x -= deltaTimeSeconds;
            if (cloud.pos.x < -25) {
                cloud.moveRight = true;
            }
        }

       
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, cloud.pos);
        RenderMesh(meshes["cloud"], shaders["VertexColor"], modelMatrix);
    }
}

Mesh* Tema2::CreateTerrain(int size, float space) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i <= size; ++i) {
        for (int j = 0; j <= size; ++j) {
            glm::vec3 pos = glm::vec3(i * space, 0.0f, j * space);
            glm::vec3 color = glm::vec3(0.0f, 1.0f, 0.0f);
            vertices.push_back(VertexFormat(pos, color));
        }
    }

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            int idx = i * (size + 1) + j;

            indices.push_back(idx);
            indices.push_back(idx + size + 1);
            indices.push_back(idx + 1);

            indices.push_back(idx + 1);
            indices.push_back(idx + size + 1);
            indices.push_back(idx + size + 2);
        }
    }

    Mesh* terrain = new Mesh("terrain");
    terrain->InitFromData(vertices, indices);
    meshes[terrain->GetMeshID()] = terrain;
    return terrain;
}

bool Tema2::checkPos(const glm::vec3& pos1, const glm::vec3& size1, const glm::vec3& pos2, const glm::vec3& size2) {
    return (pos1.x - size1.x <= pos2.x + size2.x &&
        pos1.x + size1.x >= pos2.x - size2.x &&
        pos1.z - size1.z <= pos2.z + size2.z &&
        pos1.z + size1.z >= pos2.z - size2.z);
}

glm::vec3 Tema2::getRandomPos(float minX, float maxX, float minZ, float maxZ) {
    float x = minX + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxX - minX)));
    float z = minZ + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxZ - minZ)));
    return glm::vec3(x, 0.0f, z);
}

Mesh* Tema2::CreateArrow(const glm::vec3& size) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    glm::vec3 color(1.0f, 0.0f, 0.0f);

    vertices.push_back(VertexFormat(glm::vec3(2, 0, -0.1f), color));
    vertices.push_back(VertexFormat(glm::vec3(3, 0, -0.1f), color));
    vertices.push_back(VertexFormat(glm::vec3(3, 0, 0.1f), color));
    vertices.push_back(VertexFormat(glm::vec3(2, 0, 0.1f), color));

    vertices.push_back(VertexFormat(glm::vec3(3, 0, -0.4f), color));
    vertices.push_back(VertexFormat(glm::vec3(4, 0, 0), color));
    vertices.push_back(VertexFormat(glm::vec3(3, 0, 0.4f), color));


    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);

    indices.push_back(4);
    indices.push_back(5);
    indices.push_back(6);

    Mesh* arrow = new Mesh("arrow");
    arrow->InitFromData(vertices, indices);
    return arrow;
}

glm::vec3 Tema2::Target() const {
    if (!package) {
        return packagePos;
    }
    return destination;
}

float Tema2::GetAngle(const glm::vec3& currentPos, const glm::vec3& targetPos) const {
    glm::vec2 direction = glm::vec2(
        targetPos.x - currentPos.x,
        targetPos.z - currentPos.z
    );
    return atan2(direction.y, direction.x);
}

Mesh* Tema2::CreateCuboid(const std::string& name, glm::vec3 corner, float X, float Y, float Z, glm::vec3 color) {
    std::vector<VertexFormat> vertices = {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(X, 0, 0), color),
        VertexFormat(corner + glm::vec3(X, Y, 0), color),
        VertexFormat(corner + glm::vec3(0, Y, 0), color),
        VertexFormat(corner + glm::vec3(0, 0, -Z), color),
        VertexFormat(corner + glm::vec3(X, 0, -Z), color),
        VertexFormat(corner + glm::vec3(X, Y, -Z), color),
        VertexFormat(corner + glm::vec3(0, Y, -Z), color)
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,  0, 2, 3,
        4, 5, 6,  4, 6, 7,
        0, 3, 7,  0, 7, 4,
        1, 2, 6,  1, 6, 5,
        3, 2, 6,  3, 6, 7,
        0, 1, 5,  0, 5, 4
    };

    Mesh* cuboid = new Mesh(name);
    cuboid->InitFromData(vertices, indices);
    return cuboid;
}

Mesh* Tema2::CreateTree(const glm::vec3& size) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    glm::vec3 trunkColor(0.45f, 0.25f, 0.0f);
    glm::vec3 leavesColor(0.0f, 0.5f, 0.0f);

    const int segments = 20;
    float trunkRadius = size.x / 8;
    float trunkHeight = size.y;
    float coneRadius = size.x;
    float coneHeight = size.y;

    vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), trunkColor));

    for (int i = 0; i < segments; i++) {
        float angle = (2.0f * M_PI * i) / segments;
        float x = trunkRadius * cos(angle);
        float z = trunkRadius * sin(angle);
        vertices.push_back(VertexFormat(glm::vec3(x, 0, z), trunkColor));
    }

    vertices.push_back(VertexFormat(glm::vec3(0, trunkHeight, 0), trunkColor));

    for (int i = 0; i < segments; i++) {
        float angle = (2.0f * M_PI * i) / segments;
        float x = trunkRadius * cos(angle);
        float z = trunkRadius * sin(angle);
        vertices.push_back(VertexFormat(glm::vec3(x, trunkHeight, z), trunkColor));
    }

    for (int i = 0; i < segments; i++) {
        indices.push_back(0);
        indices.push_back(1 + i);
        indices.push_back(1 + ((i + 1) % segments));
    }

    int topCenterIdx = 1 + segments;
    for (int i = 0; i < segments; i++) {
        indices.push_back(topCenterIdx);
        indices.push_back(topCenterIdx + 1 + ((i + 1) % segments));
        indices.push_back(topCenterIdx + 1 + i);
    }

    for (int i = 0; i < segments; i++) {
        int bottomFirst = 1 + i;
        int bottomSecond = 1 + ((i + 1) % segments);
        int topFirst = topCenterIdx + 1 + i;
        int topSecond = topCenterIdx + 1 + ((i + 1) % segments);

        indices.push_back(bottomFirst);
        indices.push_back(topFirst);
        indices.push_back(bottomSecond);

        indices.push_back(bottomSecond);
        indices.push_back(topFirst);
        indices.push_back(topSecond);
    }

    int coneBaseCenter = vertices.size();
    vertices.push_back(VertexFormat(glm::vec3(0, trunkHeight, 0), leavesColor));

    int coneBaseStart = vertices.size();
    for (int i = 0; i < segments; i++) {
        float angle = (2.0f * M_PI * i) / segments;
        float x = coneRadius * cos(angle);
        float z = coneRadius * sin(angle);
        vertices.push_back(VertexFormat(glm::vec3(x, trunkHeight, z), leavesColor));
    }

    int tip = vertices.size();
    vertices.push_back(VertexFormat(glm::vec3(0, trunkHeight + coneHeight, 0), leavesColor));

    for (int i = 0; i < segments; i++) {
        indices.push_back(coneBaseCenter);
        indices.push_back(coneBaseStart + ((i + 1) % segments));
        indices.push_back(coneBaseStart + i);
    }

    for (int i = 0; i < segments; i++) {
        indices.push_back(coneBaseStart + i);
        indices.push_back(coneBaseStart + ((i + 1) % segments));
        indices.push_back(tip);
    }

    Mesh* tree = new Mesh("obstacle1");
    tree->InitFromData(vertices, indices);
    return tree;
}

Mesh* Tema2::CreateBuilding(const glm::vec3& size) {
    return CreateCuboid("obstacle2", glm::vec3(-size.x / 2, 0, -size.z / 2), size.x, size.y, size.z, glm::vec3(0.4f, 0.4f, 0.4f));
}

Mesh* Tema2::CreateDrone() {
    glm::vec3 corner = glm::vec3(-0.75f, 0, 0.1f);
    glm::vec3 greyColor = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 blackColor = glm::vec3(0.0f, 0.0f, 0.0f);

    float bodyLength = 1.5f, bodyWidth = 0.2f, bodyHeight = 0.2f;
    float propellerLength = 0.4f, propellerWidth = 0.05f, propellerHeight = 0.05f;
    float cubeSize = 0.2f;

    Mesh* body1 = CreateCuboid("body1", corner, bodyLength, bodyHeight, bodyWidth, greyColor);
    Mesh* body2 = CreateCuboid("body2", glm::vec3(-0.1f, 0, 0.75f), bodyWidth, bodyHeight, bodyLength, greyColor);
    Mesh* propeller = CreateCuboid("propeller", glm::vec3(-propellerLength / 2, 0, propellerWidth / 2),
        propellerLength, propellerHeight, propellerWidth, blackColor);
    Mesh* cube = CreateCuboid("cube", glm::vec3(-cubeSize / 2, 0, cubeSize / 2),
        cubeSize, cubeSize, cubeSize, greyColor);

    meshes[body1->GetMeshID()] = body1;
    meshes[body2->GetMeshID()] = body2;
    meshes[propeller->GetMeshID()] = propeller;
    meshes[cube->GetMeshID()] = cube;

    return nullptr;
}

bool Tema2::checkCollision(const glm::vec3& pos) {
    if (gameOver) return true;

    float droneRadius = 0.75f;
    float droneHeight = 0.45f;

    for (size_t i = 0; i < obstacles.size(); i++) {
        glm::vec3 obstaclePos = obstacles[i].first;
        glm::vec3 obstacleSize = obstacles[i].second;

        bool collision = false;

        if (i < 15) {
            float trunkRadius = obstacleSize.x / 8;
            float trunkHeight = obstacleSize.y;

            float dx = pos.x - obstaclePos.x;
            float dz = pos.z - obstaclePos.z;
            float distanceXZ = sqrt(dx * dx + dz * dz);

            if (distanceXZ < (trunkRadius + droneRadius) &&
                pos.y < (obstaclePos.y + trunkHeight) &&
                pos.y > obstaclePos.y) {
                collision = true;
            }

            float topBaseRadius = obstacleSize.x;
            float topHeight = obstacleSize.y;
            float coneTop = obstaclePos.y + trunkHeight + topHeight;

            if (pos.y + droneHeight >= trunkHeight &&
                pos.y <= coneTop) {
                float currHeight = (coneTop - pos.y) / topHeight;
                float rad = topBaseRadius * currHeight;

                if (distanceXZ < (rad + droneRadius)) {
                    collision = true;
                }
            }
        }
        else {
            float minX = obstaclePos.x - obstacleSize.x / 2;
            float maxX = obstaclePos.x + obstacleSize.x / 2;
            float minY = obstaclePos.y;
            float maxY = obstaclePos.y + obstacleSize.y;
            float minZ = obstaclePos.z - 3 * obstacleSize.z / 2;
            float maxZ = obstaclePos.z - obstacleSize.z / 2;

            if (pos.x + droneRadius > minX && pos.x - droneRadius < maxX &&
                pos.y + droneHeight > minY && pos.y < maxY &&
                pos.z + droneRadius > minZ && pos.z - droneRadius < maxZ) {
                collision = true;
            }
        }

        if (collision) {
            static glm::vec3 lastCollisionPos = glm::vec3(0);
            if (glm::distance(pos, lastCollisionPos) > 1.0f) {
                lives--;
                printf("Collision! Lives remaining: %d\n", lives);
                lastCollisionPos = pos;

                if (lives <= 0) {
                    gameOver = true;
                    printf("GAME OVER!\n");
                }
            }
            return true;
        }
    }

    return false;
}

Mesh* Tema2::CreatePlatform(const glm::vec3& size) {
    glm::vec3 corner = glm::vec3(-size.x / 2, 0, -size.z / 2);
    return CreateCuboid("platform", corner, size.x, size.y, size.z, glm::vec3(0.6f, 0.6f, 0.6f));
}

Mesh* Tema2::CreatePackage(const glm::vec3& size) {
    glm::vec3 corner = glm::vec3(-size.x / 2, 0, -size.z / 2);
    return CreateCuboid("package", corner, size.x, size.y, size.z, glm::vec3(0.8f, 0.6f, 0.4f));
}

Mesh* Tema2::CreateCircle(float radius, int segments) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;
    glm::vec3 color(1.0f, 0.0f, 0.0f);

    vertices.push_back(VertexFormat(glm::vec3(0, 0.1f, 0), color));

    for (int i = 0; i < segments; i++) {
        float angle = (2.0f * M_PI * i) / segments;
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        vertices.push_back(VertexFormat(glm::vec3(x, 0.1f, z), color));
    }

    for (int i = 0; i < segments; i++) {
        indices.push_back(0);
        indices.push_back(1 + i);
        indices.push_back(1 + ((i + 1) % segments));
    }

    Mesh* circle = new Mesh("destination");
    circle->InitFromData(vertices, indices);
    return circle;
}

void Tema2::NewPackage() {
    bool ok = false;
    while (!ok) {
        packagePos = getRandomPos(-20.0f, 20.0f, -20.0f, 20.0f);
        packagePos.y = 1.5f;

        ok = true;
        for (const auto& obstacle : obstacles) {
            if (checkPos(packagePos, glm::vec3(packageSize + 1),
                obstacle.first, obstacle.second)) {
                ok = false;
                break;
            }
        }
    }
}

void Tema2::NewDest() {
    if (!package) return;

    bool ok = false;
    while (!ok) {
        destination = getRandomPos(-20.0f, 20.0f, -20.0f, 20.0f);
        destination.y = platformY;

        ok = true;
        for (const auto& obstacle : obstacles) {
            if (checkPos(destination, glm::vec3(destinationRadius * 2 + 0.5f),
                obstacle.first, obstacle.second)) {
                ok = false;
                break;
            }
        }
    }
}

bool Tema2::Pickup() {
    if (package) return false;

    float dx = dronePos.x - packagePos.x;
    float dy = dronePos.y - packagePos.y;
    float dz = dronePos.z - packagePos.z;
    float distance = sqrt(dx * dx + dy * dy + dz * dz);

    return distance < (packageSize + 1.0f);
}

bool Tema2::ReachDest() {
    if (!package) return false;

    float dx = dronePos.x - destination.x;
    float dz = dronePos.z - destination.z;
    float distance = sqrt(dx * dx + dz * dz);

    return distance < destinationRadius;
}


void Tema2::Init() {
    camera = new implemented::Camera2();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    camera2 = new implemented::Camera2();
    camera2->Set(glm::vec3(0, 50.0f, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));

    float orthoSize = 20.0f;
    minimapProjection = glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, 0.01f, 200.0f);

    {
        Shader* shader = new Shader("TerrainShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    CreateDrone();
    CreateTerrain(50, 1);

    Mesh* cloud = CreateCloud(glm::vec3(2.0f));
    meshes[cloud->GetMeshID()] = cloud;
    CloudsInit();

    obstacles.clear();

    for (int i = 0; i < 15; i++) {
        bool ok = false;
        glm::vec3 pos;
        int max = 50;
        int count = 0;

        while (!ok && count < max) {
            pos = getRandomPos(-20, 20, -20, 20);
            ok = true;

            for (const auto& obstacle : obstacles) {
                if (checkPos(pos, glm::vec3(2.0f, 3.0f, 2.0f), obstacle.first, obstacle.second)) {
                    ok = false;
                    break;
                }
            }
            count++;
        }

        if (ok) {
            obstacles.push_back({pos, glm::vec3(2.0f, 3.0f, 2.0f) });
        }
    }

    for (int i = 0; i < 10; i++) {
        bool ok = false;
        glm::vec3 pos;
        int max = 50;
        int count = 0;

        while (!ok && count < max) {
            pos = getRandomPos(-20, 20, -20, 20);
            ok = true;

            for (const auto& obstacle : obstacles) {
                if (checkPos(pos, glm::vec3(3.0f, 2.0f, 3.0f), obstacle.first, obstacle.second)) {
                    ok = false;
                    break;
                }
            }
            count++;
        }

        if (ok) {
            obstacles.push_back({pos, glm::vec3(3.0f, 2.0f, 3.0f)});
        }
    }

    platformY = 1.0f;
    packageSize = 0.5f;
    destinationRadius = 1.0f;
    package = false;

    Mesh* platform = CreatePlatform(glm::vec3(2.0f, platformY, 2.0f));
    Mesh* package = CreatePackage(glm::vec3(packageSize));
    Mesh* destination = CreateCircle(destinationRadius, 32);
    
    meshes[platform->GetMeshID()] = platform;
    meshes[package->GetMeshID()] = package;
    meshes[destination->GetMeshID()] = destination;
   
    NewPackage();

    Mesh* arrow = CreateArrow(glm::vec3(1.0f));
    meshes[arrow->GetMeshID()] = arrow;
  
    Mesh* obstacle1 = CreateTree(glm::vec3(2.0f, 3.0f, 2.0f));
    Mesh* obstacle2 = CreateBuilding(glm::vec3(3.0f, 2.0f, 3.0f));
    meshes[obstacle1->GetMeshID()] = obstacle1;
    meshes[obstacle2->GetMeshID()] = obstacle2;

    projectionMatrix = glm::perspective(fov, aspect, znear, zfar);
   
}

void Tema2::FrameStart()
{
    glClearColor(0, 0.5f, 0.8f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, dronePos);
        modelMatrix = glm::rotate(modelMatrix, rotation, glm::vec3(0, 1, 0));
        RenderMesh(meshes["body1"], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, dronePos);
        modelMatrix = glm::rotate(modelMatrix, rotation, glm::vec3(0, 1, 0));
        RenderMesh(meshes["body2"], shaders["VertexColor"], modelMatrix);

        glm::vec3 propellerOffsets[4] = {
            glm::vec3(0.65f, 0, 0),
            glm::vec3(-0.65f, 0, 0),
            glm::vec3(0, 0, 0.65f),
            glm::vec3(0, 0, -0.65f)
        };

        static float propellerRotation = 0;
        propellerRotation += deltaTimeSeconds * 5.0f;

        for (int i = 0; i < 4; i++) {
            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, dronePos);
            modelMatrix = glm::rotate(modelMatrix, rotation, glm::vec3(0, 1, 0));
            modelMatrix = glm::translate(modelMatrix, propellerOffsets[i]);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.2f, 0));
            RenderMesh(meshes["cube"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, dronePos);
            modelMatrix = glm::rotate(modelMatrix, rotation, glm::vec3(0, 1, 0));
            modelMatrix = glm::translate(modelMatrix, propellerOffsets[i]);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.4f, 0));
            modelMatrix = glm::rotate(modelMatrix, propellerRotation, glm::vec3(0, 1, 0));
            RenderMesh(meshes["propeller"], shaders["VertexColor"], modelMatrix);
        }
    }

    float X = camDistance * glm::sin(camAngleY) * glm::cos(camAngleX);
    float Y = camDistance * glm::sin(camAngleX);
    float Z = camDistance * glm::cos(camAngleY) * glm::cos(camAngleX);

    glm::vec3 camPos = dronePos + glm::vec3(X, Y + 2.0f, Z);
    camera->Set(camPos, dronePos, glm::vec3(0, 1, 0));

    if (meshes.find("terrain") != meshes.end()) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-25, 0, -25));
        Shader* shader = shaders["TerrainShader"];
        shader->Use();

        GLint loc_model = glGetUniformLocation(shader->program, "model");
        glUniformMatrix4fv(loc_model, 1, GL_FALSE, glm::value_ptr(modelMatrix));

        GLint loc_view = glGetUniformLocation(shader->program, "view");
        glUniformMatrix4fv(loc_view, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

        GLint loc_projection = glGetUniformLocation(shader->program, "projection");
        glUniformMatrix4fv(loc_projection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        GLint loc_player_pos = glGetUniformLocation(shader->program, "pos");
        glUniform3fv(loc_player_pos, 1, glm::value_ptr(dronePos));

        RenderMesh(meshes["terrain"], shaders["TerrainShader"], modelMatrix);
    }

    Clouds(deltaTimeSeconds);

    for (size_t i = 0; i < obstacles.size(); i++) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, obstacles[i].first);

        if (i < 15) {
            RenderMesh(meshes["obstacle1"], shaders["VertexColor"], modelMatrix);
        }
        else {
            RenderMesh(meshes["obstacle2"], shaders["VertexColor"], modelMatrix);
        }
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, packagePos - glm::vec3(0, platformY, -1.5f));
        RenderMesh(meshes["platform"], shaders["VertexColor"], modelMatrix);

        if (!package) {
            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, packagePos);
            RenderMesh(meshes["package"], shaders["VertexColor"], modelMatrix);
        } else {
            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, dronePos - glm::vec3(0, 0.5f, 0));
            modelMatrix = glm::rotate(modelMatrix, rotation, glm::vec3(0, 1, 0));

            RenderMesh(meshes["package"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, destination - glm::vec3(0, platformY / 2, -2));
            RenderMesh(meshes["platform"], shaders["VertexColor"], modelMatrix);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, destination + glm::vec3(0, platformY / 2, 0));
            RenderMesh(meshes["destination"], shaders["VertexColor"], modelMatrix);
        }
    }

    if (Pickup()) {
        package = true;
        NewDest();
    }

    if (score == 0 && first == true) {
        printf("SCORE: %d\n", score);
        first = false;
    }

    if (ReachDest()) {
        package = false;
        score++;
        printf("Good job! SCORE: %d\n", score);
        NewPackage();
    }

    glm::vec3 targetPos;
    if (package) {
        targetPos = destination;
    }
    else {
        targetPos = packagePos;
    }

    glm::vec3 direction = glm::normalize(targetPos - dronePos);
    float angle = atan2(direction.x, direction.z);
    angle -= M_PI / 2;

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, dronePos + glm::vec3(0, 0.01f, 0));
    modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
    RenderMesh(meshes["arrow"], shaders["VertexColor"], modelMatrix);

    glm::ivec2 resolution = window->GetResolution();
    int minimapSize = (int)(resolution.y * 0.4f);
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(resolution.x - minimapSize, resolution.y - minimapSize, minimapSize, minimapSize);

    camera2->Set(dronePos + glm::vec3(0, 3.0f, 0), dronePos, glm::vec3(0, 0, -1));

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-25, 0, -25));
        Render2(meshes["terrain"], shaders["VertexColor"], modelMatrix, camera2, minimapProjection);

        for (size_t i = 0; i < obstacles.size(); i++) {
            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, obstacles[i].first);
            if (i < 15) {
                Render2(meshes["obstacle1"], shaders["VertexColor"], modelMatrix, camera2, minimapProjection);
            } else {
                Render2(meshes["obstacle2"], shaders["VertexColor"], modelMatrix, camera2, minimapProjection);
            }
        }

        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, dronePos);
        modelMatrix = glm::rotate(modelMatrix, rotation, glm::vec3(0, 1, 0));
        Render2(meshes["destination"], shaders["VertexColor"], modelMatrix, camera2, minimapProjection);

        if (!package) {
            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, packagePos);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
            Render2(meshes["package"], shaders["VertexColor"], modelMatrix, camera2, minimapProjection);
        } else {
            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, destination);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
            Render2(meshes["destination"], shaders["VertexColor"], modelMatrix, camera2, minimapProjection);
        }
    }
}

void Tema2::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

void Tema2::Render2(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix,
    implemented::Camera2* cam, const glm::mat4& projection)
{
    if (!mesh || !shader || !shader->program)
        return;

    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(cam->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    float speed = 5.0f;
    glm::vec3 newPos = dronePos;

    
    glm::vec3 forward = glm::normalize(glm::vec3(
        -cos(rotation),
        0,
        sin(rotation)
    ));
    glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
    
    if (window->KeyHold(GLFW_KEY_W)) {
        newPos += right * speed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        newPos -= right * speed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        newPos += forward * speed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        newPos -= forward * speed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_E)) {
        newPos += glm::vec3(0, speed * deltaTime, 0);
    }
    if (window->KeyHold(GLFW_KEY_Q)) {
        if (newPos.y - speed * deltaTime > 1.5f)
        {
            newPos -= glm::vec3(0, speed * deltaTime, 0);
        }
    }
    
    if (!checkCollision(newPos)) {
        dronePos = newPos;
    }
}

void Tema2::OnKeyPress(int key, int mods)
{

}

void Tema2::OnKeyRelease(int key, int mods)
{
  
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        if (window->GetSpecialKeyState() == 0) {
            const float sensitivity = 0.005f;

            camAngleY -= deltaX * sensitivity;
            camAngleX = glm::clamp(camAngleX - deltaY * sensitivity,
                -glm::pi<float>() / 4,
                glm::pi<float>() / 4);

            rotation = camAngleY;
        }
    }
}
