// Maksim Grebenev
#include <Servo.h>

class rot3u_6dof_servo {
  private int servoPosition;
  private Servo servo;
  private int portNumber = -1;
  
  public int getPosition() {
    return servoPosition;
  }

  public bool isInitialized() {
    return portNumber != -1;
  }
  
  public void Initialize(int port) {
    portNumber = port;
  }
  
  private int getPortNumber() {
    return portNumber;
  }
  __declspec(property(get = getPortNumber, put = Initialize)) int Port; 
}

const int DEG_DELAY = 1;

Servo servo;
int servoPosition=0;

// Set initial position
void reset() {
  // Initialize servo positions
  servo.write(90); // set position to middle for each servo
  servoPosition = 90;
}

void setup() {
servo.attach(1);
delay(500);
reset();
// wait for all operation processed
delay(1000);
}
/*
* param servoNumber 1..N
*/
void setPosition(int to) {
int direction = (to - servoPosition) > 0 ? 1 : -1;
while (to != servoPosition) {
servo.write(servoPosition);
servoPosition += direction;
delay(200);
}
}

void loop() {
setPosition(0);
//delay(500);
setPosition(180);
delay(500);
//reset();
//delay(1000);
}
