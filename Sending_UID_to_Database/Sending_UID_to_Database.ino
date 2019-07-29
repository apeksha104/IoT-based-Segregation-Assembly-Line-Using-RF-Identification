#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>

#define RST_PIN 5          
#define SS_PIN 15         
#define FIREBASE_HOST "#############"
#define FIREBASE_AUTH "#########"
#define WIFI_SSID "#########"
#define WIFI_PASSWORD "########"

int j=0;

MFRC522 mfrc522(SS_PIN, RST_PIN);         // Create MFRC522 instance



void setup() {
	
	Serial.begin(9600);		                  // Initialize serial communications with the PC
	
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);   // Connect to WiFi
  Serial.print("connecting");             
  while (WiFi.status() != WL_CONNECTED) {
  Serial.print(".");
  delay(500);
  } 

  Serial.println();                       // WiFi Connected 
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);  // Connect to Firebase
  delay(500);	
	
	while (!Serial);		  // Do nothing if no serial port is opened 
	SPI.begin();			    // Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
  }



void loop() {
  int i;
  String uid = "";
	if ( ! mfrc522.PICC_IsNewCardPresent()) {    // Look for new Cards
		return;
	}
	if ( ! mfrc522.PICC_ReadCardSerial()) {      // Read Card
		return;
	}

	
	Serial.println("UID: ");
  for(i=0;i < mfrc522.uid.size; i++){
    uid += mfrc522.uid.uidByte[i];             // Store the UID in a String
   }
  Serial.println(uid);                         // Display UID on Serial Monitor
  mfrc522.PICC_HaltA();                        // Stop reading the PICC
  delay(500);

  j+=1;                                        // Increment Customer Counter for Database
  String customer = "/Customer ";
  String uidd = "/UID";
  String path = customer + j + uidd;           // Store Database Path in a String
  
  
  Firebase.setString(path, uid);               // Push UID to Path stored in String in Database
  
    
  if (Firebase.failed()) {                     // handle error
      Serial.print("setting /message failed:");
      Serial.println(Firebase.error());  
      return;
  }
   return;
  }

