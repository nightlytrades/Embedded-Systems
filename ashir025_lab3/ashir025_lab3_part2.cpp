/* Name, Email: Amir Shirazi, ashir025@ucr.edu.com
* Discussion Section: 21
* Assignment: Lab #3 Exercise #2 
*
*
* I acknowledge all content contained herein, excluding 
*   template or example code, is my own original work.
*
* Demo Link: https://www.youtube.com/shorts/lR_iogXsT70
*/
enum States {INIT, ACTIVE, RESET} SM1_state = INIT;
int X_button = 8;
int Y_button = 9;

const int b_size = 4;
const int b[b_size] = {2, 3, 4, 5};

int b_buf = 0x00;

int s_buf = 0x00;

const int s_size = 8;
int s[s_size];

int setLEDS = 0;
int myLightValue = 0;

int ledXState = LOW;
int lastButtonXState;
int currentButtonXState;

int ledYState = LOW;
int lastButtonYState;
int currentButtonYState;

void checkButtonStates() {

  lastButtonXState    = currentButtonXState;      // save the last state
  currentButtonXState = digitalRead(X_button); // read new state

  if (lastButtonXState == HIGH && currentButtonXState == LOW) {

    if (setLEDS < 4) {
      setLEDS++;
    } else {
      setLEDS = 4;
    }

  }

  lastButtonYState    = currentButtonYState;      // save the last state
  currentButtonYState = digitalRead(Y_button); // read new state

  if (lastButtonYState == HIGH && currentButtonYState == LOW) {

    if (setLEDS > 0) {
      setLEDS--;
    } else {
      setLEDS = 0;
    }
  }
}

void changeLEDs() {

  if (setLEDS == 0) {
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
  }

  if (setLEDS == 1) {
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
  }
  if (setLEDS == 2) {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
  }
  if (setLEDS == 3) {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
  }
  if (setLEDS == 4) {
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

  pinMode(X_button, INPUT);
  pinMode(Y_button, INPUT_PULLUP);

  // put your setup code here, to run once:
  for (int i = 0; i < b_size; i++) {
    pinMode(b[i], OUTPUT);
  }

  Serial.begin(9600);

}


int SM1_Tick(int state) {

  switch (state) {
    case INIT:
      state = ACTIVE;
      break;
    case ACTIVE:
      if (myLightValue <= 200) {
        state = RESET;
      }
      break;
    case RESET:
      if (myLightValue > 200) {
        state = ACTIVE;
      }
      break;
  }
  switch (state) {
    case INIT:
      break;
    case ACTIVE:
      Serial.println(myLightValue);
      myLightValue = analogRead(A1);
      checkButtonStates();
      changeLEDs();
      break;
    case RESET:
      Serial.println(myLightValue);
      myLightValue = analogRead(A1);
      setLEDS = 0;
      changeLEDs();
      break;
  }
  return state;
}

void loop() {

  SM1_state = (States)SM1_Tick(SM1_state);

}
