#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

//Methods
//Engine
void LoadGame();
void Logic();
void KeyboardAndMouse();
void DrawScreen();
void Quit();
void AI();

//Utils
char check_symbols();
void update();
void alternate();
void win(char s);
void reset();


//Game Info
const char GAME_NAME[35] = "Tic Tac Toe";
const unsigned int SCREEN_W = 700, SCREEN_H = 600, TILE_SIZE = 200, PADDING = 50;
unsigned int p1_score=0, p2_score=0;
bool againstAI = true;
char thelast = ' ';

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
char aiSymbol = P2;

//Game Engine Variables
SDL_Event occur;
SDL_Surface *screen;

TTF_Font *font;
char tiles[3][3] = {{EMPTY, EMPTY, EMPTY}, {EMPTY, EMPTY, EMPTY}, {EMPTY, EMPTY, EMPTY}};
SDL_Rect p_tiles[3][3];
SDL_Surface *s_tiles[3][3] = {{NULL,NULL,NULL}, {NULL,NULL,NULL}, {NULL,NULL,NULL}};

//Score
SDL_Rect p_actualSymbol, p_p1score, p_p2score;
SDL_Surface *s_actualSymbol=NULL, *s_p1score=NULL, *s_p2score=NULL;

//Game utilities

void reset(){
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            tiles[i][j] = EMPTY;
        }
    }
    update();
}

void win(char s){
    std::cout << "The player " << s << " win!" << std::endl;
    if(s == P1){
        p1_score++;
    }else{
        p2_score++;
    }
    reset();
}

void alternate(){
    if(againstAI){
        thelast = aiSymbol;
        actSymbol = P1;
        aiSymbol = P2;
    }else{
        thelast = actSymbol;
        if(actSymbol == P1){
            actSymbol = P2;
            aiSymbol = P1;
        }else if(actSymbol == P2){
            actSymbol = P1;
            aiSymbol = P2;
        }
    }
}

int check_winner(){
    char tmpSymb = P1;
    for(int j=0; j<2; j++){
        for(int i=0; i<3; i++){
            if(tiles[i][0] == tmpSymb && tiles[i][1] == tmpSymb && tiles[i][2] == tmpSymb){
                win(tmpSymb);
                return 1;
            }
            if(tiles[0][i] == tmpSymb && tiles[1][i] == tmpSymb && tiles[2][i] == tmpSymb){
                win(tmpSymb);
                return 1;
            }
        }

        if(tiles[1][1] == tmpSymb){
            if(tiles[0][0] == tmpSymb && tiles[2][2] == tmpSymb){
                win(tmpSymb);
                return 1;
            }else if(tiles[0][2] == tmpSymb && tiles[2][0] == tmpSymb){
                win(tmpSymb);
                return 1;
            }
        }
        tmpSymb = P2;
    }

    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            if(tiles[i][j] == EMPTY){
                return -1;
            }
        }
    }

    return 0;
}

char check_symbols(char t, char s){

    if(t == EMPTY){
        alternate();
        return s;
    }
    return t;
}

void update(){

    char c2[5] = {'S', ':', ' ', actSymbol, ' '};
    s_actualSymbol = TTF_RenderText_Shaded(font, c2, c_white, c_black);
    p_actualSymbol = {SCREEN_W-90-PADDING, 20+PADDING, 0, 0};


    char c3[9] = "P1: ", c4[9];
    sprintf(c4, "%d", p1_score);
    strcat(c3, c4);
    s_p1score = TTF_RenderText_Shaded(font, c3, c_white, c_black);
    p_p1score = {SCREEN_W-120-PADDING, 100+PADDING, 0, 0};

    char c5[9] = "P2: ", c6[9];
    sprintf(c6, "%d", p2_score);
    strcat(c5, c6);
    s_p2score = TTF_RenderText_Shaded(font, c5, c_white, c_black);
    p_p2score = {SCREEN_W-120-PADDING, 160+PADDING, 0, 0};

    if(check_winner() == 0){
        std::cout << "TIE!" << std::endl;
        reset();
    }

    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            char c[2] = {tiles[i][j], (char)0};
            s_tiles[i][j] = TTF_RenderText_Shaded(font, c, c_white, c_black);
            p_tiles[i][j] = {PADDING+(TILE_SIZE * j), PADDING+(TILE_SIZE * i), 0, 0};
        }
    }
}

//Game Engine Methods

void LoadGame(){

    srand(static_cast<unsigned int>(time(0)));

    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    font = TTF_OpenFont("arial.ttf", 64);

    SDL_WM_SetCaption(GAME_NAME, NULL);
    screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, SDL_HWSURFACE);
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

                    if(againstAI){
                        alternate();
                        AI();
                    }

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

    SDL_BlitSurface(s_actualSymbol, NULL, screen, &p_actualSymbol);
    SDL_BlitSurface(s_p1score, NULL, screen, &p_p1score);
    SDL_BlitSurface(s_p2score, NULL, screen, &p_p2score);

    SDL_Flip(screen);

}

void AI(){

    int r1 = rand() % 3;
    int r2 = rand() % 3;
    std::cout << r1 << " " << r2 << std::endl;

    if(tiles[r1][r2] == EMPTY){
        tiles[r1][r2] = aiSymbol;
        thelast = aiSymbol;
        alternate();
        update();
    }else{
        AI();
    }
}

void Quit(){
    TTF_Quit();
    SDL_Quit();
}

int timer = 0;
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

        KeyboardAndMouse();
        DrawScreen();

        timer++;
        std::cout << timer << std::endl;

        if(timer >= 300){
            if(thelast == actSymbol && againstAI){
                AI();
            }
            update();
            timer = 0;
        }

    }

    Quit();
    return 0;
}
