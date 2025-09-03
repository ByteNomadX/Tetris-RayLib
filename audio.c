#include "./headers/audio.h"
#include "raylib.h"

static bool soundsPaused = false;

void playSound(Sound sound) {
	if(!soundsPaused) {
		PlaySound(sound);
	}
}

void toggleSounds() {
	soundsPaused = !soundsPaused;
}

void updateRepeatingSound(RepeatingSound* rs) {
	if (!(rs->playing)) return;

	rs->timer += GetFrameTime();
	if (rs->timer >= rs->delay) {
		playSound(rs->sound);
		rs->count++;

		rs->currPitch += rs->pitchStep;
		SetSoundPitch(rs->sound, rs->currPitch);
		rs->timer = 0.0f;
	}

	// end of repeats
	if(rs->count >= rs->countTarget) {
		rs->count = 0;
		rs->countTarget = 0;
		rs->currPitch = rs->startPitch;
		rs->playing = false;
		rs->timer = 0.0f;
	}
}

AudioManager* createAudioManager(double musicVolume) {
	AudioManager* am = MemAlloc(sizeof(AudioManager));

	InitAudioDevice();
  am->music = LoadMusicStream("./audio/Tetris.mp3");
  am->music.looping = true;

  SetMusicVolume(am->music, musicVolume);

	am->figurePlaced = LoadSound("./audio/figure_placed.wav");
  am->figureRotateFail = LoadSound("./audio/rotate_fail.wav");
  am->figureRotate = LoadSound("./audio/figure_rotate.wav");
	am->lineClear = LoadSound("./audio/line_clear.wav");
  am->gameOver = LoadSound("./audio/game_over.wav");

	am->musicPaused = true;

	return am;
}

RepeatingSound* createRepeatingSound(Sound sound, float delay, float pitchStep, float startPitch) {
	RepeatingSound* rs = MemAlloc(sizeof(RepeatingSound));

	rs->delay = delay;
	rs->sound = sound;
	
	rs->pitchStep = pitchStep;
	rs->startPitch = startPitch;
	rs->currPitch = startPitch;

	rs->count = 0;
	rs->countTarget = 0;
	
	rs->timer = 0.0f;
	
	rs->playing = false;

	return rs;
}

void triggerRepeatingSound(RepeatingSound* rs, int count) {
	rs->playing = true;
	rs->countTarget = count;
}