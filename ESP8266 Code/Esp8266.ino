#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>


#define FIREBASE_HOST "xxxxxx"
#define FIREBASE_AUTH "xxxxxx"
#define WIFI_SSID "xxxxxx"
#define WIFI_PASSWORD "xxxxxxx"

//change FIREBASE_HOST with your FIREBASE
//change FIREBASE_AUTH with your FIREBASE
//change SSID with your WIFI name
//change PASSWORD with your WIFI password


String data;
String karakter;

void setup() {
  Serial.begin(9600);

//  // connect to wifi.
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


int n = 0;
int StringCount= 0;

void loop() {
  data = Serial.read();

  karakter = ',';
  String data_array[15];

  while (data.length() > 0)
  {
    int index = data.indexOf(',');
    if (index == -1) 
    {
      data_array[StringCount++] = (data);
      break;
    }
    else
    {
      data_array[StringCount++] = data.substring(0, index);
      data = data.substring(index+1);
    }
  }

  for (int i = 0; i < StringCount; i++)
  {
    Serial.print(i);
    Serial.print(": \"");
    Serial.print(data_array[i]);
    Serial.println("\"");
  }

  // set string value
  Firebase.pushString("/Sensor/Temp1:", data_array[0]);
  Firebase.pushString("/Sensor/Temp2:", data_array[1]);
  Firebase.pushString("/Sensor/Temp3:", data_array[2]);
  Firebase.pushString("/Sensor/Temp4:", data_array[3]);
  Firebase.pushString("/Sensor/Temp5:", data_array[4]);
  Firebase.pushString("/Sensor/CH4:", data_array[5]);
  Firebase.pushString("/Sensor/Smoke:", data_array[6]);
  Firebase.pushString("/Sensor/Karbon:", data_array[7]);
  Firebase.pushString("/Sensor/H2:", data_array[8]);
  Firebase.pushString("/Sensor/Vol1:", data_array[9]);
  Firebase.pushString("/Sensor/Arus1:", data_array[10]);
  Firebase.pushString("/Sensor/Vol2:", data_array[11]);
  Firebase.pushString("/Sensor/Arus2:", data_array[12]);
  Firebase.pushString("/Sensor/Vol3:", data_array[13]);
  Firebase.pushString("/Sensor/Arus3:", data_array[14]);                              

}
