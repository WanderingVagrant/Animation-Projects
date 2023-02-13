#pragma once
#include "Tokenizer.h"
#include "Keyframe.h"
#include <vector>


class Channel {
	int numKeyframes;
	const char* extrapIn, * extrapOut;
	std::vector<Keyframe*> keyframes;
	bool load();
	void eval(float t);

};