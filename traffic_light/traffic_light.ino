/* traffic_light controller

https://circuits4you.com/2018/12/31/esp32-devkit-esp32-wroom-gpio-pinout/

Things to note:
the left light is refered to in a reference to traffic heading parallel an
observer, and the right light perpendicular to the observer
*/

#include "secrets.h"
#include <ESPUI.h>
#include <WiFi.h>

const int TOTAL_CYCLE_DURATION = 32000;

// pin config
const int RIGHT_GREEN = 13;
const int LEFT_GREEN = 12;
const int RIGHT_YELLOW = 14;
const int LEFT_YELLOW = 27;
const int RIGHT_RED = 26;
const int LEFT_RED = 25;

const int OUTPUT_PINS_SIZE = 6;
const int OUTPUT_PINS[OUTPUT_PINS_SIZE] = {
    LEFT_GREEN, LEFT_YELLOW, LEFT_RED, RIGHT_GREEN, RIGHT_YELLOW, RIGHT_RED,
};

int MILLIS_LABEL_ID;
int LEFT_LIGHT_LABEL_ID;
int RIGHT_LIGHT_LABEL_ID;

enum SIDE { LEFT, RIGHT };

void setup(void) {
  Serial.begin(115200);

  // set all output pins to OUTPUT mode
  for (int i = 0; i < OUTPUT_PINS_SIZE; i++) {
    pinMode(OUTPUT_PINS[i], OUTPUT);
  }

  set_green_red();

  WiFi.setHostname(SECRET_HOSTNAME);

  // try to connect to existing network
  WiFi.begin(SECRET_SSID, SECRET_PASSWORD);
  Serial.print("\n\nTry to connect to existing network ");
  Serial.print(SECRET_SSID);

  // Wait for connection
  do {
    delay(500);
    Serial.print(".");
  } while (WiFi.status() != WL_CONNECTED);

  Serial.println("\n\nWiFi parameters:");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // create millis label
  MILLIS_LABEL_ID = ESPUI.label("Millis:", ControlColor::Emerald, "");
  LEFT_LIGHT_LABEL_ID =
      ESPUI.label("Left Light Status:", ControlColor::Emerald, "");
  RIGHT_LIGHT_LABEL_ID =
      ESPUI.label("Right Light Status:", ControlColor::Emerald, "");

  ESPUI.begin("traffic_light Control");
}

void loop(void) {

  // used to keep track of previous run time
  static unsigned long old_millis_time = 0;
  if (millis() - old_millis_time > 250) {
    ESPUI.print(MILLIS_LABEL_ID, String(millis()));
    old_millis_time = millis();
  }
  normal_mode();
}

void normal_mode(void) {
  enum statuses {
    green_red,
    yellow_red,
    red_red_1, // transition from straight traffic to cross traffic
    red_green,
    red_yellow,
    red_red_2, // transition from cross traffic to straight traffic
  };

  static unsigned long next_green_red = 0;
  static unsigned long next_yellow_red = 12000;
  static unsigned long next_red_red_1 = 15000;
  static unsigned long next_red_green = 16000;
  static unsigned long next_red_yellow = 28000;
  static unsigned long next_red_red_2 = 31000;

  unsigned long now = millis();
  static enum statuses pending_status = green_red;
  // switch from prior status after time in ms defined in the if block passes
  switch (pending_status) {
  case green_red:
    if (now > next_green_red) {
      Serial.println(": yellow_red");

      pending_status = yellow_red;
      set_green_red();

      next_green_red = next_green_red + TOTAL_CYCLE_DURATION;
    }
    break;

  case yellow_red:
    if (now > next_yellow_red) {
      Serial.println(": red_red_1");

      pending_status = red_red_1;
      set_yellow_red();

      next_yellow_red = next_yellow_red + TOTAL_CYCLE_DURATION;
    }
    break;

  case red_red_1:
    if (now > next_red_red_1) {
      Serial.println(": red_green");

      pending_status = red_green;
      set_red_red();

      next_red_red_1 = next_red_red_1 + TOTAL_CYCLE_DURATION;
    }
    break;

  case red_green:
    if (now > next_red_green) {
      Serial.println(": red_yellow");

      pending_status = red_yellow;
      set_red_green();

      next_red_green = next_red_green + TOTAL_CYCLE_DURATION;
    }
    break;

  case red_yellow:
    if (now > next_red_yellow) {
      Serial.println(": red_red_2");

      pending_status = red_red_2;
      set_red_yellow();

      next_red_yellow = next_red_yellow + TOTAL_CYCLE_DURATION;
    }
    break;

  case red_red_2:
    if (now > next_red_red_2) {
      Serial.println(": green_red");

      pending_status = green_red;
      set_red_red();

      next_red_red_2 = next_red_red_2 + TOTAL_CYCLE_DURATION;
    }
    break;
  }
}

void set_green(SIDE side) {
  switch (side) {
  case LEFT:
    digitalWrite(LEFT_GREEN, HIGH);
    digitalWrite(LEFT_YELLOW, LOW);
    digitalWrite(LEFT_RED, LOW);

    ESPUI.print(LEFT_LIGHT_LABEL_ID, "GREEN");
    break;

  case RIGHT:
    digitalWrite(RIGHT_GREEN, HIGH);
    digitalWrite(RIGHT_YELLOW, LOW);
    digitalWrite(RIGHT_RED, LOW);

    ESPUI.print(RIGHT_LIGHT_LABEL_ID, "GREEN");
    break;
  }
}

void set_yellow(SIDE side) {
  switch (side) {
  case LEFT:
    digitalWrite(LEFT_GREEN, LOW);
    digitalWrite(LEFT_YELLOW, HIGH);
    digitalWrite(LEFT_RED, LOW);

    ESPUI.print(LEFT_LIGHT_LABEL_ID, "YELLOW");
    break;

  case RIGHT:
    digitalWrite(RIGHT_GREEN, LOW);
    digitalWrite(RIGHT_YELLOW, HIGH);
    digitalWrite(RIGHT_RED, LOW);

    ESPUI.print(RIGHT_LIGHT_LABEL_ID, "YELLOW");
    break;
  }
}

void set_red(SIDE side) {
  switch (side) {
  case LEFT:
    digitalWrite(LEFT_GREEN, LOW);
    digitalWrite(LEFT_YELLOW, LOW);
    digitalWrite(LEFT_RED, HIGH);

    ESPUI.print(LEFT_LIGHT_LABEL_ID, "RED");
    break;

  case RIGHT:
    digitalWrite(RIGHT_GREEN, LOW);
    digitalWrite(RIGHT_YELLOW, LOW);
    digitalWrite(RIGHT_RED, HIGH);

    ESPUI.print(RIGHT_LIGHT_LABEL_ID, "RED");
    break;
  }
}

void set_green_red(void) {
  set_green(LEFT);
  set_red(RIGHT);
}

void set_yellow_red(void) {
  set_yellow(LEFT);
  set_red(RIGHT);
}

void set_red_red(void) {
  set_red(LEFT);
  set_red(RIGHT);
}

void set_red_green(void) {
  set_red(LEFT);
  set_green(RIGHT);
}

void set_red_yellow(void) {
  set_red(LEFT);
  set_yellow(RIGHT);
}
