/* BIBLIOTECAS */
#include "WiFiEsp.h"
#include "SoftwareSerial.h"

/* PARÂMETROS */
#define IP 192,168,220,181
#define velmotor 3
#define IN1 11
#define IN2 10
#define IN3 9
#define IN4 8
#define temp 1000
int vel = 0;

extern char ssid[];
extern char pass[];

SoftwareSerial Serial_pins(6, 7); //OS PINOS 6 E 7 EMULAM, RESPECTIVAMENTE, O TX E O RX PARA COMUNICAÇÃO SERIAL

int status = WL_IDLE_STATUS; 
/*STATUS TEMPORÁRIO ATRIBUÍDO QUANDO O WIFI É INICIALIZADO E PERMANECE ATIVO
ATÉ QUE O NÚMERO DE TENTATIVAS EXPIRE (RESULTANDO EM WL_NO_SHIELD) OU QUE UMA CONEXÃO SEJA ESTABELECIDA
(RESULTANDO EM WL_CONNECTED)*/

WiFiEspServer server(8080); //CONEXÃO REALIZADA NA PORTA 80

RingBuffer buf(8); //BUFFER PARA AUMENTAR A VELOCIDADE E REDUZIR A ALOCAÇÃO DE MEMÓRIA

void setup(){
  /* DEFINIÇÃO DOS PINOS */
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.begin(9600); //INICIALIZA A SERIAL
  Serial_pins.begin(9600); //INICIALIZA A SERIAL PARA O ESP8266
  WiFi.init(&Serial_pins); //INICIALIZA A COMUNICAÇÃO SERIAL COM O ESP8266
  WiFi.config(IPAddress(IP)); //COLOQUE UMA FAIXA DE IP DISPONÍVEL DO SEU ROTEADOR

  //INÍCIO - VERIFICA SE O ESP8266 ESTÁ CONECTADO AO ARDUINO, CONECTA A REDE SEM FIO E INICIA O WEBSERVER
  if(WiFi.status() == WL_NO_SHIELD){
    while (true);
    Serial.print(".");
  }
  while(status != WL_CONNECTED){
    status = WiFi.begin(ssid, pass);
  }
  server.begin();
  //FIM - VERIFICA SE O ESP8266 ESTÁ CONECTADO AO ARDUINO, CONECTA A REDE SEM FIO E INICIA O WEBSERVER
}

void loop(){
  WiFiEspClient client = server.available(); //ATENDE AS SOLICITAÇÕES DO CLIENTE
  if (client) { //SE CLIENTE TENTAR SE CONECTAR, FAZ
    buf.init(); //INICIALIZA O BUFFER
    while (client.connected()){ //ENQUANTO O CLIENTE ESTIVER CONECTADO, FAZ
      if(client.available()){ //SE EXISTIR REQUISIÇÃO DO CLIENTE, FAZ
        char c = client.read(); //LÊ A REQUISIÇÃO DO CLIENTE
        buf.push(c); //BUFFER ARMAZENA A REQUISIÇÃO

        //IDENTIFICA O FIM DA REQUISIÇÃO HTTP E ENVIA UMA RESPOSTA
        if(buf.endsWith("\r\n\r\n")) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("");
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head>");
          client.println("</head>");
          client.println("<body>");
          client.println("<h1>Programa Funcionando</h1>");
          client.println("<hr />");
          client.println("</body>"); 
          client.println("</html>");
          client.stop();
          break;

        }
        if(buf.endsWith("GET /L")) { //SE O PARÂMETRO DA REQUISIÇÃO VINDO POR GET FOR IGUAL A "L", INICIA A ROTINA LESTE
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("");
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head>");
          client.println("</head>");
          client.println("<body>");
          client.println("<h1>Carro ligado</h1>");
          client.println("<hr />");
          client.println("</body>"); 
          client.println("</html>");
          delay(1000);
          digitalWrite(IN1, HIGH);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, LOW);
          client.stop();
        }
        else{ //SENÃO, FAZ
        if (buf.endsWith("GET /S")) { //SE O PARÂMETRO DA REQUISIÇÃO VINDO POR GET FOR IGUAL A "S", PARA O MOVIMENTO DO CARRINHO
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("");
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head>");
          client.println("</head>");
          client.println("<body>");
          client.println("<h1>Carro desligado</h1>");
          client.println("<hr />");
          client.println("</body>"); 
          client.println("</html>");
          delay(1000);
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, LOW);
          client.stop();
          }
        }
      }
    }
  }
}

//MÉTODO DE RESPOSTA A REQUISIÇÃO HTTP DO CLIENTE
void sendHttpResponse(WiFiEspClient client){
  client.println("HTTP/1.1 200 OK"); //ESCREVE PARA O CLIENTE A VERSÃO DO HTTP
  client.println("Content-Type: text/html"); //ESCREVE PARA O CLIENTE O TIPO DE CONTEÚDO(texto/html)
  client.println("");
  client.println("<!DOCTYPE HTML>"); //INFORMA AO NAVEGADOR A ESPECIFICAÇÃO DO HTML
  client.println("<html>"); //ABRE A TAG "html"
  client.println("<head>"); //ABRE A TAG "head"
  client.println("<link rel='icon' type='image/png' href='https://blogmasterwalkershop.com.br/arquivos/artigos/sub_wifi/icon_mws.png'/>"); //DEFINIÇÃO DO ICONE DA PÁGINA
  client.println("<link rel='stylesheet' type='text/css' href='https://blogmasterwalkershop.com.br/arquivos/artigos/sub_wifi/webpagecss.css' />"); //REFERENCIA AO ARQUIVO CSS (FOLHAS DE ESTILO)
  client.println("<title>MasterWalker Shop - Adaptador Conector 3.3V / 5V para o Modulo WiFi ESP8266 ESP-01</title>"); //ESCREVE O TEXTO NA PÁGINA
  client.println("</head>"); //FECHA A TAG "head"
  
  //AS LINHAS ABAIXO CRIAM A PÁGINA HTML
  client.println("<body>"); //ABRE A TAG "body"
  client.println("<img alt='masterwalkershop' src='https://blogmasterwalkershop.com.br/arquivos/artigos/sub_wifi/logo_mws.png' height='156' width='700' />"); //LOGO DA MASTERWALKER SHOP
  client.println("<p style='line-height:2'><font>Adaptador Conector 3.3V / 5V para o Modulo WiFi ESP8266 ESP-01</font></p>"); //ESCREVE O TEXTO NA PÁGINA
  client.println("<font>ESTADO ATUAL DO LED</font>"); //ESCREVE O TEXTO NA PÁGINA
  
  if (statusLed == HIGH){ //SE VARIÁVEL FOR IGUAL A HIGH (1), FAZ
    client.println("<p style='line-height:0'><font color='green'>LIGADO</font></p>"); //ESCREVE "LIGADO" NA PÁGINA
    client.println("<a href=\"/L\">APAGAR</a>"); //COMANDO PARA APAGAR O LED (PASSA O PARÂMETRO /L)
  }else{ //SENÃO, FAZ
    if (statusLed == LOW){ //SE VARIÁVEL FOR IGUAL A LOW (0), FAZ
    client.println("<p style='line-height:0'><font color='red'>DESLIGADO</font></p>"); //ESCREVE "DESLIGADO" NA PÁGINA
    client.println("<a href=\"/H\">ACENDER</a>"); //COMANDO PARA ACENDER O LED (PASSA O PARÂMETRO /H)
    }
  }
  client.println("<hr />"); //TAG HTML QUE CRIA UMA LINHA NA PÁGINA
  client.println("<hr />"); //TAG HTML QUE CRIA UMA LINHA NA PÁGINA
  client.println("</body>"); //FECHA A TAG "body"
  client.println("</html>"); //FECHA A TAG "html"
  delay(1); //INTERVALO DE 1 MILISSEGUNDO
}