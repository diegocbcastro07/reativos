#include <SDL2/SDL.h>
#include <assert.h>

#define tamQuadrado 30

SDL_Window* window;
SDL_Event e;
SDL_Rect r = {100,100, tamQuadrado, tamQuadrado};
SDL_Rect r2 = {150,250, tamQuadrado, tamQuadrado};
float speed = 0.5;
float speed2 = 0.3;
int dir = 0;
int dir2 = 0;
int dt = 0;

void desenho(SDL_Renderer* renderer, SDL_Rect* r1, SDL_Rect* r2){
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0x00);
    SDL_RenderFillRect(renderer, NULL);
    
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0xFF, 0x00);
    SDL_RenderFillRect(renderer, r1);
    
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0x00);
    SDL_RenderFillRect(renderer, r2);

    SDL_RenderPresent(renderer);
}

SDL_Window* inicializa(){
	int err = SDL_Init(SDL_INIT_EVERYTHING);
	assert(err == 0);

	window = SDL_CreateWindow("trab_SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
	assert(window != NULL);
}

int finaliza(SDL_Event e){
	if(e.type == SDL_QUIT) {
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
}


void andaQuadrado(){
	switch (dir)
	{
		case 0:
			r.x += speed * dt;
			if(r.x > 330) {
				r.x = 330;
				dir = 1;
			}
			break;

		case 1:
			r.y += speed * dt;
			if(r.y > 370) {
				r.y = 370;
				dir = 2;
			}
			break;
		case 2:
			r.x -= speed * dt;
			if(r.x < 20) {
				r.x = 20;
				dir = 3;
			}
			break;
		case 3:
			r.y -= speed * dt;
			if(r.y < 180) {
				r.y = 180;
				dir = 0;
			}
			break;
	}

	switch (dir2)
	{
		case 0:
			r2.x += speed2 * dt;
			if(r2.x > 330) {
				r2.x = 330;
				dir2 = 1;
			}
			break;

		case 1:
			r2.y += speed2 * dt;
			if(r2.y > 370) {
				r2.y = 370;
				dir2 = 2;
			}
			break;
		case 2:
			r2.x -= speed2 * dt;
			if(r2.x < 20) {
				r2.x = 20;
				dir2 = 3;
			}
			break;
		case 3:
			r2.y -= speed2 * dt;
			if(r2.y < 180) {
				r2.y = 180;
				dir2 = 0;
			}
			break;
	}
		
}

void testaMouse(){

	if(e.type == SDL_MOUSEBUTTONDOWN) {
		SDL_MouseButtonEvent* me = (SDL_MouseButtonEvent*) &e;
		int x = 0,y = 0;
		x = me->x;
		y = me->y;
		if((x >= r.x) && (x <= r.x + tamQuadrado) && (y >= r.y) && (y <= r.y + tamQuadrado)) {
			speed = 0;
		}
		if((x >= r2.x) && (x <= r2.x + tamQuadrado) && (y >= r2.y) && (y <= r2.y + tamQuadrado)) {
			speed2 = 0;
		}
	}
}

int main (int argc, char* args[]) {

	window = inicializa();
	SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,0);
	assert(renderer != NULL);
	int time = SDL_GetTicks();

	while(1) {

		SDL_WaitEventTimeout(&e, 1);
		dt = SDL_GetTicks() - time;
		time = SDL_GetTicks();
		
		testaMouse();
		andaQuadrado();
		desenho(renderer, &r, &r2);
		finaliza(e);
	}
}
