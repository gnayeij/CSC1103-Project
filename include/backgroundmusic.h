// backgroundmusic.h
#ifndef BACKGROUND_MUSIC_H
#define BACKGROUND_MUSIC_H

// Structure to hold sound effect
typedef struct SoundEffect{
    const char* filename;
    void* sound;
} SoundEffect;

// Function declarations
int init_BGM_audio(const char* music_file);
void start_background_music();
void stop_background_music();
void set_music_volume(float volume);
void cleanup_audio();

// New sound effect functions
int load_sound_effect(const char* filename, SoundEffect* sound_effect);
void play_sound_effect(SoundEffect* sound_effect);
void unload_sound_effect(SoundEffect* sound_effect);

#endif