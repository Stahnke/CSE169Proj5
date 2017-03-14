#include "LocomotiveLeg.h"

#define PI 3.1415926535897932384626433832795

LocomotiveLeg::LocomotiveLeg(Joint * root, vector<glm::vec3> lerp_points, Joint * hip) : root(root), lerp_points(lerp_points), hip(hip) {

	leg = this->hip->FindJoint(0);

	lerp_point1 = lerp_points[0];
	lerp_point2 = lerp_points[1];
	lerp_point3 = lerp_points[2];
	lerp_point4 = lerp_points[3];
}

LocomotiveLeg::~LocomotiveLeg() {

}

void LocomotiveLeg::Update(float curTime, float dutyFactor) {

	this->curTime = curTime;
	//cout << "CURRENT STEP TIME: " << this->curTime << endl;

	//Do a lerp between the lerp_points
	//if on the ground
	if (curTime <= dutyFactor)
	{
		cur_goal[0] = Lerp(lerp_point4[0], lerp_point1[0], curTime / dutyFactor);
		cur_goal[1] = Lerp(lerp_point4[1], lerp_point1[1], curTime / dutyFactor);
		cur_goal[2] = Lerp(lerp_point4[2], lerp_point1[2], curTime / dutyFactor);
	}

	else if (curTime > dutyFactor)
	{
		cur_goal[0] = CalcBezier(0, (curTime - dutyFactor ) / (1.0 - dutyFactor) );
		cur_goal[1] = CalcBezier(1, (curTime - dutyFactor ) / (1.0 - dutyFactor) );
		cur_goal[2] = CalcBezier(2, (curTime - dutyFactor ) / (1.0 - dutyFactor) );
	}

	//cout << "Current Goal:" << " [x] = " << cur_goal[0] << " [y] = " << cur_goal[1] << " [z] = " << cur_goal[2] << endl;

	//convert our goal to world coordinates
	cur_goal_world = glm::vec3(root->getWorldMat() * glm::vec4(cur_goal,1.0f));
	//cout << "Current Goal World:" << " [x] = " << cur_goal_world[0] << " [y] = " << cur_goal_world[1] << " [z] = " << cur_goal_world[2] << endl;

	CalcIK();
}

float LocomotiveLeg::Lerp(float y1, float y2, float u) {

	//cout << " U = " << u << " ";

	return (y1 * (1 - u) + y2 * u);
}

float LocomotiveLeg::CalcBezier(int coord, float u) {
	
	return Lerp(Lerp(Lerp(lerp_point1[coord], lerp_point2[coord], u), Lerp(lerp_point2[coord], lerp_point3[coord], u), u), Lerp(Lerp(lerp_point2[coord], lerp_point3[coord], u), Lerp(lerp_point3[coord], lerp_point4[coord], u), u), u);
}

void LocomotiveLeg::CalcIK() {

	//Step 1 Get unrotated hip
	glm::mat4 hip_unrotated_star;
	glm::mat4 hip_unrotated;

	hip_unrotated_star = glm::mat4(1.0f);
	hip_unrotated_star[3] = glm::vec4(hip->GetOffset(), 1.0f);
	hip_unrotated = root->getWorldMat() * hip_unrotated_star;

	//Step 2 Transform goal to hip space
	glm::vec4 goal_hip_space;

	goal_hip_space = glm::inverse(hip_unrotated) * glm::vec4(cur_goal_world, 1.0f);

	//Step 3 find knee x angle
	float KneeXAng;
	float theta;

	float t_len = 1.0f;
	float c_len = 0.75f;

	theta = glm::acos( ( (t_len * t_len) + (c_len * c_len) - ( glm::length(goal_hip_space) * glm::length(goal_hip_space) ) ) / (2 * t_len * c_len) );
	KneeXAng = theta - PI;

	//Step 4 find hip x angle
	float HipXAng;
	float alpha;
	float beta;

	alpha = glm::asin( (c_len * glm::sin(theta) ) / (glm::length(goal_hip_space) ) );
	beta = glm::asin( -(goal_hip_space[2]) / glm::length(goal_hip_space) );

	HipXAng = alpha + beta;

	//Step 5 Calc hip z angle
	float HipZAng;

	HipZAng = glm::atan( (goal_hip_space[0] - hip->GetOffset()[0]) / (goal_hip_space[1] - hip->GetOffset()[1]) );

	//Step 6 apply the rotations
	leg->SetDOF(0, (KneeXAng));
	hip->SetDOF(0, (HipXAng));
	//hip->SetDOF(2, HipZAng);

	/*cout << "THETA = " << theta << endl;
	cout << "GOAL LENGTH = " << glm::length(goal_hip_space) << endl;
	cout << "HipXAng = " << HipXAng << endl;
	cout << "ALPHA = " << alpha << endl;
	cout << "BETA = " << beta << endl;
	cout << "goal_hip_space[2] = " << goal_hip_space[2] << endl;
	cout << "hip_offset[2] = " << hip->GetOffset()[2] << endl;
	cout << "goal - hip / goal_length= " << -(goal_hip_space[2] - hip->GetOffset()[2]) / glm::length(goal_hip_space) << endl;*/
	
}