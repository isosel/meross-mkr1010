// for WPA2 encrypted connections
// By Mickael Cherouise
// Thanks to Ilias Nafili alias IliShu for his help in understanding http protocol and node.js server.

#include <SPI.h>
#include <WiFiNINA.h>

#define SECRET_SSID "XXXXX"
#define SECRET_PASS "XXXXX"

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int status = WL_IDLE_STATUS;
int compteur;
int buttonStateOff = LOW;
int buttonStateOn = LOW;
bool boolonoff = false;
char charonoff;
// Sart Plug Adress: .183 My Node Server: .107 MKR1010: .190
int portMSS210 = 80;
char serverMSS210[] = "192.168.1.183";
//IPAddress serverMSS210(192,168,1,183);
bool clientconnected = false;
String postData;
WiFiClient myClient;
char codeResponse;

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
      
    requestFINAL();

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

    requestFINAL();

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

void requestFINAL() // using WiFiNINA only
{
  myClient.stop();
      
  if (myClient.connect(serverMSS210, portMSS210)) 
  {
    Serial.println("Connected !");
    
    myClient.print
    (
      String("POST ") + "/config" + " HTTP/1.1\r\n" +
      "Host: " + "192.168.1.183" + "\r\n" +
      "Content-Type: application/json\r\n" +
      "Content-Length: " + postData.length() + "\r\n" +
      "Accept-Encoding: gzip\r\n" +
      "User-Agent: Arduino/2.2.0\r\n" +
      "Accept: */*\r\n" +
      "Cache-Control: no-cache\r\n" +
      "Connection: close\r\n" +
      "\r\n" + // CR+LF pour signifier la fin du header
      postData
    );

//    while(myClient.available())
//    {
//      codeResponse = myClient.read();
//      Serial.write(codeResponse);
//    }
  } 
  else 
  {
    // if you couldn't make a connection:
    Serial.println("Connection failed");
  }  
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
