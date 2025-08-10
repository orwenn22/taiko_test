#ifndef SCENE_H
#define SCENE_H

struct InputEvent;
class SceneManager;

class Scene {
public:
    explicit Scene(int type = 0);
    virtual ~Scene();

    /**
     * Called when the scene becomes the active scene in the SceneManager.
     * Whenever this gets called, m_scene_manager has already been set.
     * The default implementation provided in the base Scene class does nothing.
     */
    virtual void OnEnter();

    /**
     * Called when the scene is no longer the active scene in the SceneManager.
     * This function is invoked before the current scene is replaced or destroyed.
     * The default implementation provided in the base Scene class does nothing.
     * Note: DO NOT CHANGE THE CURRENT STATE IN THIS FUNCTION IT WILL BREAK THINGS
     * @param getting_destroyed Indicates whether the scene is being destroyed
     * (true if the scene is being deleted, false if it is only being replaced).
     */
    virtual void OnExit(bool getting_destroyed);

    virtual void HandleInput(const InputEvent &input_event);
    virtual void Update(float dt);
    virtual void Draw();

    int GetType();

protected:
    SceneManager *GetManager();

private:
    friend SceneManager;
    SceneManager *m_scene_manager;
    int m_type;
};



#endif //SCENE_H
