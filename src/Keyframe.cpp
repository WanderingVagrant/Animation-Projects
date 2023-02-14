#include "..\include\Keyframe.h"
const glm::mat4 Keyframe::HERM = glm::mat4(2, -3, 0, 1,
									-2, 3, 0, 0,
									1, -2, 1, 0,
									1, -1, 0, 0);
void Keyframe::computeTangents()
{
	if (ruleIn == 2) {
		if (prev != 0) {
			tangentIn = (value - prev->value) / (time - prev->time);
		}
	}

	if (ruleOut == 2) {
		if (next != 0) {
			tangentOut = (next->value - value) / (next->time - time);
		}
	}

	if (ruleIn == 3) {
		if (prev != 0) {
			if (next != 0) {
				tangentIn = (next->value - prev->value) / (next->time - prev->time);
			}
			else {
				tangentIn = (value - prev->value) / (time - prev->time);
			}
		}
	}

	if (ruleOut == 3) {
		if (next != 0) {
			if (prev != 0) {
				tangentOut = (next->value - prev->value) / (next->time - prev->time);
			}
			else {
				tangentOut = (next->value - value) / (next->time - time);
			}
		}
	}
}

void Keyframe::computeCoefficients()
{
	if (next == 0) {
		return;
	}
	float scale = next->time - time;
	coef = HERM * glm::vec4(value, next->value, scale * tangentOut, scale * next->tangentIn);

	//Scale by t1-t0
	coef.x /= (scale *scale *scale);
	coef.y /= (scale * scale);
	coef.z /= scale;
}
