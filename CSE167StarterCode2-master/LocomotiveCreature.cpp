#include "LocomotiveCreature.h"


LocomotiveCreature::LocomotiveCreature(Joint * root) : root(root){
	
	vector<float> walk_triggers;
	walk_triggers.push_back(0.75f);
	walk_triggers.push_back(0.25f);
	walk_triggers.push_back(0.5f);
	walk_triggers.push_back(0.0f);

	float walk_dutyFactor = 0.7f;

	vector<float> gallop_triggers;
	gallop_triggers.push_back(0.0f);
	gallop_triggers.push_back(0.2f);
	gallop_triggers.push_back(0.4f);
	gallop_triggers.push_back(0.6f);

	float gallop_dutyFactor = 0.5f;

	walk_gait = new Gait(walk_dutyFactor, walk_triggers);
	gallop_gait = new Gait(gallop_dutyFactor, gallop_triggers);
	cur_gait = walk_gait;

	LocomotiveLeg * tempLeg;

	/*foot positions
	0.75 - 2.75 - 1.75
	- 0.75 - 2.75 - 1.75
	0.75 - 2.75 1.75
	- 0.75 - 2.75 1.75*/

	vector<glm::vec3> lerp_points1;
	vector<glm::vec3> lerp_points2;
	vector<glm::vec3> lerp_points3;
	vector<glm::vec3> lerp_points4;

	Joint * hip1;
	Joint * hip2;
	Joint * hip3;
	Joint * hip4;

	hip1 = root->FindJoint(1);
	lerp_points1.push_back(glm::vec3(0.75f, -2.3f, -1.3f));
	//lerp_points1.push_back(glm::vec3(0.75f, -1.5f, -1.75f));
	lerp_points1.push_back(glm::vec3(0.75f, -1.25f, -1.3f));
	lerp_points1.push_back(glm::vec3(0.75f, -1.25f, -2.3f));
	lerp_points1.push_back(glm::vec3(0.75f, -2.3f, -2.3f));
	tempLeg = new LocomotiveLeg(root, lerp_points1, hip1);
	legs.push_back(tempLeg);

	hip2 = root->FindJoint(2);
	lerp_points2.push_back(glm::vec3(-0.75f, -2.3f, -1.3f));
	lerp_points2.push_back(glm::vec3(-0.75f, -1.25f, -1.3f));
	lerp_points2.push_back(glm::vec3(-0.75f, -1.25f, -2.3f));
	lerp_points2.push_back(glm::vec3(-0.75f, -2.3f, -2.3f));
	tempLeg = new LocomotiveLeg(root, lerp_points2, hip2);
	legs.push_back(tempLeg);

	hip3 = root->FindJoint(3);
	lerp_points3.push_back(glm::vec3(0.75f, -2.3f, 2.3f));
	lerp_points3.push_back(glm::vec3(0.75f, -1.25f, 2.3f));
	lerp_points3.push_back(glm::vec3(0.75f, -1.25f, 1.3f));
	lerp_points3.push_back(glm::vec3(0.75f, -2.3f, 1.3f));
	tempLeg = new LocomotiveLeg(root, lerp_points3, hip3);
	legs.push_back(tempLeg);

	hip4 = root->FindJoint(4);
	lerp_points4.push_back(glm::vec3(-0.75f, -2.3f, 2.3f));
	lerp_points4.push_back(glm::vec3(-0.75f, -1.25f, 2.3f));
	lerp_points4.push_back(glm::vec3(-0.75f, -1.25f, 1.3f));
	lerp_points4.push_back(glm::vec3(-0.75f, -2.3f, 1.3f));
	tempLeg = new LocomotiveLeg(root, lerp_points4, hip4);
	legs.push_back(tempLeg);
}

LocomotiveCreature::~LocomotiveCreature() {

}

void LocomotiveCreature::Update(float stepSize) {
	cur_gait->Update(stepSize);
	vector<float> curTimes = cur_gait->GetCurTimes();
	vector<float> dutyFactors = cur_gait->GetDutyFactors();

	for(int i = 0; i < legs.size(); i++)
	{
		legs[i]->Update(curTimes[i], dutyFactors[i]);
	}

	//Move the root forwards or backwards
	root->AddOffsetZ(-stepSize * cur_gait->GetDutyFactors()[0]);
}