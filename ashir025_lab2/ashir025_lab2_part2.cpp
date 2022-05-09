/* Name, Email: Amir Shirazi, ashir025@ucr.edu.com
* Discussion Section: 21
* Assignment: Lab #2  Exercise #2 
*
*
* I acknowledge all content contained herein, excluding 
*   template or example code, is my own original work.
*
* Demo Link: https://www.youtube.com/shorts/2tvkshjJhhk
*/
const int b_size = 4;
const int b[b_size] = {2, 3, 4, 5};

int b_buf = 0x00;

int s_buf = 0x00;

const int s_size = 8;
int s[s_size];

int X = 8;
int Y = 9;


int ledXState = LOW;
int lastButtonXState;
int currentButtonXState;

int ledYState = LOW;
int lastButtonYState;
int currentButtonYState;

int checker = 0;


void checkButtonStates() {

  lastButtonXState    = currentButtonXState;      // save the last state
  currentButtonXState = digitalRead(X); // read new state

  if (lastButtonXState == HIGH && currentButtonXState == LOW) {

    if (checker < 4) {
      checker++;
    } else {
      checker = 0;
    }

  }

  lastButtonYState    = currentButtonYState;      // save the last state
  currentButtonYState = digitalRead(Y); // read new state

  if (lastButtonYState == HIGH && currentButtonYState == LOW) {

    if (checker > 0) {
      checker--;
    } else {
      checker = 4;
    }

  }

}

void changeLEDs() {

  if (checker == 0) {
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
  }

  if (checker == 1) {
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
  }
  if (checker == 2) {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
  }
  if (checker == 3) {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
  }
  if (checker == 4) {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
  }


}

void readData() {
  if (Serial.available()) {
    s_buf = Serial.parseInt();

    for (int i = (s_size - 1); i >= 0; i--) {
      s[i] = (s_buf >> i) & 0x01;
    }
  }
}

void resetBuffer() {
  for (int i = 0; i < b_size; i++) {
    digitalWrite(b[i], LOW);
  }
}

void writeBuffer(unsigned char b_temp, int size = b_size) {
  for (int i = (size - 1); i >= 0; i--) {
    if ((b_temp >> i) & 0x01) {
      digitalWrite(b[i], HIGH);
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < b_size; i++) {
    pinMode(b[i], OUTPUT);
  }

  Serial.begin(9600);

  pinMode(X, INPUT);
  pinMode(Y, INPUT_PULLUP);

}

void loop() {
  // put your main code here, to run repeatedly:
  resetBuffer();
  readData();
  //b_buf = analogRead(A1);
  //b_buf = map(b_buf, 0, 1023, 0, 16);
  b_buf = map(s_buf, 0, 256, 0, 15);
  checkButtonStates();
  changeLEDs();
  writeBuffer(b_buf);


}
