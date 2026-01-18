#define BLYNK_TEMPLATE_ID "TMPL6d16ml4BU"
#define BLYNK_TEMPLATE_NAME "smart window"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>
#include <Adafruit_BMP085.h>

char auth[] = "2StzBP9qE3jsBu8KTGriLM2zNUBL";
char ssid[] = "Galaxy A22";
char pass[] = "0122333.";

Servo rightWindowServo, leftWindowServo, rightClothServo, leftClothServo, rightHingeServo, leftHingeServo, flameServo;

#define RIGHT_WINDOW_PIN  4
#define LEFT_WINDOW_PIN   5
#define RIGHT_CLOTH_PIN   12
#define LEFT_CLOTH_PIN    27
#define RIGHT_HINGE_PIN   18
#define LEFT_HINGE_PIN    19
#define FLAME_SERVO_PIN   2
#define buz 15
#define MQ2_PIN 33

#define RAIN_SENSOR_PIN 34
#define LIGHT_SENSOR_PIN 35
#define FLAME_SENSOR_PIN 32

Adafruit_BMP085 bmp;
bool isAutomatic = true;
unsigned long lastLightHighTime = 0;
bool lightWasHigh = false;

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);

  rightWindowServo.attach(RIGHT_WINDOW_PIN);
  leftWindowServo.attach(LEFT_WINDOW_PIN);
  rightClothServo.attach(RIGHT_CLOTH_PIN);
  leftClothServo.attach(LEFT_CLOTH_PIN);
  rightHingeServo.attach(RIGHT_HINGE_PIN);
  leftHingeServo.attach(LEFT_HINGE_PIN);
  flameServo.attach(FLAME_SERVO_PIN);

  rightWindowServo.write(0);
  leftWindowServo.write(180);
  rightClothServo.write(90);
  leftClothServo.write(90);
  rightHingeServo.write(108);
  leftHingeServo.write(82);
  flameServo.write(0);

  pinMode(buz, OUTPUT);
  digitalWrite(buz, LOW);
  pinMode(MQ2_PIN, INPUT);
}

void moveServoSmoothly(Servo &servo, int startPos, int endPos, int delayTime) {
  if (startPos < endPos) {
    for (int pos = startPos; pos <= endPos; pos++) {
      servo.write(pos);
      delay(delayTime);
    }
  } else {
    for (int pos = startPos; pos >= endPos; pos--) {
      servo.write(pos);
      delay(delayTime);
    }
  }
}

void loop() {
  Blynk.run();

  if (isAutomatic) {
    int flameValue = analogRead(FLAME_SENSOR_PIN);
    Serial.print("Flame Sensor Value: ");
    Serial.println(flameValue);

    if (flameValue < 1000) {
      Serial.println("Flame detected! Moving servo and activating buzzer...");
      rightHingeServo.write(5);
      leftHingeServo.write(175);
      delay(100);
      flameServo.write(90);

      for (int i = 0; i < 3; i++) {
        digitalWrite(buz, HIGH);
        delay(800);
        digitalWrite(buz, LOW);
        delay(200);
      }
    } else {
      flameServo.write(0);
    }

    int rainValue = analogRead(RAIN_SENSOR_PIN);
    Serial.print("Rain Sensor Value: ");
    Serial.println(rainValue);

    if (rainValue < 1000) {
      moveServoSmoothly(rightClothServo, rightClothServo.read(), 135, 10);
      moveServoSmoothly(leftClothServo, leftClothServo.read(), 45, 10);
      delay(1000);
      moveServoSmoothly(rightHingeServo, rightHingeServo.read(), 108, 10);
      moveServoSmoothly(leftHingeServo, leftHingeServo.read(), 72, 10);
      delay(5000);
      rainValue = analogRead(RAIN_SENSOR_PIN);
      Serial.print("Rain Sensor Value after delay: ");
      Serial.println(rainValue);

      if (rainValue >= 1000) {
        moveServoSmoothly(rightHingeServo, rightHingeServo.read(), 5, 20);
        moveServoSmoothly(leftHingeServo, leftHingeServo.read(), 175, 20);
        delay(1000);
        moveServoSmoothly(rightClothServo, rightClothServo.read(), 90, 20);
        moveServoSmoothly(leftClothServo, leftClothServo.read(), 90, 20);
        Serial.println("No rain detected after delay, resetting hinges.");
      }
    } else {
      int lightValue = analogRead(LIGHT_SENSOR_PIN);
      Serial.print("Light Sensor Value: ");
      Serial.println(lightValue);

      int rightWindowPos = map(lightValue, 0, 4095, 0, 110);
      rightWindowServo.write(rightWindowPos);

      if (lightValue > 2000) {
        if (!lightWasHigh) {
          lastLightHighTime = millis();
          lightWasHigh = true;
        }

        if (millis() - lastLightHighTime >= 2000) {
          leftWindowServo.write(90);
        }
      } else {
        if (lightWasHigh) {
          leftWindowServo.write(180);
          lightWasHigh = false;
        }
      }
    }

    int smokeDetected = digitalRead(MQ2_PIN);
    if (smokeDetected == LOW) {
      digitalWrite(buz, HIGH);
      Blynk.logEvent("smoke_alert", "Smoke detected! Take action!");
      Blynk.virtualWrite(V9, smokeDetected);
      delay(5000);
      digitalWrite(buz, LOW);
    } else {
      digitalWrite(buz, LOW);
    }

    delay(1000);
    if (bmp.begin()) {
      static float lastTemp = 0;
      float temperature = bmp.readTemperature();
      if (abs(temperature - lastTemp) >= 1) {
        lastTemp = temperature;
        Blynk.virtualWrite(V6, temperature);
        Serial.print("Temperature: "); Serial.print(temperature); Serial.println(" *C");
      }
    } else {
      Serial.println("BMP180 sensor not detected!");
    }
  }
}

void executeMainCommand() {
  moveServoSmoothly(rightClothServo, rightClothServo.read(), 180, 20);
  moveServoSmoothly(leftClothServo, leftClothServo.read(), 0, 20);
  delay(100);
  moveServoSmoothly(leftHingeServo, leftHingeServo.read(), 0, 20);
  moveServoSmoothly(rightHingeServo, rightHingeServo.read(), 180, 20);
}

BLYNK_WRITE(V10) {
  isAutomatic = param.asInt();
}

BLYNK_WRITE(V5) {
  if (param.asInt() == 1) {
    executeMainCommand();
  }
}
