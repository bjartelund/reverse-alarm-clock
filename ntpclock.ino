#include <TM1637Display.h> // https://github.com/avishorp/TM1637  
#include <NTPClient.h>     // https://github.com/arduino-libraries/NTPClient
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define LED_RED 13
#define LED_YELLOW 12
#define LED_GREEN 16

// Set up the character arrays for the Wifi ssid and password
const char *ssid     = "Leg";
const char *password = "Leg34Love";

int currentminute = 0;
int prevminute = 0;

// Set the UTC Time offset in seconds
// const long utcOffsetInSeconds = -21600;
const long utcOffsetInSeconds = 7200;

// Define NTP Client to get the time
WiFiUDP ntpUDP;

// Set up the Network Time Protocol Client, update with fresh time every 10 minutes
NTPClient timeClient(ntpUDP, "85.21.78.23", utcOffsetInSeconds, 600000);

// Module connection pins (Digital Pins)
#define CLK 15
#define DIO 14   

// Set up the TM1637 Display
TM1637Display display(CLK, DIO);

void setup() {
    pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
    // Set the brightness of the display ( 0xff is brightest )
    display.setBrightness(0x01);

    // Connect to the wifi point
    WiFi.begin(ssid, password);

    // Wait for the wifi to be connected
    while ( WiFi.status() != WL_CONNECTED ) {
        delay(500);
    }

    // Start the time client
    timeClient.begin();

}

void loop() {  
  display.setBrightness(0x01);
  currentminute= timeClient.getMinutes();
  if ( currentminute > prevminute )
  {
    
    // Update the time client to get the current time
    timeClient.update();
    if ( currentminute == 59 ) 
    {
      prevminute=-1; 
    }
    else 
    {
      prevminute=timeClient.getMinutes();
    }
    
  
    // Display the hours
   
   if ( 7 <= timeClient.getHours() && timeClient.getHours() < 19)
   {
      digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
   digitalWrite(LED_GREEN, HIGH);
   }
   else if ( 6 <= timeClient.getHours() && timeClient.getHours() < 20)
   {
      digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, HIGH);
   digitalWrite(LED_GREEN, LOW);
    }

   else 
   {
          digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_YELLOW, LOW);
   digitalWrite(LED_GREEN, LOW);
    }
    // Display the minutes
   display.showNumberDecEx(timeClient.getHours(), 0b11100000, true, 2, 0);
   display.showNumberDecEx(currentminute, 0b11100000, true, 2, 2); 
  
}
}
