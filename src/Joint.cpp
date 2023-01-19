#include "Joint.h"
#include <iostream>
Joint::Joint()
{
	Local = glm::mat4(1.0f);
	World = glm::mat4(1.0f);
	cube = NULL;
	Offset = glm::vec3(0, 0, 0);
	dofs.push_back({ 0, -100000, 100000 });
	dofs.push_back({ 0, -100000, 100000 });
	dofs.push_back({ 0, -100000, 100000 });
	boxmin = glm::vec3(-0.1, -0.1, -0.1);
	boxmax = glm::vec3(0.1, 0.1, 0.1);
}

Joint::~Joint()
{
}

void Joint::Update(glm::mat4 pWorld)
{

		

	World = pWorld * Local;

	cube->update(World);

	for (Joint* child : childs) {
		child->Update(World);
	}
}

bool Joint::Load(Tokenizer& token) {
	token.FindToken("{");

	while (1) {	
		char temp[256];
		token.GetToken(temp);
		if (strcmp(temp, "offset") == 0) {
			Offset.x = token.GetFloat();
			Offset.y = token.GetFloat();
			Offset.z = token.GetFloat();
		}
		else if (strcmp(temp, "boxmin") == 0) {
			boxmin.x = token.GetFloat();
			boxmin.y = token.GetFloat();
			boxmin.z = token.GetFloat();
		}
		else if (strcmp(temp, "boxmax") == 0) {
			boxmax.x = token.GetFloat();
			boxmax.y = token.GetFloat();
			boxmax.z = token.GetFloat();
		}
		else if (strcmp(temp, "rotxlimit") == 0) {
			dofs[0].min = token.GetFloat();
			dofs[0].max = token.GetFloat();
		}
		else if (strcmp(temp, "rotylimit") == 0) {
			dofs[1].min = token.GetFloat();
			dofs[1].max = token.GetFloat();
		}
		else if (strcmp(temp, "rotzlimit") == 0) {
			dofs[2].min = token.GetFloat();
			dofs[2].max = token.GetFloat();
		}
		else if (strcmp(temp, "pose") == 0) {
			dofs[0].value = token.GetFloat();
			dofs[1].value = token.GetFloat();
			dofs[2].value = token.GetFloat();
		}
		else if (strcmp(temp, "balljoint") == 0) {
			Joint* jnt = new Joint();
			jnt->Load(token);
			AddChild(jnt);
		}
		else if (strcmp(temp, "}") == 0) {

			cube = new Cube(boxmin, boxmax);
			for (int i = 0; i < dofs.size(); ++i) {
				if (dofs[i].value < dofs[i].min) {
					std::cout << "Value too small\n";
					dofs[i].value = dofs[i].min;
				}
				else if (dofs[i].value > dofs[i].max) {
					std::cout << "Value too big\n";
					dofs[i].value = dofs[i].max;
				}
			}
			Local = glm::translate(Offset) *
				glm::rotate(dofs[2].value, glm::vec3(0.0f, 0.0f, 1.0f)) *
				glm::rotate(dofs[1].value, glm::vec3(0.0f, 1.0f, 0.0f)) *
				glm::rotate(dofs[0].value, glm::vec3(1.0f, 0.0f, 0.0f));
			return true;
		}
		else token.SkipLine(); // Unrecognized token
	}
}

void Joint::AddChild(Joint * child)
{
	childs.push_back(child);
}
void Joint::Draw(const glm::mat4& viewProjMtx, GLuint shader)
{
	cube->draw(viewProjMtx, shader);
	for (Joint* child : childs) {
		child->Draw(viewProjMtx, shader);
	}
}
;