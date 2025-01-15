#include <Keypad.h>
#include <RH_ASK.h>
#include <SPI.h>

RH_ASK driver;

const byte Rows = 3;
const byte Cols = 2;

char keys[Rows][Cols] = 
{
  {'0', '1'},
  {'2', '3'},
  {'4', '5'}
};

byte rowPins[Rows] = {3, 4, 5}; byte colPins[Cols] = {6,  7};

const int piezoPin = 8; 
const int ledPin = 2;

int notes[3][2] = 
{
  {261, 277},  // C, C#
  {294, 311},  // D, D#
  {349, 370}   // F, F#
};

int flattenedNotes[6];

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, Rows, Cols);

void setup()
{
  Serial.begin(9600);

  Serial.println("Press Keys: ");

  int index = 0;

  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 2; j++)
    {
      flattenedNotes[index++] = notes[i][j];
    }
  }

  if (!driver.init())
  {
    Serial.println("RF initialization failed!");

    while (1);
  }

  else
  {
    Serial.println("Transmitter Ready");
  }
}

void loop()
{
  char key = keypad.getKey();

  if (key)
  {
    Serial.print("Key Pressed: ");
    Serial.println(key);

    int oneDindex = key - '0';  // Convert char to int (e.g., '0' -> 0, '1' -> 1)

    if (oneDindex >= 0 && oneDindex < 6) // Ensure valid range
    {
      Serial.print("Playing note: ");
      Serial.println(flattenedNotes[oneDindex]);

      playTone(flattenedNotes[oneDindex], 500);

      sendMessage(flattenedNotes[oneDindex]);
    }
    else
    {
      Serial.println("Invalid key press.");
    }
  }
}

void playTone(int note, int duration)
{
  digitalWrite(ledPin, HIGH);
  tone(piezoPin, note, duration);
  delay(duration);
  noTone(piezoPin);
  digitalWrite(ledPin, LOW);
}

void sendMessage(int note)
{
  char message[10];
  sprintf(message, "%i", note);
  driver.send((uint8_t *)message, strlen(message));
  driver.waitPacketSent();
}
