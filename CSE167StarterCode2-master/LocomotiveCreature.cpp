#include "LocomotiveCreature.h"


LocomotiveCreature::LocomotiveCreature(Joint * root) : root(root){
	
	sprintOkay = false;
	sprinting = false;
	turnRadian = 0.0f;

	vector<float> walk_triggers;
	walk_triggers.push_back(0.75f);
	walk_triggers.push_back(0.25f);
	walk_triggers.push_back(0.5f);
	walk_triggers.push_back(0.0f);

	float walk_dutyFactor = 0.7f;
	maxWalkSpeed = 0.0005f;

	vector<float> gallop_triggers;
	gallop_triggers.push_back(0.0f);
	gallop_triggers.push_back(0.5f);
	gallop_triggers.push_back(0.5f);
	gallop_triggers.push_back(0.0f);

	float gallop_dutyFactor = 0.4f;
	maxGallopSpeed = 0.002f;

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

void LocomotiveCreature::Update() {

	speed += accel;

	if (sprintOkay == false)
	{
		if (sprinting == false)
		{
			if (speed >= maxWalkSpeed)
			{
				speed = maxWalkSpeed;
			}

			else if (speed <= -maxWalkSpeed)
			{
				speed = -maxWalkSpeed;
			}
		}
		else
		{
			if (speed > maxWalkSpeed)
			{
				speed -= (glm::abs(accel * 3));
				if (speed <= maxWalkSpeed)
				{
					speed = maxWalkSpeed;
					sprinting = false;
				}
			}

			else if (speed < -maxWalkSpeed)
			{
				speed += (glm::abs(accel * 3));
				if (speed >= -maxWalkSpeed)
				{
					speed = -maxWalkSpeed;
					sprinting = false;
				}
			}
		}
	}

	else
	{	
		sprinting = true;
		if (speed >= maxGallopSpeed)
		{
			speed = maxGallopSpeed;
		}

		else if (speed <= -maxGallopSpeed)
		{
			speed = -maxGallopSpeed;
		}
	}

	//Set the gait
	if (speed > maxWalkSpeed || speed < -maxWalkSpeed)
	{
		cur_gait = gallop_gait;
	}

	else
	{
		cur_gait = walk_gait;
	}


	cur_gait->Update(speed);
	vector<float> curTimes = cur_gait->GetCurTimes();
	vector<float> dutyFactors = cur_gait->GetDutyFactors();

	for(int i = 0; i < legs.size(); i++)
	{
		legs[i]->Update(curTimes[i], dutyFactors[i]);
	}

	//Move the root forwards or backwards
	if (movementOkay)
	{
		float x = sin(root->GetDOF(1));
		float y = 0;
		float z = cos(root->GetDOF(1));
		glm::vec3 directionVec = glm::vec3(x,y,z);
		glm::normalize(directionVec);
		root->AddOffset(-speed * cur_gait->GetDutyFactors()[0] * directionVec);
		//root->Translate(glm::vec3(0.0f, 0.0f, -speed * cur_gait->GetDutyFactors()[0]));
	}
	
	
	//Rotate the body to the current turn position
	root->SetDOF(1, this->turnRadian);
}

void LocomotiveCreature::SetAccel(float accel) {
	this->accel = accel;
}

void LocomotiveCreature::ComeToStop(float stoppingAccel) {

	accel = 0.0f;

	if (speed > 0)
	{
		speed -= stoppingAccel;
		if (speed < 0)
			speed = 0;
	}

	else if (speed < 0)
	{
		speed += stoppingAccel;
		if (speed > 0)
			speed = 0;
	}
}

void LocomotiveCreature::SetSprint(bool sprintOkay) {
	this->sprintOkay = sprintOkay;
}

void LocomotiveCreature::TurnCreature(float turnRadian) {
	this->turnRadian = this->turnRadian + turnRadian;
}

void LocomotiveCreature::SetMovementOkay(bool movementOkay) {
	this->movementOkay = movementOkay;
}