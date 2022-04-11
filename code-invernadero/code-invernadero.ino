#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

int relay1 = 7;
int relay2 = 6;
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11
#define sensorTierra A0
DHT dht(DHTPIN, DHTTYPE);
int cont = 0;
//int contador=0;

void setup() {
  
  lcd.begin(16, 2);
  dht.begin();
  lcd.setCursor(0, 0); 
  pinMode(sensorTierra,INPUT);
  pinMode (relay1, OUTPUT);
  pinMode (relay2, OUTPUT);
  Serial.begin(9600);
}
void loop() {
  
  if (cont < 10) {
    lcd.clear(); 
    lcd.setCursor(0, 0); 
    
    lcd.print("Invernadero ");
    delay (300);
    
    cont = cont + 1;
  } else {
    
    lcd.clear(); // Borra pantalla
    lcd.setCursor(0, 0); // Inicio del cursor
    
    float humedad = dht.readHumidity();
    float temperatura = dht.readTemperature();


    lcd.clear(); // Borra pantalla
    lcd.setCursor(0, 0); // Inicio del cursor
    lcd.print("Temper. = ");
    lcd.print(temperatura);
    lcd.setCursor(0, 1); // Siguiente renglón.
    lcd.print("Humedad = ");
    lcd.print(humedad);
    delay(3000);
    
    int humedadTierra = mapValue();
    
    sensorTemperatura(temperatura);
    sensorHumedadTierra(humedadTierra);

  }
}
void sensorTemperatura( float temperatura ){
  if (temperatura > 20) {  // Aca hay que cambiar para establecer la temperatura minima a que debe encenderse el sistema de ventilacion
    digitalWrite(relay2, LOW);
    lcd.clear ();
    lcd.setCursor(0, 0);
    lcd.print ("Temp Alta");
    lcd.setCursor(0, 1);
    lcd.print ("Ventilacion");//se enciende ventilacion
    delay (4000);
  } else {
    digitalWrite(relay2, HIGH);
    lcd.clear ();
    lcd.setCursor(0, 0);
    lcd.print ("Temp normal");
    lcd.setCursor(0, 1);
    lcd.print ("Sin.Ventilacion");
    delay (4000);
  }

}
void sensorHumedadTierra(int lectura){
//en 80 ya se acerca  a un espacio inundado
    if (lectura >= 55 ) {

      lcd.clear ();
      lcd.setCursor(0, 0);
      lcd.print ("Humedad Normal");
      lcd.setCursor(0, 1);
      lcd.print ("desconecto riego");
      digitalWrite (relay1, HIGH);
      delay (4000);
      lcd.clear ();
      
    } else if (lectura < 55) {
      lcd.clear ();
      lcd.setCursor(0, 0);
      lcd.print ("Humedad baja");
      lcd.setCursor(0, 1);
      lcd.print ("conecto riego");
      digitalWrite (relay1, LOW);
      delay (4000);
      lcd.clear ();
    } 
    
}
int mapValue(){
  //1023 muy seco
//0    muy humedo

//300 representa el 29.33%
//530 representa el 51%

//manejare estos valores para que no se encuentre
//muy arido o muy  humedo
  int lectura = analogRead (sensorTierra);
  int lecturaPorcentaje = map (lectura, 1023,0, 0, 100);
  
  Serial.println("Lectura del sensor");
  Serial.println(analogRead(sensorTierra));
  
  Serial.println("Lectura en porcentaje");
  Serial.println(lecturaPorcentaje);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print ("H Suelo: ");
  lcd.print (lecturaPorcentaje);
  lcd.println ("%");
  delay(3000);
  return lecturaPorcentaje;
}
