#include "..\include\Animation.h"

bool Animation::Load(Tokenizer& token)
{
	token.FindToken("{");
	token.FindToken("range");
	start = token.GetFloat();
	end = token.GetFloat();
	token.FindToken("numchannels");

	//std::cout << "Loading Channels " << "\n\n";
	int channelnum = token.GetInt();
	for (int i = 0; i < channelnum; ++i) {
		token.FindToken("channel");
		Channel* newcha = new Channel();
		//std::cout << "Loading Channel: " << "\n\n";
		newcha->Load(token);
		channels.push_back(newcha);
	}

	return true;
}

void Animation::update(float t, std::vector<Joint*>& joints)
{
	//printf("Updating at time %f\n", t);

	if (joints.size() == 0) {
		return;
	}
	int i = 0;
	joints[0]->Offset.x = channels[i++]->eval(t);
	joints[0]->Offset.y = channels[i++]->eval(t);
	joints[0]->Offset.z = channels[i++]->eval(t);

	//printf("Offset Updated\n");

	for (Joint* j : joints) {

		//printf("Updating Joint: %s\n", j->name);
		j->dofs[0].value = channels[i++]->eval(t);
		j->dofs[1].value = channels[i++]->eval(t);
		j->dofs[2].value = channels[i++]->eval(t);
		//printf("Updated Joint: %s\n", j->name);
	}
}
