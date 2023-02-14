#pragma once
#include "Tokenizer.h"
#include "Keyframe.h"
#include <vector>
#include <map>
#include <iostream>

class Channel {
public:
	int numKeyframes;
	int extrapIn, extrapOut; //0-constant, 1-linear, 2-cycle, 3-cycle offset, 4-bounce
	std::vector<Keyframe*> keyframes;
	bool Load(Tokenizer& token);
	void eval(float t);

};