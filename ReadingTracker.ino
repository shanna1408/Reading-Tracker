#include <Stepper.h>

const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution
const int RevolutionsPerMinute = 15;         // Adjustable range of 28BYJ-48 stepper is 0~17 rpm
float totalPercent = 0;

char input;
String message = "";
float readingGoal = 7*20; // 140 minutes per week
float minutesRead = 0;


// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

void setup() {
  myStepper.setSpeed(RevolutionsPerMinute);
  // initialize the serial port:
  Serial.begin(9600);
  // Set the light
  pinMode(5, OUTPUT);
  Serial.println("Starting");
}

void controlCurtain(String direction, float percentage){
  int intDir = 0;
  if (direction == "up"){
    intDir = -1;
  } else {
    intDir = 1; 
  }
  myStepper.step(4.35*percentage*stepsPerRevolution*intDir);
}

void printInfo(float percentIncrease) {
  Serial.print("Minutes Read: ");
  Serial.print(minutesRead);
  Serial.print("/");
  Serial.println(readingGoal);
  Serial.print("Percent increase: ");
  Serial.println(percentIncrease);
  Serial.print("Total percent: ");
  Serial.println(totalPercent);
}

void loop() { 
  // Get time input 
  delay(2000);
  Serial.println("Enter minutes read");
  while(Serial.available()){
      input = Serial.read();
      message += input;
  }

  if (message!=""){
    Serial.println(message);

    // Convert input to int
    float time = message.toFloat();
    message.trim();
    
    float percentIncrease;
    if (totalPercent < 1 && time > 0) {
      if (totalPercent == 0) {
        digitalWrite(5, HIGH);
      }

      if ((time + minutesRead)<=readingGoal) {
        minutesRead += time;
        percentIncrease = time/readingGoal;
        totalPercent += percentIncrease;
        printInfo(percentIncrease);
      } else {
        minutesRead = readingGoal;
        percentIncrease = 1 - totalPercent;
        totalPercent = 1;
        printInfo(percentIncrease);
      }
      controlCurtain("up", percentIncrease);
    }

    if (message.equals("reset")) {
      controlCurtain("down", totalPercent);
      digitalWrite(5, LOW);
      totalPercent = 0;
      minutesRead = 0;
    }
    message = "";
  }
}
