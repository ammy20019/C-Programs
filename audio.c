#include <SDL2/SDL.h>
#include "audio.h"

void *bg_music(void *arg){
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return NULL;
    }
    SDL_AudioSpec wavSpec_bg;
    Uint32 wavLength_bg;
    Uint8 *wavBuffer_bg;
    if (SDL_LoadWAV("bg_music_55perc_less.wav", &wavSpec_bg, &wavBuffer_bg, &wavLength_bg) == NULL) {
        printf("Failed to load WAV file! SDL Error: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec_bg, NULL, 0);
    SDL_QueueAudio(deviceId, wavBuffer_bg, wavLength_bg);
    SDL_PauseAudioDevice(deviceId, 0);

    SDL_Delay(70000);
    //SDL_Delay(120000);

    SDL_CloseAudioDevice(deviceId);
    SDL_FreeWAV(wavBuffer_bg);
    //SDL_Quit();

    return NULL;
}

void *mia_intro(void *args) {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return NULL;
    }
    SDL_AudioSpec wavSpec_mia;
    Uint32 wavLength_mia;
    Uint8 *wavBuffer_mia;
    if (SDL_LoadWAV("intro_mia.wav", &wavSpec_mia, &wavBuffer_mia, &wavLength_mia) == NULL) {
        printf("Failed to load WAV file! SDL Error: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec_mia, NULL, 0);
    SDL_QueueAudio(deviceId, wavBuffer_mia, wavLength_mia);
    SDL_PauseAudioDevice(deviceId, 0);

    SDL_Delay(48000);

    SDL_CloseAudioDevice(deviceId);
    SDL_FreeWAV(wavBuffer_mia);
    //SDL_Quit();

    return NULL;
}

void *lena_intro(void *args) {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return NULL;
    }
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;
    if (SDL_LoadWAV("lena_story.wav", &wavSpec, &wavBuffer, &wavLength) == NULL) {
        printf("Failed to load WAV file! SDL Error: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
    SDL_QueueAudio(deviceId, wavBuffer, wavLength);
    SDL_PauseAudioDevice(deviceId, 0);

    SDL_Delay(25500);
    SDL_PauseAudioDevice(deviceId, 1); // Pause audio
    SDL_Delay(3000); // Wait for 3 seconds
    SDL_PauseAudioDevice(deviceId, 0);
    SDL_Delay(14000);

    SDL_CloseAudioDevice(deviceId);
    SDL_FreeWAV(wavBuffer);
    SDL_Quit();

    return NULL;
}

void *lena_vineyard(void *args) {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return NULL;
    }
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;
    if (SDL_LoadWAV("lena_vineyard.wav", &wavSpec, &wavBuffer, &wavLength) == NULL) {
        printf("Failed to load WAV file! SDL Error: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
    SDL_QueueAudio(deviceId, wavBuffer, wavLength);
    SDL_PauseAudioDevice(deviceId, 0);

    SDL_Delay(8200);
    SDL_PauseAudioDevice(deviceId, 1); // Pause audio
    SDL_Delay(1500); // Wait for 3 seconds
    SDL_PauseAudioDevice(deviceId, 0);
    SDL_Delay(2000);

    SDL_CloseAudioDevice(deviceId);
    SDL_FreeWAV(wavBuffer);
    SDL_Quit();

    return NULL;
}

//gcc audio_in_console.c -o sound_check -I/opt/homebrew/Cellar/sdl2/2.32.4/include -L/opt/homebrew/Cellar/sdl2/2.32.4/lib -lSDL2 && ./sound_check

//with pkgs here in local: gcc audio_in_console.c -o sound_check -ISDL_lib/include -LSDL_lib/lib -lSDL2 && ./sound_check 

//gcc audio.c monopoly.c character.c board_layout.c sea_shore.c -o monopoly -ISDL_lib/include -LSDL_lib/lib -lSDL2 && ./monopoly          

