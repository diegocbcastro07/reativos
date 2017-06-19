#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL_image.h>

#define INICIO_TELA 0 
#define LARG_TELA 500 //largura
#define ALT_TELA 500 //altura
#define TAMANHO_V 15 //tamanho vertical
#define TAMANHO_H 100 // tamanho horiz
#define POS_Y 1000

int pos_x = 200;
SDL_Rect quadrados[12];

struct quadrado {
      int x;
      int y;
      bool desenha;
}; 

struct quadrado grid[12];

int desenhaTela(SDL_Window **w, SDL_Renderer **r);
void desenhaBoneco(SDL_Rect *boneco);
void desenhaBolinha(SDL_Rect *bola);
void desenhaGrid();
void geraGrid();
bool IntersectRect(const SDL_Rect * r1, const SDL_Rect * r2);
void desenhaQuadrado(SDL_Rect *quadrado, struct quadrado q);
void carregaImagem(SDL_Window *window, SDL_Renderer *renderer, char *imagem);
 
int main(int argc, char *args[]){
    geraGrid();
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    bool quit = false;
    SDL_Rect boneco;
    SDL_Rect bola;
    desenhaBoneco(&boneco);
    desenhaBolinha(&bola);
    desenhaGrid();
	int vel_bola_x = 2;
	int vel_bola_y = 3;    
    int bola_x = vel_bola_x;
    int bola_y = vel_bola_y;
    boneco.x = pos_x;
	int vel = 20;
	char *IMG_GAME_OVER = "gameOver.jpg";
	if(desenhaTela(&window, &renderer) < 0){
        exit(EXIT_FAILURE);
    }

    SDL_Event e;
	SDL_Texture* img_bola = IMG_LoadTexture(renderer, "bola.png");
	SDL_Texture* img_fundo = IMG_LoadTexture(renderer, "backgroung.jpg");
	SDL_Texture* img_boneco = IMG_LoadTexture(renderer, "boneco.png");
    while(!quit){
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                quit = true;
            }else if(e.type == SDL_KEYDOWN){
                switch(e.key.keysym.sym){
                    case SDLK_LEFT:
                        boneco.x -= vel;
                        break;
                    case SDLK_RIGHT:
                        boneco.x += vel;
                        break;
                    default:
                        printf("Aperte '->' ou '<-' para movimentar o boneco.\n");
                        break;
                }
            }
        }
 
        //pisição limite do boneco
        if(boneco.x < INICIO_TELA){
            boneco.x = 0;
        }
        if(boneco.x + TAMANHO_H > LARG_TELA){
            boneco.x = LARG_TELA - TAMANHO_H;
        }
        if(boneco.y + boneco.h > LARG_TELA){
            boneco.y = ALT_TELA - boneco.h;
        }
 
        //move bolinha
        bola.x += bola_x;
        bola.y += bola_y;
 
        // colisão da bolinha
        if(bola.y < 0){
    	    bola_y = -bola_y;
        }
        if(bola.y + bola.h > ALT_TELA){
			carregaImagem(window, renderer, IMG_GAME_OVER);
		}
        if(bola.x < 0 || bola.x + bola.w > LARG_TELA){
            bola_x = -bola_x;
        }
 
        //colisão com o boneco
        if(IntersectRect(&boneco, &bola)){
        	if(rand() % 2 == 0){
           		bola_x = bola_x;
            	bola_y = -bola_y;
        	}
        	else{
            	bola_x = -bola_x;
            	bola_y = -bola_y;
        	}
        }

        //colisão grid
        int j;
        for( j = 0; j < 12; j++ ){
            if(IntersectRect(&quadrados[j], &bola) && grid[j].desenha){
                if(rand() % 2 == 0){
                    grid[j].desenha= false;
                    bola_x = bola_x;
                    bola_y = -bola_y;
                }
                else{
                    grid[j].desenha= false;
                    bola_x = -bola_x;
                    bola_y = -bola_y;
                }
            }
         }

        //limpa a tela
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        //desenha
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderCopy(renderer, img_fundo, NULL, NULL);		

		SDL_RenderFillRect(renderer, &bola);
		SDL_RenderCopy(renderer, img_bola, NULL, &bola);

		SDL_RenderCopy(renderer, img_boneco, NULL, &boneco);

        int i;
        for( i = 0; i < 12; i++ ){
            if(grid[i].desenha == true){
				SDL_SetRenderDrawColor(renderer, rand() % 255, rand() %  255, rand() %  255, 255);	
                SDL_RenderFillRect(renderer, &quadrados[i]);
            }
        }

        SDL_RenderPresent(renderer);
    }
 
	SDL_DestroyTexture(img_bola);
	SDL_DestroyTexture(img_fundo);
    SDL_Quit();
    return 0;
}
 
int desenhaTela(SDL_Window **w, SDL_Renderer **r){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("ERRO ao iniciar! %s\n", SDL_GetError());
        return -1;
    }
 
    if((*w = SDL_CreateWindow("mini game", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, LARG_TELA, ALT_TELA, 0)) == NULL){
        printf("ERRO ao abrir a tela! %s\n", SDL_GetError());
        return -1;
    }
 
    if((*r = SDL_CreateRenderer(*w, -1, SDL_RENDERER_PRESENTVSYNC)) == NULL){
        printf("Erro no render! %s\n", SDL_GetError());
        return -1;
    }
 
    return 0;
}
 
void desenhaBoneco(SDL_Rect *boneco){ 
    boneco->x = pos_x; // Position P1 paddle 2 paddle-widths away from screen edge
    boneco->y = POS_Y; // Position Paddle in direct centre
    boneco->w = TAMANHO_H; //largura
    boneco->h = TAMANHO_V; // altura
}

void desenhaBolinha(SDL_Rect *bola){
    bola->x = LARG_TELA / 4;
    bola->y = ALT_TELA / 2;
    bola->w = 15;
    bola->h = 15;
}

void desenhaQuadrado(SDL_Rect *quadrado, struct quadrado q){
    quadrado->x = q.x;
    quadrado->y = q.y;
    quadrado->w = 60;
    quadrado->h = 20;
}
void geraGrid(){
    int i;
    for(i = 0; i < 12; i++ ){
        grid[i].x = (LARG_TELA-252)/2 + (62*(i%4));
        grid[i].y = 50 + (i%3)*22;
        grid[i].desenha = true;
    }
}
void desenhaGrid(){
    int i;
    for( i = 0; i < 12; i++ ){
        desenhaQuadrado(&quadrados[i], grid[i]);
        
    }
}

bool IntersectRect(const SDL_Rect * r1, const SDL_Rect * r2){
    return !(r2->x >= (r1->x + r1->w) || 
       (r2->x + r2->w) <= r1->x || 
       r2->y >= (r1->y + r1->h) ||
       (r2->y + r2->h) <= r1->y);      
}

void carregaImagem(SDL_Window *window, SDL_Renderer *renderer, char *imagem){
	SDL_Surface *img = NULL;
    SDL_Rect src;
    SDL_Rect dst;
    int imgFlags = IMG_INIT_PNG;
	img = IMG_Load(imagem);
	int i =0;
	int j =0;

    for (i = 0; i < 4; i++) {
    	for (j = 0; i < 4; i++) {
        	dst.x = 500*i;
            dst.y = 500*j;
            SDL_BlitSurface(img, NULL, SDL_GetWindowSurface(window), &dst);
        }
    }

    int r = SDL_BlitSurface(SDL_GetWindowSurface(window), &src, SDL_GetWindowSurface(window), &dst);
        
	while (1) {
		SDL_Event e;
		if ( SDL_PollEvent(&e) ) {
			if (e.type == SDL_QUIT)
				break;
		} 
                SDL_UpdateWindowSurface(window);
                SDL_Delay(10);
	}
    SDL_FreeSurface(img);
	SDL_DestroyWindow(window);
}

