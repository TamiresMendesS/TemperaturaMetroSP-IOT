/*
  Objetos Inteligentes Conectados 
  Projeto de IOT | Turma: 05J
  Pedro Giovannini Andrade - RA: 10420266
  Tamires Mendes da Silva - RA: 10420266
*/

// Bibliotecas
#include <WiFi.h> //https://www.arduinolibraries.info/libraries/wi-fi
#include <PubSubClient.h> //https://www.arduinolibraries.info/libraries/pub-sub-client

// Definições e Constantes
char SSIDName[] = "Wokwi-GUEST"; // nome da rede WiFi
char SSIDPass[] = ""; // senha da rede WiFI

// Configuração do BrokerMQTT
char BrokerURL[] = "broker.hivemq.com"; //URL do broker MQTT
char BrokerUserName[] = ""; //nome do usuario para autenticar no broker MQTT
char BrokerPassword[] = ""; //senha para autenticar no broker MQTT
char MQTTClientName[] = "mqtt-projiot"; //nome do cliente MQTT
int BrokerPort = 1883; //porta do broker MQTT

const int POT_PIN = 34; // Potenciometro
char Topico_01[] = "ProjetoIOT/Lotacao"; // Lotação = Qtde de passageiros no vagão
char Topico_02[] = "MACK10420266/Chave"; 

// Variáveis globais e objetos
WiFiClient espClient; 
PubSubClient clienteMQTT(espClient); 

// Função para reconectar ao broker
void mqttReconnect() {
  while (!clienteMQTT.connected()) {
    Serial.println("Conectando-se ao broker MQTT...");
    Serial.println(MQTTClientName);
    if (clienteMQTT.connect(MQTTClientName, BrokerUserName, BrokerPassword)) {
      Serial.print(MQTTClientName);
      Serial.println(" conectado!");
      clienteMQTT.subscribe(Topico_02);
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
}

// Loop
void loop() {
  if (!clienteMQTT.connected()) { 
    mqttReconnect(); 
  }
  clienteMQTT.loop(); 

  int ocupacao = analogRead(POT_PIN); // Valor do Potenciometro
  String lotacao; // Variável que vai printar o estado da lotação

  if (ocupacao < 1300) {
    lotacao = "Vazio";
  } else if (ocupacao < 2800) {
    lotacao = "Medio";
  } else {
    lotacao = "Lotado";
  }

  Serial.println("----------------------");
  Serial.print("Valor potenciometro: ");
  Serial.println(ocupacao);

  Serial.print("Lotacão: ");
  Serial.println(lotacao);
  Serial.print("----------------------");

  clienteMQTT.publish(Topico_01, String(ocupacao).c_str()); //deixar só esse

  delay(5000);
}