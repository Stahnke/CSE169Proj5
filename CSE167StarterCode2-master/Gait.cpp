#include "Gait.h"

Gait::Gait(float dutyFactor, vector<float> triggers) {

	maxTime = 1.0f;
	curTime = 0.0f;
	Step * temp_step;

	//front right
	temp_step = new Step(triggers[0], maxTime, dutyFactor);
	steps.push_back(temp_step);
	//front left
	temp_step = new Step(triggers[1], maxTime, dutyFactor);
	steps.push_back(temp_step);
	//back right
	temp_step = new Step(triggers[2], maxTime, dutyFactor);
	steps.push_back(temp_step);
	//back right
	temp_step = new Step(triggers[3], maxTime, dutyFactor);
	steps.push_back(temp_step);
}

Gait::~Gait() {

}

void Gait::Update(float stepSize) {
	if (curTime >= maxTime)
	{
		curTime = 0.0f;
	}

	else if (curTime < 0.0f)
	{
		curTime = maxTime;
	}

	curTime += stepSize;
	cout << "CURRENT GAIT TIME: " << curTime << endl;
	for each(Step * step in steps)
	{
		step->Update(curTime);
	}
}

vector<float> Gait::GetCurTimes() {
	vector<float> curTimes;
	for each (Step * step in steps)
	{
		curTimes.push_back(step->GetCurTime());
	}

	return curTimes;
}

vector<float> Gait::GetDutyFactors() {
	vector<float> dutyFactors;
	for each (Step * step in steps)
	{
		dutyFactors.push_back(step->GetDutyFactor());
	}

	return dutyFactors;
}

