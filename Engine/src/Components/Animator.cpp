#include "Animator.hpp"

#include "Time.hpp"
#include "Renderable.hpp"

#include <cmath>

using namespace Engine;

BoneTransform::BoneTransform(glm::vec3 pos, glm::quat rot)
 : position(pos), rotation(rot) {	
	calcMatrix();
}

BoneTransform::BoneTransform(BoneTransform a, BoneTransform b, float p){
	glm::vec3 apos = a.getPosition();
	glm::vec3 bpos = b.getPosition();
	position = apos + (bpos - apos)*p;    
	rotation = glm::slerp(a.getRotation(), b.getRotation(), p);
	calcMatrix();
}

BoneTransform::BoneTransform(glm::mat4 mat){
	position = glm::vec3(mat[3]);
	rotation = glm::quat(mat);
	calcMatrix();
}

void BoneTransform::calcMatrix(){
	matrix = glm::translate(glm::mat4(1.0), position);
	matrix *= glm::mat4_cast(rotation);
}

void Bone::setMatrix(glm::mat4 newPosition){
	matrix = newPosition * inverseBind;
}

void Animator::update(){
	if(anim != NULL){	
		animTime += Time::getDeltaUpdate();
		animTime = fmod(animTime, anim->duration);
		//animTime = 0;

		//get the last and next frames
		Keyframe last = anim->frames[0]; 
		Keyframe next = anim->frames[0];
		for(auto & frame : anim->frames){
			next = frame;
			if(frame.time > animTime){
				break;	
			}
			last = next;
		}
		float p = (animTime - last.time) / (next.time - last.time);
		//update the pose
		std::vector<BoneTransform> pose;
		for(int i = 0; i < last.pose.size(); i++){
			pose.push_back(BoneTransform(last.pose[i], next.pose[i], p));
		}
		updateBone(&(skel.bones[skel.root]), glm::mat4(1.0), pose);
	} else {
		updateDefaultBone(&(skel.bones[skel.root]), glm::mat4(1.0));		
	}	
	//generate a vector of matrices or something and pass it to the renderable
	std::vector<glm::mat4> bones;
	for(auto & bone : skel.bones){
		bones.push_back(bone.getMatrix());
	}
	Renderable* renderable = (Renderable*)object->getComponent("renderable");
	if(renderable){
		renderable->setBoneTransforms(bones);
	}
}

void Animator::updateDefaultBone(Bone* bone, glm::mat4 parent){
	glm::mat4 gBonePos = parent * bone->transform.getMatrix();
	for(auto & child : bone->children){
		updateDefaultBone(&(skel.bones[child]), gBonePos);		
	}
	bone->setMatrix(gBonePos);
}

void Animator::updateBone(Bone* bone,glm::mat4 parent,std::vector<BoneTransform> pose){
	bone->transform = pose[bone->id];
	//this should be the global
	glm::mat4 bonePosition = parent * pose[bone->id].getMatrix();
	for(auto & child : bone->children){
		updateBone(&(skel.bones[child]), bonePosition, pose);
	}
	bone->setMatrix(bonePosition);
}

