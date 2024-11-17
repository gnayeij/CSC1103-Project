#define MINIAUDIO_IMPLEMENTATION
#include "include/miniaudio.h"
#include "include/backgroundmusic.h"
#include <stdio.h>
#include <stdlib.h>

// Global audio engine and sound
ma_engine engine;
ma_sound background_music;

// Initialize audio system
int init_BGM_audio(const char* music_file) {
    ma_result result;
    
    // Initialize engine
    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize audio engine\n");
        return -1;
    }
    
    // Load and initialize the background music
    result = ma_sound_init_from_file(&engine, music_file, 0, NULL, NULL, &background_music);
    if (result != MA_SUCCESS) {
        printf("Failed to load background music: %s\n", music_file);
        ma_engine_uninit(&engine);
        return -1;
    }
    
    // Set the sound to loop
    ma_sound_set_looping(&background_music, MA_TRUE);
    
    return 0;
}

// Control functions for background music
void start_background_music() {
    ma_sound_start(&background_music);
}

void stop_background_music() {
    ma_sound_stop(&background_music);
}

void set_music_volume(float volume) {
    // Volume should be between 0.0 and 1.0
    ma_sound_set_volume(&background_music, volume);
}

// Cleanup function
void cleanup_audio() {
    ma_sound_uninit(&background_music);
    ma_engine_uninit(&engine);
}

// sound effects
int load_sound_effect(const char* filename, SoundEffect* sound_effect) {
    sound_effect->filename = filename;
    sound_effect->sound = malloc(sizeof(ma_sound));
    if (sound_effect->sound == NULL) {
        return -1;
    }
    
    ma_result result = ma_sound_init_from_file(&engine, filename, 0, NULL, NULL, (ma_sound*)sound_effect->sound);
    if (result != MA_SUCCESS) {
        free(sound_effect->sound);
        sound_effect->sound = NULL;
        printf("Failed to load sound effect: %s\n", filename);
        return -1;
    }
    
    ma_sound_set_looping((ma_sound*)sound_effect->sound, MA_FALSE);
    return 0;
}

void play_sound_effect(SoundEffect* sound_effect) {
    if (sound_effect->sound != NULL) {
        ma_sound_seek_to_pcm_frame((ma_sound*)sound_effect->sound, 0);
        ma_sound_start((ma_sound*)sound_effect->sound);
    }
}

void unload_sound_effect(SoundEffect* sound_effect) {
    if (sound_effect->sound != NULL) {
        ma_sound_uninit((ma_sound*)sound_effect->sound);
        free(sound_effect->sound);
        sound_effect->sound = NULL;
    }
}