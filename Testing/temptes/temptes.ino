#include <Adafruit_MLX90614.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const int trigPin = 14;  //D5
const int echoPin = 12;  //D6
long duration;
float distance;
float temp;
float temp_kal;

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
String apiWritekey = "JICIWUB31KO6ZFLH";
const char* ssid = "GHAZI GANTENG";
const char* password = "Alhamdulillah";
const char* server = "api.thingspeak.com";
WiFiClient client;

void setup() {
  Serial.begin(9600);
  Serial.println("Deteksi suhu tubuh dan jarak");  
  mlx.begin();
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  }
  Serial.println("WiFi is connected");
}
 
void loop() {
  temp = mlx.readObjectTempC();
  temp_kal = (temp-73.231)/(-1.0549);
  Serial.print("Ambient : "); Serial.print(mlx.readAmbientTempC());  Serial.println("*C");
  Serial.print("Object (sebelum kalibrasi) : "); Serial.print(temp); Serial.println("*C");
  Serial.print("Object : "); Serial.print(temp_kal); Serial.println("*C");
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance= duration*0.034/2;
  Serial.print("Distance : ");
  Serial.print(distance); Serial.println(" cm"); 
  delay(7500);
  Serial.println();
  
  if (client.connect(server,80))
  {
    String tsData = apiWritekey;
    tsData +="&field1=";
    tsData += String(temp);
    tsData += "\r\n\r\n";
    tsData +="&field2=";
    tsData += String(temp_kal);
    tsData += "\r\n\r\n";
    tsData +="&field3=";
    tsData += String(distance);
    tsData += "\r\n\r\n";
    
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiWritekey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");
    client.print(tsData);
   }

  client.stop(); 

  Serial.println();
  delay(7500);
}
