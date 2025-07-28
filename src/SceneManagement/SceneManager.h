#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

struct InputEvent;
class Scene;

class SceneManager {
public:
    explicit SceneManager(Scene *scene = nullptr);
    ~SceneManager();

    void HandleInput(const InputEvent &input_event);
    void Update(float dt);
    void Draw();

    void SetScene(Scene *scene, bool delete_previous_scene = true);
    Scene *GetScene() const { return m_scene; }

    int GetSceneType();

private:
    void CheckNextScene();

    Scene *m_scene;
    Scene *m_next_scene;
    bool m_delete_previous_scene;
};



#endif //SCENEMANAGER_H
