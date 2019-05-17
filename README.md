<img src="https://www.aljoschatheil.de/smartee.png" alt="LOGO" title="" />

<hr>
A smart tea machine that pulls your tea bag out of the hot water when the tea is ready. 


#HARDWARE
1x ESP8266 NodeMCU v2 </br>
1x Rotary Encoder</br>
1x 12er LED Ring</br>
1x Piezo Buzzer</br>
1x Small Breadboard</br>
1x USB to Micro USB Kabel</br>
Jumper Wires (Male to Male & Male to Female)</br>

#Code
//LED
#include <Adafruit_NeoPixel.h>
#define ledpin D2
Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, ledpin, NEO_GRB + NEO_KHZ800);
int lednum = 12;
uint32_t color1 = strip.Color(255, 0, 255);
uint32_t color2 = strip.Color(0, 255, 0);

//Poti
#include <Encoder.h>
const int CLK = D7;
const int DT = D6;
const int SW = D4;
int buttonState;
int schalter = 0;
int altePosition = -999;
int neuePosition = 1;
Encoder meinEncoder(DT, CLK);

//Buzzer
const int buzzer = D1;

//Motor
#include <Servo.h>
Servo myservo;
int myservostatus = 180;    //0 = im Uhrzeigersinn, 90 = Stop, 180 = gegen Uhrzeigersinn

int go = false;


void setup() {
  Serial.begin(115200);

  //LED
  strip.begin();
  strip.setBrightness(30); //adjust brightness here
  strip.show(); // Initialize all pixels to 'off'

  //Poti
  meinEncoder.write(0);
  neuePosition = meinEncoder.read() / 4;
  pinMode(SW, INPUT_PULLUP);

  //Buzzer
  pinMode(buzzer, OUTPUT);

  //Motor
  myservo.attach(D3);
}

void loop() {


  if (go == false) {

    //Poti
    neuePosition = meinEncoder.read() / 4;

    if (neuePosition != altePosition) {
      //    Serial.print("ALT: ");
      //    Serial.print(altePosition);
      //    Serial.print("  - NEU: ");
      //    Serial.println(neuePosition);

      if (neuePosition > altePosition) {
        strip.setPixelColor(neuePosition, color1);
        strip.show();
      } else if (neuePosition < altePosition) {
        strip.setPixelColor(neuePosition, color1);
        strip.setPixelColor(neuePosition + 1, 0);

        strip.show();
      }


      altePosition = neuePosition;
    }

    if (neuePosition < 0) {
      meinEncoder.write(0);
    } else if (neuePosition > lednum - 1) {
      meinEncoder.write(44);
    }




    //Servo
    buttonState = digitalRead(SW);

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
      delay(1000);
      Serial.println(i);
      strip.setPixelColor(i, color2);
      strip.show();
    }
    delay(1000);
    strip.setPixelColor(1, color2);

    Serial.print("FIN");
    strip.clear();
    meinEncoder.write(0);

    myservo.write(0);

    delay(3000);

    myservo.write(90);

    tone(buzzer, 1000);
    delay(1000);
    noTone(buzzer);

    delay(5000);

    tone(buzzer, 1000);
    delay(700);
    noTone(buzzer);
    delay(400);
    tone(buzzer, 1000);
    delay(700);
    noTone(buzzer);

    strip.clear();

    go = false;

  }

}


# Links
[Portfoilio](https://www.aljoschatheil.de/)

