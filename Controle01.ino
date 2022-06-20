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


//Variáveis globais.
unsigned long time = 0, now1=0, now2=0, now3=0,now4=0;
int led_verm=4, led_verd=11, led_am=12, buzzer=3, qt1=0,qt2=0;
float read_sensor; 

/*-----------Funções de alerta-------------------------*/

/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\FUNCAO TEMPERATURA MEDIANA\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
void normal() //para quando o feio estiver nem quente nem frio
{
  qt1=0; qt2=0;
  lcd.setCursor(0,1);
  lcd.print("Freio normal     "); // freio em condicao normal
  digitalWrite(led_verd, HIGH); // liga led verde
  digitalWrite(led_am, LOW);
  digitalWrite(led_verm, LOW);
}


void freio_frio() // para quando o freio nao estiver quente
{
  qt1=0; qt2=0;
  lcd.setCursor(0,1);
  lcd.print("Sistema frio!     ");
  digitalWrite(led_verd, HIGH); // liga led verde
  digitalWrite(led_am, LOW);
  digitalWrite(led_verm, LOW);
}


void cuidado(){
  digitalWrite(led_verd, LOW);
  digitalWrite(led_am, HIGH); // liga led amarelo
  digitalWrite(led_verm, LOW);
 
  if((millis()-now1)>=2000) {
       now1=millis();
       lcd.setCursor(0,1);
       lcd.print("Perda de freio!         ");
       if(qt1<2){
          tone(buzzer,1500, 750);
       }
       qt1++;
    }
  if((millis()-now2)>=4000) {
        now2=millis();
        lcd.setCursor(0,1);
        lcd.print("Cuidado!           ");
      } 
}

void perigo() //para quando o freio passar da temp. de perda de freio
{
  digitalWrite(led_verd, LOW);
  digitalWrite(led_am, LOW);
  digitalWrite(led_verm, HIGH); // liga led vermelho
  
  if((millis()-now3)>=2000) {
       now3=millis();
       lcd.setCursor(0,1);
       lcd.print("Sem freios!      ");
      //Freio superaquecido
       if(qt2<=2){
          tone(buzzer,1500,1000);
       }
       qt2++;
    }
  if((millis()-now4)>=4000) {
         now4=millis();
         lcd.setCursor(0,1);
         lcd.print("");
      } 
}


void setup()
{
    Serial.begin(9600);
    //Configurando LCD
    lcd.init();                     
    lcd.backlight();
    lcd.print("    TEMPTOR   ");
    delay(3000);
    lcd.clear();
    lcd.print("Inicializando...");
    delay(2000);
    lcd.clear();
    delay(1000);
    lcd.print("Inicializando...");
    delay(2000);
    lcd.clear();
    pinMode(buzzer,OUTPUT); // buzzer
    pinMode(led_verd,OUTPUT); // led verde
    pinMode(led_am,OUTPUT); // led amarelo
    pinMode(led_verm,OUTPUT); // led vermelho
    time = millis();
}
 
/* Programa principal */
void loop(){ 
    lcd.setCursor(0,0); 
    lcd.print("Temp:");  
   /*-----------Média móvel-------------*/
    if (millis() - time >= 3000) {
    time = millis();
    read_sensor=termopar.readCelsius();
    Serial.print("Temperatura: "); 
    Serial.print(read_sensor);
    Serial.println("C"); 
    
    lcd.setCursor(8,0);
    lcd.print(read_sensor);
    lcd.setCursor(14,0);
    lcd.print("C");
    
    /* Teste de temperatura */
    if (read_sensor <=100) { 
      freio_frio();
    } 
    
    else if(read_sensor > 100 && read_sensor <= 250) { 
        normal();
    } 
    
    else if(read_sensor > 250 && read_sensor <= 400) { 
      cuidado();
    } 
    
    else if(read_sensor > 400) { 
      perigo();
    }

    
  }
    
}
