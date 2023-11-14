#include "RenderSystem.h"

RenderSystem::RenderSystem(int width, int height) {

    this->width = width;
    this->height = height;

    rend.Init();
}

void RenderSystem::setCamera(Entity* camera)
{
    this->camera = camera;
}

void RenderSystem::tick(World* world, float deltaTime) 
{
    
    world->each<Skybox>([&](Entity* ent, ComponentHandle<Skybox> meshComp) {

        Texture texture = textureManager.GetTexture(meshComp->textureFilepath);

        Mesh mesh = meshManager.GetMesh(meshComp->meshFilepath);

        glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)(width / height), 0.1f, 100.0f);

        ComponentHandle<Camera> cam = camera->get<Camera>();

        rend.DrawSkybox(mesh, texture, proj, cam.get());

    });
    
    world->each<Sprite>([&](Entity* ent, ComponentHandle<Sprite> sprite) {

        ComponentHandle<Transform2D> transform = ent->get<Transform2D>();

        Texture texture = textureManager.GetTexture(sprite->filepath);

        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);
           
        if (sprite->autoSize) {
            rend.DrawSprite(texture, projection, transform->position, texture.GetSize(), transform->rotation, sprite->color, sprite->shaderName);
        }
        else {
            rend.DrawSprite(texture, projection, transform->position, sprite->size, transform->rotation, sprite->color, sprite->shaderName);
        }

    });

    world->each<MeshComponent>([&](Entity* ent, ComponentHandle<MeshComponent> meshComp) {

        

        ComponentHandle<Transform3D> transform = ent->get<Transform3D>();

        Texture texture = textureManager.GetTexture(meshComp->textureFilepath);

        Mesh mesh = meshManager.GetMesh(meshComp->meshFilepath);

        glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)(width / height), 0.1f, 100.0f);

        ComponentHandle<Camera> cam = camera->get<Camera>();

        rend.DrawMesh(mesh, texture, proj, transform->position, transform->scale, cam.get(), meshComp->shaderName);

    });

    
}