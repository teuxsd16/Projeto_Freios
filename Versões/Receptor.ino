#include <SimpleEspNowConnection.h>
#include <ESP8266WiFi.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

//Estrutura para envio dos dados. Deve ser a mesma tanto no emissor como no receptor.
typedef struct struct_message
{
  String a;
} struct_message;

unsigned long time1 = 0, now1=0, now2=0, now3=0,now4=0;
int led_verm=D6, led_verd=D8, led_am=D7, buzzer=D5, qt1=0;


//Cria uma struct_message chamada myData
struct_message myData;


void normal() //para quando o feio estiver nem quente nem frio
{
  qt1=0; 
  lcd.setCursor(0,1);
  lcd.print("Freio normal     "); // freio em condicao normal
  digitalWrite(led_verd, HIGH); // liga led verde
  digitalWrite(led_am, LOW);
  digitalWrite(led_verm, LOW);
}


void freio_frio() // para quando o freio nao estiver quente
{
  qt1=0; 
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
       if(qt1<=3){
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
  lcd.setCursor(0,1);
  lcd.print("Sem freios!      ");
  
  if((millis()-now3)>=2000) {
       now3=millis();
      //Freio superaquecido
       tone(buzzer,1500,1000);
    }
 
}


//Funcao de Callback executada quando a mensagem for recebida
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len)
{
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.println();
  Serial.print("Bytes recebidos: ");
  Serial.println(len);
  Serial.print("String: ");
  Serial.println(myData.a);

  float request = myData.a.toFloat();
  lcd.setCursor(8,0);
  lcd.print(request);
  lcd.setCursor(14,0);
  lcd.print("C");

   
  /* Teste de temperatura */
    if (request <=20) { 
      freio_frio();
    } 
    
    else if(request > 20 && request <= 30) { 
        normal();
    } 
    
    else if(request > 30 && request <= 32) { 
      cuidado();
    } 
    
    else if(request > 32) { 
      perigo();
    }
}

void setup()
{
  Serial.begin(9600);
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
  time1 = millis();  

  //Coloca o dispositivo no modo Wi-Fi Station
  WiFi.mode(WIFI_STA);

  //Inicializa o ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Erro ao inicializar o ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop()
{
  lcd.setCursor(0,0); 
  lcd.print("Temp:"); 
  //Neste programa nao temos comandos no loop
}
