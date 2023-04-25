//includes library
#include <Adafruit_NeoPixel.h>
//declares matrix; (how many, pin);
int matrixCount = 16 * 16;
int matrixPin = 7;

Adafruit_NeoPixel matrix(matrixCount, matrixPin);

//declares colors needed
uint32_t red = matrix.Color(255, 0, 0);
uint32_t green = matrix.Color(0, 255, 0);
uint32_t yellow = matrix.Color(255, 255, 0);
uint32_t white = matrix.Color(255, 255, 255);
uint32_t blue = matrix.Color(0, 0, 255);
uint32_t purple = matrix.Color(255, 0, 255);
uint32_t gold = matrix.Color(255, 187, 51);
uint32_t orange = matrix.Color(255, 165, 0);
uint32_t rando = matrix.Color(random(0,255), random(100,255), random(0,100));

//toggle-- to read previous reading to help debug superspeed code reader
boolean lastState;
boolean buttonReading = false;
boolean currentState = false;

int button = 3;

static int  lightState = 0;

void setup() {
  Serial.begin(9600);
  matrix.begin();
  pinMode(button, INPUT);
}

void loop()
{
  // matrix.setBrightness(255);
  lastState = currentState;
  currentState = debounce(button, lastState);

  if (currentState == true && lastState == false) {
    lightState ++;
    matrix.show();
    if (lightState > 2) {
      lightState = 0;
      matrix.show();
    }
  }
  switch (lightState) {
    case 0:
      setAll(gold);

      Serial.println("in case 0");
      break;

    case 1:
      clockWork(red, 50);

      // 256 pixels in 300,000 millisecons (to get from top to bottom in 5 minutes)
      //1171.875 milliseconds must be between each index postion change
      //50 ms only here for class presentation

      Serial.println("in case 1");

      break;

    case 2:

      sweep(red, blue, green, 100);
      Serial.println("in case 2");
      break;
  }
}
//NEEDER
void setAll(uint32_t aColor) {
  for (int i = 0; i < matrix.numPixels(); i++) {
    matrix.setPixelColor(i, aColor);
  }
  matrix.show();
}

void clockWork(uint32_t aColor, int anInterval) {

  static int index = 0;
  static unsigned long startTime = millis();
  unsigned long currentTime = millis();
  if (currentTime - startTime >= anInterval) {
    //turn on 0th neo pix
    matrix.setPixelColor(index, aColor);
    matrix.show();
    //changes position
    index++;
    // matrix.clear();
    //starts new loop(timer)
    startTime = millis();
  }
  //sends light back to 0th neo
  if (index >= matrix.numPixels() ) {
    index = 0;
    lightState++;
    matrix.clear();

  }



}

//THIRD SETTING
void sweep (uint32_t c1, uint32_t c2, uint32_t c3, int anInterval) {
  static int state = 0;
  static unsigned long startTime = millis();
  unsigned long currentTime = millis();
  if (state == 0) {
    for (int i = 0; i < matrix.numPixels(); i++) {
      if (i % 3 == 0) {
        matrix.setPixelColor(i, c1);
      }
      else  if (i % 3 == 1)
        matrix.setPixelColor(i, c2);
      else   if (i % 3 == 2)
        matrix.setPixelColor(i, c3);
    }
    matrix.show();
  }
  if (state == 1) {
    for (int i = 0; i < matrix.numPixels(); i++) {
      if (i % 3 == 0) {
        matrix.setPixelColor(i, c3);
      }
      else if (i % 3 == 1)
        matrix.setPixelColor(i, c1);

      else if (i % 3 == 2)
        matrix.setPixelColor(i, c2);
    }
    matrix.show();
  }
  if (state == 2) {
    for (int i = 0; i < matrix.numPixels(); i++) {
      if (i % 3 == 0) {
        matrix.setPixelColor(i, c2);
      }
      else  if (i % 3 == 1)
        matrix.setPixelColor(i, c3);

      else   if (i % 3 == 2)
        matrix.setPixelColor(i, c1);
    }
    matrix.show();
  }
  if (currentTime - startTime >= anInterval) {
    state ++;
    if (state > 2) {
      state = 0;
    }
    startTime = millis();
  }
}

boolean debounce(int aButtonPin, boolean aLastReading) {
  boolean currentReading = digitalRead(aButtonPin);
  if (currentReading == true && aLastReading == false) {
    delay(30);
  }

  return currentReading;

}
