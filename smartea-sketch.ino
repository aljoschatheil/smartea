//LED
#include <Adafruit_NeoPixel.h>
#define ledpin D2
Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, ledpin, NEO_GRB + NEO_KHZ800);
int lednum = 12;
uint32_t color1 = strip.Color(0, 0, 255);
uint32_t color2 = strip.Color(0, 255, 0);
uint32_t color3 = strip.Color(255, 255, 255);

//POTI
#include <Encoder.h>
const int CLK = D7;
const int DT = D6;
const int SW = D4;
int buttonState;
int schalter = 0;
int altePosition = -999;
int neuePosition = 1;
Encoder meinEncoder(DT, CLK);

//BUZZER
const int buzzer = D1;

//MOTOR
#include <Servo.h>
Servo myservo;
int myservostatus = 90;    //0 = im Uhrzeigersinn, 90 = Stop, 180 = gegen Uhrzeigersinn

//ALLGEMEIN
int go = false;
int minute = 10000;
int wartezeit = 5000;
int motordrehzeit = 2200;


//SETUP
void setup() {
  Serial.begin(115200);

  //LED
  strip.begin();
  strip.setBrightness(30); //adjust brightness here
  strip.show(); // Initialize all pixels to 'off'

  //POTI
  meinEncoder.write(0);
  neuePosition = meinEncoder.read() / 4;
  pinMode(SW, INPUT_PULLUP);

  //BUZZER
  pinMode(buzzer, OUTPUT);

  //MOTOR
  myservo.attach(D3);

  Serial.println("Smartea ready!");
}


//LOOP
void loop() {

  //Wenn die Maschiene gerade keine Zeit runter zaehlt
  if (go == false) {

    //Poti
    neuePosition = meinEncoder.read() / 4;

    //Servo
    buttonState = digitalRead(SW);

    //Nur Werte von 1 b is 12 zulassen
    if (neuePosition < 0) {
      meinEncoder.write(0);
    } else if (neuePosition > lednum - 1) {
      meinEncoder.write(44);
    }

    //Zeiteinstellung
    if (neuePosition != altePosition) {
      if (neuePosition > altePosition) {
        strip.setPixelColor(neuePosition, color1);
        strip.show();
      } else if (neuePosition < altePosition) {
        strip.setPixelColor(neuePosition, color1);
        strip.setPixelColor(neuePosition + 1, 0);
        strip.show();
      }
      altePosition = neuePosition;

      Serial.print("TIME: ");
      Serial.print(neuePosition + 1);
      Serial.println(" min");
    }

    if (buttonState == 0 && schalter == 0) {
      Serial.print("GO -  ");
      Serial.println(neuePosition + 1);
      go = true;
      schalter = 1;
    } else if (buttonState == 1) {
      schalter = 0;
    }
  } else if (go == true) {
    for (int i = neuePosition; i > -1; i--) {
      for (int j = 0; j <= minute; j += minute / 30) {
        delay(minute / 60);
        strip.setPixelColor(i, color2);
        strip.show();
        delay(minute / 60);
        strip.setPixelColor(i, color1);
        strip.show();
        Serial.println("blink");
      }

      Serial.println(i);
      strip.setPixelColor(i, color2);
      strip.show();
    }
    delay(1000);
    strip.setPixelColor(1, color2);

    Serial.print("FIN");
    strip.clear();
    meinEncoder.write(0);

    //Motor Start
    myservo.write(0);

    for (int l = 0; l < 26; l++) {
      if (l % 12 == 0) {
        strip.setPixelColor(11, 0);
        strip.show();
      }

      strip.setPixelColor(l % 12 - 1, 0);
      strip.show();

      strip.setPixelColor(l % 12, color3);
      strip.show();
      delay(100);
    }

    //Motor Stop
    myservo.write(90);

    //Alle LEDs gruen
    for (int l = 0; l < 12; l++) {
      strip.setPixelColor(l, color2);
      strip.show();
    }

    //Tone
    tone(buzzer, 1000);
    delay(1000);
    noTone(buzzer);

    //Tee auf Trinktemperatur
    delay(wartezeit);
    tone(buzzer, 1000);
    delay(700);
    noTone(buzzer);
    delay(400);
    tone(buzzer, 1000);
    delay(700);
    noTone(buzzer);

    //Alle LEDs aus
    strip.clear();
    strip.show();

    go = false;

    //Erste LED an
    strip.setPixelColor(0, color1);
    strip.show();
  }
}
