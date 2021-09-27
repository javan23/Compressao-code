#include <SPI.h>
#include <LoRa.h>
#include "FS.h"
#include "SPIFFS.h"
#define FORMAT_SPIFFS_IF_FAILED true
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#define LORA
#undef WIFI

#include "Compress.h"


int free_heap = 99999999; // INICIALIZA A VARIAVEL DE HEAP

void atualiza_heap() {

  if (free_heap > ESP.getFreeHeap())
    free_heap = ESP.getFreeHeap();

}
/*
  Get_msn é a função responsável por fazer a alocação dinâmica para a mensagem comprimida e mensagem compactada.
  Nesta mesma é colocada a mensagem a ser comprimida, retirada do arquivo de texto

  int qtd_msn : Quantidade de mensagens a colocadas na variavel mensagem.
  char ** mensagem : variável que contem a mensagem a ser comprimida.
  char ** compress_mensagem : variável que contem a comprimida.

  /spiffs/uni_test_sequencial.txt" : Arquivo de texto que contém as mensagens salvas.

*/
int get_msn(char **mensagem, unsigned char **compress_mensagem, int qtd_msn) {
  *mensagem = (char*) calloc(35 * qtd_msn, sizeof(char));
  *compress_mensagem = (unsigned char*) calloc(70 * qtd_msn, sizeof(char));

  if (mensagem == NULL || compress_mensagem == NULL){
    Serial.println("Falha na alocação de memória do get_msn");
    return 1;
    }
  FILE *fp;
  fp = fopen("/spiffs/GPS_AMOSTRA.txt", "r");

  char aux[100] = {0};
  int j = 0;
  int idx = rand() % (310 - qtd_msn);

  while (j < qtd_msn)
  {

    memset(aux, 0, sizeof(char) * 100);
    fseek( fp, 52 * idx, SEEK_SET );
    fgets(aux, 50, fp);
    char *token;
    token = strtok(aux, "#");
    // printf("\n TOKEN %s \n",token);
    sprintf(*mensagem, "%s%s", *mensagem, token);
    j++;
    idx++;

    if (idx > 310) {
      idx = 0;
    }
  }
  //printf("mensagem %s",*mensagem);
  fclose(fp);
  return 0;
}



#define NO_SLEEP_RUN 65535
#define DEBUG_SERIAL

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  60*5       /* Time ESP32 will go to sleep (in seconds) */

//#define NO_SLEEP_DELAY 1000*10*1  // in ms
#define NO_SLEEP_DELAY 1000*0.0001*1  // in ms

//ThingSpeak stuff
#include "ThingSpeak.h"
#include <WiFi.h>

char ssid[] = "INCORPORADORA GIARDINO";   // your network SSID (name)
char pass[] = "272593866";   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

// channel termopar
unsigned long myChannelNumber = 897238;
const char * myWriteAPIKey = "FN5FJWBHVOVI2UDQ";
#define CHANNELID "<8>"

//channel monitoramento
//unsigned long myChannelNumber = 798033;
//const char * myWriteAPIKey = "PAIKQKAUDP1DW2O5";
//#define CHANNELID "<9>"

#include "max6675.h"

int thermoDO = 34;
int thermoCLK = 25;

int thermoCS1 = 12;
int thermoCS2 = 13;
int thermoCS3 = 21;
int thermoCS4 = 17;



MAX6675 thermocouple1(thermoCLK, thermoCS1, thermoDO);
MAX6675 thermocouple2(thermoCLK, thermoCS2, thermoDO);
MAX6675 thermocouple3(thermoCLK, thermoCS3, thermoDO);
MAX6675 thermocouple4(thermoCLK, thermoCS4, thermoDO);


// This is the LoRa Node side sketch for the MQTT example: http://wiki.dragino.com/index.php?title=MQTT_Forward_Instruction#Example_to_communicate_to_a_simple_MQTT_server

float tem1;
float tem2;
float tem3;
float tem4;
char tem_1[8] = {"\0"}, tem_2[8] = {"\0"}, tem_3[8] = {"\0"}, tem_4[8] = {"\0"}, count_1[8] = {"\0"};
char *node_id = CHANNELID;  //From LG01 via web Local Channel settings on MQTT.Please refer <> dataformat in here.
uint8_t datasend[36];
RTC_DATA_ATTR int count = 0;
unsigned long new_time, old_time = 0;
#include <U8g2lib.h>   // https://github.com/olikraus/U8g2_Arduino

// I2C OLED Display works with SSD1306 driver
#define OLED_SDA 4
#define OLED_SCL 15
#define OLED_RST 16

U8G2_SSD1306_128X64_NONAME_F_SW_I2C Display(U8G2_R0, /* clock=*/ OLED_SCL, /* data=*/ OLED_SDA, /* reset=*/ OLED_RST); // Full framebuffer, SW I2C


// SPI LoRa Radio
#define LORA_SCK 5        // GPIO5 - SX1276 SCK
#define LORA_MISO 19     // GPIO19 - SX1276 MISO
#define LORA_MOSI 27    // GPIO27 -  SX1276 MOSI
#define LORA_CS 18     // GPIO18 -   SX1276 CS
#define LORA_RST 14   // GPIO14 -    SX1276 RST
#define LORA_IRQ 26  // GPIO26 -     SX1276 IRQ (interrupt request)

int totalSize = 0;
bool logger = true;


bool wifi_connected = false;


void setup()
{
#ifdef DEBUG_SERIAL
  Serial.begin(9600);
  while (!Serial);
  Serial.println(F("Start MQTT Example"));
#endif


  // Very important for SPI pin configuration!
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);
  // Very important for LoRa Radio pin configuration!
  LoRa.setPins(LORA_CS, LORA_RST, LORA_IRQ);

  if (!LoRa.begin(915000000))
  {
#ifdef DEBUG_SERIAL
    Serial.println("Starting LoRa failed!");
#endif
    while (1);
  }
  // Setup Spreading Factor (6 ~ 12)
  LoRa.setSpreadingFactor(9);

  // Setup BandWidth, option: 7800,10400,15600,20800,31250,41700,62500,125000,250000,500000
  //Lower BandWidth for longer distance.
  LoRa.setSignalBandwidth(125000);

  // Setup Coding Rate:5(4/5),6(4/6),7(4/7),8(4/8)
  LoRa.setCodingRate4(5);
  LoRa.setSyncWord(52);
  LoRa.setPreambleLength(8);
  //LoRa.setTxPower(17 );
  LoRa.setTxPower(20, PA_OUTPUT_RFO_PIN);
#ifdef DEBUG_SERIAL
  Serial.println("LoRa init succeeded.");
#endif
  //LoRa.onReceive(onReceive);
  //LoRa.receive();
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);


  if (count < NO_SLEEP_RUN) {
    Display.begin();
    Display.enableUTF8Print();    // enable UTF8 support for the Arduino print() function
    Display.setFont(u8g2_font_ncenB08_tr);
  }

  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
#ifdef DEBUG_SERIAL
    Serial.println("SPIFFS Mount Failed");
#endif
    return;
  }

  listDir(SPIFFS, "/", 0);
  if (count == 0) appendFile(SPIFFS, "/log.txt", "\nNewLog\n");
  if (totalSize > 1468006) // 1.4*1024*1024, SPIFFS partition almost full, turn off the logger
    logger = false;

  //Thingspeak
#ifdef WIFI
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
#endif
}

void max6675Temp() {
  tem1 = 0; tem2 = 0; tem3 = 0; tem4 = 0;
  delay(20000); // twenty seconds warnup
  for (int i = 0; i < 5; i++) {
    tem1 += thermocouple1.readCelsius();
    tem2 += thermocouple2.readCelsius();
    tem3 += thermocouple3.readCelsius();
    tem4 += thermocouple4.readCelsius();
    delay(200);
  }
  tem1 /= 5;
  tem2 /= 5;
  tem3 /= 5;
  tem4 /= 5;

}

void max6675TempNoSleep() {

  tem1 = thermocouple1.readCelsius();
  tem2 = thermocouple2.readCelsius();
  tem3 = thermocouple3.readCelsius();
  tem4 = thermocouple4.readCelsius();

}


void sendWifiData() {

  // set the fields with the values
  ThingSpeak.setField(1, tem1);
  ThingSpeak.setField(2, tem2);
  ThingSpeak.setField(3, tem3);
  ThingSpeak.setField(4, tem4);
  ThingSpeak.setField(5, count);


  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

#ifdef DEBUG_SERIAL
  if (x == 200) {
    Serial.println("Channel update successful.");
  }
  else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
#endif


}


void max6675Write()
{
  char data[100] = "\0";
  for (int i = 0; i < 8; i++)
  {
    data[i] = node_id[i];
  }

  dtostrf(tem1, 0, 1, tem_1);
  dtostrf(tem2, 0, 1, tem_2);
  dtostrf(tem3, 0, 1, tem_3);
  dtostrf(tem4, 0, 1, tem_4);

  dtostrf(count, 0, 0, count_1);

  // Serial.println(tem_1);
  strcat(data, "");
  strcat(data, tem_1);
  strcat(data, "&");
  strcat(data, tem_2);
  strcat(data, "&");
  strcat(data, tem_3);
  strcat(data, "&");
  strcat(data, tem_4);
  strcat(data, "&");
  strcat(data, count_1);
  strncpy((char *)datasend, data, 30);

#ifdef DEBUG_SERIAL
  Serial.println((char *)datasend);
  Serial.println(sizeof datasend);
#endif
  if ((count % 1000) == 0) {
    if (getFileSize(SPIFFS, "/log.txt") > 1468006) {
      logger = false;
    }
  }
  if (logger)
    appendFile(SPIFFS, "/log.txt", data);
  appendFile(SPIFFS, "/log.txt", "\n");


}




void SendData()
{

  LoRa.beginPacket();
  LoRa.print((char *)datasend);
  LoRa.endPacket();
}



void loop()
{ delay(5000);
  UBaseType_t uxHighWaterMark;
  uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL ); // Função do FREERTOS para auxiliar na aferição da memória Heap.

  Serial.println("");
  // -- COMPRESSÃO--
  for (int qtd_msn = 1; qtd_msn < 40; qtd_msn++) { // Laço que determina a quantidade de mensagem por compressão, indo de 1 a grupos de 39.
    for (int i = 0; i < 100; i++) { // Laço que determina a quantidade de compressões por grupo
      free_heap = 99999999;
      int aux_heap = ESP.getFreeHeap();

     // Serial.printf(" \n HEAP %ld \n", free_heap=ESP.getFreeHeap());
      char *var; // Mensagem a ser comprimida
      unsigned char *mensagem;  // Mensagem a comprimida
      if(get_msn(&var, &mensagem, qtd_msn))
          continue;
      atualiza_heap();
      int idx = -1000;
      
      unsigned long start = micros();
      int tamanho_compactado = 0;

      compress_(var, mensagem, strlen(var), &tamanho_compactado);
      atualiza_heap();
      unsigned long  stop = micros() - start;




      //----- DESCOMPRESSÃO
      char *mensagem_descompress = (char*) calloc(35 * qtd_msn, sizeof(char));
      if(mensagem_descompress==NULL)
      {
        Serial.println("FALHA NA DESCOMPRESSÃO");
        free(var);
        free(mensagem);
        continue;
        }
      int compress_vlr=descompress_(mensagem_descompress, tamanho_compactado, mensagem);
      if(compress_vlr==3)
      {
        Serial.println("FALHA NA DESCOMPRESSÃO");
        free(mensagem_descompress);
        free(var);
        free(mensagem);
        continue;
        }

      int comparacao = strcmp(mensagem_descompress, var);

      float tx = (float)(100 * tamanho_compactado / strlen(var)) / 100;
                      //'qtd_msn', 'in', 'out', 'tx','tempo_comp', 'valido','ERRO','STACK','HEAP'
      Serial.printf("TAGBUSCA /%d/%d/%d/%f/%ld/%d/%d/%ld /%ld \n", qtd_msn, strlen(var), tamanho_compactado, 100 * tx, stop, comparacao, 0, uxTaskGetStackHighWaterMark( NULL ), aux_heap - free_heap);

      free(mensagem_descompress);
      free(var);
      free(mensagem);


      count++;
      if (count < NO_SLEEP_RUN) {
        // Display Info
        Display.clearBuffer();
        Display.setCursor(0, 12); Display.print("MQTT #" + (String)qtd_msn );
        Display.setCursor(0, 24); Display.print("T1: " + (String)i);
        Display.setCursor(0, 36); Display.print("T2: " + (String)1);
        Display.setCursor(0, 48); Display.print("T3: " + (String)tem3 + " t4: " + (String)tem4);
        //          Display.setCursor(0,60); Display.print("Size: " +(String)tamanho_comprimido);
        Display.sendBuffer();
        delay(10);
      } else {
        delay(10000);
        esp_deep_sleep_start();
      }
      //LoRa.receive();
    }
  }

}

//ThingSpeak stuff
void connectWifi() {
  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    for (int i = 0; i < 3 ; i++) {

      if (WiFi.status() != WL_CONNECTED) {

        WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
        Serial.print(".");
        delay(5000);
      } else break;
    }
    Serial.println("\nConnected.");
  }
}



void onReceive(int packetSize) {

  // received a packet
  Serial.print("Received packet : ");

  // read packet
  for (int i = 0; i < packetSize; i++) {
    Serial.print((char)LoRa.read());
  }
  Serial.print("\n\r");
}

//Filesystem stuff
void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
  totalSize = 0;
#ifdef DEBUG_SERIAL
  Serial.printf("Listing directory: %s\r\n", dirname); // PROPRIAS DO ARDUINO, PERGUNTAR AO MÁRCIO ESSAS LIB SÃO DO ESP OU DO ARDUINO OU O QUE ? POSSO UTILIZA-LAS JUNTAS?
#endif

  File root = fs.open(dirname);
  if (!root) {
#ifdef DEBUG_SERIAL
    Serial.println("- failed to open directory");
#endif
    return;
  }
  if (!root.isDirectory()) {
#ifdef DEBUG_SERIAL
    Serial.println(" - not a directory");
#endif
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
#ifdef DEBUG_SERIAL
      Serial.print("  DIR : ");
      Serial.println(file.name());
#endif
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    } else {
#ifdef DEBUG_SERIAL
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("\tSIZE: ");
      Serial.println(file.size());
#endif
      totalSize += file.size();
    }
    file = root.openNextFile();
  }
}

void readFile(fs::FS &fs, const char * path) {
#ifdef DEBUG_SERIAL
  Serial.printf("Reading file: %s\r\n", path);
#endif
  File file = fs.open(path);
  file.seek(51, SeekSet);
  if (!file || file.isDirectory()) {
#ifdef DEBUG_SERIAL
    Serial.println("- failed to open file for reading");
#endif
    return;
  }
#ifdef DEBUG_SERIAL
  Serial.println("- read from file:");
#endif
  while (file.available()) {
#ifdef DEBUG_SERIAL
    Serial.write(file.read());
#endif
  }
}
void readFile_estudodecaso(fs::FS &fs, const char * path, int qtd) {
#ifdef DEBUG_SERIAL
  Serial.printf("Reading file: %s\r\n", path);
#endif
  //File file = fs.open(path);
  FILE* file = fopen("/spiffs/arquivoteste_uni.txt", "r");
  if (file == NULL) {
    ESP_LOGE("ERRO", "Failed to open file for reading");
    return;
  }

  //  file.seek(51,SeekSet);
  /*  if(!file || file.isDirectory()){
        #ifdef DEBUG_SERIAL
        Serial.println("- failed to open file for reading");
        #endif
        return;
    }
    #ifdef DEBUG_SERIAL
    Serial.println("- read from file:");
    #endif*/
  int count = 0;
  char aux[100] = {0};
  while (count < qtd ) {
#ifdef DEBUG_SERIAL
    count++;
    fgets(aux, 50 * count, file);
    fseek( file, 52 * count, SEEK_SET );
    Serial.println(aux);
    //file.seek(51*count,SeekSet);
#endif
  }
  fclose(file);
}

void writeFile(fs::FS &fs, const char * path, const char * message) {
#ifdef DEBUG_SERIAL

  Serial.printf("Writing file: %s\r\n", path);
#endif

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
#ifdef DEBUG_SERIAL
    Serial.println("- failed to open file for writing");
#endif
    return;
  }
  if (file.print(message)) {
#ifdef DEBUG_SERIAL
    Serial.println("- file written");
#endif
  } else {
#ifdef DEBUG_SERIAL
    Serial.println("- frite failed");
#endif

  }
}


int getFileSize(fs::FS &fs, const char * path) {

  File file = fs.open(path);
  if (!file || file.isDirectory()) {
#ifdef DEBUG_SERIAL
    Serial.println("- failed to open file for reading");
#endif

    return -1;
  }

  return file.size();
}


void appendFile(fs::FS &fs, const char * path, const char * message) {
#ifdef DEBUG_SERIAL
  Serial.printf("Appending to file: %s\r\n", path);
#endif
  File file = fs.open(path, FILE_APPEND);
  if (!file) {
#ifdef DEBUG_SERIAL
    Serial.println("- failed to open file for appending");
#endif
    return;
  }
  if (file.print(message)) {
#ifdef DEBUG_SERIAL

    Serial.println("- message appended");
#endif
  } else {
#ifdef DEBUG_SERIAL

    Serial.println("- append failed");
#endif
  }
}