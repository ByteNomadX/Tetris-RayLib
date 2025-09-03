#ifndef AUDIO_H
#define AUDIO_H

#include "raylib.h"

typedef struct AudioManager {
	Music music;
	bool musicPaused;

	Sound figurePlaced;
	Sound figureRotate;
	Sound figureRotateFail;
	Sound lineClear;
	Sound gameOver;

} AudioManager;

typedef struct RepeatingSound {
	int countTarget;
	int count;
	float timer;
	float delay;

	float pitchStep; // if its zero, pitch will not change;
	float currPitch;
	float startPitch;

	Sound sound;

	bool playing;
} RepeatingSound;

void playSound(Sound sound);
void toggleSounds();
void updateRepeatingSound(RepeatingSound* sound);
void triggerRepeatingSound(RepeatingSound* rs, int count);

RepeatingSound* createRepeatingSound(Sound sound, float delay, float pitchStep, float startPitch);
AudioManager* createAudioManager(double musicVolume);

#endif