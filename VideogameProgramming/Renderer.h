#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Components.h"

using namespace std;

class Renderer
{
public:
    Renderer();

    void Init();

    void DrawSprite(Texture& texture, glm::mat4 proj, glm::vec2 position,
        glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f,
        glm::vec3 color = glm::vec3(1.0f), string shaderName = "default");

    void DrawMesh(Mesh& mesh, Texture& texture, glm::mat4 proj, glm::vec3 position, float scale,glm::vec3 rotation, Camera camera, Texture& normalsTexture,float far,
        string shaderName = "default");
    void DrawMesh(Mesh& mesh, Texture& texture, glm::mat4 projection, glm::vec3 position, float scale, glm::vec3 rotation, Camera cam, Texture& normalsTexture, Texture& rough, Texture& metallic, Texture& emissive, Texture& mixedAO, float far, float time,
        string shaderName = "default");

    void DrawSkybox(Mesh& mesh, Texture& texture1, Texture& texture2, Texture& texture3, Texture& texture4, glm::mat4 projection, Camera cam,float ticks);

    void Delete();
private:
    std::shared_ptr<Shader> shaderDefault;
    std::shared_ptr<Shader> shaderUnlit;
    std::shared_ptr<Shader> shaderInverted;
    std::shared_ptr<Shader> shaderRepeating;
    std::shared_ptr<Shader> shaderSkybox;
    std::shared_ptr<Shader> shaderBullet;
    std::shared_ptr<Shader> shaderUser;
    std::shared_ptr<Shader> shaderEnemy;
    std::shared_ptr<VAO> vao_quad;
    std::shared_ptr<VBO> vbo;
    std::shared_ptr<EBO> ebo;
};