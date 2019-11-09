/*
  * Código para Placa de Substituição/ acrescimo
  * 
  * Autores:
  * Denilson Gomes Vaz da Silva   <denilsongomes@alu.ufc.br>
  * Graduando em Engenharia da Computação - UFC
 */

// -- Bibliotecas --
#include <TimerOne.h> //Bivlioteca para usar interrupção do TIMER1

// -- Variaveis e Constantes --
volatile int numeroAzul1 = 0;
volatile int numeroAzul2 = 0;
volatile int numeroVermelho1 = 0;
volatile int numeroVermelho2 = 0;
volatile int mux = 0;

// -- Entradas --
int botAzul = 2; //Botao para incrementar para incrementar numero Azul
int botVermelho = 3; //Botao para incrementar numero Vermelho
int primeiroDigito = 4; //Botão para incrementar primeiro ou segundo digito de cada numero

// -- Saídas --
int displayAzul1 = 5; //Variavel para ativar display Azul1
int displayAzul2 = A4; //Variavel para ativar display Azul2
int displayVermelho1 = 13; //Variavel para ativar display Vermelho1
int displayVermelho2 = 0; //Variavel para ativar display Vermelho2
int displays[] = {displayAzul1, displayAzul2, displayVermelho1, displayVermelho2};

byte seven_seg_digits[10][7] = {
 //Para leds acionando em HIGH (Display catodo Comum)
 { 1,1,1,1,1,1,0 },  //DIGITO 0 
 { 0,1,1,0,0,0,0 },  //DIGITO 1
 { 1,1,0,1,1,0,1 },  //DIGITO 2
 { 1,1,1,1,0,0,1 },  //DIGITO 3
 { 0,1,1,0,0,1,1 },  //DIGITO 4
 { 1,0,1,1,0,1,1 },  //DIGITO 5
 { 1,0,1,1,1,1,1 },  //DIGITO 6
 { 1,1,1,0,0,0,0 },  //DIGITO 7
 { 1,1,1,1,1,1,1 },  //DIGITO 8
 { 1,1,1,0,0,1,1 },  //DIGITO 9
};

void setup() {

  Serial.begin(9600); //Habilita comunicação Serial

  //Entradas
  pinMode(primeiroDigito, INPUT_PULLUP); //Botão incrementa primeiro digito do numero
  pinMode(botAzul, INPUT_PULLUP); //Botão azul como entrada
  pinMode(botVermelho, INPUT_PULLUP); //Botão vermelho como entrada

  //Interrupções
  attachInterrupt(0, azul, FALLING); //Chama azul() quando botão azul é presionado
  attachInterrupt(1, vermelho, FALLING); //Chama vermelho() quando botão vermelho é presionado

  Timer1.initialize(100000); // Inicializa o Timer1 e configura para um período de 10 milisegundos
  Timer1.attachInterrupt(callback); // Chama callback() a cada interrupção do Timer1
  
  //Saídas
  pinMode(displayAzul1, OUTPUT); //Sinal para ativar displayAzul1
  pinMode(displayAzul2, OUTPUT); //Sinal para ativar displayAzul2
  pinMode(displayVermelho1, OUTPUT); //Sinal para ativar displayVermelho1
  pinMode(displayVermelho2, OUTPUT); //Sinal para ativar displayVermelho2
  
  //Seguimentos dos Displays
  //Caso os pinos dos seguimentos mudem é preciso alterar a função
  pinMode(12, OUTPUT); //PINO 12 -> SEGMENTO A  
  pinMode(11, OUTPUT); //PINO 11 -> SEGMENTO B
  pinMode(10, OUTPUT); //PINO 10 -> SEGMENTO C
  pinMode(9, OUTPUT); //PINO 9 -> SEGMENTO D
  pinMode(8, OUTPUT); //PINO 8 -> SEGMENTO E
  pinMode(7, OUTPUT); //PINO 7 -> SEGMENTO F
  pinMode(6, OUTPUT); //PINO 6 -> SEGMENTO G
 

  // Configuração inicial
//  estVermelho=0;
//  estAzul=0;
//  som =0;
//  pontosAzul =0;
//  pontosVermelho =0;

  //Começa a multiplexação
  digitalWrite(displayAzul1,LOW);
  digitalWrite(displayAzul2,LOW);
  digitalWrite(displayVermelho1,HIGH);
  digitalWrite(displayVermelho2,LOW);
}

//Função chamada quando o botão vermelho é pressionado
void vermelho() {
  noInterrupts(); //Desativa as interrupções
  if(digitalRead(4)){ //se o botao primeiroDigito tiver acionado
    numeroVermelho1 = (numeroVermelho1++)%10; //Incrementa o primeiro digito vermelho
  }else{ //Se não
    numeroVermelho2 = (numeroVermelho2++)%10; //Incrementa o segundo digito vermelho
  } 
}

//Função chamada quando o botão azul é pressionado
void azul() {
  noInterrupts(); //Desativa as interrupções
  if(digitalRead(4)){ //se o botao primeiroDigito tiver acionado
    numeroAzul1 = (numeroAzul1++)%10; //Incrementa o primeiro digito azul
  }else{ //Se não
    numeroAzul2 = (numeroAzul2++)%10;; //Incrementa o segundo digito azul
  }
}

//Função chamada a cada interrupção do Timer
void callback()
{
  //Atualiza o valor de mux
  mux = mux++;
  mux = mux%4;
  //Apaga todos os displays
  digitalWrite(displayAzul1, LOW); //Troca o estado do display azul1
  digitalWrite(displayAzul2, LOW); //Troca o estado do display azul2
  digitalWrite(displayVermelho1, LOW); //Troca o estado do display vermelho1
  digitalWrite(displayVermelho2, LOW); //Troca o estado do display vermelho2
  //Atualiza o valor a ser exibido
  delay(100);
  if(0 == mux){ //caso formos ligar o display azul1
    ligaSegmentosDisplay(numeroAzul1); //Exibimos o digito azul1
  }
  if(1 == mux){ //caso formos ligar o display azul2
    ligaSegmentosDisplay(numeroAzul2); //Exibimos o digito azul2
  }
  if(2 == mux){ //caso formos ligar o display vermelho1
    ligaSegmentosDisplay(numeroVermelho1); //Exibimos o digito vermelho1
  }
  if(3 == mux){ //caso formos ligar o display vermelho2
    ligaSegmentosDisplay(numeroVermelho2); //Exibimos o digito vermelho2
  }
  delay(100);

  //ativa o display que deve exibir o valor
  for(int i=0;i<4;i++){
   digitalWrite(displays[i], i == mux);
  }
} 


//Função que exibe a pontuação no display
void ligaSegmentosDisplay(byte digit){
  byte pino = 12; //Pino do seguimento A

  for (byte contadorSegmentos = 0; contadorSegmentos < 7; ++contadorSegmentos){ //PARA "contadorSegmentos"
    //IGUAL A 0, ENQUANTO "contadorSegmentos" MENOR QUE 7, INCREMENTA "contadorSegmentos"
    digitalWrite(pino, seven_seg_digits[digit][contadorSegmentos]); //PERCORRE O ARRAY E LIGA OS
    //SEGMENTOS CORRESPONDENTES AO DIGITO
    --pino; //DECREMENTA "pino"
  }
}

void loop() {
  
}
