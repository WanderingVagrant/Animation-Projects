#pragma once

class Keyframe {
public:
	float time;
	float value;
	float tangentIn, tangentOut;
	const char* ruleIn, *ruleOut;
	float a, b, c, d; //Cubic Coeeficients
	Keyframe* prev, *next;

	void computeTangents();
	void computeCoefficients();
};