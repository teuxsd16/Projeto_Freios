#include <SimpleEspNowConnection.h>

//Programa: ESP-NOW com ESP8266 NodeMCU - Emissor
//Autor: Arduino e Cia
#include <max6675.h>
#include <ESP8266WiFi.h>

#define GPIO_SO     D6  
#define GPIO_CS     D8
#define GPIO_CLK    D5 
// The Basic Function Of The ESP NODEMCU
MAX6675 termopar(GPIO_CLK, GPIO_CS, GPIO_SO);

float read_sensor;
unsigned long time1 = 0;

//Coloque na linha abaixo o Mac Address do NodeMCU receptor
uint8_t broadcastAddress[] = {0x40, 0x91, 0x51, 0x4E, 0xF5, 0x34};

//Estrutura para envio dos dados. Deve ser a mesma tanto no emissor como no receptor.
typedef struct struct_message
{
  String a;
} struct_message;


//Cria uma struct_message chamada myData
struct_message myData;

//Callback quando os dados sao enviados
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus)
{
  Serial.print("Status dos dados enviados: ");
  if (sendStatus == 0) {
    Serial.println("Envio ok!");
  }
  else {
    Serial.println("Falha no envio!");
  }
}

void setup()
{
  Serial.begin(9600);

  //Coloca o dispositivo no modo Wi-Fi Station
  WiFi.mode(WIFI_STA);

  //Inicializa o ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Erro ao inicializar o ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);

  //Registra o destinatario da mensagem
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop()
{
   if (millis() - time1 >= 3000) {
    time1 = millis();
    read_sensor=termopar.readCelsius();
    char read1[8];
    Serial.print("Temperatura: "); 
    Serial.print(read_sensor);
    Serial.println("C");
    dtostrf(read_sensor, 1, 2, read1); 
    myData.a = read1;
    //Envia a mensagem usando o ESP-NOW
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

   }

}
