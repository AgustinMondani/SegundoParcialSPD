#include <IRremote.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define ledRojo 7
#define ledVerde 6
#define TMP 0
#define Receptor 11

#define Tecla_1 0xEF10BF00   
#define Tecla_2 0xEE11BF00 
#define Tecla_Power 0xFF00BF00

LiquidCrystal_I2C lcd(34,16,2);
Servo riego;
IRrecv receptor(Receptor); // Con IRrecv irrecv estamos indicando que el pin (11) será el que recibe los datos
float temperatura = 0;
bool bandera = false;

void alarmaIncendio(float temp);
void estacion(float temp);

void setup()
{
  lcd.init();   //inicialioza la pantalla
  lcd.backlight();  //prende la luz de la pantalla 
  receptor.enableIRIn(); // Inicializa el receptor
  riego.attach(9);//adjuntmaos la variable servo al pin conectado
  pinMode(ledRojo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  Serial.begin(9600);
}

void loop()
{     
  temperatura = map(analogRead(TMP),0,1023,-50,450); // mapeo de las temperaturas
  if(receptor.decode())
  {
    Serial.println(receptor.decodedIRData.decodedRawData, HEX);//imprime el codigo en hexadecimal por el puerto serial
    delay(50);
	receptor.resume();
    if((receptor.decodedIRData.decodedRawData) == Tecla_Power)
    {
      bandera = !bandera;
      lcd.clear();
    }
  }
  
  
   if(bandera)
  {
   	digitalWrite(ledRojo, LOW);
    digitalWrite(ledVerde, HIGH);
     
	switch((receptor.decodedIRData.decodedRawData))
    {
      case Tecla_1:
        lcd.setCursor(0,0);
        lcd.print(temperatura);
      	estacion(temperatura);
      break;
      case Tecla_2:
        alarmaIncendio(temperatura);
      	lcd.clear();
      break;
    }
   }else
   {
      digitalWrite(ledVerde, LOW);
      digitalWrite(ledRojo, HIGH);
   }
} 
 

void alarmaIncendio(float temp)//funcion para la alarma 
{
  if(temp > 60)
  {
    lcd.setCursor(0,0); // seteo en el cursor
  	lcd.print("--- WARNING ---");
    lcd.setCursor(0,1);
  	lcd.print("--- WARNING ---");
    
  	riego.write(90);//movimiento del servo
  	delay(1000);
  	riego.write(180);
  	delay(1000);
  	riego.write(0);
  	delay(1000);
  }else
  {
    lcd.setCursor(0,0); // seteo en el cursor
  	lcd.print("----- SAFE -----");
    lcd.setCursor(0,1);
  	lcd.print("----- SAFE -----");
    delay(1000);
  }
}


void estacion(float temp)//funcion para las estaciones 
{
  if(temp < 0)
  {
    lcd.setCursor(0,1);
    lcd.print("INVIERNO ");
  }else if (temp < 15)
  {
  	lcd.setCursor(0,1);
    lcd.print("OTONIO   ");
  }else if(temp < 25)
  {
    lcd.setCursor(0,1);
    lcd.print("PRIMAVERA");
  }else
  {
    lcd.setCursor(0,1);
    lcd.print("VERANO   ");
  }
}