#include "Game.hpp"

#include "Animator.hpp"
#include "ShaderProgram.hpp"
#include "VAO.hpp"

class MyGame : public Engine::Game {
public:
    MyGame(){};
    virtual void start();
    virtual void update();
private:
    
    Engine::ShaderProgram *simpleAnimated, *simple;
    Engine::VAO *cubeVAO, *playerVAO, *robotVAO, *hallVAO;

    void addJointInPose(Engine::Skeleton sk, std::vector<Engine::BoneTransform> pose, Engine::Bone* b, glm::mat4 parent);
    //this is inherited from Engine::Game
    //Engine::GameObject* root;
};
