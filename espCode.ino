/* BIBLIOTECAS */
#include "WiFiEsp.h"
#include "SoftwareSerial.h"

/* PARÂMETROS */
#define IP 192,168,113,181
#define vel_m_e A1
#define vel_m_d A0
#define MD_H 11
#define MD_AH 10
#define ME_H 9
#define ME_AH 8
#define temp 1000

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
  pinMode(MD_H, OUTPUT);
  pinMode(MD_AH, OUTPUT);
  pinMode(ME_H, OUTPUT);
  pinMode(ME_AH, OUTPUT);

  pinMode(vel_m_e, OUTPUT);
  pinMode(vel_m_d, OUTPUT);

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

  //DEFINE VELOCIDADES DOS MOTORES ESQUERDO E DIREITO
  analogWrite(vel_m_e, 254);
  analogWrite(vel_m_d, 140);
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
          client.println("<h1>RoboCoffee pronto</h1>");
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
          client.println("<p>Rotina LESTE Iniciada</p>");
          client.println("<hr/>");
          client.println("</body>"); 
          client.println("</html>");
          //VIRA PARA DIREITA
          digitalWrite(MD_H, LOW);
          digitalWrite(MD_AH, LOW);
          digitalWrite(ME_H, HIGH);
          digitalWrite(ME_AH, LOW);
          delay(540);
          //PAUSA
          digitalWrite(MD_H, LOW);
          digitalWrite(MD_AH, LOW);
          digitalWrite(ME_H, LOW);
          digitalWrite(ME_AH, LOW);
          delay(temp);
          //AVANÇA 
          digitalWrite(MD_H, HIGH);
          digitalWrite(MD_AH, LOW);
          digitalWrite(ME_H, HIGH);
          digitalWrite(ME_AH, LOW);
          delay(2000);
          //PAUSA
          digitalWrite(MD_H, LOW);
          digitalWrite(MD_AH, LOW);
          digitalWrite(ME_H, LOW);
          digitalWrite(ME_AH, LOW);
          delay(temp);
          //VIRA PARA ESQUERDA
          digitalWrite(MD_H, HIGH);
          digitalWrite(MD_AH, LOW);
          digitalWrite(ME_H, LOW);
          digitalWrite(ME_AH, LOW);
          delay(600);
          //PAUSA
          digitalWrite(MD_H, LOW);
          digitalWrite(MD_AH, LOW);
          digitalWrite(ME_H, LOW);
          digitalWrite(ME_AH, LOW);
          delay(temp);
          //AVANÇA 
          digitalWrite(MD_H, HIGH);
          digitalWrite(MD_AH, LOW);
          digitalWrite(ME_H, HIGH);
          digitalWrite(ME_AH, LOW);
          delay(500);
          //PAUSA
          digitalWrite(MD_H, LOW);
          digitalWrite(MD_AH, LOW);
          digitalWrite(ME_H, LOW);
          digitalWrite(ME_AH, LOW);
          delay(temp);
          //VIRA PARA ESQUERDA
          digitalWrite(MD_H, HIGH);
          digitalWrite(MD_AH, LOW);
          digitalWrite(ME_H, LOW);
          digitalWrite(ME_AH, LOW);
          delay(600);
          //PAUSA
          digitalWrite(MD_H, LOW);
          digitalWrite(MD_AH, LOW);
          digitalWrite(ME_H, LOW);
          digitalWrite(ME_AH, LOW);
          delay(temp);
          //AVANÇA
          digitalWrite(MD_H, HIGH);
          digitalWrite(MD_AH, LOW);
          digitalWrite(ME_H, HIGH);
          digitalWrite(ME_AH, LOW);
          delay(4000);
          //PAUSA
          digitalWrite(MD_H, LOW);
          digitalWrite(MD_AH, LOW);
          digitalWrite(ME_H, LOW);
          digitalWrite(ME_AH, LOW);
          client.println("<p>Rotina LESTE Finalizada</p>");
          client.stop();
        }
        else  if(buf.endsWith("GET /O")) { //SE O PARÂMETRO DA REQUISIÇÃO VINDO POR GET FOR IGUAL A "O", EXECUTA ROTINA OESTE
                client.println("HTTP/1.1 200 OK");
                client.println("Content-Type: text/html");
                client.println("");
                client.println("<!DOCTYPE HTML>");
                client.println("<html>");
                client.println("<head>");
                client.println("</head>");
                client.println("<body>");
                client.println("<p>Rotina OESTE Iniciada</p>");
                client.println("<hr/>");
                client.println("</body>"); 
                client.println("</html>");
                //VIRA PARA ESQUERDA
                digitalWrite(MD_H, HIGH);
                digitalWrite(MD_AH, LOW);
                digitalWrite(ME_H, LOW);
                digitalWrite(ME_AH, LOW);
                delay(540);
                //PAUSA
                digitalWrite(MD_H, LOW);
                digitalWrite(MD_AH, LOW);
                digitalWrite(ME_H, LOW);
                digitalWrite(ME_AH, LOW);
                delay(temp);
                //AVANÇA 
                digitalWrite(MD_H, HIGH);
                digitalWrite(MD_AH, LOW);
                digitalWrite(ME_H, HIGH);
                digitalWrite(ME_AH, LOW);
                delay(2000);
                //PAUSA
                digitalWrite(MD_H, LOW);
                digitalWrite(MD_AH, LOW);
                digitalWrite(ME_H, LOW);
                digitalWrite(ME_AH, LOW);
                delay(temp);
                //VIRA PARA DIREITA
                digitalWrite(MD_H, LOW);
                digitalWrite(MD_AH, LOW);
                digitalWrite(ME_H, HIGH);
                digitalWrite(ME_AH, LOW);
                delay(600);
                //PAUSA
                digitalWrite(MD_H, LOW);
                digitalWrite(MD_AH, LOW);
                digitalWrite(ME_H, LOW);
                digitalWrite(ME_AH, LOW);
                delay(temp);
                //AVANÇA 
                digitalWrite(MD_H, HIGH);
                digitalWrite(MD_AH, LOW);
                digitalWrite(ME_H, HIGH);
                digitalWrite(ME_AH, LOW);
                delay(500);
                //PAUSA
                digitalWrite(MD_H, LOW);
                digitalWrite(MD_AH, LOW);
                digitalWrite(ME_H, LOW);
                digitalWrite(ME_AH, LOW);
                delay(temp);
                //VIRA PARA DIREITA
                digitalWrite(MD_H, LOW);
                digitalWrite(MD_AH, LOW);
                digitalWrite(ME_H, HIGH);
                digitalWrite(ME_AH, LOW);
                delay(600);
                //PAUSA
                digitalWrite(MD_H, LOW);
                digitalWrite(MD_AH, LOW);
                digitalWrite(ME_H, LOW);
                digitalWrite(ME_AH, LOW);
                delay(temp);
                //AVANÇA
                digitalWrite(MD_H, HIGH);
                digitalWrite(MD_AH, LOW);
                digitalWrite(ME_H, HIGH);
                digitalWrite(ME_AH, LOW);
                delay(4000);
                //PAUSA
                digitalWrite(MD_H, LOW);
                digitalWrite(MD_AH, LOW);
                digitalWrite(ME_H, LOW);
                digitalWrite(ME_AH, LOW);
                client.println("<p>Rotina OESTE Finalizada</p>");
                client.stop();
              }
              else  if(buf.endsWith("GET /I")) { //SE O PARÂMETRO DA REQUISIÇÃO VINDO POR GET FOR IGUAL A "O", EXECUTA ROTINA OESTE
                      client.println("HTTP/1.1 200 OK");
                      client.println("Content-Type: text/html");
                      client.println("");
                      client.println("<!DOCTYPE HTML>");
                      client.println("<html>");
                      client.println("<head>");
                      client.println("</head>");
                      client.println("<body>");
                      client.println("<p>Rotina INTERMEDIÁRIA iniciada</p>");
                      client.println("<hr/>");
                      client.println("</body>"); 
                      client.println("</html>");
                      //AVANÇA 
                      digitalWrite(MD_H, HIGH);
                      digitalWrite(MD_AH, LOW);
                      digitalWrite(ME_H, HIGH);
                      digitalWrite(ME_AH, LOW);
                      delay(3000);
                      //PAUSA
                      digitalWrite(MD_H, LOW);
                      digitalWrite(MD_AH, LOW);
                      digitalWrite(ME_H, LOW);
                      digitalWrite(ME_AH, LOW);
                      delay(temp);
                      //VIRA PARA DIREITA
                      digitalWrite(MD_H, LOW);
                      digitalWrite(MD_AH, LOW);
                      digitalWrite(ME_H, HIGH);
                      digitalWrite(ME_AH, LOW);
                      delay(600);
                      //PAUSA
                      digitalWrite(MD_H, LOW);
                      digitalWrite(MD_AH, LOW);
                      digitalWrite(ME_H, LOW);
                      digitalWrite(ME_AH, LOW);
                      delay(temp);
                      //AVANÇA 
                      digitalWrite(MD_H, HIGH);
                      digitalWrite(MD_AH, LOW);
                      digitalWrite(ME_H, HIGH);
                      digitalWrite(ME_AH, LOW);
                      delay(500);
                      //PAUSA
                      digitalWrite(MD_H, LOW);
                      digitalWrite(MD_AH, LOW);
                      digitalWrite(ME_H, LOW);
                      digitalWrite(ME_AH, LOW);
                      delay(temp);
                      //VIRA PARA DIREITA
                      digitalWrite(MD_H, LOW);
                      digitalWrite(MD_AH, LOW);
                      digitalWrite(ME_H, HIGH);
                      digitalWrite(ME_AH, LOW);
                      delay(600);
                      //PAUSA
                      digitalWrite(MD_H, LOW);
                      digitalWrite(MD_AH, LOW);
                      digitalWrite(ME_H, LOW);
                      digitalWrite(ME_AH, LOW);
                      delay(temp);
                      //AVANÇA
                      digitalWrite(MD_H, HIGH);
                      digitalWrite(MD_AH, LOW);
                      digitalWrite(ME_H, HIGH);
                      digitalWrite(ME_AH, LOW);
                      delay(3000);
                      //PAUSA
                      digitalWrite(MD_H, LOW);
                      digitalWrite(MD_AH, LOW);
                      digitalWrite(ME_H, LOW);
                      digitalWrite(ME_AH, LOW);
                      client.println("<p>Rotina OESTE Finalizada</p>");
                      client.stop();
                    }
              
      }
    }
  }
}