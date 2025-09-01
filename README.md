# Monitoramento Ambiental em Tempo Real

Este projeto implementa um sistema de monitoramento climático utilizando o microcontrolador ESP32 e sensores de baixo custo. Os dados coletados (temperatura, umidade, luminosidade e poluição simulada) são exibidos em tempo real em uma página web hospedada pelo próprio ESP32.

## Funcionalidades

- Criação de um ponto de acesso Wi-Fi pelo ESP32.
- Leitura de temperatura e umidade com o sensor DHT11.
- Medição de luminosidade com um LDR.
- Simulação da qualidade do ar utilizando um potenciômetro.
- Exibição em uma interface web interativa.
- LED vermelho acende em situações críticas (temperatura > 30 °C ou umidade < 40%).
- LED amarelo indica níveis de luminosidade acima do limite configurado.
- Atualização dinâmica da página web a cada 2 segundos (sem precisar recarregar).

## Hardware Utilizado

- ESP32
- Protoboard
- Jumpers
- LEDs sinalizadores
- Resistores
- Sensor DHT11 (temperatura e umidade)
- LDR (luminosidade)
- Potenciômetro (simulação de poluição)

## Software Utilizado

- PlatformIO integrado ao VS Code
- Biblioteca DHT sensor library
- Bibliotecas WiFi e WebServer (nativas do ESP32)
- Navegador web para exibição dos dados

## Circuito

- O sistema foi montado em uma protoboard, interligando o ESP32, os sensores e LEDs de alerta.

## Resultados

- O sistema funcionou conforme esperado, exibindo os dados em tempo real.
- LEDs atuaram corretamente em condições críticas.
- Interface web apresentou informações de forma clara e intuitiva.
- Poluição foi simulada com potenciômetro, mas o sistema pode ser expandido com sensores reais.
