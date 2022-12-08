/* Name, Email: Amir Shirazi, ashir025@ucr.edu.com
* Discussion Section: 21
* Assignment: Lab #4  Exercise #1 
*
*
* I acknowledge all content contained herein, excluding 
*   template or example code, is my own original work.
*
* Demo Link: https://youtube.com/shorts/-d5BwT20SI8?feature=share
*/
enum States {INIT, L1A, L1B, L1C, L1D, L2A, L2B, L2C, L2D} SM1_state = INIT;
unsigned long lastRan;
unsigned long period;
bool triggerOnce = true;
int X_button = 8;
//int Y_button = 9;

const int b_size = 4;
const int b[b_size] = {2, 3, 4, 5};

int b_buf = 0x00;

int s_buf = 0x00;

const int s_size = 8;
int s[s_size];

int setLEDS = 0;
int myLightValue = 0;

int ledPattern1[] = {0b0001, 0b0011, 0b0111, 0b1111};

int ledPattern2[] = {0b1111, 0b0000, 0b1100, 0b0011};

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

bool stateSwitch = true;
int buttonState;
int lastButtonState = LOW;

void monitorLDRToggle() {
  myLightValue = analogRead(A1);

  int reading;

  if (myLightValue <= 200) {
    reading = HIGH;
  } else {
    reading = LOW;
  }

  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) {
        stateSwitch = !stateSwitch;
      }
    }
  }

  if (stateSwitch) {
    if (triggerOnce == true) {
      SM1_state = INIT;
      triggerOnce = false;
    }
  } else {
    if (triggerOnce == false) {
      SM1_state = L2A;
      triggerOnce = true;
    }
  }
  Serial.println(stateSwitch);

  lastButtonState = reading;
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
  // pinMode(Y_button, INPUT_PULLUP);

  // put your setup code here, to run once:
  for (int i = 0; i < b_size; i++) {
    pinMode(b[i], OUTPUT);
  }

  Serial.begin(9600);
  period = 500;
  lastRan = 0;

}


int SM1_Tick(int state) {

  //controls switching of the states
  switch (state) {
    case INIT:
      state = L1A;
      break;
    case L1A:
      state = L1B;
      break;
    case L1B:
      state = L1C;
      break;
    case L1C:
      state = L1D;
      break;
    case L1D:
      state = L1A;
      break;
    case L2A:
      state = L2B;
      break;
    case L2B:
      state = L2C;
      break;
    case L2C:
      state = L2D;
      break;
    case L2D:
      state = L2A;
      break;
  }
  //controls the outputs of each state
  switch (state) {
    case INIT:
      break;
    case L1A:

      resetBuffer();
      writeBuffer(ledPattern1[0]);
      break;
    case L1B:
      resetBuffer();
      writeBuffer(ledPattern1[1]);
      break;
    case L1C:
      resetBuffer();
      writeBuffer(ledPattern1[2]);
      break;
    case L1D:
      resetBuffer();
      writeBuffer(ledPattern1[3]);
      break;
    case L2A:
      resetBuffer();
      writeBuffer(ledPattern2[0]);
      break;
    case L2B:
      resetBuffer();
      writeBuffer(ledPattern2[1]);
      break;
    case L2C:
      resetBuffer();
      writeBuffer(ledPattern2[2]);
      break;
    case L2D:
      resetBuffer();
      writeBuffer(ledPattern2[3]);
      break;
  }
  return state;
}


void loop() {

  if ((millis() - lastRan) > period) {
    SM1_state = (States)SM1_Tick(SM1_state);
    lastRan = millis();
  }

  monitorLDRToggle();

}
