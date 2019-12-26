#pragma once

#include "VAO.hpp"
#include "Animator.hpp"

#include <iostream>
#include <string>

#include <glm/glm.hpp>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

namespace Engine {

class ColladaLoader {
public:
    static void loadModel(std::string file, VAO* vao);
    static void readController(xmlNode* controller, std::vector<unsigned int>& indices, std::vector<glm::vec3>& weights);
    
    static Animation loadAnimation(std::string file, Skeleton sk);
    static Skeleton loadSkeleton(std::string file);
private:
    static std::vector<glm::vec3> readVec3Source(xmlNode* source);
    static std::vector<float> readFloatSource(xmlNode* source);
    static std::vector<std::string> readNameSource(xmlNode* source);
    static std::vector<glm::mat4> readMat4Source(xmlNode* source);
    static glm::mat4 readMat(xmlNode* source);

    static unsigned int parseJointLayer(xmlNode* joint, glm::mat4 parentMat, std::vector<Bone> & bones, std::vector<std::string> jointNames, std::vector<glm::mat4> jointBinds, bool isRoot);
};

}
