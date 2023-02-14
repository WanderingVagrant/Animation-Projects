#pragma once
#include "core.h"
class Keyframe {
public:
	static const glm::mat4 HERM;
	float time;
	float value;
	float tangentIn, tangentOut;
	int ruleIn, ruleOut;  //1 flat, lineear 2, smooth 3, 0 Fixed
	glm::vec4 coef; //Cubic Coeeficients
	Keyframe* prev, *next;

	void computeTangents();
	void computeCoefficients();
};