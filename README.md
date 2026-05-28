# Temperatura Metrô

<H6>Objetos Inteligentes Conectados<br>Pedro Giovannini Andrade | RA: 10425498<br>Tamires Mendes da Silva | RA: 10420266 </H6> 

## Sobre o Projeto

O projeto consiste em um sensor potenciômetro que simula a quantidade de passageiros em um vagão de trem. Esses dados são enviados ao Node-RED com auxílio de um BrokerMQTT (HiveMQ) e relacionados com dados da temperatura de São Paulo, por meio de uma API (OpenWeather) em tempo real.
A ideia é fazer uma relação entre a quantidade de passageiros e a temperatura da capital para ajustar a temperatura interna do vagão de forma automatizada.

## Motivação do Projeto

O desenvolvimento do projeto sucedeu em torno da temperatura ambiente dos vagões do transporte metroferroviário da cidade de São Paulo, uma pauta pouco levantada em torno do conforto no deslocamento durante trajetos realizados no dia-a-dia. Buscamos encontrar uma forma de unir a segurança e o conforto ao cotidiano do passageiro, sem interferir negativamente em sua jornada ao utilizar este meio de transporte. Seguindo a ODS número de 11 “Tornar as cidades e os assentamentos humanos inclusivos, seguros, resilientes e sustentáveis”, a ideia é utilizar sensores que possam monitorar a temperatura atual dos vagões e a adaptar de acordo com a quantidade de pessoas e da situação climática externa, da própria capital.

## Tecnologias Utilizadas

* ESP32 ¹
  * Potenciômetro
* ESP32 ²
  * LEDs (3 - RED, GREEN, BLUE)
  * SSD1306 OLED Display
* BrokerMQTT
  * HiveMQ
* AWS EC2
  * Node-RED
* InfluxDB
* Grafana

## Circuitos

<div>
  <img width="465" height="348" alt="Sensores" src="https://github.com/user-attachments/assets/a9dc29de-e159-4e27-8120-6742acd9f880" />
  <img width="465" height="348" alt="Atuadores" src="https://github.com/user-attachments/assets/e39e6676-3fd5-4ed4-a694-b338d121b95e" />
</div>

---
_Link apresentação YouTube:_
