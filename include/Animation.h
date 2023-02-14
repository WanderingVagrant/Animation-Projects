#pragma once
#include <vector>
#include "Channel.h"
#include <iostream>
#include "Joint.h"

class Animation {
public:
	float start, end;
	std::vector<Channel*> channels;
	bool Load(Tokenizer& token);
	void update(float t, std::vector<Joint*> joints);

};