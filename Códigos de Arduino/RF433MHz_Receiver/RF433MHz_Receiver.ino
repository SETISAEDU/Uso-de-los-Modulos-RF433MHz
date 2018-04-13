// Librerías:
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <VirtualWire.h>

// Instanciacion del LCD i2c:
LiquidCrystal_I2C lcd(0x27, 16, 2);

// definicion de nuevos caracter
#define CIRCLE 0
#define BLANK 1
#define TEMP 2
#define HUM 3

byte customChar[8] = {
  B00100,
  B01010,
  B00100,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte customChar1[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

byte customChar2[8] = {
  B00100,
  B01110,
  B01110,
  B01110,
  B01110,
  B11101,
  B11101,
  B01110
};

byte customChar3[8] = {
  B00100,
  B00100,
  B01110,
  B01010,
  B10001,
  B10001,
  B01110,
  B00000
};

//Variables de los datos recibidos
float dato1 = 0.0;
float dato2 = 0.0;


void setup()
{
  //Inicializacion del LCD
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);  // Se pone el cursor en la primera posición de la pantalla.
  lcd.print("Iniciando...");
  delay(1000);
  lcd.createChar(CIRCLE, customChar);
  lcd.createChar(BLANK, customChar1);
  lcd.createChar(TEMP, customChar2);
  lcd.createChar(HUM, customChar3);
  lcd.clear();

  // Se inicializa el RF
  vw_setup(2000);  // velocidad: Bits per segundo
  vw_rx_start();   // Se inicia como receptor
}

void loop()
{
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  //verificamos si hay un dato valido en el RF
  if (vw_get_message((uint8_t *)buf, &buflen))
  {
    int i;
    String  DatoCadena = "";
    if ((char)buf[0] == 'i') //verificamos el inicio de trama
    {
      for (i = 1; i < buflen; i++)
      {
        DatoCadena.concat((char)buf[i]);
      }
      dato1 = DatoCadena.toFloat();
      Serial.print("Humedad recibido: ");
      Serial.println(dato1);
      lcd.setCursor(0, 0);  // Se pone el cursor en la primera posición de la pantalla.
      lcd.print("Hum: "); lcd.print(dato1); lcd.print(" %");
      lcd.setCursor(15, 0); lcd.write(HUM);
    }
    else if ((char)buf[0] == 'f') //verificamos el inicio de trama
    {
      for (i = 1; i < buflen; i++)
      {
        DatoCadena.concat((char)buf[i]);
      }
      dato2 = DatoCadena.toFloat();
      Serial.print("Temp recibido: ");
      Serial.println(dato2);
      lcd.setCursor(0, 1);  // Se pone el cursor en la primera posición de la pantalla.
      lcd.print("Temp: "); lcd.print(dato2); lcd.write(BLANK); lcd.write(CIRCLE); lcd.print("C");
      lcd.setCursor(15, 1); lcd.write(TEMP);
    }
  }
}

