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
