#include "Scene.h"

Scene::Scene(int type) : m_scene_manager(nullptr), m_type(type) {
}

Scene::~Scene() = default;

void Scene::OnEnter() { }
void Scene::OnExit(bool getting_destroyed) { }

void Scene::HandleInput(const InputEvent &input_event) {}
void Scene::Update(float dt) {}
void Scene::Draw() {}

int Scene::GetType() {
    return m_type;
}

SceneManager * Scene::GetManager() {
    return m_scene_manager;
}
