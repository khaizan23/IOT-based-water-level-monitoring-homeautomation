#define BLYNK_TEMPLATE_ID "TMPLPH9rUlUC"
#define BLYNK_DEVICE_NAME "Water level Monitoring System"
#define BLYNK_AUTH_TOKEN "7oNsYxbLR4H2Sao0_xuFqwllutqz0RyB"
#define APP_DEBUG
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include "ThingSpeak.h"

int cm = 0;
int keyIndex = 0;
WiFiClient client;
String myStatus = "";
unsigned long myChannelNumber = 1902031;
const char *myWriteAPIKey = "RT0YO8TFGU07XURO";

const int trigPin = 16;
const int echoPin = 17;
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;

#define RelayPin1 2
#define RelayPin2 4
#define RelayPin3 5
#define RelayPin4 18
int pump = 22;

float setVal = 7;
#define SwitchPin1 19
#define SwitchPin2 0
#define SwitchPin3 13
#define SwitchPin4 3

#define wifiLed 16

#define VPIN_BUTTON_1 V1
#define VPIN_BUTTON_2 V2
#define VPIN_BUTTON_3 V3
#define VPIN_BUTTON_4 V4
#define ultrasonic V5
int toggleState_1 = 1;
int toggleState_2 = 1;
int toggleState_3 = 1;
int toggleState_4 = 1;

int wifiFlag = 0;

bool notifyOn, notifyOff = true;
char auth[] = "RUuCNjzMmcq28JDjuixBqqmZdSW4UNXu";
char ssid[] = "Service Provider";
char pass[] = "@Matinik298-2021";
BlynkTimer timer;

void relayOnOff(int relay)
{

  switch (relay)
  {
  case 1:
    if (toggleState_1 == 1)
    {
      digitalWrite(RelayPin1, LOW); // turn on relay 1
      toggleState_1 = 0;
      Serial.println("Device1 ON");
      ThingSpeak.setField(2, 100);
    }
    else
    {
      digitalWrite(RelayPin1, HIGH); // turn off relay 1
      toggleState_1 = 1;
      Serial.println("Device1 OFF");
      ThingSpeak.setField(2, 0);
    }
    delay(100);
    break;
  case 2:
    if (toggleState_2 == 1)
    {
      digitalWrite(RelayPin2, LOW); // turn on relay 2
      toggleState_2 = 0;
      Serial.println("Device2 ON");
      ThingSpeak.setField(3, 100);
    }
    else
    {
      digitalWrite(RelayPin2, HIGH); // turn off relay 2
      toggleState_2 = 1;
      Serial.println("Device2 OFF");
      ThingSpeak.setField(3, 0);
    }
    delay(100);
    break;
  case 3:
    if (toggleState_3 == 1)
    {
      digitalWrite(RelayPin3, LOW); // turn on relay 3
      toggleState_3 = 0;
      Serial.println("Device3 ON");
      ThingSpeak.setField(4, 100);
    }
    else
    {
      digitalWrite(RelayPin3, HIGH); // turn off relay 3
      toggleState_3 = 1;
      Serial.println("Device3 OFF");
      ThingSpeak.setField(4, 0);
    }
    delay(100);
    break;
  case 4:
    if (toggleState_4 == 1)
    {
      digitalWrite(RelayPin4, LOW); // turn on relay 4
      toggleState_4 = 0;
      Serial.println("Device4 ON");
      ThingSpeak.setField(5, 100);
    }
    else
    {
      digitalWrite(RelayPin4, HIGH); // turn off relay 4
      toggleState_4 = 1;
      Serial.println("Device4 OFF");
      ThingSpeak.setField(5, 0);
    }
    delay(100);
    break;
  default:
    break;
  }
}

void with_internet()
{
  // Manual Switch Control
  if (digitalRead(SwitchPin1) == LOW)
  {
    delay(200);
    relayOnOff(1);
    Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1); // Update Button Widget
  }
  else if (digitalRead(SwitchPin2) == LOW)
  {
    delay(200);
    relayOnOff(2);
    Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2); // Update Button Widget
  }
  else if (digitalRead(SwitchPin3) == LOW)
  {
    delay(200);
    relayOnOff(3);
    Blynk.virtualWrite(VPIN_BUTTON_3, toggleState_3); // Update Button Widget
  }
  else if (digitalRead(SwitchPin4) == LOW)
  {
    delay(200);
    relayOnOff(4);
    Blynk.virtualWrite(VPIN_BUTTON_4, toggleState_4); // Update Button Widget
  }
}
void without_internet()
{
  // Manual Switch Control
  if (digitalRead(SwitchPin1) == LOW)
  {
    delay(200);
    relayOnOff(1);
  }
  else if (digitalRead(SwitchPin2) == LOW)
  {
    delay(200);
    relayOnOff(2);
  }
  else if (digitalRead(SwitchPin3) == LOW)
  {
    delay(200);
    relayOnOff(3);
  }
  else if (digitalRead(SwitchPin4) == LOW)
  {
    delay(200);
    relayOnOff(4);
  }
}

BLYNK_CONNECTED()
{

  Blynk.syncVirtual(VPIN_BUTTON_1);
  Blynk.syncVirtual(VPIN_BUTTON_2);
  Blynk.syncVirtual(VPIN_BUTTON_3);
  Blynk.syncVirtual(VPIN_BUTTON_4);
  Blynk.syncVirtual(ultrasonic);
}

// When App button is pushed - switch the state
void updates(int myField, int val)
{
  int x = ThingSpeak.writeField(myChannelNumber, myField, val, myWriteAPIKey);
  if (x == 200)
  {
    Serial.println(String(myField) + "Channel update successful.");
  }
  else
  {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
}
BLYNK_WRITE(VPIN_BUTTON_1)
{
  toggleState_1 = param.asInt();
  digitalWrite(RelayPin1, toggleState_1);
  // ThingSpeak.setField(2, toggleState_1);
  updates(2, toggleState_1);
}

BLYNK_WRITE(VPIN_BUTTON_2)
{
  toggleState_2 = param.asInt();
  digitalWrite(RelayPin2, toggleState_2);
  // ThingSpeak.setField(3, toggleState_2);
  updates(3, toggleState_2);
}

BLYNK_WRITE(VPIN_BUTTON_3)
{
  toggleState_3 = param.asInt();
  digitalWrite(RelayPin3, toggleState_3);
  // ThingSpeak.setField(4, toggleState_3);
  updates(4, toggleState_3);
}

BLYNK_WRITE(VPIN_BUTTON_4)
{
  toggleState_4 = param.asInt();
  digitalWrite(RelayPin4, toggleState_4);
  // ThingSpeak.setField(5, toggleState_4);
  updates(5, toggleState_4);
}

void checkBlynkStatus()
{

  bool isconnected = Blynk.connected();
  if (isconnected == false)
  {
    wifiFlag = 1;
    digitalWrite(wifiLed, HIGH); // Turn off WiFi LED
  }
  if (isconnected == true)
  {
    wifiFlag = 0;
    digitalWrite(wifiLed, LOW); // Turn on WiFi LED
  }
}

void updateThing()
{
  // set the fields with the values
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(20);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance
  distanceCm = duration * SOUND_SPEED / 2;

  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;

  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);
  if (distanceCm < setVal && distanceCm != 0)
  {
    Serial.println("pump is off ");
    digitalWrite(pump, LOW);
    notifyOn = true;
    // digitalWrite(pump, HIGH);
    //    if (notifyOff) {
    //      Blynk.notify("pump is off!");
    //      notifyOff = false;
    //    }
  }
  else if (distanceCm > 27.00 && distanceCm != 0)
  {
    digitalWrite(pump, HIGH);
    Serial.println("pump is on ");
    notifyOff = true;
    //    if (notifyOn) {
    //      Blynk.notify("pump is on!");
    //      notifyOn = false;
    //    }
  }
  ThingSpeak.setField(1, distanceCm);
  Blynk.virtualWrite(V5, distanceCm);
  /// figure out the status message
  myStatus = String("field1 is greater than field2");

  // set the status
  ThingSpeak.setStatus(myStatus);

  // write to the ThingSpeak channel

  // int x =  ThingSpeak.writeField(myChannelNumber, 1, distanceCm, myWriteAPIKey);
  //   if (x == 200) {
  //   Serial.println("Channel update successful.");
  // }
  // else {
  // Serial.println("Problem updating channel. HTTP error code " + String(x));
  // }
}
void setup()
{
  delay(5000);// simple delay before start
  Serial.begin(115200);
  Serial.println("testing and starting up the code now");
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);
  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);

  pinMode(pump, OUTPUT);
  pinMode(wifiLed, OUTPUT);

  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);
  pinMode(SwitchPin3, INPUT_PULLUP);
  pinMode(SwitchPin4, INPUT_PULLUP);

  // During Starting all Relays should TURN OFF
  digitalWrite(RelayPin1, toggleState_1);
  digitalWrite(RelayPin2, toggleState_2);
  digitalWrite(RelayPin3, toggleState_3);
  digitalWrite(RelayPin4, toggleState_4);
  digitalWrite(pump, LOW);
  timer.setInterval(5000L, updateThing);
  timer.setInterval(3000L, checkBlynkStatus); // check if Blynk has a notification
  Blynk.begin(auth, ssid, pass);
  while (!Serial)
  {
  }

  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
}

void loop()
{
  // put your main code here, to run repeatedly:

  delay(100);

  //=====================================
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi Not Connected");
    delay(1000);
  }
  else
  {
    Serial.println("WiFi Connected");
    Blynk.run();
  }

  timer.run(); // Initiates SimpleTimer
  if (wifiFlag == 0)
    with_internet();
  else
    without_internet();
  delay(2000);
}