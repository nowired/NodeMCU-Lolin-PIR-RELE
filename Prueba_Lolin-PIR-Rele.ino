// Incluimos las librerias que necesitamos
// Wifi
#include <ESP8266WiFi.h>
// Servidor Web
#include <ESP8266WebServer.h> 
// MQTT
#include <PubSubClient.h> 

// Definimos las variables a utilizar  
// Pin donde se encuentra conectado el Rele
#define PIN_RELE 10
// Pin donde se encuentra conectado el sensor PIR
#define PIN_PIR 4
// Habilitamos/Deshabilitamos el control a través de MQTT
#define SERVER_MQTT false
// Habilitamos/Deshabilitamos el sensor PIR para activar automáticamente el rele
#define PIR_ACTIVATED false
// Definimos el tiempo a esperar para apagar el rele tras detectar movimiento
const int TIEMPO_ENCENDIDO = 10000; //10 segundos 
// Nombre de la red wifi a la que nos conectamos
const char* ssid = "WiFi";
// Contraseña de la red wifi a la que nos conectamos
const char* pwd = "Password";

// Especificamos el puerto por defecto en el que escuchará el servidor web
ESP8266WebServer server(80); 
// comenzamos con el código
void setup()
{
  pinMode(PIN_RELE, OUTPUT);
  pinMode(PIN_PIR, INPUT_PULLUP);
  WiFi.begin(ssid, pwd);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }  
  Serial.begin(115200);
  delay(150);
  defineHTMLfunctions();
  
  if (SERVER_MQTT == true) {
  // definimos la ip del boker
   IPAddress mqtt_ip(192, 168, 0, 8);
   // definimos el puerto en el que escucha el servidor MQTT
   // topic al que nos suscribimos
   const char* mqtt_topic = "lolin";
   // Establecemos un cliente web de MQTT
   WiFiClient wifiClient;
   PubSubClient client(wifiClient);
   // Establecemos conexión con el servidor BOKER
   client.setServer(mqtt_ip,1883);
    client.publish(mqtt_topic, buffer);
  }
     
  Serial.println("\nWiFiconnected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  
}

void loop()
{
  digitalWrite(PIN_RELE, HIGH);
  ativaConMovimiento();
  server.handleClient();  
  delay(500);
}

void defineHTMLfunctions() {
  server.on("/",handleRoot);
  server.on("/toggle",changeRele);
  server.begin();
}

void ativaConMovimiento(){
  if (PIR_ACTIVATED == true) {
    int value= digitalRead(PIN_RELE);
    if (digitalRead(PIN_PIR))
    {
      activarRele();
      delay(TIEMPO_ENCENDIDO);
      desactivarRele();
    }
  }
}

void handleRoot()
{
  const char* htmlText = "";
  //Handler for the rooth path 
  if (digitalRead(PIN_RELE) == LOW) {
    htmlText = "<!DOCTYPE html><html><head><title>NodeMCU Lolin Domology.es - Control Rele desde sensor PIR</title><meta charset='utf-8' /><meta content='Carlos Gonzalez' name='author' /><meta content='Control de rele a traves de un sensor PIR' name='description' /><style>a {text-decoration: none; color: #ffffff} .button { display: inline-block; border-radius: 4px; background-color: #f4511e; border: none; color: #FFFFFF; text-align: center; font-size: 28px; padding: 20px; width: 75%; transition: all 0.5s; cursor: pointer; margin: 5px; } .button a { cursor: pointer; display: inline-block; position: relative; transition: 0.5s; } .button a:after { content: '\\00bb'; position: absolute; opacity: 0;top: 0;right: -20px;transition: 0.5s; } .button:hover a { padding-right: 25px; } .button:hover a:after { opacity: 1; right: 0; }</style></head><body><div class='button' style='vertical-align:middle'><a href='/toggle'>Activar</a></div></body></html>";
  }
  else {
    htmlText = "<!DOCTYPE html><html><head><title>NodeMCU Lolin Domology.es - Control Rele desde sensor PIR</title><meta charset='utf-8' /><meta content='Carlos Gonzalez' name='author' /><meta content='Control de rele a traves de un sensor PIR' name='description' /><style>a {text-decoration: none; color: #ffffff} .button { display: inline-block; border-radius: 4px; background-color: #f4511e; border: none; color: #FFFFFF; text-align: center; font-size: 28px; padding: 20px; width: 75%; transition: all 0.5s; cursor: pointer; margin: 5px; } .button a { cursor: pointer; display: inline-block; position: relative; transition: 0.5s; } .button a:after { content: '\\00bb'; position: absolute; opacity: 0;top: 0;right: -20px;transition: 0.5s; } .button:hover a { padding-right: 25px; } .button:hover a:after { opacity: 1; right: 0; }</style></head><body><div class='button' style='vertical-align:middle'><a href='/toggle'>Desactivar</a></div></body></html>";
  }
  server.send(200, "text/html", htmlText); 
} 

void changeRele()
{
  if (digitalRead(PIN_RELE) == LOW){
    activarRele();
  }
  else {
    desactivarRele();
  }
}
void activarRele()
{
  Serial.print("Activa Relé orden: ");
  digitalWrite(PIN_RELE, HIGH);
  Serial.println("HIGH");
  handleRoot();
}
void desactivarRele()
{
  Serial.print("Desactiva Relé orden: ");
  digitalWrite(PIN_RELE, LOW);
  Serial.println("LOW");
  handleRoot();
}



