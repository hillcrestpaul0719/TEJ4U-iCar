// minimum unit needed on a pin to get the motor to run
int motorPower = 160;
// the maximum value of customizable power you can 
int motorMax = 255 - motorPower;

// clips the customizable motor power within a range, and notifies the user if it is out of the allowed range
void clipPower(int& power){
  if(power < 0 || power > motorMax){
    Serial.println("Invalid Power");
  }
  power = max(0, min(motorMax, power));
}

// runs a motor
struct Motor {
  // control (power) pin, out1 point and out2 pin (for directions)
  int ctl, out1, out2;

  // sets up the relevant pins
  void setup(){
    pinMode(ctl, OUTPUT);
    pinMode(out1, OUTPUT);
    pinMode(out2, OUTPUT);
  }

  // moves the motor forward with [power] power
  void forward(int power){
    // notify user if the power is too large and clip it
    clipPower(power);

    // write power needed to control
    analogWrite(ctl, motorPower + power);

    // set the direction
    digitalWrite(out1, HIGH);
    digitalWrite(out2, LOW);
  }

  // move the motor backwrad with [power] power
  void backward(int power){
    // notify user if power is out of range and clip it
    clipPower(power);

    // write power needed to ctl pin
    analogWrite(ctl, motorPower + power);

    // set the direction
    digitalWrite(out1, LOW);
    digitalWrite(out2, HIGH);    
  }

  // stops the motor
  void stop(){
    digitalWrite(ctl, LOW);
  }
};

// A drivetrain configuring both motors
struct Drivetrain{
  // left and right motors
  Motor left, right;

  // class constructor
  Drivetrain(Motor left, Motor right){
    this->left = left;
    this->right = right;
  }

  void setup(){
    left.setup();
    right.setup();
  }

  // stops the drivetrain
  void stop(){
    left.stop();
    right.stop();
  }

  // moves the chassis forward
  void forward(int power){
    clipPower(power);
    left.forward(power);
    right.forward(power);
  }

  // moves the chassis backward
  void backward(int power){
    clipPower(power);
    left.backward(power);
    right.backward(power);
  }

  // rotates the chassis clockwise
  void clockwise(int power){
    clipPower(power);
    left.forward(power);
    right.backward(power);
  }

  // rotates the chassis counterclockwise
  void counterClockwise(int power){
    clipPower(power);
    left.backward(power);
    right.forward(power);
  }
};

Motor left = {3, 2, 4};
Motor right = {9, 7, 8};
// the chassis' drivetrain
Drivetrain drivetrain(left, right);

void setup(){
  drivetrain.setup();
  Serial.begin(9600);
}

void loop() {
  drivetrain.forward(50);
  delay(1000);
  drivetrain.backward(50);
  delay(1000);
  drivetrain.clockwise(50);
  delay(1000);
  drivetrain.counterClockwise(50);
  delay(1000);
}