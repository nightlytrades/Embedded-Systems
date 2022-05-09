/* Your Amir Shirasi, ashir025@ucr.edu.com:
* Discussion Section: 21
* Assignment: Lab #1  Exercise #2 
*   	
		As Seen in the Demo Video the LED lights do not turn on in order from left to right. 
		I ran into some issues last minute and was not able to fix that.
*
* I acknowledge all content contained herein, excluding 
*   template or example code, is my own original work.
*
* Demo Link: https://youtube.com/shorts/9P17jjHW-mI?feature=share
*/

// Array of Output Pin variables, set to the pins being used
const int b_size = 4;
const int b[b_size] = {2, 3, 4, 5};

//button pin setup
const char BUTTON_X = 8;
const char BUTTON_Y = 9;
  
// Output Buffer
int b_buf = 0x00;
// Input Variables

// Serial Monitor Buffer
int s_buf = 0x00; 

/* 's' is an array of integers of size 8. Note that arrays start at 0
 We will use this to be able to see the individual bit values of the s_buf

 */
const int s_size = 8;
int s[s_size];

// We can also define our own helper functions. It is a good idea to use helper functions whenever they make sense.
// Normally we also define the return type (void for none)

// Read from the serial monitor into s_buf and the s[] array for individual bits
void readData()
{
    if(Serial.available())
        s_buf = Serial.parseInt();

    for(int i = (s_size - 1); i>=0; i--)  { 
        s[i] = (s_buf >> i) & 0x01; // What's going on here?
        // ">>" bit shifting 
        // "&" bit masking
    }

}
// Reset the Output Buffer. 
void resetBuffer() {
    for(int i = 0; i < b_size; i++) {
        // Note this is an arduino function call to the pins
        digitalWrite(b[i], LOW);
    }
}
// Writes to the buffer. Note this function ORs the current value with the new value
// Note that size is an optional argument with default size the same as the buffer
void writeBuffer(unsigned char b_temp, int size = b_size)
{
    for (int i = (size - 1); i >= 0; i--) {
        if ((b_temp >> i) & 0x01) {
        digitalWrite(b[i], HIGH);
        }
    }

}


void setup() {
    // OUTPUT is a defined macro in Arduino!
  	
  	pinMode(BUTTON_X,INPUT);//pushdown
  	pinMode(BUTTON_Y,INPUT_PULLUP);//pullup
  
    for(int i = 0; i < b_size; i++)
    {
        pinMode(b[i], OUTPUT);
    }
    // We will also read from the serial monitor
    Serial.begin(9600);
}


void loop() {
    readData();
 
    // Note that here we arbitrarily connect b_buf & s_buf together. The important point is that we preserve the current value of the output buffer into a variable b_buf before writing it to the pins. If we resetBuffer or readData() again, we still know the values the output is set to, because it is in b_buf. 
    //b_buf=s_buf;
  	//b_buf = map(b_buf,0,255,0,15);
  	//Read Button
  
  	
  	for(int i = 0; i < b_size; i++) {
  		if (digitalRead(BUTTON_X)==HIGH && digitalRead(BUTTON_Y)==HIGH)
   			{
      		digitalWrite(b[i],HIGH);
          	
      
   			}
  		else if(digitalRead(BUTTON_Y)==LOW )
    	{
 
      		digitalWrite(b[i], LOW);
          
          
    		}
    }
}
