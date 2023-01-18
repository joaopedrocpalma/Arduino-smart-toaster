/*
  Smart Toaster
  This system simulates how a smart toaster design would function
  By pressing the button the whole system initiates, an internal countdown starts, which you can control
  through app input, followed by a LED turning on to know when the system is on
  During this time, the temperature of the toaster is displayed on the LCD screen, when the system finishes
  toasting, the LED turns off and a buzzer makes a sound to let the user know when it's done.
*/

#include <LiquidCrystal.h>  // Need to import this library in order to use the LCD display

const int buttonPin = 0;    // Button pin number
const int ledPin = 1;       // Led pin number
const int buzzerPin = 13;   // Buzzer pin number

LiquidCrystal lcd(12, 11, 10, 9, 8, 7);   // Defining the pins that the display uses

const float BETA = 3950;    // Beta coefficient

unsigned long tForToast = 3000;       // Time for the toast to finish + time defined for the temperature check
//unsigned long tForTempCheck = 2000;   // Time between each temperature check

int buttonState;
int analogValue;

float celsius;
float currentTemp;

bool isActive = false;

void setup()
{
  lcd.begin(16, 2);           // Starting the lcd

  pinMode(buttonPin, INPUT);  // Setting up the pins
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  lcd.print("Ready to use");
}

void loop()
{  
  isActive = GetButtonState(); // Calling GetButtonState function to know if the system is currently active

  if(isActive == true){
    lcd.clear();
    CalculateTemperature();    // Calling the CalculateTemperature function to check for the temperature 
  }
  else                         // Display's the default message when the system isn't in use
  {
    //lcd.clear();                  // This code glitches a bit, due to the loop and the state of the button
    //lcd.print("Ready to use");
  }  
}

bool GetButtonState(){     // Gets current button states, gets called in the void loop function, returns a bool depending on the system state
  bool active;

  buttonState = digitalRead(buttonPin);   // Update this variable every loop, to check for user input
  
  if(buttonState == HIGH){                 // If the button is pressed
    lcd.clear();
    lcd.print("Heating up!");
    
    digitalWrite(ledPin, HIGH);           // Turns on display LED
    delay(tForToast);                     // Waits for a certain time until toast is done
    
    tone(buzzerPin, 200, 500);            // Play a sound to warn the user the toast is done
    
    noTone(buzzerPin);
    active = true;                        // Sets this variable to true, so we know the system is currenly in use
  }
  else{       
    digitalWrite(ledPin, LOW);            // Turns off LED
    active = false;
  }

  return active;
}

float CalculateTemperature(){  // Gets current temperature every x seconds, gets called in the void loop function
  analogValue = analogRead(A0);           // Update this variable every loop, to constantly check temperature
  float celsius = 1 / (log(1 / (1023. / analogValue - 1)) / BETA + 1.0 / 298.15) - 273.15;  // Calculates the temperature in celsius
  //Serial.print("Temperature: ");    // Testing the output of the temperature sensor
  //Serial.print(celsius);
  //Serial.println(" ℃");
  lcd.print("Temp.: ");  
  lcd.print(celsius);
  lcd.print(" C");
  //delay(tForTempCheck);

  return celsius;
}