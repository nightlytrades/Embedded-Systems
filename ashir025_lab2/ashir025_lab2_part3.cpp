/* Name, Email: Amir Shirazi, ashir025@ucr.edu.com
* Discussion Section: 21
* Assignment: Lab #2  Exercise #3 
*
*
* I acknowledge all content contained herein, excluding 
*   template or example code, is my own original work.
*
* Demo Link: https://www.youtube.com/shorts/QM48KltKKBU
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

bool checker = false;

int maxLightLevel = 0;
bool runOnce = true;


void checkButtonStates() {

  lastButtonXState    = currentButtonXState;      // save the last state
  currentButtonXState = digitalRead(X); // read new state

  if (lastButtonXState == HIGH && currentButtonXState == LOW) {

    checker = false;

  }

  lastButtonYState    = currentButtonYState;      // save the last state
  currentButtonYState = digitalRead(Y); // read new state

  if (lastButtonYState == HIGH && currentButtonYState == LOW) {

    checker = true;

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

  checkButtonStates();

  //mode X
  if (checker == false) {
    b_buf = analogRead(A1);
    b_buf = map(b_buf, 200, 300, 0, 16);
    writeBuffer(b_buf);
    runOnce = true;
    //mode Y
  } else {
    if (runOnce == true) {
      maxLightLevel = analogRead(A1);
      runOnce = false;
    }
    b_buf = analogRead(A1);
    b_buf = map(b_buf, maxLightLevel, 1023, 0, 16);
    writeBuffer(b_buf);
  }

}
