
// ************************************************
//
// carrero_arduino.ino
// Author: Fernando Leira Cortel
//
// ************************************************

// Ultrasonics
#define TRIG_R 4
#define ECHO_R 30
#define TRIG_C 3
#define ECHO_C 31
#define TRIG_L 2
#define ECHO_L 32

// Front Motors
#define ENF_L 9 // Red
#define ENF_R 10 // Purple
#define IN1_F 23 // Yellow
#define IN2_F 25 // Orange
#define IN3_F 27 // Green
#define IN4_F 29 // Blue

// Rear Motors
#define ENR_L 5 // Brown
#define ENR_R 6 // Blue
#define IN1_R 22 // Red
#define IN2_R 24 // Orange
#define IN3_R 26 // Green
#define IN4_R 28 // Yellow

// Motors Directions
uint8_t in1_f = 1;
uint8_t in2_f = 0;
uint8_t in3_f = 1;
uint8_t in4_f = 0;
uint8_t in1_r = 1;
uint8_t in2_r = 0;
uint8_t in3_r = 1;
uint8_t in4_r = 0;

// Motors Speeds
uint8_t front_left_speed = 255;
uint8_t front_right_speed = 255;
uint8_t rear_left_speed = 255;
uint8_t rear_right_speed = 255;

// Vehicle Transmissions Positions
enum vehicle_position { P, R, D, DD };
vehicle_position current_state = P;
vehicle_position last_state = P;

// Duration and distance for Ultrasonics
long duration;
int distance;
char Incoming_value = 0;

void setup()
{
  delay(1);
  
  // Start Serial
  Serial.begin(9600);

  // Built-in LED to Output 
  pinMode(LED_BUILTIN, OUTPUT);

  // Set Mode of Motor Pins to Output
  pinMode(ENF_L, OUTPUT);
  pinMode(ENF_R, OUTPUT);
  pinMode(IN1_F, OUTPUT);
  pinMode(IN2_F, OUTPUT);
  pinMode(IN3_F, OUTPUT);
  pinMode(IN4_F, OUTPUT);
  pinMode(ENR_L, OUTPUT);
  pinMode(ENR_R, OUTPUT);
  pinMode(IN1_R, OUTPUT);
  pinMode(IN2_R, OUTPUT);
  pinMode(IN3_R, OUTPUT);
  pinMode(IN4_R, OUTPUT);

  // Set Ultrasonic Sensors
  pinMode(TRIG_R, OUTPUT);
  pinMode(ECHO_R, INPUT);
  pinMode(TRIG_C, OUTPUT);
  pinMode(ECHO_C, INPUT);
  pinMode(TRIG_L, OUTPUT);
  pinMode(ECHO_L, INPUT);

  setMotorsPositions();
}

void loop()
{
  // Check if there is a change in position
  if (last_state != current_state)
  {
    setMotorsPositions();
    last_state = current_state;

    front_left_speed = 0;
    front_right_speed = 0;
    rear_left_speed = 0;
    rear_right_speed = 0;
    setMotorsSpeeds();
  }

  // Check for input through Bluetooth
  if (Serial.available() > 0)
  {
    Incoming_value = Serial.read();      //Read the incoming data and store it into variable Incoming_value
    handleInput();
  }

  setMotorsSpeeds();
  
  //pulseDistance(TRIG_R, ECHO_R);
  //delay(100);
}

void handleInput()
{
  if(Incoming_value == '1')            //Checks whether value of Incoming_value is equal to 1 
    digitalWrite(LED_BUILTIN, HIGH);  //If value is 1 then LED turns ON
  else if(Incoming_value == '0')       //Checks whether value of Incoming_value is equal to 0
    digitalWrite(LED_BUILTIN, LOW);   //If value is 0 then LED turns OFF
  else if (Incoming_value == 'P')
    current_state = P;
  else if (Incoming_value == 'R')
    current_state = R;
  else if (Incoming_value == 'D')
    current_state = D;
  else if (Incoming_value == 'X')
    current_state = DD;
  else if (Incoming_value == 'f'){
    front_left_speed = 255;
    front_right_speed = 255;
    rear_left_speed = 255;
    rear_right_speed = 255;
  }
  else if (Incoming_value == 'd'){
    front_left_speed = 0;
    front_right_speed = 0;
    rear_left_speed = 0;
    rear_right_speed = 0;
  }
  else if (Incoming_value == 'l'){
    front_left_speed = 255;
    front_right_speed = 0;
    rear_left_speed = 0;
    rear_right_speed = 255;
  }
  else if (Incoming_value == 'r'){
    front_left_speed = 0;
    front_right_speed = 255;
    rear_left_speed = 255;
    rear_right_speed = 0;
  }
}

void setMotorsPositions()
{
  // Set new Position State for Front Motors
  switch (current_state)
  {
    case P:
      in1_f = 0;
      in2_f = 0;
      in3_f = 0;
      in4_f = 0;
      in1_r = 0;
      in2_r = 0;
      in3_r = 0;
      in4_r = 0;
      break;
    case R:
      in1_f = 1;
      in2_f = 0;
      in3_f = 1;
      in4_f = 0;
      in1_r = 1;
      in2_r = 0;
      in3_r = 1;
      in4_r = 0;
      break;
    case D:
      in1_f = 0;
      in2_f = 1;
      in3_f = 0;
      in4_f = 1;
      in1_r = 0;
      in2_r = 1;
      in3_r = 0;
      in4_r = 1;
      break;
    case DD:
      in1_f = 0;
      in2_f = 1;
      in3_f = 0;
      in4_f = 1;
      in1_f = 0;
      in2_f = 1;
      in3_f = 0;
      in4_f = 1;
      break;
  }

  // Apply changes
  digitalWrite(IN1_F, in1_f);
  digitalWrite(IN2_F, in2_f);
  digitalWrite(IN3_F, in3_f);
  digitalWrite(IN4_F, in4_f);
  digitalWrite(IN1_R, in1_r);
  digitalWrite(IN2_R, in2_r);
  digitalWrite(IN3_R, in3_r);
  digitalWrite(IN4_R, in4_r);
}

void setMotorsSpeeds()
{
  analogWrite(ENF_L, front_left_speed);
  analogWrite(ENF_R, front_right_speed); 
  analogWrite(ENR_L, rear_left_speed);
  analogWrite(ENR_R, rear_right_speed); 
}

void pulseDistance(int trig, int echo)
{
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  distance = duration * 0.034 / 2; // Speed of sound divided by 2
  Serial.print("Distance: ");
  Serial.println(distance);
}
