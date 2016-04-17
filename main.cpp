#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif

#include <iostream>
#include <stdlib.h>
#include <time.h>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

//Game Info
const char GAME_NAME[35] = "Tic Tac Toe";
const unsigned int SCREEN_W = 600, SCREEN_H = 600, TILE_SIZE = 200, PADDING = 50;

enum{
    EMPTY = '-',
    P1 = 'X',
    P2 = 'O'
};

//Colors
SDL_Color c_white = {255, 255, 255};
SDL_Color c_black = {0, 0, 0};

//Controllers
bool unlocked = true;
char actSymbol = P1;

//Game Engine Variables
SDL_Event occur;
SDL_Surface *screen;

TTF_Font *font;
char tiles[3][3] = {{EMPTY, EMPTY, EMPTY}, {EMPTY, EMPTY, EMPTY}, {EMPTY, EMPTY, EMPTY}};
SDL_Rect p_tiles[3][3];
SDL_Surface *s_tiles[3][3] = {{NULL,NULL,NULL}, {NULL,NULL,NULL}, {NULL,NULL,NULL}};

//Game utilities
char check_symbols();
void update();

char check_symbols(char t, char s){
    if(t == EMPTY){
        return s;
    }
    return t;
}

void update(){
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            char c[4] = {tiles[i][j], ' ',' ',' '};
            s_tiles[i][j] = TTF_RenderText_Shaded(font, c, c_white, c_black);
            p_tiles[i][j] = {PADDING+(TILE_SIZE * j), PADDING+(TILE_SIZE * i), 0, 0};
        }
    }
}

//Game Engine Methods
void LoadGame();
void Logic();
void KeyboardAndMouse();
void DrawScreen();
void Quit();

void LoadGame(){

    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    font = TTF_OpenFont("arial.ttf", 64);

    SDL_WM_SetCaption(GAME_NAME, NULL);
    screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_HWSURFACE);

}

void Logic(){

}

void KeyboardAndMouse(){
    if(occur.type == SDL_MOUSEBUTTONDOWN && unlocked){
        unlocked = false;
        int mx = occur.motion.x, my = occur.motion.y;
        //std::cout << occur.motion.x << " - " << occur.motion.y << std::endl;
        for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
                SDL_Rect _actTile = p_tiles[i][j];
                char _tile = tiles[i][j];
                if((_actTile.x < mx && _actTile.x + _actTile.w > mx) &&
                  (_actTile.y < my && _actTile.y + _actTile.h > my)){
                    std::cout << "I touched the [" << i << ", " << j << "], that equals to " << _tile << std::endl;

                    tiles[i][j] = check_symbols(_tile, actSymbol);

                    update();
                }
            }
        }
    }else if(occur.type == SDL_MOUSEBUTTONUP){
        unlocked = true;
    }
}

void DrawScreen(){

    SDL_FillRect(screen, NULL, 0);

    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            SDL_BlitSurface(s_tiles[i][j], NULL, screen, &p_tiles[i][j]);
        }
    }

    SDL_Flip(screen);

}

void Quit(){
    TTF_Quit();
    SDL_Quit();
}

int main ( int argc, char** argv )
{
    LoadGame();

    update();

    bool running = true;

    while(running){

        SDL_PollEvent(&occur);

        if(occur.type == SDL_QUIT){
            running = false;
        }

        Logic();
        KeyboardAndMouse();
        DrawScreen();

    }

    Quit();
    return 0;
}
