#include <bits/stdc++.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL.h>

using namespace std;
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern const int SCREEN_WIDTH = 800;
extern const int SCREEN_HEIGHT = 600;
extern int modenumber;
extern SDL_Event e;
extern SDL_Rect paddle1;
extern SDL_Rect paddle2;
extern SDL_Rect ballRect;
extern string path;
extern Mix_Music *music;
extern Mix_Chunk *sound;
extern Mix_Chunk *over;
extern Mix_Chunk *win;
extern Mix_Chunk *ballhitwall;
extern Mix_Chunk *combo;
extern SDL_Texture *ball;
extern SDL_Texture *backgroundImage;
extern SDL_Texture *startBackgroundImage;
extern SDL_Surface *icon;
extern SDL_Texture *blankBackgroundImage;
const Uint32 FPS = 120;
const Uint32 frameDelay = 1000 / FPS;
extern bool isRunning;
extern bool isPlaying;

void displayText(SDL_Renderer *renderer, string textContent, int x, int y);
void render(SDL_Renderer *renderer);
void startGame();
void chooseMode();
void displayScore(SDL_Renderer *ren, int a, int b);
void endTurn();
void endGame(SDL_Renderer *ren, bool &isRunning);
void update();
void playAgain(bool &isPlaying, SDL_Renderer *ren);
void movePlayerPaddle(SDL_Rect &paddle, SDL_Event e);
void moveComputerPaddle(SDL_Rect &paddle1, SDL_Rect &paddle2);
SDL_Texture *LoadTexture(std::string filePath, SDL_Renderer *renderTarget);
void logSDLError(std::ostream &os,
                 const std::string &msg, bool fatal = false);
void initSDL(SDL_Window *&window, SDL_Renderer *&renderer);
void quitSDL(SDL_Window *window, SDL_Renderer *renderer);

// const double PI = 3.14159265358979323846;
// const int SCREEN_WIDTH = 800;
// const int SCREEN_HEIGHT = 600;
// const string WINDOW_TITLE = "Pong";
// extern const int PADDLE_HEIGHT;
// extern const int PADDLE_WIDTH;
// extern const int BALL_SIZE;
// extern const int BALL_SPEED;
// extern bool turn, startNew;
// extern int score1;
// extern int score2;