#include "..\include\Channel.h"

bool Channel::Load(Tokenizer& token)
{
	token.FindToken("{");
	token.FindToken("extrapolate");
	char temp[256];
	token.GetToken(temp);
	//std::cout << "extrapIn " << temp << "\n\n";
	//extrapIn
	if (strcmp(temp, "constant") == 0) {
		extrapIn = 0;
	}
	else if (strcmp(temp, "linear") == 0) {
		extrapIn = 1;
	}
	else if (strcmp(temp, "cycle") == 0) {
		extrapIn = 2;
	}
	else if (strcmp(temp, "cycle_offset") == 0) {
		extrapIn = 3;
	}
	else if (strcmp(temp, "bounce") == 0) {
		extrapIn = 4;
	}
	//extrapOut
	token.GetToken(temp);
	//std::cout << "Extrapout: " << temp << "\n\n";
	if (strcmp(temp, "constant") == 0) {
		extrapOut = 0;
	}
	else if (strcmp(temp, "linear") == 0) {
		extrapOut = 1;
	}
	else if (strcmp(temp, "cycle") == 0) {
		extrapOut = 2;
	}
	else if (strcmp(temp, "cycle_offset") == 0) {
		extrapOut = 3;
	}
	else if (strcmp(temp, "bounce") == 0) {
		extrapOut = 4;
	}

	//std::cout << "Loading Frames " << "\n\n";
	//Keyfram load
	token.FindToken("keys");
	numKeyframes = token.GetInt();
	token.FindToken("{");
	for (int i = 0; i < numKeyframes; ++i) {
		Keyframe* key = new Keyframe();

		//prev and prev next
		if (i > 0) {
			key->prev = keyframes[i - 1];
			key->prev->next = key;
		}

		//Time and vlaue
		key->time = token.GetFloat();
		key->value = token.GetFloat();

		//In Rule
		token.GetToken(temp);
		//std::cout << "Inrule " << temp << "\n\n";
		if (atof(temp)) {
			key->tangentIn = float(atof(temp));
			key->ruleIn = 0;
			//std::cout << "Loading float " << "\n\n";
		}
		else if (strcmp(temp, "flat") == 0){
			key->ruleIn = 1;
		}
		else if (strcmp(temp, "linear") == 0) {
			key->ruleIn = 2;
		}
		else if (strcmp(temp, "smooth") == 0) {
			key->ruleIn = 3;
		}


		//Out rule
		token.GetToken(temp);
		//std::cout << "Outrule " << temp << "\n\n";
		if (atof(temp)) {
			key->tangentOut = float(atof(temp));
			key->ruleOut = 0;
		}
		else if (strcmp(temp, "flat") == 0) {
			key->ruleOut = 1;
		}
		else if (strcmp(temp, "linear") == 0) {
			key->ruleOut = 2;
		}
		else if (strcmp(temp, "smooth") == 0) {
			key->ruleOut = 3;
		}

		//Add Key
		keyframes.push_back(key);
	}
	token.FindToken("}");
	token.FindToken("}");


	//Compute tangents and oceeficients
	for (Keyframe* k : keyframes) {
		k->computeTangents();
	}
	//Compute tangents and oceeficients
	for (Keyframe* k : keyframes) {
		k->computeCoefficients();
	}
	
	/*
	//DEBUG
	for (Keyframe* k : keyframes) {
		printf("TangentIn: %f", k->tangentIn);
		printf("TangentOut: %f\n", k->tangentOut);

	}
	*/
	return true;
}

float Channel::eval(float t)
{
	if (numKeyframes == 0) {
		return 0;
	}

	if (t < keyframes[0]->time) {
		return keyframes[0]->value;
	}
	if (t > keyframes[numKeyframes-1]->time) {
		return keyframes[numKeyframes-1]->value;
	}

	float u;
	for (int i = 1; i < numKeyframes; ++i) {
		if (t < keyframes[i]->time) {
			u = t - keyframes[i-1]->time;
			glm::vec4 co = keyframes[i - 1]->coef;
			return (co.w + u * (co.z + u * (co.y + u * (co.x))));
		}
	}
	return keyframes[numKeyframes - 1]->value;
}
