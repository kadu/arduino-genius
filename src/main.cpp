#include <Arduino.h>
#include "OneButton.h"

#define VERMELHO 1
#define VERDE    2
#define AMARELO  3
#define AZUL     4

#define notaVermelho 150
#define notaVerde    350
#define notaAmarelo  550
#define notaAzul     800
#define notaERRO     1100

#define ledVermelho 12
#define botaoVermelho 13

#define ledVerde 10
#define botaoVerde 11

#define ledAmarelo 8
#define botaoAmarelo 9

#define ledAzul 6
#define botaoAzul 7

#define BUZZER 5


// Controladores dos botoes
OneButton bVermelho(botaoVermelho, true);
OneButton bVerde(botaoVerde, true);
OneButton bAmarelo(botaoAmarelo, true);
OneButton bAzul(botaoAzul, true);

int intervalo=500; //specified in milliseconds
int musica[30];
int verificaMusica[30];

int fase = 1;
bool estouEsperando = false;
int botaoFoiPressionado = -1;
int botaoPressionado = 0;

void(* resetFunc) (void) = 0; 

// Verifica se o usuario clicou no botao esperado, se não for o esperado, da erro e reinicia o MCU
void verifica() {  
  if(musica[botaoPressionado-1] != botaoFoiPressionado) {
    Serial.println("!!Errou");
    tone(BUZZER, notaERRO);
    for (size_t i = 0; i < 2; i++) {
      digitalWrite(ledVermelho, HIGH);
      digitalWrite(ledVerde, HIGH);
      digitalWrite(ledAmarelo, HIGH);
      digitalWrite(ledAzul, HIGH);
      delay(250);
      digitalWrite(ledVermelho, LOW);
      digitalWrite(ledVerde, LOW);
      digitalWrite(ledAmarelo, LOW);
      digitalWrite(ledAzul, LOW);
      delay(250);
    }
    noTone(BUZZER);
    delay(500);
    resetFunc();
  }
}

// Cria musicas aleatoriamete
void cria_musica() {
  for (size_t i = 0; i < 30; i++) {    
    musica[i] = random(1,5);    
  }
}

void play(int COLOR=VERMELHO, int delayTime = intervalo) {
  int _PLAYNOTE = notaVermelho;
  int _LED = ledVermelho;
  
  switch (COLOR)
  {
    case VERMELHO:
      _PLAYNOTE = notaVermelho;
      _LED = ledVermelho;      
      break;
    case VERDE:
      _PLAYNOTE = notaVerde;
      _LED = ledVerde;
      break;
    case AMARELO:
      _PLAYNOTE = notaAmarelo;
      _LED = ledAmarelo;
      break;
    case AZUL:
      _PLAYNOTE = notaAzul;
      _LED = ledAzul;
      break;

    default:
      break;
  }

  Serial.print("COR => ");
  Serial.println(COLOR);
  
  tone(BUZZER, _PLAYNOTE);
  digitalWrite(_LED, HIGH);
  delay(delayTime);
  noTone(BUZZER);
  digitalWrite(_LED, LOW);
  delay(delayTime);
}

void vermelhoClick() {
  Serial.println("bVermelho");        
  botaoFoiPressionado = VERMELHO;
  
  botaoPressionado++;
  verifica();
  play(VERMELHO);
};

void verdeClick() {
  Serial.println("bVerde");
  botaoFoiPressionado = VERDE;
  botaoPressionado++;
  verifica();
  play(VERDE);
};

void amareloClick() {
  Serial.println("bAmarelo");
  botaoFoiPressionado = AMARELO;
  botaoPressionado++;
  verifica();
  play(AMARELO);
};

void azulClick() {
  Serial.println("bAzul");
  botaoFoiPressionado = AZUL;
  botaoPressionado++;
  verifica();
  play(AZUL);
};

void setup() {
  Serial.begin(9600);
  Serial.println("Genius");

  pinMode(ledVermelho, OUTPUT); 
  pinMode(ledVerde, OUTPUT); 
  pinMode(ledAmarelo, OUTPUT); 
  pinMode(ledAzul, OUTPUT); 

  bVermelho.attachClick(vermelhoClick);
  bVerde.attachClick(verdeClick);
  bAmarelo.attachClick(amareloClick);
  bAzul.attachClick(azulClick);
  
  bVermelho.setDebounceTicks(20);
  bVerde.setDebounceTicks(20);
  bAmarelo.setDebounceTicks(20);
  bAzul.setDebounceTicks(20);

  //#Buzzer
  pinMode(BUZZER, OUTPUT);     
  randomSeed(analogRead(A0));

  for (size_t i = 0; i < 30; i++) {
    verificaMusica[i] = -1;
  }

  // Inicia jogo
  cria_musica(); 
  play(VERMELHO,100);
  play(VERDE,100);
  play(AMARELO,100);
  play(AZUL,100);
  play(VERMELHO,50);
  play(VERDE,50);
  play(AMARELO,50);
  play(AZUL,50);
  delay(1500);
  Serial.println("Starting game...");
}

void jogo() {
  if(!estouEsperando) {
    for (int i = 0; i < fase; i++) {
      play(musica[i]);
    }
    estouEsperando = true;
  } else {
      if(botaoFoiPressionado != -1) {
        verificaMusica[botaoPressionado-1] = botaoFoiPressionado;
        if(botaoPressionado == fase) {
            estouEsperando = false;
            botaoFoiPressionado = -1;
            fase++;
            botaoPressionado = 0;
            delay(50);
        }
      }
  }
  delay(100);
}

void loop() {
  bVermelho.tick();
  bVerde.tick();
  bAmarelo.tick();
  bAzul.tick();
  jogo();
} 
