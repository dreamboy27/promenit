#include <iostream>
#include <vector>
#include <math.h>
#include <SDL.h>
using namespace std;

const SDL_Color CYAN_COLOR = {0, 255, 255};
const SDL_Color BLUE_COLOR = {0, 0, 255};
const SDL_Color ORANGE_COLOR = {255, 165, 0};
const SDL_Color YELLOW_COLOR = {255, 255, 0};
const SDL_Color LIME_COLOR = {0, 255, 0};
const SDL_Color PURPLE_COLOR = {128, 0, 128};
const SDL_Color RED_COLOR = {255, 0, 0};
const SDL_Color WHITE_COLOR = {255, 255, 255};
const SDL_Color BLACK_COLOR = {0, 0, 0};
const SDL_Color GREEN_COLOR = {0, 128, 0};
const SDL_Color DEFAULT_COLOR = BLACK_COLOR;


int SDL_RenderDrawCircle(SDL_Renderer * renderer, int x, int y, int radius)
{
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius -1;
    status = 0;

    while (offsety >= offsetx) {
        status += SDL_RenderDrawPoint(renderer, x + offsetx, y + offsety);
        status += SDL_RenderDrawPoint(renderer, x + offsety, y + offsetx);
        status += SDL_RenderDrawPoint(renderer, x - offsetx, y + offsety);
        status += SDL_RenderDrawPoint(renderer, x - offsety, y + offsetx);
        status += SDL_RenderDrawPoint(renderer, x + offsetx, y - offsety);
        status += SDL_RenderDrawPoint(renderer, x + offsety, y - offsetx);
        status += SDL_RenderDrawPoint(renderer, x - offsetx, y - offsety);
        status += SDL_RenderDrawPoint(renderer, x - offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}


int SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius)
{
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius -1;
    status = 0;

    while (offsety >= offsetx) {

        status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
                                     x + offsety, y + offsetx);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
                                     x + offsetx, y + offsety);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
                                     x + offsetx, y - offsety);
        status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
                                     x + offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}


void logSDLError(std::ostream& os, const std::string &msg, bool fatal = false);
void logSDLError(std::ostream& os, const std::string &msg, bool fatal)
{
 os << msg << " Error: " << SDL_GetError() << std::endl;
 if (fatal) {
 SDL_Quit();
 exit(1);
 }
}

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const string WINDOW_TITLE = "Hogwarts Cat";

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer)
{
 if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
 logSDLError(std::cout, "SDL_Init", true);
 window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
 SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
 //window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
 if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);
 //Khi thông thu?ng ch?y v?i môi tru?ng bình thu?ng ? nhà
 renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
 SDL_RENDERER_PRESENTVSYNC);
 //Khi ch?y ? máy th?c hành WinXP ? tru?ng (máy ?o)
 //renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));
 if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);
 SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
 SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if ( SDL_WaitEvent(&e) != 0 &&
             (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}
int main(int argc, char* argv[])
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    initSDL(window, renderer);

    bool quit = false;
    bool clicked = false;
    vector <SDL_Rect> rects;
    vector <SDL_Point> points;

    SDL_Event e;
    while( !quit ){
        while( SDL_PollEvent( &e ) != 0 ){
            if( e.type == SDL_QUIT ){
                quit = true;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN){
                switch (e.button.button){
                case SDL_BUTTON_LEFT:
                    clicked = true;
                    break;
                }
            }
            else if (e.type == SDL_MOUSEBUTTONUP){
                switch (e.button.button){
                case SDL_BUTTON_LEFT:
                    clicked = false;
                    points.clear();
                    break;
                }
            }
            else if (e.type == SDL_MOUSEMOTION && clicked == true){
                int x, y;
                SDL_GetMouseState(&x, &y);
                points.push_back({x, y});
            }

            SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear( renderer );
            SDL_SetRenderDrawColor( renderer, 128, 0, 128, 0xFF );

        }
        for (int i = 0; i < points.size(); i++){
            float tgx = 0;
            float tgy = 0;
            if (i < points.size()-1){
            tgx = (points[i+1].x - points[i].x)/15;
            tgy = (points[i+1].y - points[i].y)/15;
            }

            SDL_RenderDrawCircle(renderer, points[i].x, points[i].y, 10);
            SDL_RenderFillCircle(renderer, points[i].x, points[i].y, 10);
            for (int j = 1; j <= 15; j++){

            SDL_RenderDrawCircle(renderer, points[i].x+tgx*j, points[i].y+tgy*j, 10);
            SDL_RenderFillCircle(renderer, points[i].x+tgx*j, points[i].y+tgy*j, 10);
            }
        }
        SDL_RenderPresent(renderer);

    }
//         waitUntilKeyPressed();
//       quitSDL(window, renderer);
 return 0;
}

