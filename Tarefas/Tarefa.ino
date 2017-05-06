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

void setup() {
  
  // led pin de saida
  pinMode(ledPin, OUTPUT);
  
  // led pin de entrada
  pinMode(buttonPinUP, INPUT);
  pinMode(buttonPinDown, INPUT);
  
  estado = 0;
  antes = millis();
}

void loop() {
  
  digitalWrite(ledPin, estado);
  unsigned long agora = millis();
  
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

void parar() {
  if(buttonState == HIGH && buttonState2 == HIGH && stateAnteriorBTN1==1 && stateAnteriorBTN2==1){
    if (abs(tempo - tempo2) < pausa) {
       while(true)
          digitalWrite(ledPin, estado); // botão parado
     }
  }
}
