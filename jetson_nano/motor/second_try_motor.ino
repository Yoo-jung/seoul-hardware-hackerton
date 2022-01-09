//second try
//sudo chmod a+rw /dev/ttyACM0

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <ros.h>
#include <geometry_msgs/Twist.h>

const int right_pwm_pin = 5;
const int right_dir_pin = A0;
const int left_pwm_pin = 6;
const int left_dir_pin = A1;
const bool left_fwd = true;
const bool right_fwd = false;

// Default_speed
const int default_vel = 130;

ros::NodeHandle  nh;

void MoveFwd(const size_t speed) { //전진 10
  digitalWrite(right_dir_pin, right_fwd);
  digitalWrite(left_dir_pin, left_fwd);
  analogWrite(right_pwm_pin, speed);
  analogWrite(left_pwm_pin, speed);
}

void MoveBack(const size_t speed) { //후진 01
  digitalWrite(right_dir_pin, left_fwd);
  digitalWrite(left_dir_pin, right_fwd);
  analogWrite(right_pwm_pin, speed);
  analogWrite(left_pwm_pin, speed);
}

void MoveRight(const size_t speed) { //오른쪽 11
  digitalWrite(right_dir_pin, left_fwd);
  digitalWrite(left_dir_pin, left_fwd);
  analogWrite(right_pwm_pin, speed);
  analogWrite(left_pwm_pin, speed);
}

void MoveLeft(const size_t speed) { //왼쪽 00
  digitalWrite(right_dir_pin, right_fwd);
  digitalWrite(left_dir_pin, right_fwd);
  analogWrite(right_pwm_pin, speed);
  analogWrite(left_pwm_pin, speed);
}

void MoveStop() {
  digitalWrite(right_dir_pin, right_fwd);
  digitalWrite(left_dir_pin, left_fwd);
  analogWrite(right_pwm_pin, 0);
  analogWrite(left_pwm_pin, 0);
}

void cmd_vel_cb(const geometry_msgs::Twist & msg) {
 
  const float x = msg.linear.x;
  const float y = msg.linear.y;
  const float z = msg.linear.z;
  const float x_rotation = msg.angular.x;
  const float y_rotation = msg.angular.y;
  const float z_rotation = msg.angular.z;
 
  if (x > 0 && z_rotation == 0) { //직진
    MoveFwd(default_vel);
  }

  else if(x < 0 && z_rotation == 0){ //후진
    MoveBack(default_vel);
  }

  else if(x > 0 && z_rotation > 0){ //왼쪽
    MoveLeft(200);
  }

  else if(x > 0 && z_rotation < 0){ //오른쪽
    MoveRight(200);
  }

  else { //정지
    MoveStop();
  }
}
ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", cmd_vel_cb);
void setup() {
  pinMode(right_pwm_pin, OUTPUT);  
  pinMode(right_dir_pin, OUTPUT);
  pinMode(left_pwm_pin, OUTPUT);
  pinMode(left_dir_pin, OUTPUT);
 
  digitalWrite(right_dir_pin, right_fwd);
  digitalWrite(left_dir_pin, left_fwd);
  nh.initNode();
  nh.subscribe(sub);
}

void loop() {
  nh.spinOnce();
  delay(1);
}
