#include <bits/stdc++.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL.h>

using namespace std;
SDL_Window *window;
SDL_Renderer *renderer;
bool isRunning = true;
bool isPlaying = true;
bool modeset = true;
SDL_Event e;
int modenumber = 0;
int ballnumber = 0;
const double PI = 3.14159265358979323846;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const string WINDOW_TITLE = "Pong";
const int PADDLE_HEIGHT = 100;
const int PADDLE_WIDTH = 20;
const int BALL_SIZE = 22;
int BALL_SPEED = 12;
int SPEED = 15;
const int HP_WIDTH = 100;
const int HP_HEIGHT = 30;
int comboCount1 = 0;
int comboCount2 = 0;
SDL_Rect paddle1 = {20, SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT};
SDL_Rect paddle2 = {SCREEN_WIDTH - PADDLE_WIDTH - 20, SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT};
SDL_Rect ballRect = {SCREEN_WIDTH / 2 - BALL_SIZE / 2, SCREEN_HEIGHT / 2 - BALL_SIZE / 2, BALL_SIZE, BALL_SIZE};
SDL_Rect comboCountRect1 = {SCREEN_WIDTH / 4 - 115 / 2 - 30, 35, 115, 30};
SDL_Rect comboCountRect2 = {SCREEN_WIDTH * 3 / 4 - 115 / 2 + 30, 35, 115, 30};
string path = ".\\src\\images\\ball.png";
Mix_Chunk *sound;
Mix_Chunk *over;
Mix_Chunk *win;
Mix_Chunk *ballhitwall;
Mix_Chunk *combo;
Mix_Music *music;
SDL_Texture *ball;
SDL_Texture *backgroundImage;
SDL_Texture *startBackgroundImage;
SDL_Texture *blankBackgroundImage;
SDL_Texture *comboCount1Image;
SDL_Texture *comboCount2Image;
string comboCount1Str;
string comboCount2Str;
SDL_Surface *icon = IMG_Load(".\\src\\images\\uetlogo.png");
bool turn = true;
bool startNew = true;
int score1 = 0;
int score2 = 0;
float velX = BALL_SPEED / 2, velY = 0;
int cpuPaddleSpeed = 10;

SDL_Texture *LoadTexture(std::string filePath, SDL_Renderer *renderTarget)
{
    SDL_Texture *texture = nullptr;
    SDL_Surface *surface = IMG_Load(filePath.c_str());
    if (surface == NULL)
        std::cout << "Could not load the image at: " << filePath << ": " << SDL_GetError() << std::endl;
    else
    {
        texture = SDL_CreateTextureFromSurface(renderTarget, surface);
        SDL_FreeSurface(surface);
    }

    return texture;
}

void displayText(SDL_Renderer *renderer, string textContent, int x, int y)
{
    TTF_Font *font = TTF_OpenFont("src/fonts/munro.ttf", 35);
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, textContent.c_str(), color);
    SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect;
    textRect.x = x;
    textRect.y = y;
    SDL_QueryTexture(text, NULL, NULL, &textRect.w, &textRect.h);
    SDL_RenderCopy(renderer, text, NULL, &textRect);
}

void displayColorText(SDL_Renderer *renderer, string textContent, int x, int y)
{
    TTF_Font *font = TTF_OpenFont("src/fonts/munro.ttf", 35);
    SDL_Color color = {71, 113, 158, 255};
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, textContent.c_str(), color);
    SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect;
    textRect.x = x;
    textRect.y = y;
    SDL_QueryTexture(text, NULL, NULL, &textRect.w, &textRect.h);
    SDL_RenderCopy(renderer, text, NULL, &textRect);
}

void chooseMode()
{
    blankBackgroundImage = LoadTexture(".\\src\\images\\playagainbg.png", renderer);
    ball = LoadTexture(path, renderer);
    SDL_RenderCopy(renderer, startBackgroundImage, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);
    bool start = true;
    while (start)
    {

        if (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                isRunning = false;
                isPlaying = false;
                start = false;
            }
            if (e.type == SDL_KEYDOWN)
            {

                if (e.key.keysym.sym == SDLK_SPACE)
                {
                    string mode[] = {"Easy", "Medium", "Hard"};
                    string ballNumber[] = {"1", "2"};

                    while (true)
                    {

                        SDL_RenderClear(renderer);
                        SDL_RenderCopy(renderer, blankBackgroundImage, NULL, NULL);
                        int rectwidth = 250;
                        int rectheight = 60;
                        SDL_Rect easyRect = {SCREEN_WIDTH / 2 - rectwidth / 2, SCREEN_HEIGHT / 2 - rectheight * 2 - 20, rectwidth, rectheight};
                        SDL_Rect normalRect = {SCREEN_WIDTH / 2 - rectwidth / 2, SCREEN_HEIGHT / 2 - rectheight / 2, rectwidth, rectheight};
                        SDL_Rect hardRect = {SCREEN_WIDTH / 2 - rectwidth / 2, SCREEN_HEIGHT / 2 + rectheight + 20, rectwidth, rectheight};

                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                        SDL_RenderDrawRect(renderer, &easyRect);
                        SDL_RenderDrawRect(renderer, &normalRect);
                        SDL_RenderDrawRect(renderer, &hardRect);
                        switch (modenumber)
                        {
                        case 0:
                            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                            SDL_RenderFillRect(renderer, &easyRect);
                            displayColorText(renderer, mode[0], SCREEN_WIDTH / 2 - 25, SCREEN_HEIGHT / 2 - rectheight * 2 - 10);
                            displayText(renderer, mode[1], SCREEN_WIDTH / 2 - 45, SCREEN_HEIGHT / 2 - rectheight / 2 + 10);
                            displayText(renderer, mode[2], SCREEN_WIDTH / 2 - 25, SCREEN_HEIGHT / 2 + rectheight + 30);
                            break;
                        case 1:
                            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                            SDL_RenderFillRect(renderer, &normalRect);
                            displayText(renderer, mode[0], SCREEN_WIDTH / 2 - 25, SCREEN_HEIGHT / 2 - rectheight * 2 - 10);
                            displayColorText(renderer, mode[1], SCREEN_WIDTH / 2 - 45, SCREEN_HEIGHT / 2 - rectheight / 2 + 10);
                            displayText(renderer, mode[2], SCREEN_WIDTH / 2 - 25, SCREEN_HEIGHT / 2 + rectheight + 30);
                            break;
                        case 2:
                            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                            SDL_RenderFillRect(renderer, &hardRect);
                            displayText(renderer, mode[0], SCREEN_WIDTH / 2 - 25, SCREEN_HEIGHT / 2 - rectheight * 2 - 10);
                            displayText(renderer, mode[1], SCREEN_WIDTH / 2 - 45, SCREEN_HEIGHT / 2 - rectheight / 2 + 10);
                            displayColorText(renderer, mode[2], SCREEN_WIDTH / 2 - 25, SCREEN_HEIGHT / 2 + rectheight + 30);
                            break;
                        default:
                            break;
                        }

                        SDL_RenderPresent(renderer);
                        if (SDL_PollEvent(&e) != 0)
                        {
                            if (e.type == SDL_QUIT)
                            {
                                isRunning = false;
                                isPlaying = false;
                                start = false;
                                break;
                            }
                            if (e.type == SDL_KEYDOWN)
                            {

                                if (e.key.keysym.sym == SDLK_DOWN)
                                {
                                    if (modenumber < 2)
                                        modenumber++;
                                }
                                else if (e.key.keysym.sym == SDLK_UP)
                                {
                                    if (modenumber > 0)
                                        modenumber--;
                                }
                                else if (e.key.keysym.sym == SDLK_SPACE)
                                {
                                    start = false;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void startGame()
{
    SDL_SetWindowIcon(window, icon);
    backgroundImage = LoadTexture(".\\src\\images\\pongbg3.png", renderer);
    startBackgroundImage = LoadTexture(".\\src\\images\\pongstartbg.png", renderer);
    chooseMode();
}

void displayScore(SDL_Renderer *ren, int a, int b)
{
    displayText(ren, to_string(a), 315, 30);
    displayText(ren, to_string(b), SCREEN_WIDTH - 330, 30);
}

void render(SDL_Renderer *renderer)
{
    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    // SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, backgroundImage, NULL, NULL);

    comboCount1Str = to_string((comboCount1 == 4 || comboCount1 == 8) ? 4 : comboCount1 % 4);
    comboCount2Str = to_string((comboCount2 == 4 || comboCount2 == 8) ? 4 : comboCount2 % 4);
    comboCount1Image = LoadTexture(".\\src\\images\\leftbar" + comboCount1Str + ".png", renderer);
    comboCount2Image = LoadTexture(".\\src\\images\\rightbar" + comboCount2Str + ".png", renderer);
    SDL_RenderCopy(renderer, comboCount1Image, NULL, &comboCountRect1);
    SDL_RenderCopy(renderer, comboCount2Image, NULL, &comboCountRect2);
    SDL_DestroyTexture(comboCount1Image);
    SDL_DestroyTexture(comboCount2Image);
    if (startNew)
    {
        paddle1.y = SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2;
        paddle2.y = SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2;
        ballRect.x = SCREEN_WIDTH / 2 - BALL_SIZE / 2;
        ballRect.y = SCREEN_HEIGHT / 2 - BALL_SIZE / 2;

        if (turn)
        {
            ballRect.x = paddle1.x + (paddle1.w * 4);
            velX = BALL_SPEED;
        }
        else
        {
            ballRect.x = paddle2.x - (paddle2.w * 4);
            velX = -BALL_SPEED;
        }
        velY = 0;
        ballRect.y = SCREEN_HEIGHT / 2 - (BALL_SIZE / 2);
        turn = !turn;
    }

    if (comboCount1 % 4 == 0 && comboCount1 != 0 && score1 < 10)
    {
        //     Mix_PlayChannel( -1, combo, 0 );
        SDL_SetRenderDrawColor(renderer, 244, 158, 57, 255);
        SDL_RenderFillRect(renderer, &paddle1);
        paddle1.h = PADDLE_HEIGHT * 2.5;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &paddle2);
    }
    else if (comboCount2 % 4 == 0 && comboCount2 != 0 && score2 < 10)
    {
        // Mix_PlayChannel( -1, combo, 0 );
        SDL_SetRenderDrawColor(renderer, 230, 158, 57, 255);
        SDL_RenderFillRect(renderer, &paddle2);
        paddle2.h = PADDLE_HEIGHT * 2.5;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &paddle1);
    }

    else
    {
        paddle1.h = paddle2.h = PADDLE_HEIGHT;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &paddle1);
        SDL_RenderFillRect(renderer, &paddle2);
    }

    SDL_RenderCopy(renderer, ball, nullptr, &ballRect);
    displayScore(renderer, score1, score2);
    SDL_RenderPresent(renderer);
}

void movePlayerPaddle(SDL_Rect &paddle, SDL_Event e)
{
    if (e.type == SDL_KEYDOWN)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_UP:
            paddle.y -= SPEED;
            if (paddle.y <= 0)
            {
                paddle.y += SPEED;
            }
            break;

        case SDLK_DOWN:
            paddle.y += SPEED;
            if (paddle.y > SCREEN_HEIGHT - paddle.h)
            {
                paddle.y -= SPEED;
            }
            break;
        }
    }

    // Computer plays
    // cpuPaddleSpeed=12;
    // srand(time(NULL));
    // int delay = rand() % 30 + 10;
    // if(paddle2.y<ballRect.y-delay && ballRect.x>2*SCREEN_WIDTH/3){
    //     paddle2.y+=cpuPaddleSpeed;
    //     if(paddle2.y>SCREEN_HEIGHT-PADDLE_HEIGHT){
    //         paddle2.y-=cpuPaddleSpeed;
    //     }
    // }
    // else if(paddle2.y>ballRect.y+delay && ballRect.x>2*SCREEN_WIDTH/3){
    //     paddle2.y-=cpuPaddleSpeed;
    //     if(paddle2.y<0){
    //         paddle2.y+=cpuPaddleSpeed;
    //     }
    // }
}

void moveComputerPaddle(SDL_Rect &paddle1, SDL_Rect &paddle2)
{
    if (modenumber == 0)
    {
        cpuPaddleSpeed = 8;
        BALL_SPEED = 12;
    }
    if (modenumber == 1)
    {
        cpuPaddleSpeed = 10;
        BALL_SPEED = 14;
    }
    if (modenumber == 2)
    {
        cpuPaddleSpeed = 20;
        BALL_SPEED = 18;
        SPEED = 17;
    }
    srand(time(NULL));
    int delay = rand() % 30 + 10;
    if (paddle1.y < ballRect.y - delay && ballRect.x < SCREEN_WIDTH / 3)
    {
        paddle1.y += cpuPaddleSpeed;
        if (paddle1.y > SCREEN_HEIGHT - paddle1.h)
        {
            paddle1.y -= cpuPaddleSpeed;
        }
    }
    else if (paddle1.y > ballRect.y + delay && ballRect.x < SCREEN_WIDTH / 3)
    {
        paddle1.y -= cpuPaddleSpeed;
        if (paddle1.y < 0)
        {
            paddle1.y += cpuPaddleSpeed;
        }
    }
}

void update()
{
    if (SDL_HasIntersection(&ballRect, &paddle1))
    {
        Mix_PlayChannel(-1, sound, 0);
        double r = (paddle1.y + (paddle1.h / 2)) - (ballRect.y + (BALL_SIZE / 2));
        double n = r / (paddle1.h / 2);
        double angle = n * (5 * PI / 12);
        velX = BALL_SPEED * cos(angle);
        velY = BALL_SPEED * -sin(angle);
        cpuPaddleSpeed += 1;
    }
    if (SDL_HasIntersection(&ballRect, &paddle2))
    {
        Mix_PlayChannel(-1, sound, 0);
        double r = (paddle2.y + (paddle2.h / 2)) - (ballRect.y + (BALL_SIZE / 2));
        double n = r / (paddle2.h / 2);
        double angle = n * (5 * PI / 12);
        velX = -BALL_SPEED * cos(angle);
        velY = BALL_SPEED * -sin(angle);
    }
    if (ballRect.y < 0 || ballRect.y + BALL_SIZE > SCREEN_HEIGHT)
    {
        Mix_PlayChannel(-1, ballhitwall, 0);
        velY = -velY;
    }
    ballRect.x += velX;
    ballRect.y += velY;
    if (paddle1.y < 0)
        paddle1.y = 0;
    if (paddle1.y + paddle1.h > SCREEN_HEIGHT)
        paddle1.y = SCREEN_HEIGHT - paddle1.h;
    if (paddle2.y < 0)
        paddle2.y = 0;
    if (paddle2.y + paddle2.h > SCREEN_HEIGHT)
        paddle2.y = SCREEN_HEIGHT - paddle2.h;
}

void endTurn()
{
    if (ballRect.x < SCREEN_WIDTH || ballRect.x > 0)
    {
        startNew = false;
    }
    if (ballRect.x >= SCREEN_WIDTH + 20)
    {
        score1 += 1;
        comboCount1 += 1;
        comboCount2 = 0;
        startNew = true;
        SDL_Delay(500);
    }
    if (ballRect.x <= -BALL_SIZE - 20)
    {
        score2 += 1;
        comboCount2 += 1;
        comboCount1 = 0;
        startNew = true;
        SDL_Delay(500);
    }
}

void endGame(SDL_Renderer *ren, bool &isRunning)
{

    if (score1 == 10)
    {
        Mix_FadeOutMusic(200);
        Mix_PlayChannel(-1, over, 0);
        displayText(ren, "WINNER!!!", 50, 30);
        SDL_RenderPresent(ren);
        SDL_Delay(2500);
        isRunning = false;
    }
    if (score2 == 10)
    {
        Mix_FadeOutMusic(200);
        Mix_PlayChannel(-1, win, 0);
        displayText(ren, "WINNER!!!", SCREEN_WIDTH - 150, 30);
        SDL_RenderPresent(ren);
        SDL_Delay(2500);
        isRunning = false;
    }
}

void playAgain(bool &isPlaying, SDL_Renderer *ren)
{
    if (isRunning == false && isPlaying == true)
    {
        SDL_RenderClear(ren);

        SDL_RenderCopy(ren, blankBackgroundImage, NULL, NULL);
        displayText(ren, "Press Space to play again", SCREEN_WIDTH / 2 - 170, SCREEN_HEIGHT / 2 - 20);

        SDL_RenderPresent(ren);
        Uint32 returnEvent = 0;
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                isPlaying = false;
            }
            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_SPACE)
                {
                    Mix_PlayMusic(music, 3);
                    modenumber = 0;
                    chooseMode();
                    isRunning = true;
                    score1 = 0;
                    score2 = 0;
                }
            }
        }
    }
}

void logSDLError(std::ostream &os,
                 const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal)
    {
        SDL_Quit();
        exit(1);
    }
}

void initSDL(SDL_Window *&window, SDL_Renderer *&renderer)
{
    TTF_Init();
    // Mix_Init(MIX_INIT_OGG);
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);

    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    // window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
    //    SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == nullptr)
        logSDLError(std::cout, "CreateWindow", true);

    // Khi thông thường chạy với môi trường bình thường ở nhà
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    // Khi chạy ở máy thực hành WinXP ở trường (máy ảo)
    // renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));
    if (renderer == nullptr)
        logSDLError(std::cout, "CreateRenderer", true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0)
    {
        cout << "Error initializing SDL_mixer: " << Mix_GetError() << endl;
    }
    music = Mix_LoadMUS(".\\src\\sounds\\bgmusic.wav");
    sound = Mix_LoadWAV("E:\\Documents\\Workspace\\SDL\\Pong\\src\\sounds\\ballhit.wav");
    over = Mix_LoadWAV("E:\\Documents\\Workspace\\SDL\\Pong\\src\\sounds\\over.wav");
    win = Mix_LoadWAV("E:\\Documents\\Workspace\\SDL\\Pong\\src\\sounds\\win.wav");
    ballhitwall = Mix_LoadWAV("E:\\Documents\\Workspace\\SDL\\Pong\\src\\sounds\\ballhitwall.wav");
    combo = Mix_LoadWAV(".\\src\\sounds\\combo.wav");
    blankBackgroundImage = LoadTexture(".\\src\\images\\playagainbg.png", renderer);
    Mix_PlayMusic(music, 3);
}

void quitSDL(SDL_Window *window, SDL_Renderer *renderer)
{
    SDL_FreeSurface(icon);
    SDL_DestroyTexture(ball);
    SDL_DestroyTexture(backgroundImage);
    SDL_DestroyTexture(startBackgroundImage);
    SDL_DestroyTexture(blankBackgroundImage);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
