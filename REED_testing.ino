int reedState = 0; //starts open

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT); // define the blinky
  pinMode(A0, INPUT); // define the reed switch
}

void loop() {
  // put your main code here, to run repeatedly:

  // read the reed switch (heh)
  reedState = digitalRead(A0);

  // logic that reading
  if (reedState == HIGH) { // for some reason HIGH is ON...?
    digitalWrite(13, HIGH); // turn on the LED
  } else {
    digitalWrite(13, LOW); // turn off the LED
  }
}
