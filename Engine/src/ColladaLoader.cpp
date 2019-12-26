#include "ColladaLoader.hpp"

#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define PI 3.141592
#define CORRECTION glm::rotate(glm::mat4(1.0),(float)-PI/(float)2.0,glm::vec3(1, 0, 0))

using namespace Engine;

//only searches through children(not recursive)
std::vector<xmlNode*> getNodesIn(xmlNode* node, std::string name){
    std::vector<xmlNode*> out;
    xmlNode* currNode = NULL;
    for(currNode = node->children; currNode; currNode = currNode->next){
        if(currNode->type == XML_ELEMENT_NODE){
            if(!strcmp((char*)currNode->name, name.c_str())){
                out.push_back(currNode);  
            }     
        }     
    }
    return out;
}

xmlNode* getNodeByProp(xmlNode* parent, std::string prop, std::string val){
    xmlNode* out = NULL;
    for(xmlNode* currNode = parent->children; currNode; currNode=currNode->next){
        if(currNode->type == XML_ELEMENT_NODE){
            char* s = (char*)xmlGetProp(currNode, (const xmlChar*)prop.c_str());
            if(s != NULL){
                std::string elemVal = s;
                if(val == elemVal){
                    out = currNode;
                    break;
                }
            }
            //we have not broken; therefore, this node doesn't match
            xmlNode* n = getNodeByProp(currNode, prop, val);
            if(n != NULL){
                out = n;
                break;
            }
        }     
    }
    return out;
}

std::vector<xmlNode*> getNodesRec(xmlNode* parent, std::string name){
    std::vector<xmlNode*> out;
    for(xmlNode* node = parent->children;node;node=node->next){
        if(node->type == XML_ELEMENT_NODE){
            if(name == std::string((char*)node->name)){
                out.push_back(node);
            } 
            std::vector<xmlNode*> temp = getNodesRec(node, name);
            for(auto & t : temp){
                out.push_back(t);
            }
        }
    }
    return out;
}

int getIndex(std::vector<std::string> names, std::string name){
    unsigned int i = 0;
    for(auto & n : names){
        if(n == name){
            return i;    
        }
        i++;
    }
    return -1; 
}

void ColladaLoader::loadModel(std::string file, VAO* vao){ 
    std::vector<glm::vec3> objectPositions;
    std::vector<glm::vec3> objectNormals;
    std::vector<unsigned int> objectIndices;
    std::vector<glm::vec3> objectWeights;
    
    LIBXML_TEST_VERSION;

    xmlDoc* doc = xmlReadFile(file.c_str(), NULL, 0);
    if(doc == NULL){
        std::cout << "Failed to read xml in: " << file << std::endl;
        return;    
    }
    xmlNode* root = xmlDocGetRootElement(doc);
    xmlNode* geometries = getNodesIn(root, "library_geometries")[0];
    xmlNode* controllers = getNodesIn(root, "library_controllers")[0];
    unsigned int count = 0;
    for(xmlNode* geometry=geometries->children;geometry; geometry=geometry->next){
        if(geometry->type == XML_ELEMENT_NODE){
            //runs for every geometry
            std::string geometryName = (char*)xmlGetProp(geometry, (xmlChar*)"id");
            xmlNode* mesh = getNodesIn(geometry, "mesh")[0];
            if(mesh == NULL){
                std::cout << "We have a problem" << std::endl;    
            } else {
                std::vector<glm::vec3> positionData;
                std::vector<glm::vec3> normalData;
                std::vector<unsigned int> indiceData;
                std::vector<glm::vec3> weightData;

                int i = 0;
                std::vector<xmlNode*> tempTriangles = getNodesIn(mesh, "triangles");
                xmlNode* triangles;
                bool isPoly = false;
                if(tempTriangles.size() == 0){
                    triangles = getNodesIn(mesh, "polylist")[0];
                    isPoly = true;
                } else {
                    triangles = tempTriangles[0]; 
                }
                //get the vertex source
                xmlNode* vInput = getNodeByProp(triangles, "semantic", "VERTEX");
                std::string vName = (char*)xmlGetProp(vInput, (xmlChar*)"source");
                vName = vName.substr(1);
                xmlNode* vertices = getNodeByProp(mesh, "id", vName);

                xmlNode* vPosInput = getNodeByProp(vertices, "semantic", "POSITION");
                std::string vPosName=(char*)xmlGetProp(vPosInput, (xmlChar*)"source");
                vPosName = vPosName.substr(1);
                const unsigned int vOffset = std::stoi(std::string((char*)xmlGetProp(vInput, (xmlChar*)"offset")));
                xmlNode* vPosSource = getNodeByProp(mesh, "id", vPosName);
                
                //get the normal source
                xmlNode* nInput = getNodeByProp(triangles, "semantic", "NORMAL");
                std::string nName = (char*)xmlGetProp(nInput, (xmlChar*)"source");
                nName = nName.substr(1);
                const unsigned int nOffset = std::stoi(std::string((char*)xmlGetProp(nInput, (xmlChar*)"offset")));
                xmlNode* nSource = getNodeByProp(mesh, "id", nName);

                //get the controller source
                std::string geoTag = std::string("#") + geometryName;
                xmlNode* controller = getNodeByProp(controllers, "source", geoTag);
                
                positionData = readVec3Source(vPosSource);
                normalData = readVec3Source(nSource);
                if(controller){
                    readController(controller, indiceData, weightData);
                } else {
                    for(int v = 0; v < positionData.size(); v++){
                        indiceData.push_back(1);
                        indiceData.push_back(0);
                        indiceData.push_back(0);
                        weightData.push_back(glm::vec3(1, 0, 0));
                    }
                }

                std::vector<xmlNode*> inputs = getNodesIn(triangles, "input");
                xmlNode* indices = getNodesIn(triangles, "p")[0];
                std::string vertIndexDataStr = (char*)xmlNodeGetContent(indices);
                std::stringstream vertIndexData(vertIndexDataStr);
                bool shouldQuit = false;
                while(!shouldQuit){
                    for(int i = 0; i < inputs.size(); i++){
                        unsigned int index = 0;
                        if(!(vertIndexData >> index)){
                            shouldQuit = true;
                            break;
                        } 
                        if(i == vOffset){
                            glm::vec3 pos = positionData[index];
                            pos = glm::vec3(CORRECTION*glm::vec4(pos.x, pos.y, pos.z, 1.0));
                            objectPositions.push_back(pos);
                            objectWeights.push_back(weightData[index]);
                            for(int ind=0; ind<3; ind++){
                                objectIndices.push_back(indiceData[index*3 + ind]); 
                            }
                        } else if(i == nOffset){
                            glm::vec3 norm = normalData[index];
                            norm = glm::vec3(CORRECTION*glm::vec4(norm.x, norm.y, norm.z, 0.0));
                            objectNormals.push_back(norm);
                        }
                    }
                }
            }
        } 
    }
    
    std::cout << objectPositions.size() << std::endl;
    vao->addFloatBuffer(0,glm::value_ptr(objectPositions[0]),objectPositions.size(),3);
    vao->addFloatBuffer(1,glm::value_ptr(objectNormals[0]),objectNormals.size(),3);
    vao->addFloatBuffer(3,glm::value_ptr(objectWeights[0]),objectWeights.size(),3);
    vao->addUIntBuffer(2,&objectIndices[0],objectIndices.size()/3, 3);

    xmlFreeDoc(doc);
    xmlCleanupParser();
    return;
}

void ColladaLoader::readController(xmlNode* controller, 
        std::vector<unsigned int>& indices, std::vector<glm::vec3>& weights){
    xmlNode* vWeight = getNodesIn(controller, "vertex_weights")[0];
    
    xmlNode* wInput = getNodeByProp(vWeight, "semantic", "WEIGHT");
    std::string wID = (char*)xmlGetProp(wInput, (xmlChar*)"source");
    wID = wID.substr(1);
    xmlNode* wSource = getNodeByProp(controller, "id", wID);
    std::vector<float> skinWeights = readFloatSource(wSource);
    
    xmlNode* vcNode = getNodesIn(vWeight, "vcount")[0];
    std::string vcDataStr = (char*)xmlNodeGetContent(vcNode);
    std::istringstream vcData(vcDataStr);

    xmlNode* vNode = getNodesIn(vWeight, "v")[0];
    std::string vDataStr = (char*)xmlNodeGetContent(vNode);
    std::istringstream vData(vDataStr);

    unsigned int vPos = 0;
    bool shouldQuit = false;
    while(!shouldQuit){
        unsigned int vcount;
        if(!(vcData >> vcount)){
            shouldQuit = true;
            break;
        } else {
            //get the 3 largest weights, put them in normalize
            std::vector<float> finalWeights(3, 0.0);
            std::vector<unsigned int> finalIndices(3, 0);
            for(int i = 0; i < vcount; i++){
                //sort the loaded indices with a relatively laborous
                //process
                unsigned int wi, index;
                vData >> index; vData >> wi;
                float w = skinWeights[wi];
                for(int fw = 0; fw < 3; fw++){
                    if(w > finalWeights[fw]){
                        finalWeights.insert(finalWeights.begin()+fw,w);
                        finalIndices.insert(finalIndices.begin()+fw,index);
                        break; 
                    } 
                }
            }
            bool shouldPrint = false;
            if(finalWeights.size() > 4) shouldPrint = true;
            finalWeights.resize(3);
            finalIndices.resize(3);
            glm::vec3 t_v = glm::make_vec3(&finalWeights[0]);
            t_v = t_v / (t_v.x + t_v.y + t_v.z);
            weights.push_back(t_v);
            for(auto & i : finalIndices){
                indices.push_back(i); 
            }
        }
    }
}

Animation ColladaLoader::loadAnimation(std::string file, Skeleton sk){
    xmlDoc* doc = xmlReadFile(file.c_str(), NULL, 0);
    xmlNode* root = xmlDocGetRootElement(doc);
    
    std::vector<Keyframe> keyframes;

    xmlNode* animation = getNodesIn(root, "library_animations")[0];
    std::vector<xmlNode*> animations = getNodesIn(animation, "animation");
    for(auto & a : animations){
        //get the id of the bone we are animating
        xmlNode* channel = getNodesIn(a, "channel")[0];
        std::string target = (char*)xmlGetProp(channel, (xmlChar*)"target");
        //TO DO: Support other targets
        target = target.substr(0, target.find("/"));
        int id = -1;
        for(int i = 0; i < sk.bones.size(); i++){
            if(sk.bones[i].name == target){
                id = i;
                break;
            }
        }
        if(id < 0){
            //this could be the skeleton
            continue; 
            //throw 1; 
        }

        xmlNode* timeSource = getNodesIn(a, "source")[0];
        xmlNode* transformSource = getNodesIn(a, "source")[1];
        std::vector<float> times = readFloatSource(timeSource);
        std::vector<glm::mat4> transform = readMat4Source(transformSource);
        if(times.size() > 0){
            for(int t = 0; t < times.size(); t++){
                glm::mat4 trans = transform[t];
                if(id == sk.root){
                    trans = CORRECTION*trans; 
                }
                BoneTransform transT = BoneTransform(trans);
                bool found = false;
                for(int i = 0; i < keyframes.size(); i++){
                    if(times[t] < keyframes[i].time){
                        Keyframe k(times[t],std::vector<BoneTransform>(sk.bones.size()));
                        k.pose[id] = transT; 
                        keyframes.insert(keyframes.begin()+i, k); 
                        found = true;
                        break;
                    } else if(times[t] == keyframes[i].time){
                        keyframes[i].pose[id] = transT; 
                        found = true;
                        break;
                    }
                }
                if(!found){
                    Keyframe k
                        (times[t],std::vector<BoneTransform>(sk.bones.size()));
                    k.pose[id] = transT; 
                    keyframes.push_back(k);     
                }
            }
        }
    }

    xmlFreeDoc(doc);
    xmlCleanupParser();
    Animation a = Animation(keyframes);
    return a;
}

Skeleton ColladaLoader::loadSkeleton(std::string file){
    xmlDoc* doc = xmlReadFile(file.c_str(), NULL, 0);
    xmlNode* root = xmlDocGetRootElement(doc);
    
    //parse the hierarchy and populate as necessary
    xmlNode* libScene = getNodesIn(root, "library_visual_scenes")[0];
    xmlNode* scene = getNodesIn(libScene, "visual_scene")[0];
    xmlNode* armature = getNodeByProp(scene, "id", "Armature");
    if(armature == NULL){
        xmlFreeDoc(doc);
        xmlCleanupParser();
        return Skeleton(); 
    }
    xmlNode* rootNode = getNodesIn(armature, "node")[0];
    xmlNode* hips = getNodesIn(rootNode, "node")[0];

    xmlNode* controllers = getNodesIn(root, "library_controllers")[0];
    xmlNode* nameSource = getNodesRec(controllers, "source")[0];
    xmlNode* bindSource = getNodesRec(controllers, "source")[1];
    std::vector<std::string> jointNames = readNameSource(nameSource);
    std::vector<glm::mat4> jointBinds = readMat4Source(bindSource);

    Skeleton out;
    out.bones = std::vector<Bone>(jointNames.size());
    glm::mat4 correctedMat = CORRECTION;
    out.root = parseJointLayer(rootNode,correctedMat,out.bones,jointNames,jointBinds, true);

    xmlFreeDoc(doc);
    xmlCleanupParser();
    return out;
}

unsigned int ColladaLoader::parseJointLayer(xmlNode* joint, glm::mat4 parentMat, std::vector<Bone> & bones, std::vector<std::string> jointNames, std::vector<glm::mat4> jointBinds, bool isRoot){
    //take care of this element
    xmlNode* btNode = getNodesIn(joint, "matrix")[0]; 
    //this is the local transform used to calc local pos and rot
    glm::mat4 lJTransform = glm::transpose(readMat(btNode));
    BoneTransform lTrans;
    glm::mat4 gJTransform = parentMat*lJTransform;
    if(isRoot){
        lTrans = BoneTransform(gJTransform);
    } else {
        lTrans = BoneTransform(lJTransform);
    }

    //take care of children nodes
    std::vector<unsigned int> childBones;
    std::vector<xmlNode*> childNodes = getNodesIn(joint, "node");
    for(auto & node : childNodes){
        childBones.push_back(parseJointLayer(node,gJTransform,bones,jointNames,jointBinds, false));
    }

    glm::mat4 m = glm::transpose(gJTransform);
    const char* n = (char*)xmlGetProp(joint, (xmlChar*)"sid");
    std::string name = (char*)xmlGetProp(joint, (xmlChar*)"id");
    int id;
    if(n != NULL){
        std::string sid = n;
        id = getIndex(jointNames, sid);
        if(id < 0){
            id = bones.size();
            glm::mat4 inverseBind = glm::inverse(gJTransform);
            bones.push_back(
                Bone(name,id,lTrans,inverseBind,childBones));
        } else {
            bones[id] = Bone(name,id,lTrans,glm::inverse(gJTransform),childBones);
        }
    } else {
        id = bones.size();
        bones.push_back(
            Bone(name,id,lTrans,glm::inverse(gJTransform),childBones));
    }

    return id;
}

glm::mat4 ColladaLoader::readMat(xmlNode* source){
    std::string dataStr = (char*)xmlNodeGetContent(source);
    std::stringstream data(dataStr);
    double temp[16];
    while(data >> temp[0]){
        for(int i = 1; i < 16; i++){
            data >> temp[i]; 
        }
    }
    return glm::make_mat4(temp);
}

std::vector<float> ColladaLoader::readFloatSource(xmlNode* source){
    xmlNode* floatArray = getNodesIn(source, "float_array")[0];

    std::string dataStr = (char*)xmlNodeGetContent(floatArray);
    std::istringstream data(dataStr);
    
    std::vector<float> out;
    float x;
    while(data >> x){
        out.push_back(x); 
    }
    return out;
}

std::vector<glm::vec3> ColladaLoader::readVec3Source(xmlNode* source){
    xmlNode* floatArray = getNodesIn(source, "float_array")[0];

    std::string dataStr = (char*)xmlNodeGetContent(floatArray);
    std::istringstream data(dataStr);
    
    std::vector<glm::vec3> out;
    float x, y, z;
    while(data >> x){
        data >> y;
        data >> z;
        out.push_back(glm::vec3(x, y, z)); 
    }

    return out;
}

std::vector<std::string> ColladaLoader::readNameSource(xmlNode* source){
    xmlNode* nameArray = getNodesIn(source, "Name_array")[0];

    std::string dataStr = (char*)xmlNodeGetContent(nameArray);
    std::istringstream data(dataStr);

    std::vector<std::string> out;
    std::string s;
    while(data >> s){
        out.push_back(s);    
    }
    return out;
}

std::vector<glm::mat4> ColladaLoader::readMat4Source(xmlNode* source){
    xmlNode* floatArray = getNodesIn(source, "float_array")[0];

    std::string dataStr = (char*)xmlNodeGetContent(floatArray);
    std::istringstream data(dataStr);

    std::vector<glm::mat4> out;
    float temp[16];
    while(data >> temp[0]){
        for(int i = 1; i < 16; i++){
            data >> temp[i]; 
        }
        glm::mat4 mTemp = glm::transpose(glm::make_mat4(temp));
        out.push_back(mTemp);
    }

    return out;
}
