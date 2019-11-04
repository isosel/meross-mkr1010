// for WPA2 encrypted connections 
#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

#define SECRET_SSID "devolo-8db"
#define SECRET_PASS "DRVVWVEBAULTWEDL"

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int status = WL_IDLE_STATUS;
int compteur;
int buttonStateOff = LOW;
int buttonStateOn = LOW;
bool boolonoff = false;
char charonoff;
// ip meross: .132 Iliass:  .143 port 3000
int portMSS210 = 3000;
String serverMSS210 = "192.168.1.143";
bool clientconnected = false;
String postData;
WiFiClient myWifiClient2;
//WiFiClient myWifiClient3;
//WiFiClient myClient1;
HttpClient myClient2 = HttpClient(myWifiClient2, serverMSS210, portMSS210);
//WebSocketClient myClient3 = WebSocketClient(myWifiClient3, serverMSS210, portMSS210);
//WiFiClient myClient4;
int codeResponse;
char* postResponse;

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
    //postData = "{\"header\":{\"from\":\"/app/339299-e1853779db9fc7866cf6dfe0b697b0b8/subscribe\",\"messageId\":\"36f3d4b5da1545dd127176bdbbf8f2cf\",\"method\":\"SET\",\"namespace\":\"Appliance.Control.ToggleX\",\"payloadVersion\":1,\"sign\":\"195a5ea4fb950c3cb475cd5daa5d4e64\",\"timestamp\":1571919319},\"payload\":{\"togglex\":{\"channel\":0,\"onoff\":0}}}";
    postData = String("{")+"\""+"header:{"+"\""+"from"+"\""+":"+"\""+"/app/339299-e1853779db9fc7866cf6dfe0b697b0b8/subscribe"+"\""+","+"\""+"messageId"+"\""+":"+"\""+"36f3d4b5da1545dd127176bdbbf8f2cf"+"\""+","+"\""+"method"+"\""+":"+"\""+"SET"+"\""+","+"\""+"namespace"+"\""+":"+"\""+"Appliance.Control.ToggleX"+"\""+","+"\""+"payloadVersion"+"\""+":1,"+"\""+"sign"+"\""+":"+"\""+"195a5ea4fb950c3cb475cd5daa5d4e64"+"\""+","+"\""+"timestamp"+"\""+":1571919319},"+"\""+"payload"+"\""+":{"+"\""+"togglex"+"\""+":{"+"\""+"channel"+"\""+":0,"+"\""+"onoff"+"\""+":0}}}";
    Serial.println(postData);
      
    request2();

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
    //postData = "{\"header:{\"from\":\"/app/339299-e1853779db9fc7866cf6dfe0b697b0b8/subscribe\",\"messageId\":\"36f3d4b5da1545dd127176bdbbf8f2cf\",\"method\":\"SET\",\"namespace\":\"Appliance.Control.ToggleX\",\"payloadVersion\":1,\"sign\":\"195a5ea4fb950c3cb475cd5daa5d4e64\",\"timestamp\":1571919319},\"payload\":{\"togglex\":{\"channel\":0,\"onoff\":1}}}";
    postData = String("{")+"\""+"header:{"+"\""+"from"+"\""+":"+"\""+"/app/339299-e1853779db9fc7866cf6dfe0b697b0b8/subscribe"+"\""+","+"\""+"messageId"+"\""+":"+"\""+"36f3d4b5da1545dd127176bdbbf8f2cf"+"\""+","+"\""+"method"+"\""+":"+"\""+"SET"+"\""+","+"\""+"namespace"+"\""+":"+"\""+"Appliance.Control.ToggleX"+"\""+","+"\""+"payloadVersion"+"\""+":1,"+"\""+"sign"+"\""+":"+"\""+"195a5ea4fb950c3cb475cd5daa5d4e64"+"\""+","+"\""+"timestamp"+"\""+":1571919319},"+"\""+"payload"+"\""+":{"+"\""+"togglex"+"\""+":{"+"\""+"channel"+"\""+":0,"+"\""+"onoff"+"\""+":1}}}";
    Serial.println(postData);

    request2();

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
//  if (myClient1.connect(serverMSS210, portMSS210)) 
//  {
//    Serial.println("Connected.");
//    //HTTP request 1st version:
//    myClient1.println("POST /config HTTP/1.1");
//    myClient1.print("Host:");myClient1.println(serverMSS210);
//    myClient1.println("Content-Type:application/json");
//    myClient1.println("Connection:close");
//    myClient1.print("Content-Length:");myClient1.println(postData.length());
//    myClient1.println("Accept-Encoding:gzip");
//    myClient1.println("User-Agent:ARDUINO1");
//    myClient1.println();
//    myClient1.println(postData);
//    Serial.print("Code received:");
//    Serial.println(myClient1.read());
//  }
//  else
//  {
//    Serial.println("Connection failed.");
//  }  
}

void request2()//not working
{   
  Serial.println("envoie de request2()");
  
  myClient2.beginRequest();
  myClient2.post("/config");
  myClient2.sendHeader("Host", serverMSS210);
  myClient2.sendHeader("Content-Type", "application/json");
  myClient2.sendHeader("Content-Length", postData.length());
  //myClient2.sendHeader("Connection", "close");
  myClient2.sendHeader("Accept-Encoding", "gzip");
  //myClient2.sendHeader("User-Agent", "ArduinocacaHAHAHAHA");
  myClient2.sendHeader("Cache-Control", "no-cache");
  myClient2.sendHeader("Accept", "*/*");
  myClient2.beginBody();
  myClient2.print(postData);
  myClient2.endRequest();
  
  int statusCode = myClient2.responseStatusCode();
  String response = myClient2.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);    
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

void request4() // using WiFiNINA only
{
//  myClient4.stop();
//
//  // if there's a successful connection:
//    Serial.print(
//      String("HáéBH¤rÐ×EYN@À¨kÀ¨ü¦P=³U&PPOST ") + "/config" + " HTTP/1.1\r\n" +
//      "Host:" + serverMSS210 + "\r\n" +
//      "Content-Type:text/plain\r\n" +
//      "Content-Length:" + postData.length() + "\r\n" +
//      "Accept-Encoding:gzip\r\n" +
//      "User-Agent:Arduino\r\n" +
//      "Accept:*/*\r\n" +
//      "Cache-Control:no-cache\r\n" +
//      "Connection:close\r\n" +
//      "\r\n" + // CR+LF pour signifier la fin du header
//      postData + "\r\n");
//      
//  if (myClient4.connect(serverMSS210, portMSS210)) 
//  {
//    Serial.println("Connected !");
//    
//    myClient4.print(
//      String("POST ") + "/config" + " HTTP/1.1\r\n" +
//      "Host:" + serverMSS210 + "\r\n" +
//      "Content-Type:application/json\r\n" +
//      "Content-Length:" + postData.length() + "\r\n" +
//      "Accept-Encoding:gzip\r\n" +
//      "User-Agent:Arduino\r\n" +
//      "Accept:*/*\r\n" +
//      "Cache-Control:no-cache\r\n" +
//      "Connection:close\r\n" +
//      "\r\n" + // CR+LF pour signifier la fin du header
//      postData + "\n");
//
//    codeResponse = myClient4.read();
//    Serial.println(codeResponse);
//  } 
//  else 
//  {
//    // if you couldn't make a connection:
//    Serial.println("Connection failed");
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
