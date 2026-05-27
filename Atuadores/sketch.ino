/*
  Objetos Inteligentes Conectados 
  Projeto de IOT | Turma: 05J
  Pedro Giovannini Andrade - RA: 10420266
  Tamires Mendes da Silva - RA: 10420266
*/

// Bibliotecas
#include <WiFi.h> //https://www.arduinolibraries.info/libraries/wi-fi
#include <PubSubClient.h> //https://www.arduinolibraries.info/libraries/pub-sub-client
// Bibliotecas relacionadas ao OLED - design e config
#include <Wire.h> 
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Definições do OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Definições dos LEDs
#define LEDV_VERM 25 // LED vermelho
#define LEDV_VERD 26 // LED verde
#define LEDV_AZUL 27 // LED azul

// Definições e Constantes
char SSIDName[] = "Wokwi-GUEST"; //nome da rede WiFi
char SSIDPass[] = ""; //senha da rede WiFI

// Configuração do BrokerMQTT
char BrokerURL[] = "broker.hivemq.com"; //URL do broker MQTT
char BrokerUserName[] = ""; //nome do usuario para autenticar no broker MQTT
char BrokerPassword[] = ""; //senha para autenticar no broker MQTT
char MQTTClientName[] = "mqtt-mack-pub-sub"; //nome do cliente MQTT
int BrokerPort = 1883; //porta do broker MQTT

// Variáveis globais e objetos
WiFiClient espClient; //instancia o objeto espClient que pode se conectar a um endereço IP e porta de Internet especificados, conforme definido em clienteMQTT.connect()
PubSubClient clienteMQTT(espClient); //instancia o objeto clienteMQTT parcialmente inicializado cujo argumento eh o objeto para conectar na Internet

// Função para configurar design do OLED com a temperatura
void atualizaOLED(float temp) {
  display.clearDisplay(); 
  
  display.setTextSize(1.5);   
  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(temp, 1);   
  display.print(" C");
  
  display.setTextSize(1);
  display.setCursor(0, 50);
  
  // Mensagem de status baseada na temp
  if(temp < 20) {
    display.print("FRIO");
  } else if(temp <= 24) {
    display.print("IDEAL");
  } else { 
    display.print("QUENTE");
  }

  display.display();
}

// Função que aguarda ser chamada para receber mensagem do MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  float tempVagao = msg.toFloat();
  Serial.print("Temp Vagao: ");
  Serial.println(tempVagao);

  // RESET: Apaga todos antes de decidir qual ligar
  analogWrite(LEDV_AZUL, 0);
  analogWrite(LEDV_VERD, 0);
  analogWrite(LEDV_VERM, 0);

  // Lógica de cores 
  if (tempVagao < 20) {
    Serial.println("LED AZUL");
    analogWrite(LEDV_AZUL, 255);
  }
  else if (tempVagao = 24) {
    Serial.println("LED VERDE");
    analogWrite(LEDV_VERD, 255);
  }
  else {
    Serial.println("LED VERMELHO");
    analogWrite(LEDV_VERM, 255);
  }

  atualizaOLED(tempVagao);
}

// Função para reconectar ao broker
void mqttReconnect() {
  while (!clienteMQTT.connected()) {
    Serial.println("Conectando-se ao broker MQTT...");
    Serial.println(MQTTClientName);
    if (clienteMQTT.connect(MQTTClientName, BrokerUserName, BrokerPassword)) {
      Serial.print(MQTTClientName);
      Serial.println(" conectado!");
      clienteMQTT.subscribe("ProjetoIOT/TemperaturaVagao");
    } else {
      Serial.print("failed, rc=");
      Serial.print(clienteMQTT.state());
      Serial.println(" tente novamente em 5 segundos.");
      delay(5000);
    }
  }
}

// Setup
void setup() {
  clienteMQTT.setServer(BrokerURL, BrokerPort);
  clienteMQTT.setCallback(callback);
  Serial.begin(9600);
  Serial.print("Conectando-se ao Wi-Fi");

  WiFi.begin(SSIDName, SSIDPass); 
  while (WiFi.status() != WL_CONNECTED) { 
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP()); 

  // Inicializa o OLED no endereço 0x3C
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("Falha ao iniciar o OLED"));
  }
  // OLED
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Sistema Iniciado");
  display.display();

  // LEDs
  pinMode(LEDV_VERM, OUTPUT);
  pinMode(LEDV_VERD, OUTPUT);
  pinMode(LEDV_AZUL, OUTPUT);

  analogWrite(LEDV_VERM, 0);
  analogWrite(LEDV_VERD, 0);
  analogWrite(LEDV_AZUL, 0);
}

// Loop
void loop() {
  if (!clienteMQTT.connected()) { 
    mqttReconnect(); 
  }
  clienteMQTT.loop(); 
}