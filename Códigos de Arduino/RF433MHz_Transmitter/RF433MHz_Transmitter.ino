//Librerias
#include "DHT.h"
#include <VirtualWire.h>

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);


void setup()
{
  //Se inicia el sensor
  dht.begin(); 
  // Se inicializa el RF
  vw_setup(2000); // velocidad: Bits per segundo
}

void loop()
{
  float h = dht.readHumidity(); //Se lee la humedad
  float t = dht.readTemperature(); //Se lee la temperatura

  char buf[VW_MAX_MESSAGE_LEN]; // Cadena para enviar
  String str = "";

  str = "i" + String(h); //convertimos el float a String y agregamos un inicio de trama
  str.toCharArray(buf, sizeof(buf)); //convertimos el String en un array
  vw_send((uint8_t *)buf, strlen(buf)); //Enviamos el array
  vw_wait_tx(); //Esperamos hasta que el mensaje se envie

  str = "f" + String(t); //convertimos el float a String y agramos un inicio de trama
  str.toCharArray(buf, sizeof(buf)); //convertimos el String en un array
  vw_send((uint8_t *)buf, strlen(buf)); ////Enviamos el array
  vw_wait_tx(); //Esperamos hasta que el mensaje se envie

  delay(20);
}
