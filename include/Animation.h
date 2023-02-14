#pragma once
#include <vector>
#include "Channel.h"
#include <iostream>

class Animation {
public:
	float start, end;
	std::vector<Channel*> channels;
	bool Load(Tokenizer& token);
};