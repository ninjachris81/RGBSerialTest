#include <FastLED.h>
#define NUM_LEDS 144

CRGBArray<NUM_LEDS> leds;

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<WS2811,3, GRB>(leds, NUM_LEDS).setCorrection( TypicalSMD5050 ); 
  Serial.println(F("Usage:"));
  Serial.println(F("rgb (0-255),(0-255),(0-255)"));
  Serial.println(F("br (0-255)"));

  FastLED.setBrightness(100);
  fill_solid( leds, NUM_LEDS, CRGB::Red);
  FastLED.show();
}

void loop(){
  if(Serial.available()) {
    String s = Serial.readStringUntil('\n');
    if (s.startsWith("rgb ")) {
      CRGB newColor;
      s = s.substring(4);
      newColor.r = constrain(getValue(s, ',', 0), 0, 255);
      newColor.g = constrain(getValue(s, ',', 1), 0, 255);
      newColor.b = constrain(getValue(s, ',', 2), 0, 255);

      Serial.print(F("Setting color "));
      Serial.print(newColor.r);
      Serial.print(",");
      Serial.print(newColor.g);
      Serial.print(",");
      Serial.println(newColor.b);

      fill_solid( leds, NUM_LEDS, newColor);
      FastLED.show();
    } else if (s.startsWith("br ")) {
      uint8_t newBrightness = constrain(s.substring(3).toInt(), 0, 255);

      Serial.print(F("Setting brightness: "));
      Serial.println(newBrightness);
      
      FastLED.setBrightness(newBrightness);
      FastLED.show();
    } else {
      Serial.println("Unknown command");
    }
  }
  delay(10);
}

int getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]).toInt() : 0;
}
