#include <stdint.h>

const int buttonPinUP = 2;
const int buttonPinDown = 3;
const int ledPin = 13;      
int estado;
unsigned long antes;

// variables will change:
int buttonState = 0; // estado do botão 1 para saber se ele está em alta ou baixa
int buttonState2 = 0;// estado do botão 2 para saber se ele está em alta ou baixa
int stateAnteriorBTN1 = 0; // usado para fazer o botão só aumentar uma vez por click, mesmo se o botão estiver precionado só vai aumentar uma vez
int stateAnteriorBTN2 = 0; // usado para fazer o botão só aumentar uma vez por click, mesmo se o botão estiver precionado só vai aumentar uma vez
int velocidade = 1000;// Tempo em ms do intervalo
int pausa = 500; // em menos de 500ms parar
long tempo; // tempo do botão 1
long tempo2; // tempo do botão 2
 
/* take 64 bits of data in v[0] and v[1] and 128 bits of key[0] - key[3] */
 
void encipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]) {
    unsigned int i;
    uint32_t v0=v[0], v1=v[1], sum=0, delta=0x9E3779B9;
    for (i=0; i < num_rounds; i++) {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
        sum += delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
    }
    v[0]=v0; v[1]=v1;
}
 
void decipher(unsigned int num_rounds, uint32_t v[2], uint32_t const key[4]) {
    unsigned int i;
    uint32_t v0=v[0], v1=v[1], delta=0x9E3779B9, sum=delta*num_rounds;
    for (i=0; i < num_rounds; i++) {
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
        sum -= delta;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
    }
    v[0]=v0; v[1]=v1;
}

#define LED 13

void setup () {
    pinMode(LED, OUTPUT);
    Serial.begin(9600);

  // led pin de saida
  pinMode(ledPin, OUTPUT);
  
  // led pin de entrada
  pinMode(buttonPinUP, INPUT);
  pinMode(buttonPinDown, INPUT);
  
  estado = 0;
  antes = millis();
}
 
uint32_t key[] = { 1, 2};
uint32_t v[]   = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 
                  110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 
                  210, 220, 230, 240, 250, 260, 270, 280, 290, 300,
                  310, 320, 330, 340, 350, 360, 370, 380, 390, 400,
                  410, 420, 430, 440, 450, 460, 470, 480, 490, 500};

void loop () {
  
  int i;  
  static int led = 0;
  
 for(i=0; i<50; i++){
    piscaPisca();
    decipher(32, &v[i], key);
  }
  
  for(i=0; i<50; i++){        
     piscaPisca();
    encipher(32, &v[i], key);
  }
}

void parar() {
  if(buttonState == HIGH && buttonState2 == HIGH && stateAnteriorBTN1==1 && stateAnteriorBTN2==1){
    if (abs(tempo - tempo2) < pausa) {
       while(true)
          digitalWrite(ledPin, estado); // botão parado
     }
  }
}

void piscaPisca(){
  digitalWrite(ledPin, estado);
    unsigned long agora = millis();
    Serial.print("\t"); 
    Serial.print("\t"); 
  
    if(agora >= antes+velocidade){
      estado = !estado;
      antes = agora;
    }
  
    // leitura do status do botão
    buttonState = digitalRead(buttonPinUP);
    buttonState2 = digitalRead(buttonPinDown);
    if(buttonState == LOW && stateAnteriorBTN1==1){
      stateAnteriorBTN1= 0;
    }
    if(buttonState == HIGH && stateAnteriorBTN1==0) {
      tempo = agora;
      stateAnteriorBTN1 = 1;
      parar();
      if(velocidade > 100)
        velocidade = velocidade-100;
    }


    if(buttonState2 == LOW && stateAnteriorBTN2==1){
      stateAnteriorBTN2= 0;
    }
    if(buttonState2 == HIGH && stateAnteriorBTN2==0) {
      tempo2 = agora;
      stateAnteriorBTN2 = 1;
      parar();
      if(velocidade < 2000)
        velocidade = velocidade+100;
    }
}

