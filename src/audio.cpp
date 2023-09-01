#include "audio.h"
#include <iostream>

bool Audio::Init() {
	return BASS_Init(-1, 44100, 0, 0, NULL);
}

HCHANNEL Audio::play(const char* filename, float volume, bool loop) {
	HCHANNEL hSampleChannel; // Handler to store one channel

	// Load sample from disk
	// flags: BASS_SAMPLE_LOOP, BASS_SAMPLE_3D, ...
	if (loop) {
		sample = BASS_SampleLoad(false, filename, 0, 0, 3, BASS_SAMPLE_LOOP);
	}
	else {
		sample = BASS_SampleLoad(false, filename, 0, 0, 3, 0);
	}
	
	if (sample == 0) {
		std::cout << "audio sample error" << std::endl;
	}

	// Store sample channel in handler
	channel = BASS_SampleGetChannel(sample, false);
	BASS_ChannelPlay(channel, true);
	return channel;

}

bool Audio::Stop(HCHANNEL HChannel) {
	return BASS_ChannelStop(HChannel);
}
