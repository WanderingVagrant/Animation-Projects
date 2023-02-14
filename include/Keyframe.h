#pragma once

class Keyframe {
public:
	float time;
	float value;
	float tangentIn, tangentOut;
	int ruleIn, ruleOut;  //1 flat, lineear 2, smooth 3, 0 Fixed
	float a, b, c, d; //Cubic Coeeficients
	Keyframe* prev, *next;

	void computeTangents();
	void computeCoefficients();
};