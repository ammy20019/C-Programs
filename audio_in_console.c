#include <SDL2/SDL.h>
#include <stdio.h>

#define MAX_VOLUME 128  // Max volume for SDL

int main() {
    // Initialize SDL's audio subsystem
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;  // Exit if initialization fails
    }

    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;

    // Load the WAV file
    if (SDL_LoadWAV("bg_music_55perc_less.wav", &wavSpec, &wavBuffer, &wavLength) == NULL) {
        printf("Failed to load WAV file! SDL Error: %s\n", SDL_GetError());
        SDL_Quit();  // Quit SDL if loading WAV fails
        return 1;
    }

    // Open the audio device
    SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
    if (deviceId == 0) {
        printf("Failed to open audio device: %s\n", SDL_GetError());
        SDL_FreeWAV(wavBuffer);
        SDL_Quit();  // Quit SDL if device opening fails
        return 1;
    }

    // Play audio
    SDL_QueueAudio(deviceId, wavBuffer, wavLength);
    SDL_PauseAudioDevice(deviceId, 0);  // Unpause to start playing audio

    // Wait for the audio to finish playing
    SDL_Delay(5000);  // Wait for the audio length

    // Cleanup
    SDL_CloseAudioDevice(deviceId);
    SDL_FreeWAV(wavBuffer);
    SDL_Quit();  // Quit SDL

    return 0;
}
