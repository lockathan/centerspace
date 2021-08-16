#ifndef SoundTemplate_h
#define SoundTemplate_h

#include <Templates/Template.h>

class SoundTemplate : public Template
{
public:
	SoundTemplate();
	~SoundTemplate();
	
	std::string& getFileName();
	void setFileName(const std::string& fileName);

	bool loop();
	void setLoop(bool loop);

private:
	std::string mFileName;
	bool mLoop;
};


#endif