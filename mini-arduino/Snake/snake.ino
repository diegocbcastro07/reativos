#include <SPI.h>
#include <GD2.h>

#define CIMA 1
#define DIREITA 2
#define BAIXO 3
#define ESQUERDA 4
#define VELOCIDADE 1
#define DISTANCIA_ENTRE_PONTOS 3
#define TAMANHO_HORIZONTAL 479
#define TAMANHO_VERTICAL 271
#define NAO_HOUVE_TOQUE -32768
int inicio = 0;
int fim = 0;
int tem_bolinha = 0;
int pontuacao = 0;

struct ponto {
      int x;
      int y; 
}; 

struct snake {
  struct ponto pontos[100];
  int tamanho;
  int direcao;
};

struct ponto primeiro_ponto = {240, 136};
struct ponto bolinha = {0,0};
struct ponto segundo_ponto = {235, 136};
struct ponto terceiro = {235, 136};
struct snake cobra;

void draw_bolinha(){
  if(tem_bolinha == 0){
     tem_bolinha = 1;
     bolinha.x = random(0, TAMANHO_HORIZONTAL-5);
     bolinha.y = random(0, TAMANHO_VERTICAL-5);
  }
  GD.cmd_text(bolinha.x, bolinha.y, 31, OPT_CENTER, ".");
};

int troca_direcao(int direcao,int x,int y){
     if(direcao == CIMA || direcao == BAIXO){
         if(x>240){
             return ESQUERDA; 
         }
         else{
            return DIREITA; 
         }
      }
     else if(direcao == DIREITA || direcao == ESQUERDA){
         if(y>135){
             return BAIXO; 
         }
         else{
            return CIMA; 
         }
   }
};

struct snake movimenta(struct snake s){
  struct snake cobra = s;
  int i = s.tamanho;
  //movimenta dependendo da direcao
  if(cobra.direcao == CIMA){
      //andas as pecas de tras 
      while(i>0){
        cobra.pontos[i].x = cobra.pontos[i-1].x;
        cobra.pontos[i].y = cobra.pontos[i-1].y + DISTANCIA_ENTRE_PONTOS;
        i--;
      }
    cobra.pontos[0].y += -VELOCIDADE;
  }
  else if(cobra.direcao == DIREITA){
     //andas as pecas de tras 
      while(i>0){
        cobra.pontos[i].x = cobra.pontos[i-1].x - DISTANCIA_ENTRE_PONTOS;
        cobra.pontos[i].y = cobra.pontos[i-1].y;
        i--;
      }
    cobra.pontos[0].x += VELOCIDADE;
  }
  else if(cobra.direcao == BAIXO){
     //andas as pecas de tras 
      while(i>0){
        cobra.pontos[i].x = cobra.pontos[i-1].x;
        cobra.pontos[i].y = cobra.pontos[i-1].y -DISTANCIA_ENTRE_PONTOS;
        i--;
      }
    cobra.pontos[0].y += VELOCIDADE;
  }
  else if(cobra.direcao == ESQUERDA){
     //andas as pecas de tras 
      while(i>0){
        cobra.pontos[i].x = cobra.pontos[i-1].x + DISTANCIA_ENTRE_PONTOS;
        cobra.pontos[i].y = cobra.pontos[i-1].y;
        i--;
      }
    cobra.pontos[0].x += -VELOCIDADE;
  }
  return cobra;
};

void draw(struct snake cobra){
  int i = 0;
  while(i < cobra.tamanho){
     GD.cmd_text(cobra.pontos[i].x, cobra.pontos[i].y, 31, OPT_CENTER, ".");
     i++;
  }
};


void detecta_colisao(struct snake cobra){
    if(cobra.pontos[0].x > TAMANHO_HORIZONTAL || cobra.pontos[0].x < 0 || cobra.pontos[0].y > TAMANHO_VERTICAL-10 || cobra.pontos[0].y < 0){
      fim = 1;
    }
};

void restart(){
  cobra.tamanho = 3;
  cobra.pontos[0] = primeiro_ponto;
  cobra.pontos[1] = segundo_ponto;
  cobra.pontos[2] = terceiro;
  cobra.direcao = DIREITA;
  pontuacao = 0;
  tem_bolinha = 0;
};

void pontua(){
   int i = 0;
   if(cobra.pontos[i].x > (bolinha.x - 5) && cobra.pontos[i].x < (bolinha.x + 5) ){
     if(cobra.pontos[i].y > (bolinha.y - 5) && cobra.pontos[i].y < (bolinha.y + 5) ){
        tem_bolinha = 0;
        cobra.tamanho++;
        pontuacao++;
     }
   }
};

void draw_pontuacao(){
    GD.cmd_number(30, 30, 21, OPT_CENTER, pontuacao);
};


void setup()
{
  GD.begin(0);
  cobra.tamanho = 3;
  cobra.pontos[0] = primeiro_ponto;
  cobra.pontos[1] = segundo_ponto;
  cobra.pontos[2] = terceiro;
  cobra.direcao = DIREITA;
}

void loop()
{
  GD.get_inputs();
  GD.Clear();
  //mensagem ao iniciar
  if(fim == 0){
    GD.cmd_text(240, 136, 31, OPT_CENTER, "Tap to Start");
  }
  //mensagem ao acabar
  else if(fim == 1){
    GD.cmd_text(240, 106, 31, OPT_CENTER, "GAME OVER");
    GD.cmd_text(240, 136, 24, OPT_CENTER, "Tap to Restart");
    GD.cmd_text(240, 166, 21, OPT_CENTER, "score: ");
    GD.cmd_number(265, 166, 21, OPT_CENTER, pontuacao);
  }
  //se houver toque
  if (GD.inputs.x != NAO_HOUVE_TOQUE && GD.inputs.y != NAO_HOUVE_TOQUE) {
        inicio = 1;
        fim = 0;
        restart();
  }
  if(inicio == 1){
    
    //iniciou o jogo
    while(fim == 0){
      GD.get_inputs();
      GD.Clear();
      
      draw_bolinha();
      
      //detecta colisao
      detecta_colisao(cobra);

      //Pontua detecta se passou pela bolinha
      pontua();
      
      //testa se houve toque, caso tenha chama a funcao para troca da direcao
      if (GD.inputs.x != NAO_HOUVE_TOQUE && GD.inputs.y != NAO_HOUVE_TOQUE) {
          cobra.direcao = troca_direcao(cobra.direcao, GD.inputs.x, GD.inputs.y);
      }
      
      //faz o deslocamento da cobra
      cobra = movimenta(cobra);
    
      //desenha a cobra
      draw(cobra);
      draw_pontuacao();
      GD.swap();
    }
     
  }
  GD.swap();  
}
