#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#define SWITCH_TIME 10000  // 10 seconds

int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *background = NULL;
    SDL_Event event;

    int running = 1;
    Uint32 lastSwitch = 0;
    int currentSound = 0;

    Mix_Music *music1 = NULL;
    Mix_Music *music2 = NULL;

    // Init SDL (video + audio)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }

    // Init SDL_image
    if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))) {
        printf("IMG_Init error: %s\n", IMG_GetError());
        return 1;
    }

    // Create window
    window = SDL_CreateWindow(
        "SDL Audio + Background",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Load background image
    background = IMG_LoadTexture(renderer, "background.png");
    if (!background) {
        printf("Failed to load background: %s\n", IMG_GetError());
        return 1;
    }

    // Init SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Mix_OpenAudio error: %s\n", Mix_GetError());
        return 1;
    }

    // Load music
    music1 = Mix_LoadMUS("sound1.wav");
    music2 = Mix_LoadMUS("sound2.wav");

    if (!music1 || !music2) {
        printf("Mix_LoadMUS error: %s\n", Mix_GetError());
        return 1;
    }

    // Play first sound
    Mix_PlayMusic(music1, -1);
    lastSwitch = SDL_GetTicks();

    // Main loop
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = 0;
        }

        Uint32 now = SDL_GetTicks();

        // Switch sound every 10 seconds
        if (now - lastSwitch >= SWITCH_TIME) {
            Mix_HaltMusic();

            if (currentSound == 0) {
                Mix_PlayMusic(music2, -1);
                currentSound = 1;
            } else {
                Mix_PlayMusic(music1, -1);
                currentSound = 0;
            }

            lastSwitch = now;
        }

        // Draw background
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background, NULL, NULL);
        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    // Cleanup
    Mix_FreeMusic(music1);
    Mix_FreeMusic(music2);
    Mix_CloseAudio();

    SDL_DestroyTexture(background);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();

    return 0;
}


