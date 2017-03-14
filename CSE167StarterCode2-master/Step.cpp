#include "Step.h"

Step::Step(float stepTrigger, float maxTime, float dutyFactor) : stepTrigger(stepTrigger), maxTime(maxTime), dutyFactor(dutyFactor) {
}

Step::~Step() {

}

void Step::Update(float curTime) {

	this->curTime = curTime + stepTrigger;
	if (this->curTime >= maxTime)
	{
		this->curTime -= maxTime;
	}

}

float Step::GetCurTime() {
	return curTime;
}

float Step::GetDutyFactor() {
	return dutyFactor;
}