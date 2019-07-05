#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "****************************************"
#define FIREBASE_AUTH "****************************************"
#define WIFI_SSID "***************"
#define WIFI_PASSWORD "*************"
int cont=0;

void execute(){
  int i;
  int n = WiFi.scanNetworks();
  String nome,rssi,mac;

  StaticJsonBuffer<600> jsonBuffer;
  JsonObject& atrib = jsonBuffer.createObject();
  Serial.print("é o cont:");
  Serial.println(cont);
  //Serial.println("Scan done");
  if (n == 0)
    Serial.println("No Networks Found");
  else
  {
    if (cont == 1){
      Firebase.remove("Rede");
      cont = 0;
      delay(100);
      }
    for (int i = 0; i < n; ++i)
   {
      nome = WiFi.SSID(i);
      rssi = WiFi.RSSI(i);
      mac = WiFi.BSSIDstr(i);

      delay(10);
      
      atrib["Nome"] = nome;
      atrib["Intensidade_RSSI"] = rssi;
      atrib["MAC"] = mac;
 
      Firebase.push("Rede",atrib);

      if (Firebase.failed()) {
        Serial.print("setting /message failed:");
        Serial.println(Firebase.error());  
      return;
      }
      delay(500);
      
    } 
    delay(60000); // tempo de pausa para a próxima "escuta"
    cont = cont+1;
    
  }
  
  }

void setup() {
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  pinMode(2,OUTPUT);
}

int n = 0;

void loop() {
  execute();
  
}
