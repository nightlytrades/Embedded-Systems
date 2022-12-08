/* Name, Email: Amir Shirazi, ashir025@ucr.edu.com
* Discussion Section: 21
* Assignment: Lab #3  Exercise #1 
*
*
* I acknowledge all content contained herein, excluding 
*   template or example code, is my own original work.
*
* Demo Link: https://www.youtube.com/shorts/MDZMSmtUKmA
*/
enum States {INIT, Mode_0, Mode_1} SM1_state = INIT;
int X_button = 8;
int Y_button = 9;

const int b_size = 4;
const int b[b_size] = {2, 3, 4, 5};

int b_buf = 0x00;

int s_buf = 0x00;

const int s_size = 8;
int s[s_size];

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

  pinMode(X_button, INPUT);
  pinMode(Y_button, INPUT_PULLUP);

  // put your setup code here, to run once:
  for (int i = 0; i < b_size; i++) {
    pinMode(b[i], OUTPUT);
  }

  Serial.begin(9600);

}

int minLightLevel=0;
bool runOnce = true;

int SM1_Tick(int state) {

  switch (state) {
    case INIT:
      state = Mode_0;
      break;
    case Mode_0:
      if (digitalRead(Y_button) == LOW) {
        state = Mode_1;
      }
      break;
    case Mode_1:
      if (digitalRead(X_button) == HIGH) {
        state = Mode_0;
      }
      break;
  }
  switch (state) {
    case INIT:
      break;
    case Mode_0:
      resetBuffer();
      b_buf = analogRead(A1);
      b_buf = map(b_buf, 0, 1023, 0, 16);
      writeBuffer(b_buf);
      runOnce = true;
      break;
    case Mode_1:
      if (runOnce == true) {
        minLightLevel = analogRead(A1);
        runOnce = false;
      }
      b_buf = analogRead(A1);
      resetBuffer();
      b_buf = map(b_buf, minLightLevel, 1023, 0, 16);
      writeBuffer(b_buf);
      break;
  }
  return state;
}

void loop() {

  SM1_state = (States)SM1_Tick(SM1_state);

}
