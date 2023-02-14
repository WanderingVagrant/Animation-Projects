#include "..\include\Keyframe.h"

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
				tangentIn = (next->value - prev->value) / (next->time - prev->time);
			}
			else {
				tangentIn = (next->value - value) / (next->time - time);
			}
		}
	}
}

void Keyframe::computeCoefficients()
{
}
