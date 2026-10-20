  const int red   = 23;
  const int green = 22;
  const int blue  = 21;

  void setup() {
    pinMode(red,   OUTPUT);
    pinMode(green, OUTPUT);
    pinMode(blue,  OUTPUT);
  }

  void loop() {
    // Red
    digitalWrite(red,   HIGH);
    digitalWrite(green, LOW);
    digitalWrite(blue,  LOW);
    delay(1000);

    // Green
    digitalWrite(red,   LOW);
    digitalWrite(green, HIGH);
    digitalWrite(blue,  LOW);
    delay(1000);

    // Blue
    digitalWrite(red,   LOW);
    digitalWrite(green, LOW);
    digitalWrite(blue,  HIGH);
    delay(1000);

    // White (all on)
    digitalWrite(red,   HIGH);
    digitalWrite(green, HIGH);
    digitalWrite(blue,  HIGH);
    delay(1000);
  }