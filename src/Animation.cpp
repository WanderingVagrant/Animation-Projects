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
	if (joints.size() == 0) {
		return;
	}
	int i = 0;
	joints[0]->Offset.x = channels[i++]->eval(t);
	joints[1]->Offset.y = channels[i++]->eval(t);
	joints[2]->Offset.z = channels[i++]->eval(t);
	for (Joint* j : joints) {
		j->dofs[0].value = channels[i++]->eval(t);
		j->dofs[1].value = channels[i++]->eval(t);
		j->dofs[3].value = channels[i++]->eval(t);
	}
}
