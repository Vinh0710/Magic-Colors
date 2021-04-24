#include "fps.h"
using namespace std;

const int SCREEN_WIDTH = 1126;
const int SCREEN_HEIGHT = 562;
int TIME_PLAY = 500;
bool init();
void close();
bool load();
bool ktra(int a, int b, int c);
SDL_Texture* loadFromRenderedText(std::string textureText, SDL_Color textColor, SDL_Rect& rect);
SDL_Texture* loadTexture(char* path);
void render(int index, SDL_Rect& rect);
SDL_Rect rectFont = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 60, 20 };
SDL_Renderer* gRenderer = NULL;
SDL_Window* gWindow = NULL;
_TTF_Font* Font = NULL;
SDL_Texture* Character = NULL;
SDL_Texture* board = NULL; SDL_Texture* wood = NULL; SDL_Texture* button = NULL; SDL_Texture* Key = NULL;
SDL_Texture* background[10];
SDL_Texture* color[15];
SDL_Texture* text[3];
SDL_Texture* menu = NULL; SDL_Texture* menuLose = NULL;
Mix_Chunk* jump = NULL;
Mix_Chunk* MixScore = NULL;
SDL_Texture* anSwer = NULL;
SDL_Event gevent;
Mix_Music* Music = NULL;
SDL_Color black = { 0,0,0 }; SDL_Color white = { 255,255,255 };
SDL_Color red = { 255,0,0 }; SDL_Color orange = { 255,165,0 }; SDL_Color blue = { 0,255,255 };
bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled!");
        }

        //Create window
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            //Create vsynced renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }

                //Initialize SDL_mixer
                if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
                {
                    printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
                    success = false;
                }
                if (TTF_Init() == -1)
                {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}
void renderMap(int index, SDL_Rect& rect) {
    switch (index) {
    case  1:  SDL_RenderCopy(gRenderer, color[1], NULL, &rect); break;
    case  2:  SDL_RenderCopy(gRenderer, color[2], NULL, &rect); break;
    case  3:  SDL_RenderCopy(gRenderer, color[3], NULL, &rect); break;
    case  4:  SDL_RenderCopy(gRenderer, color[4], NULL, &rect); break;
    case  5:  SDL_RenderCopy(gRenderer, color[5], NULL, &rect); break;
    case  6:  SDL_RenderCopy(gRenderer, color[6], NULL, &rect); break;
    case  7:  SDL_RenderCopy(gRenderer, color[7], NULL, &rect); break;
    case -1:  SDL_RenderCopy(gRenderer, color[8], NULL, &rect); break;
    case -2:  SDL_RenderCopy(gRenderer, color[9], NULL, &rect); break;
    case -3: SDL_RenderCopy(gRenderer, color[10], NULL, &rect); break;
    case -4: SDL_RenderCopy(gRenderer, color[11], NULL, &rect); break;
    case -5: SDL_RenderCopy(gRenderer, color[12], NULL, &rect); break;
    case -6: SDL_RenderCopy(gRenderer, color[13], NULL, &rect); break;
    case -7: SDL_RenderCopy(gRenderer, color[14], NULL, &rect); break;
    default: break;
    }
}
SDL_Texture* loadTexture(char* path)
{
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (loadedSurface != NULL)
    {
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 255, 255));// made nền trong suất
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
        }
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}
void close()
{
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    TTF_CloseFont(Font);
    TTF_Quit;
    Mix_Quit;
    SDL_QUIT;
}
void coloseInitImg() {
    SDL_DestroyTexture(Character);
    Character = NULL;
    SDL_DestroyTexture(wood); wood = NULL;
    SDL_DestroyTexture(board); board = NULL;
    SDL_DestroyTexture(Key); Key = NULL;
    SDL_DestroyTexture(button); button = NULL;
    SDL_DestroyTexture(menu); menu = NULL;
    SDL_DestroyTexture(menuLose); menuLose = NULL;
    for (int i = 0; i < 15; i++) {
        if (color[i] != NULL) {
            SDL_DestroyTexture(color[i]);
            color[i] = NULL;
        }
        if (i < 10 && background[i] != NULL) {
            SDL_DestroyTexture(background[i]);
            if (i < 3 && text[i] != NULL) {
                SDL_DestroyTexture(text[i]);
                text[i] = NULL;
            }
        }
    }
}
bool load()
{
    bool success = true;
    char a[] = "character.png";
    char c[] = "menu1.png"; char c2[] = "menuLose.png";
    char d[] = "button.png";
    char e[] = "Key.png"; char t[] = "board.png"; char g[] = "wood.png";
    button = loadTexture(d); board = loadTexture(t); wood = loadTexture(g);
    menu = loadTexture(c); menuLose = loadTexture(c2);
    Character = loadTexture(a);
    Key = loadTexture(e);
    for (int i = 1; i < 8; i++) {
        char a[16];
        sprintf_s(a, "color%d.png", i);
        color[i] = loadTexture(a);
        sprintf_s(a, "-color%d.png", i);
        color[i + 7] = loadTexture(a);
    }
    for (int i = 0; i < 10; i++) {
        char b[16];
        sprintf_s(b, "backg0%d.png", i);
        background[i] = loadTexture(b);
    }
    if (Character == NULL)
    {
        printf("canot open images");
        success = false;
    }
    Music = Mix_LoadMUS("Theme.wav");
    jump = Mix_LoadWAV("high.wav");
    MixScore = Mix_LoadWAV("ting.wav");
    Font = TTF_OpenFont("ARCADE.ttf", 70);
    return success;
}
bool checkMapRight(int a[][18], int b, int c) {
    if (a[b][c + 1] == 0) return false;
    return true;
}
bool checkMapLeft(int a[][18], int b, int c) {
    if (a[b][c - 1] == 0) return false;
    return true;
}
bool checkMapUp(int a[][18], int b, int c) {
    if (a[b - 1][c] == 0) return false;
    return true;
}
bool checkMapDown(int a[][18], int b, int c) {
    if (a[b + 1][c] == 0) return false;
    return true;
}
void swap(int& a, int& b) {
    int c = a;
    a = b;
    b = c;
}
bool ktra(int a, int b, int c) {
    if (a != b || a != c || b != c || a == 0 || b == 0 || c == 0) return false;
    return true;
}
void MapRight(int a[][18], int b[][18]) {
    for (int i = 2; i < 10; i++) {
        for (int j = 2; j < 16; j++) {
            if (ktra(a[i][j], a[i][j + 1], a[i][j + 2])) {
                b[i][j] = 1, b[i][j + 1] = 1, b[i][j + 2] = 1;
            }
            if (ktra(a[i][j], a[i + 1][j], a[i + 2][j])) {
                b[i][j] = 1, b[i + 1][j] = 1, b[i + 2][j] = 1;
            }
        }
    }
}
bool disappear(int a[][18], int b[][18], int& c) {
    bool d = false;
    for (int i = 2; i < 10; i++) {
        for (int j = 0; j < 16; j++) {
            if (b[i][j] == 1) {
                a[i][j] = 0; c++; d = true;
            }
        }
    }
    return d;
}
SDL_Texture* loadFromRenderedText(std::string textureText, SDL_Color textColor, SDL_Rect& rect)
{
    SDL_Surface* textSurface = TTF_RenderText_Solid(Font, textureText.c_str(), textColor);
    SDL_Texture* mTexture = NULL;
    if (textSurface == NULL)
    {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }
    else
    {
        mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        if (mTexture == NULL)
        {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }
        rect.w = textSurface->w;
        rect.h = textSurface->h;
        SDL_FreeSurface(textSurface);
    }
    return mTexture;
}
bool checkMinusN(int a[][18]) {
    for (int i = 2; i < 10; i++) {
        for (int j = 2; j < 16; j++) {
            if (a[i][j] < 0) return false;
        }
    }
    return true;
}
void swapColor(int a[][18], int row, int colum) {
    for (int i = 2; i < 10; i++) {
        for (int j = 2; j < 16; j++) {
            if (a[i][j] < 0) {
                if (row - i == 1 && colum == j) { if (a[i + 2][j] != 0) { a[i][j] *= -1; swap(a[i][j], a[i + 2][j]); } }
                else if (j - colum == 1 && row == i) { if (a[i][j - 2] != 0) { a[i][j] *= -1; swap(a[i][j], a[i][j - 2]); } }
                else if (i - row == 1 && j == colum) { if (a[i - 2][j] != 0) { a[i][j] *= -1; swap(a[i][j], a[i - 2][j]); } }
                else if (colum - j == 1 && i == row) { if (a[i][j + 2] != 0) { a[i][j] *= -1; swap(a[i][j], a[i][j + 2]); } }
                else if (colum - j == 1 && row - i == 1) { if (a[i + 2][j + 2] != 0) { a[i][j] *= -1; swap(a[i][j], a[i + 2][j + 2]); } }
                else if (colum - j == 1 && i - row == 1) { if (a[i - 2][j + 2] != 0) { a[i][j] *= -1; swap(a[i][j], a[i - 2][j + 2]); } }
                else if (j - colum == 1 && i - row == 1) { if (a[i - 2][j - 2] != 0) { a[i][j] *= -1; swap(a[i][j], a[i - 2][j - 2]); } }
                else if (j - colum == 1 && row - i == 1) { if (a[i + 2][j - 2] != 0) { a[i][j] *= -1; swap(a[i][j], a[i + 2][j - 2]); } }
            }
        }
    }
}
bool Menu(SDL_Event e, SDL_Rect menuText[], int& timeDelay, bool Bwin, bool Blose) {
    if (Bwin) return true;
    else {
        bool b = false;
        SDL_Rect constMenu[3];
        text[0] = loadFromRenderedText("Play", white, menuText[0]);
        text[1] = loadFromRenderedText("Demo", white, menuText[1]);
        text[2] = loadFromRenderedText("Exit", white, menuText[2]);
        menuText[0].x = 50; menuText[0].y = 300;
        menuText[1].x = 50; menuText[1].y = menuText[0].y + menuText[0].h;
        menuText[2].x = 50; menuText[2].y = menuText[1].y + menuText[1].h;
        constMenu[0] = menuText[0]; constMenu[1] = menuText[1]; constMenu[2] = menuText[2];
        while (!b) {

            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    b = true;
                    return false;
                }
                text[0] = loadFromRenderedText("Play", white, menuText[0]);
                text[1] = loadFromRenderedText("Demo", white, menuText[1]);
                text[2] = loadFromRenderedText("Exit", white, menuText[2]);
                if (e.type == SDL_MOUSEMOTION) {
                    if (e.button.x > menuText[0].x && e.button.x < menuText[0].x + menuText[0].w && e.button.y > menuText[0].y && e.button.y < menuText[0].y + menuText[0].h) {
                        text[0] = loadFromRenderedText("Play", red, menuText[0]);

                    }
                    if (e.button.x > menuText[1].x && e.button.x < menuText[1].x + menuText[1].w && e.button.y > menuText[1].y && e.button.y < menuText[1].y + menuText[1].h) {
                        text[1] = loadFromRenderedText("Demo", red, menuText[1]);
                    }
                    if (e.button.x > menuText[2].x && e.button.x < menuText[2].x + menuText[2].w && e.button.y > menuText[2].y && e.button.y < menuText[2].y + menuText[2].h) {
                        text[2] = loadFromRenderedText("Exit", red, menuText[2]);
                    }
                }
                else   if (e.type = SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                    if (e.button.x > menuText[0].x && e.button.x < menuText[0].x + menuText[0].w && e.button.y > menuText[0].y && e.button.y < menuText[0].y + menuText[0].h)  return true;
                    else if (e.button.x > menuText[2].x && e.button.x < menuText[2].x + menuText[2].w && e.button.y > menuText[2].y && e.button.y < menuText[2].y + menuText[2].h) return false;
                }
            }
            SDL_RenderClear(gRenderer);
            SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
            if (Blose) SDL_RenderCopy(gRenderer, menuLose, NULL, NULL);
            else  SDL_RenderCopy(gRenderer, menu, NULL, NULL);
            for (int i = 0; i < 3; i++) {
                SDL_RenderCopy(gRenderer, text[i], NULL, &constMenu[i]);
            }
            SDL_RenderPresent(gRenderer);
            timeDelay = SDL_GetTicks() / 1000;
        }
    }
    return false;
}
int main(int argc, char* argv[]) {

    bool Bwin = false, Blose = false, boolGame = false;

    int Level = 0, Score = 0, guessNumber = 5, keepTime = 0;

    string result[10] = { "superman","messi","iron man","thanos","ly tieu long","thanh long","ton ngo khong","mr.bean","kong","5439" };
    string congraWin[10] = { "Grate!!","Perfect!!","Magic!","Intelligent!","Unbelievable!","Incredible!","Unstoppable!","Wonderful!","Alian?","VICTORY!!" };
   
    do {

        bool condition = false, cap = true;
        char txt[10];
        sprintf_s(txt, "map%d.txt", Level);
        ifstream map(txt);
        if (!map) printf("can't open file map");
        int bitmap[12][18];
        for (int i = 0; i < 12; i++) {
            for (int j = 0; j < 18; j++) {
                map >> bitmap[i][j];
            }
        }
        bool quit = false, renderText = false;

        fps Fps;

        SDL_Texture* Time = NULL;
        SDL_Texture* tScore = NULL;
        SDL_Texture* Suggest = NULL;
        SDL_Texture* tLevel = NULL;

        SDL_Rect rectMap = { 0,50,64,64 };

        SDL_Rect rCharacter = { 0,10,75,100 };
        SDL_Rect rRightC[4]; SDL_Rect rLeftC[4]; SDL_Rect rUpC[4]; SDL_Rect rDownC[4];
        int  L = 0, R = 0, U = 0, D = 0;
        rRightC[0] = { 0,180,75,90 }; rRightC[1] = { 75,180,75,90 }; rRightC[2] = { 150,180,75,90 }; rRightC[3] = { 225,180,75,90 };
        rLeftC[0] = { 0,90,75,90 };  rLeftC[1] = { 75,90,75,90 };  rLeftC[2] = { 150,90,75,90 };  rLeftC[3] = { 225,90,75,90 };
        rUpC[0] = { 0,270,75,90 }; rUpC[1] = { 75,270,75,90 }; rUpC[2] = { 150,270,75,90 }; rUpC[3] = { 225,270,75,90 };
        rDownC[0] = { 0,0,75,90 }; rDownC[1] = { 75,0,75,90 }; rDownC[2] = { 150,0,75,90 }; rDownC[3] = { 225,0,75,90 };
        SDL_Rect srcRC = rRightC[0];

        SDL_Rect rbutton = { 905,253,40,40 }; SDL_Rect rBoard = { SCREEN_WIDTH - 230,0,300,SCREEN_HEIGHT };
        SDL_Rect rKey = { 905,240,150,90 }; SDL_Rect rWood = { 0,0,1000,50 };
        SDL_Rect khung = { 0,50,896,512 };
        SDL_Rect rectAs = { SCREEN_WIDTH - 230, SCREEN_HEIGHT - 185,230,40 }; SDL_Rect rSuggest = { SCREEN_WIDTH - 230,SCREEN_HEIGHT - 230,230,40 };
        SDL_Rect rectTangle = { SCREEN_WIDTH - 500,SCREEN_HEIGHT / 2,200,64 };

        SDL_Rect rTime = { 0,0,0,0 };
        SDL_Rect rScore = { 150,0,0,0 };
        SDL_Rect rLevel = { SCREEN_WIDTH - 120, 0, 0, 0 };

        SDL_Rect menuText[3];

        string inputText = "";
        bool finishInput = false;

        int timeDelay = 0, realTime = 0;

        if (Level > 9) {
            Bwin = false; Blose = false; Level = 0;
        }

        if (!Bwin) {
            if (init() == false) return 0;
        }
     
            if (load() == false) {
                return 0;
            }
            else {
             
                if (!Menu(gevent, menuText, timeDelay, Bwin, Blose)) {
                    return 0;
                }
                else {

                    while (!quit) {
                        int subMap[12][18];
                        for (int i = 0; i < 12; i++) {
                            for (int j = 0; j < 18; j++) {
                                subMap[i][j] = 0;
                            }
                        }
                        Fps.start();
                        SDL_StartTextInput();
                        bool stopInput = false;
                        while (SDL_PollEvent(&gevent) != 0) {
                            if (gevent.type == SDL_QUIT) {
                                quit = true; Bwin = false; Blose = false;
                                break;
                            }
                            else {
                                if (bitmap[(rCharacter.y + 40) / 64 + 2][rCharacter.x / 64 + 2] != 9) {
                                    if (gevent.type == SDL_KEYDOWN) {
                                        switch (gevent.key.keysym.sym) {
                                        case SDLK_DOWN: if (checkMapDown(bitmap, (rCharacter.y + 40) / 64 + 2, rCharacter.x / 64 + 2)) {
                                            rCharacter.y = (rCharacter.y + 64);
                                        } srcRC = rDownC[D]; D++; if (D > 3) D = 0; break;
                                        case SDLK_UP: if (checkMapUp(bitmap, (rCharacter.y + 40) / 64 + 2, rCharacter.x / 64 + 2)) {
                                            rCharacter.y = (rCharacter.y -= 64);
                                        } srcRC = rUpC[U]; U++; if (U > 3) U = 0; break;
                                        case SDLK_RIGHT: if (checkMapRight(bitmap, (rCharacter.y + 40) / 64 + 2, rCharacter.x / 64 + 2)) {
                                            rCharacter.x = (rCharacter.x + 64);
                                        }; srcRC = rRightC[R]; R++; if (R > 3) R = 0; break;
                                        case SDLK_LEFT:  if (checkMapLeft(bitmap, (rCharacter.y + 40) / 64 + 2, rCharacter.x / 64 + 2)) {
                                            rCharacter.x = (rCharacter.x - 64);
                                        } srcRC = rLeftC[L]; L++; if (L > 3) L = 0; break;
                                        case SDLK_9:Mix_PlayMusic(Music, -1);  break;
                                        case SDLK_0: Mix_HaltMusic(); break;
                                        case SDLK_b: if (checkMinusN(bitmap)) { bitmap[(rCharacter.y + 40) / 64 + 2][rCharacter.x / 64 + 2] *= -1; break; }
                                                   else {
                                            if (bitmap[(rCharacter.y + 40) / 64 + 2][rCharacter.x / 64 + 2] < 0) bitmap[rCharacter.y / 64 + 2][rCharacter.x / 64 + 2] *= -1;
                                            break;
                                        }
                                        case SDLK_w: swapColor(bitmap, (rCharacter.y + 40) / 64 + 2, rCharacter.x / 64 + 2); break;
                                        case SDLK_RETURN: cap = (!cap); break;
                                        default: break;
                                        }
                                    }
                                }
                                else  if (bitmap[(rCharacter.y + 40) / 64 + 2][rCharacter.x / 64 + 2] == 9) {

                                    if (gevent.type == SDL_KEYDOWN)
                                    {
                                        //Handle backspace
                                        if (gevent.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0)
                                        {
                                            inputText.pop_back();
                                            renderText = true;
                                        }
                                        else if (gevent.key.keysym.sym == SDLK_KP_ENTER) {
                                            SDL_StopTextInput(); rCharacter.x -= 64; stopInput = true;
                                        }
                                    }
                                    //Special text input event
                                    else if (gevent.type == SDL_TEXTINPUT)
                                    {
                                        inputText += gevent.text.text;
                                        renderText = true;
                                    }
                                }
                            }
                        }

                        SDL_RenderClear(gRenderer);
                        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);

                        SDL_RenderCopy(gRenderer, background[Level], NULL, &khung);
                        SDL_RenderCopy(gRenderer, wood, NULL, &rWood);
                        SDL_RenderCopy(gRenderer, board, NULL, &rBoard);

                        if (Score >= 25) { condition = true; bitmap[5][16] = 9; }

                        if (condition) { SDL_RenderCopy(gRenderer, button, NULL, &rbutton); SDL_DestroyTexture(Key); }
                        else    SDL_RenderCopy(gRenderer, Key, NULL, &rKey);

                        MapRight(bitmap, subMap);
                        if (disappear(bitmap, subMap, Score)) Mix_PlayChannel(-1, MixScore, 0);

                        for (int i = 2; i < 10; i++) {
                            rectMap.x = 0;
                            for (int j = 2; j < 16; j++) {
                                if (bitmap[i][j] != 0)	renderMap(bitmap[i][j], rectMap);
                                rectMap.x += 64;
                            }
                            rectMap.y = rectMap.y + 64;
                            if (rectMap.y > 512 - 64 + 50) rectMap.y = 50;
                        }

                        int subTime = SDL_GetTicks() / 1000 - timeDelay;
                        realTime = TIME_PLAY - subTime;
                        if (keepTime != 0) {
                            TIME_PLAY += (keepTime - realTime);
                            realTime = keepTime; keepTime = 0;
                        }

                        string sTime = "Time:" + to_string(realTime);
                        string hint = "Have " + to_string(result[Level].length()) + " " + "word";
                        string score = "Score:" + to_string(Score);
                        string sLevel = "Level:" + to_string(Level);

                        Font = TTF_OpenFont("lazy.ttf", 30);

                        Time = loadFromRenderedText(sTime, white, rTime);
                        tScore = loadFromRenderedText(score, white, rScore);
                        Suggest = loadFromRenderedText(hint, white, rSuggest);
                        tLevel = loadFromRenderedText(sLevel, white, rLevel);

                        SDL_RenderCopy(gRenderer, tScore, NULL, &rScore);
                        SDL_RenderCopy(gRenderer, Time, NULL, &rTime);
                        SDL_RenderCopy(gRenderer, Suggest, NULL, &rSuggest);
                        SDL_RenderCopy(gRenderer, tLevel, NULL, &rLevel);

                        SDL_RenderCopy(gRenderer, Character, &srcRC, &rCharacter);

                        if (stopInput) { Score -= guessNumber; }
                        if (renderText)
                        {
                            //Text is not empty
                            if (inputText != "")
                            {
                                //Render new text
                                anSwer = loadFromRenderedText(inputText.c_str(), black, rectAs);
                            }
                            //Text is empty
                            else
                            {
                                //Render space texture
                                anSwer = loadFromRenderedText(" ", white, rectAs);
                            }
                        }

                        SDL_RenderCopy(gRenderer, anSwer, NULL, &rectAs);

                        if (inputText == result[Level] && stopInput == true) {
                            Mix_HaltMusic();
                            Mix_Chunk* WinSounds = Mix_LoadWAV("Victory.wav");
                            Mix_PlayChannel(-1, WinSounds, 0);
                            SDL_Rect win;
                            Font = TTF_OpenFont("lazy.ttf", 150);
                            SDL_Texture* winGame = loadFromRenderedText(congraWin[Level], blue, win);
                            win.x = (SCREEN_WIDTH - win.w) / 2; win.y = (SCREEN_HEIGHT - win.h) / 2;
                            SDL_RenderCopy(gRenderer, winGame, NULL, &win);
                            quit = true;
                            Bwin = true; Blose = false, boolGame = true; keepTime = realTime;
                            SDL_DestroyTexture(winGame);
                        }

                        if (inputText != "" && stopInput) inputText = "";

                        if (realTime <= 0 || Score < 0 || bitmap[rCharacter.y / 64 + 2][rCharacter.x / 64 + 2] == 0 ||
                            (!checkMapDown(bitmap, rCharacter.y / 64 + 2, rCharacter.x / 64 + 2) &&
                                !checkMapUp(bitmap, rCharacter.y / 64 + 2, rCharacter.x / 64 + 2) &&
                                !checkMapRight(bitmap, rCharacter.y / 64 + 2, rCharacter.x / 64 + 2) &&
                                !checkMapLeft(bitmap, rCharacter.y / 64 + 2, rCharacter.x / 64 + 2)) || bitmap[5][15] == 0) {
                            Mix_HaltMusic();
                            Mix_Chunk* mixLose = NULL;
                            mixLose = Mix_LoadWAV("lose.wav");
                            Mix_PlayChannel(-1, mixLose, 0);
                            SDL_Rect rectLose = { SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50,0,0 };
                            Font = TTF_OpenFont("lazy.ttf", 150);
                            SDL_Texture* ttLose = loadFromRenderedText("lose!", blue, rectLose);
                            SDL_RenderCopy(gRenderer, ttLose, NULL, &rectLose);
                            Bwin = false, boolGame = true;
                            quit = true; Blose = true;
                            SDL_DestroyTexture(ttLose);
                        }

                        if ((cap == true) && (Fps.get_ticks() < 1000 / FRAMES_PER_SECOND))
                        {
                            SDL_Delay((1000 / FRAMES_PER_SECOND) - Fps.get_ticks());
                        }

                        SDL_RenderPresent(gRenderer);

                        if (boolGame) {
                           SDL_Delay(4000); 
                        }                   
                    }
                }
            }
        
                    boolGame = false;

                    if (Bwin)    Level++;

                    if (Blose || (quit && !Bwin) ) {
                        TIME_PLAY = 500;
                        Score = 0;
                        Level = 0;
                        close();
                    }

                    coloseInitImg();

                   SDL_DestroyTexture(Time);
                   SDL_DestroyTexture(Suggest);
                   SDL_DestroyTexture(tScore);
                   SDL_DestroyTexture(tLevel);

                } while (true);

                close();
   
    return 0;
}
