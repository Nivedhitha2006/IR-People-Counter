int sensorPin = 2;
int ledPin = 13; // Built-in LED

int count = 0;

int lastState = HIGH;
unsigned long lastCountTime = 0;

unsigned long minGap = 2000;   // 2 sec gap
unsigned long minBlock = 150;  // ignore noise

unsigned long blockStart = 0;

void setup() {
  pinMode(sensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  int state = digitalRead(sensorPin);
  unsigned long currentTime = millis();

  // Person starts crossing
  if (lastState == HIGH && state == LOW) {
    blockStart = currentTime;
  }

  // Person finished crossing
  if (lastState == LOW && state == HIGH) {

    unsigned long blockTime = currentTime - blockStart;
    unsigned long gap = currentTime - lastCountTime;

    // ✅ NORMAL COUNT
    if (blockTime > minBlock && gap > minGap) {

      count++;
      Serial.print("Count: ");
      Serial.println(count);

      // LED Blink
      digitalWrite(ledPin, HIGH);
      delay(200);
      digitalWrite(ledPin, LOW);

      lastCountTime = currentTime;
    }

    // ❌ MISALIGNMENT
    else if (gap < 1000) {

      Serial.println("Misalignment!");

      for (int i = 0; i < 2; i++) {
        digitalWrite(ledPin, HIGH);
        delay(150);
        digitalWrite(ledPin, LOW);
        delay(150);
      }
    }

    // ⚠️ Ignore small variations
    else {
      Serial.println("Ignored");
    }
  }

  lastState = state;
}