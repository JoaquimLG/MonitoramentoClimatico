#include <Arduino.h>
#include <DHT.h>
#include <WiFi.h>
#include <WebServer.h>

#define LEDVERMELHO 22
#define LEDAMARELO 21
#define DHTPIN 23        
#define DHTTYPE DHT11    
#define SENSORLUMINOSIDADE 35
#define POTENCIOMETRO 32

//DEFININDO O NOME DA REDE E SENHA DO PONTO DE ACESSO
const char* nomeRede = "Esp-32";
const char* senha = "senha1210";

WebServer server(80);

//CRIA O OBJETO DO SENSOR DE TEMPERATURA E UMIDADE
DHT dht(DHTPIN, DHTTYPE);

//INICIALIZANDO AS VARIAVEIS
float temperatura = 0.0;
float umidade = 0.0;
int sensorLuminosidade = 0;
int sensorPotenc = 0;

//FUNCAO DA PAGINA WEB
void paginaPrincipal() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>Status Climático</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      background: linear-gradient(to right, #74ebd5, #ACB6E5);
      color: #333;
      margin: 0;
      padding: 0;
    }
    header {
      background: rgba(255, 255, 255, 0.2);
      padding: 20px;
      box-shadow: 0 4px 6px rgba(0,0,0,0.1);
      border-bottom-left-radius: 15px;
      border-bottom-right-radius: 15px;
    }
    h1 {
      margin: 0;
      font-size: 28px;
      color: #fff;
    }
    .container {
      display: inline-block;
      margin-top: 30px;
      background: rgba(255, 255, 255, 0.8);
      padding: 20px 40px;
      border-radius: 15px;
      box-shadow: 0 4px 20px rgba(0,0,0,0.2);
    }
    p {
      font-size: 20px;
      margin: 15px 0;
    }
    span {
      font-weight: bold;
      color: #0077b6;
    }
  </style>
</head>
<body>
  <header>
    <h1>Status Climático</h1>
  </header>

  <div class="container">
    <p>🌡 Temperatura: <span id="temp">--</span> °C</p>
    <p>💧 Umidade: <span id="umid">--</span> %</p>
    <p>☀ Luminosidade: <span id="luz">--</span></p>
    <p>🏭 Poluição (simulada): <span id="pol">--</span> %</p>
  </div>

  <script>
    function atualizarDados() {
      fetch('/dados')
        .then(res => res.json())
        .then(data => {
          document.getElementById('temp').textContent = data.temperatura;
          document.getElementById('umid').textContent = data.umidade;
          document.getElementById('luz').textContent = data.luminosidade;
          document.getElementById('pol').textContent = data.poluicao;
        });
    }
    setInterval(atualizarDados, 2000);
    atualizarDados();
  </script>
</body>
</html>
)rawliteral";

  server.send(200, "text/html", html);
}

//ROTA PARA ENVIAR OS DADOS EM FORMATO JSON
void enviarDados() {
  String json = "{";
  json += "\"temperatura\":" + String(temperatura, 1) + ",";
  json += "\"umidade\":" + String(umidade, 1) + ",";
  json += "\"luminosidade\":" + String(sensorLuminosidade) + ",";
  json += "\"poluicao\":" + String(sensorPotenc) + "}";
  server.send(200, "application/json", json);
}

void setup() {
  //INICIALIZA O SERIAL
  Serial.begin(115200);
  //INICIALIZA O SENSOR DE TEMPERATURA E UMIDADE
  dht.begin();

  //CRIA O PONTO DE ACESSO E EXIBE O ENDERECO IP NO
  WiFi.softAP(nomeRede, senha);
  IPAddress ip = WiFi.softAPIP();
  Serial.print("Endereço IP: ");
  Serial.println(ip);

  //INICIALIZA O SERVIDOR 
  server.on("/", paginaPrincipal);
  server.on("/dados", enviarDados);
  server.begin();

  //DEFININDO OS PINOS 
  pinMode(LEDAMARELO, OUTPUT);
  pinMode(LEDVERMELHO, OUTPUT);
  pinMode(SENSORLUMINOSIDADE, INPUT);
  pinMode(POTENCIOMETRO, INPUT);
}

void loop() {
  server.handleClient();

  //RECEBE OS VALORES DAS VARIAVEIS 
  temperatura = dht.readTemperature();
  umidade = dht.readHumidity();
  sensorLuminosidade = analogRead(SENSORLUMINOSIDADE);
  sensorPotenc = analogRead(POTENCIOMETRO);
  
  //EXIBE OS VALORES NO MONITOR SERIAL
  Serial.printf("Temperatura: %.1f\n", temperatura);
  Serial.printf("Umidade: %.1f\n", umidade);
  Serial.printf("Luminosidade: %d\n", sensorLuminosidade);
  Serial.printf("Potenciometro: %d\n", sensorPotenc);

  //VERIFICA SE OS DADOS DE TEMPERATURA E UMIDADE FORAM RECEBIDOS CORRETAMENTE 
  if (!isnan(temperatura) && !isnan(umidade)) {
    if (temperatura > 30 || umidade < 40) { //VERIFICA SE OS VALORES ESTÃO ACIMA DO LIMITE E ACIONA UM LED VERMELHO
      digitalWrite(LEDVERMELHO, HIGH);
      delay(200);
      digitalWrite(LEDVERMELHO, LOW);
    }
  }

  if (sensorLuminosidade > 300) { //VERIFICA O VALOR DO SENSOR DE LUMINOSIDADE, > 300 ACIONA O LED AMARELO
    digitalWrite(LEDAMARELO, HIGH);
  } else {
    digitalWrite(LEDAMARELO,LOW);
  }
}
