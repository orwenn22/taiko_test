#include "SceneManager.h"

#include "Scene.h"

SceneManager::SceneManager(Scene *scene) : m_scene(nullptr), m_next_scene(nullptr), m_delete_previous_scene(false) {
    SetScene(scene, false);
}

SceneManager::~SceneManager() {
    if (m_next_scene) {
        delete m_next_scene;
        m_next_scene = nullptr;
    }

    if (m_scene) {
        delete m_scene;
        m_scene = nullptr;
    }
}

void SceneManager::HandleInput(const InputEvent &input_event) {
    if (m_scene) m_scene->HandleInput(input_event);
}

void SceneManager::Update(float dt) {
    CheckNextScene();
    if (m_scene) m_scene->Update(dt);
}

void SceneManager::Draw() {
    CheckNextScene();
    if (m_scene) m_scene->Draw();
}

void SceneManager::SetScene(Scene *scene, bool delete_previous_scene) {
    if (m_next_scene) delete m_next_scene;
    m_next_scene = scene;
    m_delete_previous_scene = delete_previous_scene;
}

int SceneManager::GetSceneType() {
    if (m_scene == nullptr) return -1;
    return m_scene->GetType();
}


/////////////////////
/// PRIVATE

void SceneManager::CheckNextScene() {
    if (m_next_scene == nullptr) return;

    if (m_scene) {
        //FIXME: if OnExit changes the current state stuff are most likely going to break.
        //       memory will also be leaked on the next line if it calls SetState(smth, false),
        //       but if that happens this probably wouldn't be the most important issue.
        m_scene->OnExit(m_delete_previous_scene);
        if (m_delete_previous_scene) delete m_scene;
    }

    m_scene = m_next_scene;
    m_scene->m_scene_manager = this;
    m_next_scene = nullptr;

    m_scene->OnEnter();
}
