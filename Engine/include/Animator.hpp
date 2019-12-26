#pragma once

#include "Component.hpp"

#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <iostream>

namespace Engine {

class BoneTransform {
public:
    BoneTransform() 
     : position(glm::vec3(0.0)), rotation(glm::quat(glm::vec3(0.0))), 
    matrix(glm::mat4(1.0)){}
    BoneTransform(glm::vec3 pos, glm::quat rot);
    BoneTransform(BoneTransform a, BoneTransform b, float p);
    BoneTransform(glm::mat4 mat);

    inline glm::vec3 getPosition() const {return position;}
    inline glm::quat getRotation() const {return rotation;}

    inline glm::mat4 getMatrix() const {return matrix;}
private:
    void calcMatrix();

    glm::vec3 position;
    glm::quat rotation;

    glm::mat4 matrix;
};


class Bone {
public:
    Bone() : id(0) {}
    Bone(std::string _name, unsigned int _id, BoneTransform trans, 
        glm::mat4 _inverseBind, std::vector<unsigned int> children)
     : name(_name), id(_id), inverseBind(_inverseBind), 
     transform(BoneTransform(trans)), children(children) {}
    Bone(std::string _name, unsigned int _id, glm::vec3 pos, glm::quat rot, 
        glm::mat4 _inverseBind, std::vector<unsigned int> children)
     : name(_name), id(_id), inverseBind(_inverseBind), 
     transform(BoneTransform(pos, rot)), children(children) {}
    
    inline glm::mat4 getMatrix() const {return matrix;}
    inline glm::mat4 getInverseBind() const {return inverseBind;}
    void setMatrix(glm::mat4 newPosition);
    
    inline glm::mat4 getBind() const {return inverseBind;}
private:
    //represents the transformation a point attached to this bone undergoes 
    glm::mat4 matrix;
    glm::mat4 inverseBind; //stores initial position as inverse matrix
public:
    std::string name; //reference for potentially reversing keyframes
    unsigned int id;
    BoneTransform transform; //stores current position in local space 
    std::vector<unsigned int> children;
};

struct Skeleton {
    Skeleton() 
     : bones(std::vector<Bone>()), root(0) {}
    Skeleton(std::vector<Bone> _bones, unsigned int _root)
     : bones(_bones), root(_root) {}
    std::vector<Bone> bones;
    unsigned int root;
};

struct Keyframe {
    Keyframe(float _time, std::vector<BoneTransform> _pose)
     : time(_time), pose(_pose) {}
    float time;
    std::vector<BoneTransform> pose;
};

struct Animation {
    Animation()
     : duration(0) {}
    Animation(std::vector<Keyframe> _frames)
     : frames(_frames) {
        duration = _frames.back().time;
    }
    float duration;
    std::vector<Keyframe> frames;
};

class Animator : public Component {
private:
    void updateBone(Bone* bone, glm::mat4 parent, std::vector<BoneTransform> pose);
    void updateDefaultBone(Bone* bone, glm::mat4 parent);
public:
    Animator(Skeleton _skel)
     : Component("animator"), skel(_skel) {
        for(auto & b : _skel.bones){
            //std::cout << b.name << std::endl;
            glm::mat4 m = glm::transpose(b.getBind());
           /*for(int i = 0; i < 4; i++){
                //std::cout << "x: " << m[i].x << " y: " << m[i].y << " z: " << m[i].z << " w: " << m[i].w << std::endl; 
            }*/ 
        }
    }

    virtual void update();

    void setAnimation(Animation* a){anim = a;
        int count = 0;
        for(auto & b : anim->frames[0].pose){
            //std::cout << count  << std::endl;
            glm::mat4 m = glm::transpose(b.getMatrix());
            for(int i = 0; i < 4; i++){
                //std::cout << "x: " << m[i].x << " y: " << m[i].y << " z: " << m[i].z << " w: " << m[i].w << std::endl; 
            }
            count++;
        }
    }
private:
    Animation* anim = NULL;
    float animTime;
    
    Skeleton skel;
};

}
