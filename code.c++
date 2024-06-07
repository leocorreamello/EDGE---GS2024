#include <HX711.h>
#include <LiquidCrystal_I2C.h>

// Variáveis do sensor ultrassônico
int pinEcho = 3;
int pinTrig = 2;
float dist = 0;

// Variáveis da balança
const int pinSCK = A0;
const int pinDT = A1;

// Variáveis LCD
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Setando a escala da balança "Calibrando"
HX711 scale;

String capacidade = "NaN";
float peso = 0;

void setup() {
  // Inicializando LCD
  lcd.init();
  lcd.backlight();

  // Declarando as portas.
  pinMode(pinTrig, OUTPUT);
  pinMode(pinEcho, INPUT);
  
  // Inicializando o serial em 57600
  Serial.begin(57600);
  Serial.println("Inicializando a escala");
  
  // Setando as portas da balança
  scale.begin(pinDT, pinSCK);
  scale.set_scale(420);
  delay(1000);
  scale.tare();
}

void loop() {
  // Disparando o sensor
  digitalWrite(pinTrig, LOW);
  delayMicroseconds(5);
  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrig, LOW);
  
  // Pegando o valor da distância pelo echo
  dist = pulseIn(pinEcho, HIGH);
  dist = dist / 58;
  
  // Pegando o valor do peso
  peso = scale.get_units(), 1;
  
  // Ajustando a distância com base no peso
  if (peso > 0) {
    dist = map(peso, 0, 50, 400, 0); // Mapeando o peso de 0 a 50 kg para uma distância de 400 a 0 cm
    dist = constrain(dist, 0, 400); // Garantindo que a distância esteja entre 0 e 400 cm
  } else {
    dist = 400; // Distância máxima se não houver peso
  }
  
  // Imprimindo os valores no serial
  Serial.print("Distância: ");
  Serial.print(dist);
  Serial.println(" cm");
  Serial.print("Peso: ");
  Serial.print(peso);
  Serial.println(" kg");

  // Determinando a capacidade com base no peso
  if (peso <= 10) {
    capacidade = "0%-25%";
  } else if (peso <= 20) {
    capacidade = "25%-50%";
  } else if (peso <= 30) {
    capacidade = "50%-75%";
  } else {
    capacidade = "75%-100%";
  }

  // Imprimindo os dados no LCD
  lcd.setCursor(0, 0);
  lcd.print("Dist: " + String(dist) + " cm");
  lcd.setCursor(0, 1);
  lcd.print("Peso: " + String(peso) + " kg");
  lcd.setCursor(0, 2);
  lcd.print("Capacidade: " + String(capacidade));

  // Delay do loop
  delay(3000);
}
