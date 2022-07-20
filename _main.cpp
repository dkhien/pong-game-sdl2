#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <bits/stdc++.h>
#include "src/headers/SDL_functions.h"

using namespace std;

int main(int argc, char *argv[])
{
    
    initSDL(window, renderer);
    Uint32 returnEvent = 0;
    startGame();
    while(isPlaying){
        while (isRunning)
        {
            Uint32 starts = SDL_GetTicks();
            render(renderer);
            moveComputerPaddle(paddle1, paddle2);
            endGame(renderer, isRunning);
            while (SDL_PollEvent(&e) != 0)
            {
                if (e.type == SDL_QUIT)
                {
                    isRunning = false;
                    isPlaying = false;
                }
                if (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN)
                    returnEvent = e.type;
            }

            if (returnEvent != 0)
            {
                movePlayerPaddle(paddle2, e);
                update();
            }
            endTurn();
            
            Uint32 end = SDL_GetTicks() - starts;
            if (frameDelay > end)
            {
                SDL_Delay(frameDelay - end);
            }
        }
        playAgain(isPlaying, renderer);
    }
    quitSDL(window, renderer);
    return 0;
}
