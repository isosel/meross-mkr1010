// for WPA2 encrypted connections 
#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

#define SECRET_SSID "xx"
#define SECRET_PASS "xx"

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int status = WL_IDLE_STATUS;
int compteur;
int buttonStateOff = LOW;
int buttonStateOn = LOW;
bool boolonoff = false;
char charonoff;

int portMSS210 = 80;
char serverMSS210[] = "192.168.1.xxx";
bool clientconnected = false;
String postData;
//WiFiClient myWifiClient2;
//WiFiClient myWifiClient3;
WiFiClient myClient1;
//HttpClient myClient2 = HttpClient(myWifiClient2, serverMSS210, portMSS210);
//WebSocketClient myClient3 = WebSocketClient(myWifiClient3, serverMSS210, portMSS210);

int codeResponse;
String postResponse;

void setup() 
{
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  pinMode(0,INPUT);
  pinMode(1, INPUT);

  
  while (!Serial) 
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) 
  {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) 
  {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) 
  {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");
  printCurrentNet();
  printWifiData();
}

void loop() 
{
  buttonStateOff = digitalRead(0);
  buttonStateOn = digitalRead(1);
  
  if(buttonStateOff == HIGH)
  {
    Serial.println("Envoie de la requête: POWER OFF");
    postData = "{\"header\":{\"from\":\"/app/339299-e1853779db9fc7866cf6dfe0b697b0b8/subscribe\",\"messageId\":\"36f3d4b5da1545dd127176bdbbf8f2cf\",\"method\":\"SET\",\"namespace\":\"Appliance.Control.ToggleX\",\"payloadVersion\":1,\"sign\":\"195a5ea4fb950c3cb475cd5daa5d4e64\",\"timestamp\":1571919319},\"payload\":{\"togglex\":{\"channel\":0,\"onoff\":0}}}";
    Serial.println(postData);

    request3();

    while(true) //wait if you hold the button...
    {
      buttonStateOff = digitalRead(0);
      if(buttonStateOff == LOW)
        break;
    }
  }
  if(buttonStateOn == HIGH)
  {
    Serial.println("Envoie de la requête: POWER ON");
    postData = "{\"header\":{\"from\":\"/app/339299-e1853779db9fc7866cf6dfe0b697b0b8/subscribe\",\"messageId\":\"36f3d4b5da1545dd127176bdbbf8f2cf\",\"method\":\"SET\",\"namespace\":\"Appliance.Control.ToggleX\",\"payloadVersion\":1,\"sign\":\"195a5ea4fb950c3cb475cd5daa5d4e64\",\"timestamp\":1571919319},\"payload\":{\"togglex\":{\"channel\":0,\"onoff\":1}}}";
    Serial.println(postData);

    request3();

    while(true) //wait if you hold the button...
    {
      buttonStateOn = digitalRead(1);
      if(buttonStateOn == LOW)
        break;
    }    
  }// fin du if(buttonStateOn)


  
  delay(100);
  compteur++;
  if(compteur>49) // check the connection
  {
    printCurrentNet();
    compteur =0;    
  }
}

void request1()
{
  myClient1.stop();
  myClient2.stop();
  myClient3.stop();
  if (myClient1.connect(serverMSS210, portMSS210)) 
  {
    Serial.println("Connecté au serveur ! Bienvenue :)");
    //HTTP request 1st version:
    myClient1.println("POST /config HTTP/1.1");
    myClient1.print("Host:");myClient1.println(serverMSS210);
    myClient1.println("Content-Type:application/json; charset=UTF-8");
    myClient1.println("Connection:close");
    myClient1.print("Content-Length:");myClient1.println(postData.length());
    myClient1.println("Accept-Encoding:gzip"); //needed
    myClient1.println("User-Agent:ARDUINO1"); //w/e
    myClient1.println("");
    myClient1.println(postData);
    myClient1.println("");
    Serial.print("Requête envoyée. Réponse: ");
    Serial.println(myClient1.read());
  }// fin du if(myClient..
  else
  {
    Serial.println("************************");
    Serial.println("**********ERROR*********");
    Serial.println("************************");
  }  
}

void request2()
{   
//  myClient1.stop();
//  myClient2.stop();
//  myClient3.stop();
//  
//  Serial.println(" envoie de request2()");                  
//  myClient2.post("/config", "application/json; charset=UTF-8", postData);
//  int statusCode = myClient2.responseStatusCode();
//  Serial.print("Status code: ");
//  Serial.println(statusCode);
//  String response2 = myClient2.responseBody();
//  Serial.print("Response: ");
//  Serial.println(response2);
//  Serial.println("Wait 10 seconds");
//  delay(10000);
}

void request3()// not working 
{
//  int countReq3=0;
//  
//  if(myClient3.ping() ==0) //it's ok
//  {
//    Serial.println("ping1 ok !!");
//  }
//  else
//  {
//    Serial.println("ping1 FAILED");
//  }
//  
//  if(myClient3.ping() ==0)
//  {
//    Serial.println("ping2 ok !!");
//  }
//  else
//  {
//    Serial.println("ping2 FAILED");
//  }
//  Serial.print("Connexion à la prise: ");
//  Serial.println(myClient3.begin("/config")); //.begin doesn t work
//  
//  while(myClient3.connected())
//  {
//    Serial.print("Still connected");Serial.println(countReq3);
//    myClient3.beginMessage(TYPE_TEXT);
//    myClient3.print("Hello");
//    myClient3.println(countReq3);
//    myClient3.endMessage();
//    countReq3++;
//    int messageSizeReq3 = myClient3.parseMessage();
//    if(messageSizeReq3 > 0)
//    {
//      Serial.println("On a recu un message:");
//      Serial.println(myClient3.readString());
//    }
//    delay(5000);
//    if(countReq3 > 3)
//      break;
//  }
}

void printWifiData() 
{
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  printMacAddress(mac);
  delay(1000);
}

void printCurrentNet() 
{
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  printMacAddress(bssid);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}

void printMacAddress(byte mac[]) 
{
  for (int i = 5; i >= 0; i--) 
  {
    if (mac[i] < 16) 
    {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) 
    {
      Serial.print(":");
    }
  }
  Serial.println();
}
