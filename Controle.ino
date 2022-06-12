#include <max6675.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <CircularBuffer.h>

CircularBuffer<int, 10> buffer;


LiquidCrystal_I2C lcd(0x27,16,2);


  
/* Definições: GPIOs do Arduino utilizado na comunicação com o 
   MAX6675 */
#define GPIO_SO       8
#define GPIO_CS       9
#define GPIO_CLK      10

/* Criação de objeto para comunicação com termopar */
MAX6675 termopar(GPIO_CLK, GPIO_CS, GPIO_SO);

unsigned long time = 0; // variável para usar o millis como delay
unsigned long now = 0; // variável para usar o millis como delay
float read_sensor; 


/*-----------Funções de alerta-------------------------*/

/*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\FUNCAO TEMPERATURA MEDIANA\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
void normal() //para quando o feio estiver nem quente nem frio
{
  lcd.setCursor(0,1);
  lcd.println("FREIO Cond. Nor"); // freio em condicao normal
  digitalWrite(11, HIGH); // liga led verde
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
}


void suavedade() // para quando o freio nao estiver quente
{
  lcd.setCursor(0,1);
  lcd.println("Freio Frio");
  digitalWrite(11, HIGH); // liga led verde
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
}


void cuidado() // para quando o freio estiver quase no limite
  {
  digitalWrite(11, LOW);
  digitalWrite(12, HIGH); // liga led amarelo
  digitalWrite(13, LOW);
  lcd.setCursor(0,1);
  lcd.println("PRE LIMITE");
  tone(7,440,500); // ativa o buzzer numa frequencia 440Hz (Lá) por 500 ms. tone(pino,freq.,tempo em ms)
  now = millis();
  while(millis() < now + 500) {
      // Pausa de 500 ms
    }
  lcd.setCursor(0,1);
  lcd.println("CUIDADO"); // muda entre PRE LIMITE e CUIDADO
  tone(7,440,500);
  now = millis();
  while(millis() < now + 500) {
      // Pausa de 500 ms
    }
}


void perigo() //para quando o freio passar da temp. de perda de freio
{
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, HIGH); // liga led vermelho
  lcd.setCursor(0,1);
  lcd.println("SEM FREIO!"); //Freio superaquecido
  tone(7,262,250);
  tone(7,294,250);
  now = millis();
  while(millis() < now + 500) {
      // Pausa de 500 ms
    }
  lcd.setCursor(0,1); // faz a tela ficar piscando
  lcd.println(" "); // apaga o que foi escrito anteriormente para ficar piscando
  tone(7,262,250);
  tone(7,294,250);
     now = millis();
    while(millis() < now + 500) {
      // Pausa de 500 ms
    }
  }



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
    time = millis();
}
 
/* Programa principal */
void loop() 
{  
    lcd.setCursor(0,0); 
    lcd.print("Temp:"); 
   /*-------------------------------------Média móvel--------------------------------------*/
    read_sensor=termopar.readCelsius();
    buffer.push(read_sensor);
    if (millis() - time >= 2) {
    time = millis();
    float avg = 0.0;
    // the following ensures using the right type for the index variable
    using index_t = decltype(buffer)::index_t;
    for (index_t i = 0; i < buffer.size(); i++) {
      avg += buffer[i] / buffer.size();
    }
    Serial.print("Temperatura: "); 
    Serial.print(avg);
    Serial.println("C"); 
    
    lcd.setCursor(8,0);
    lcd.print(avg);
    lcd.setCursor(14,0);
    lcd.print("C");
    
    /* Teste de temperatura */
     if (avg >= 0 && avg < 100) { 
      suavedade();
    } 
    
      else if (avg >= 100 && avg < 350) { 
        normal();
    } 
    
      else if (avg >= 350 && avg <= 450) { 
      cuidado();
    } 
    
      else if (avg > 450) { 
      perigo();
    }

    
  }
    
}
