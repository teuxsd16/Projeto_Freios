#include <max6675.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);
  
/* Definições: GPIOs do Arduino utilizado na comunicação com o 
   MAX6675 */
#define GPIO_SO       8
#define GPIO_CS       9
#define GPIO_CLK      10

/* Criação de objeto para comunicação com termopar */
MAX6675 termopar(GPIO_CLK, GPIO_CS, GPIO_SO);
  
void setup()
{
    Serial.begin(9600);
    //Configurando LCD
    lcd.init();                     
    lcd.backlight();
    lcd.print("Inicializando...");
    delay(3000);
    lcd.clear();
    pinMode(7,OUTPUT); // buzzer
    pinMode(11,OUTPUT); // led verde
    pinMode(12,OUTPUT); // led amarelo
    pinMode(13,OUTPUT); // led vermelho
    unsigned long now = 0; // variável para usar o millis como delay
  
}
 
/* Programa principal */
void loop() 
{  
    Serial.print("Temperatura: "); 
    Serial.print(termopar.readCelsius());
    Serial.println("C"); 

    lcd.setCursor(0,0); 
    lcd.print("Temp:"); 
    lcd.setCursor(8,0); 
    lcd.print(termopar.readCelsius());
    lcd.setCursor(14,0);
    lcd.print("C"); 
    now = millis();
	while(millis() < now + 1000) {
    // Pausa de 1s
  }
}
