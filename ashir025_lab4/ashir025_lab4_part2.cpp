/* Name, Email: Amir Shirazi, ashir025@ucr.edu.com
* Discussion Section: 21
* Assignment: Lab #4  Exercise #2 
*
*
* I acknowledge all content contained herein, excluding 
*   template or example code, is my own original work.
*
* Demo Link: https://youtube.com/shorts/N9iCsuwyJDk?feature=share
*/
enum States {INIT, L1A, L1B, L1C, L1D} SM1_state = INIT;

unsigned long lastRan;
unsigned long period;
bool triggerOnce = true;
int periodTracker = 0;

int lightThreshold = 200;
bool takeLonger = false;


const int b_size = 4;
const int b[b_size] = {2, 3, 4, 5};

int b_buf = 0x00;

int s_buf = 0x00;

const int s_size = 8;
int s[s_size];

int myLightValue = 0;

int ledPattern1[] = {0b1000, 0b0100, 0b0010, 0b0001};

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

int buttonState;
int lastButtonState = LOW;

void monitorLDRToggle() {

  myLightValue = analogRead(A1);

  int reading;

  if (myLightValue <= lightThreshold) {
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
        takeLonger = !takeLonger;
        Serial.println(takeLonger);
      }
    }
  }

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

  // put your setup code here, to run once:
  for (int i = 0; i < b_size; i++) {
    pinMode(b[i], OUTPUT);
  }

  Serial.begin(9600);
  period = 1000;
  lastRan = 0;

}


int SM1_Tick(int state) {

  //controls switching of the states
  switch (state) {
    case INIT:
      state = L1A;
      break;
    case L1A:
      if (takeLonger == 1) {
        if (periodTracker == 1) {
          periodTracker = 0;
          state = L1B;
        } else {
          periodTracker++;
        }
      } else {
        state = L1B;
      }
      break;
    case L1B:
      if (takeLonger == 1) {
        if (periodTracker == 1) {
          periodTracker = 0;
          state = L1C;
        } else {
          periodTracker++;
        }
      } else {
        state = L1C;
      }
      break;
    case L1C:
      if (takeLonger == 1) {
        if (periodTracker == 1) {
          periodTracker = 0;
          state = L1D;
        } else {
          periodTracker++;
        }
      } else {
        state = L1D;
      }
      break;
    case L1D:
      if (takeLonger == 1) {
        if (periodTracker == 1) {
          periodTracker = 0;
          state = L1A;
        }else{
          periodTracker++;
        }
      } else {
        state = L1A;
      }
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
