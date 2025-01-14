#include <RH_ASK.h>
#include <SPI.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 10, 5, 4, 3, 2);

RH_ASK driver;

const int piezoPin = 7; const int ledPin = 8;

const int notes[3][2] = 
{
  {261, 277},  // C, C#
  {294, 311},  // D, D#
  {349, 370}   // F, F#
};

void setup() 
{
  Serial.begin(9600);
  lcd.begin(16, 2);  

  if (!driver.init())          
  {
    lcd.print("Init failed!");
    Serial.println("Initialization failed!");
    while (1);
  }  
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("Receiver ready");
    Serial.println("Receiver ready");
    delay(2000);
    lcd.clear();
  }

  pinMode(piezoPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() 
{
  uint8_t buffer[RH_ASK_MAX_MESSAGE_LEN]; // Buffer for incoming message
  uint8_t bufferLength = sizeof(buffer);

  // Check for incoming messages
  if (driver.recv(buffer, &bufferLength)) 
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Receiving...");

    buffer[bufferLength] = '\0'; // Null-terminate the buffer
    int note = atoi((char *)buffer); // Convert message to integer

    if (note)
    {
      Serial.print("Received note: ");
      Serial.println(note);

      if (note == notes[0][0]) 
      {
        lcd.setCursor(0, 1);
        lcd.print("Note C");
      } 
      else if (note == notes[0][1]) 
      {
        lcd.setCursor(0, 1);
        lcd.print("Note C#");
      } 
      else if (note == notes[1][0]) 
      {
        lcd.setCursor(0, 1);
        lcd.print("Note D");
      } 
      else if (note == notes[1][1]) 
      {
      lcd.setCursor(0, 1);
        lcd.print("Note D#");
      } 
      else if (note == notes[2][0]) 
      {
        lcd.setCursor(0, 1);
        lcd.print("Note F");
      } 
      else if (note == notes[2][1]) 
      {
        lcd.setCursor(0, 1);
        lcd.print("Note F#");
      } 
      else 
      {
        lcd.setCursor(0, 1);
        lcd.print("Unknown Note");
        Serial.println("Unknown Note Received");
      }

      // Play the note
      digitalWrite(ledPin, HIGH);
      tone(piezoPin, note, 500);
      delay(500);
      noTone(piezoPin);
      digitalWrite(ledPin, LOW);
    }  
  }
}

