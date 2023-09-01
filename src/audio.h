#include "extra/bass.h"

class Audio {
public:
	
	// Handler for BASS_SampleLoad
	HSAMPLE sample;
	HCHANNEL channel;

	Audio() { sample = 0; };	// Sample = 0 here
	~Audio() { BASS_SampleFree(sample); }; 	// Free sample (BASS_SampleFree)

	// Play audio and return the channel
	HCHANNEL play(const char* filename, float volume, bool loop);

	// Manager stufF

	// Initialize BASS
	static bool Init();
	// Stop sounds
	static bool Stop(HCHANNEL channel);
	

};
