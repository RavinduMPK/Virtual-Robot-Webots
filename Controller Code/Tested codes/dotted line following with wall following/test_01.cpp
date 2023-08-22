// File:          test_robot.cpp
// Date:
// Description:
// Author:
// Modifications:

// You may need to add webots include files such as
// <webots/DistanceSensor.hpp>, <webots/Motor.hpp>, etc.
// and/or to add some other includes
#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/DistanceSensor.hpp>
#include <iostream>

#define TIME_STEP 16
#define MAX_SPEED 6.25

// All the webots classes are defined in the "webots" namespace
using namespace std;
using namespace webots;

float error;
int stage = 0;
int state;

// This is the main program of your controller.
// It creates an instance of your Robot instance, launches its
// function(s) and destroys it at the end of the execution.
// Note that only one instance of Robot should be created in
// a controller program.
// The arguments of the main function can be specified by the
// "controllerArgs" field of the Robot node




// Function for segmented wall following
void wall_following(Robot* robot, Motor* left_motor, Motor* right_motor, DistanceSensor* left_front, DistanceSensor *left_back, DistanceSensor *right_front, DistanceSensor *right_back, DistanceSensor *front_left, DistanceSensor *front_right, DistanceSensor *left_ir_2, DistanceSensor *left_ir_1, DistanceSensor *middle_ir, DistanceSensor *right_ir_1, DistanceSensor *right_ir_2, int state){
std::cout << " wall following..... " <<std::endl;
while (robot->step(TIME_STEP) != -1) {
    // Read the sensors:
    // Enter here functions to read sensor data, like:
    //  double val = ds->getValue();
    
    left_motor->setVelocity(0.0);
    right_motor->setVelocity(0.0);
    
    double left_speed = MAX_SPEED;
    double right_speed = MAX_SPEED;
    
    float left_front_value = left_front -> getValue();
    float left_back_value = left_back -> getValue();
    float right_front_value = right_front -> getValue();
    float right_back_value = right_back -> getValue();
    float front_left_value = front_left -> getValue();
    float front_right_value = front_right -> getValue();
    
    
    //std::cout << left_front_value << "  " <<std::endl;
   //std::cout << left_back_value << "  " <<std::endl;
    //std::cout << right_front_value << "  " <<std::endl;
    //std::cout << right_back_value << "  " <<std::endl;
   // std::cout << front_left_value << "  " <<std::endl;
   // std::cout << front_right_value << "  " <<std::endl;
   
   int LFSensor[5] = {0,0,0,0,0};
   
  LFSensor[0] = left_ir_2 -> getValue();
  LFSensor[1] = left_ir_1 -> getValue();
  LFSensor[2] = middle_ir -> getValue();
  LFSensor[3] = right_ir_1 -> getValue();
  LFSensor[4] = right_ir_2 -> getValue();
  
  
  
  for (int i = 0; i < 5; i++){
    if ( LFSensor[i]<=500) LFSensor[i] = 1;
    else LFSensor[i] = 0;}
  
    //std::cout << "next" <<std::endl;
    
    float gap = 1000.0;
    float min_diff = 5;
   
    
    if (state == 2){
    
    
    if ((LFSensor[0]==1)&&(LFSensor[1]==1)&&(LFSensor[2]==1)&&(LFSensor[3]==1)&&(LFSensor[0]==1)) {
    
    return;
    }
    else if (front_left_value < gap){
    error = -0.1;
    }
    
    else if (front_right_value < gap){
    error = -0.1;
    }
    
    else if ((left_back_value > gap) && (left_front_value > gap) && (left_front_value - left_back_value) > min_diff){
    error = 0.1;
    }
    
    else if ((left_back_value > gap) && (left_front_value > gap) && (left_front_value - left_back_value) > 2*min_diff){
    error = 0.2;
    }
    
    else if ((left_back_value > gap) && (left_front_value > gap) && (left_front_value - left_back_value) > 3*min_diff){
    error = 0.3;
    }
    
    else if ((left_back_value > gap) && (left_front_value > gap) && (left_front_value - left_back_value) < -min_diff){
    error = -0.1;
    }
    
    else if ((left_back_value > gap) && (left_front_value > gap) && (left_front_value - left_back_value)  < -2*min_diff){
    error = -0.2;
    }
    
    else if ((left_back_value > gap) && (left_front_value > gap) && (left_front_value - left_back_value)  < -3*min_diff){
    error = -0.3;
    }
    
    else if ((left_back_value < gap) || (left_front_value < gap)){
    error = -1.0;
    }
    
    else if ((right_back_value < gap) || (right_front_value < gap)){
    error = 1.0;
    }
    
    
    
    else if ((left_front_value == left_back_value) && (right_front_value == right_back_value)){
    error = 0;
    } 
    
    int Kp = 1;
    
    left_speed = MAX_SPEED + Kp*error;
    right_speed = MAX_SPEED - Kp*error;
    
    left_motor->setVelocity(left_speed);
    right_motor->setVelocity(right_speed);
  }
  }
}







// Function for dotted line following

void dotted_line_following(Robot* robot, Motor* left_motor, Motor* right_motor, DistanceSensor *left_ir_2, DistanceSensor *left_ir_1, DistanceSensor *middle_ir, DistanceSensor *right_ir_1, DistanceSensor *right_ir_2,int state){
std::cout << " line following..... " <<std::endl;
  int LFSensor[5] = {0,0,0,0,0};
  
  float Kp = 1.0;
  float Ki = 0.3;
  float Kd = 0.03;
    
  float previousError = 0.0;
  float error = 0.0;
  float PIDvalue = 0.0;
  
  float P = 0;
  float I = 0;
  float D = 0;
  
while (robot->step(TIME_STEP) != -1) {
  left_motor->setVelocity(0.0);
  right_motor->setVelocity(0.0);
  
  double left_speed = MAX_SPEED;
  double right_speed = MAX_SPEED;
  
  LFSensor[0] = left_ir_2 -> getValue();
  LFSensor[1] = left_ir_1 -> getValue();
  LFSensor[2] = middle_ir -> getValue();
  LFSensor[3] = right_ir_1 -> getValue();
  LFSensor[4] = right_ir_2 -> getValue();
  
 /* for (int i = 0; i < 5; ++i) {
    std::cout << LFSensor[i] << "  " <<std::endl;
  }
    std::cout << "next" <<std::endl;*/
    
  
    for (int i = 0; i < 5; i++){
    if ( LFSensor[i]<=500) LFSensor[i] = 1;
    else LFSensor[i] = 0;
   
    // Process sensor data here.
    left_speed = MAX_SPEED;
    right_speed = MAX_SPEED;
    
    
if (state == 1){

    if ((LFSensor[0]==0)&&(LFSensor[1]==0)&&(LFSensor[2]==0)&&(LFSensor[3]==0)&&(LFSensor[0]==0)) {
    return;
    }
    else if ((LFSensor[0]==0)&&(LFSensor[1]==0)&&(LFSensor[2]==0)&&(LFSensor[3]==0)&&(LFSensor[0]==1)) {
    error = 0.4;
    }
    else if ((LFSensor[0]==0)&&(LFSensor[1]==0)&&(LFSensor[2]==0)&&(LFSensor[3]==1)&&(LFSensor[0]==1)) {
    error = 0.3;
    }
    else if ((LFSensor[0]==0)&&(LFSensor[1]==0)&&(LFSensor[2]==0)&&(LFSensor[3]==1)&&(LFSensor[0]==0)) {
    error = 0.2;
    }
    else if ((LFSensor[0]==0)&&(LFSensor[1]==0)&&(LFSensor[2]==1)&&(LFSensor[3]==1)&&(LFSensor[0]==0)) {
    error = 0.1;
    }
    else if ((LFSensor[0]==0)&&(LFSensor[1]==0)&&(LFSensor[2]==1)&&(LFSensor[3]==0)&&(LFSensor[0]==0)) {
    error = 0.0;
    }
    else if ((LFSensor[0]==0)&&(LFSensor[1]==1)&&(LFSensor[2]==1)&&(LFSensor[3]==0)&&(LFSensor[0]==0)) {
    error = -0.1;
    }
    else if ((LFSensor[0]==0)&&(LFSensor[1]==1)&&(LFSensor[2]==0)&&(LFSensor[3]==0)&&(LFSensor[0]==0)) {
    error = -0.2;
    }
    else if ((LFSensor[0]==1)&&(LFSensor[1]==1)&&(LFSensor[2]==0)&&(LFSensor[3]==0)&&(LFSensor[0]==0)) {
    error = -0.3;
    }
    else if ((LFSensor[0]==1)&&(LFSensor[1]==0)&&(LFSensor[2]==0)&&(LFSensor[3]==0)&&(LFSensor[0]==0)) {
    error = -0.4;
    }
    }
    
 else if (state == 3){
 std::cout << " state 3 " <<std::endl;
 
 
 for (int i = 0; i < 5; ++i) {
    std::cout << LFSensor[i] << "  " <<std::endl;
  }
    std::cout << "next" <<std::endl;
 
 
    if ((LFSensor[0]==1)&&(LFSensor[1]==1)&&(LFSensor[2]==1)&&(LFSensor[3]==1)&&(LFSensor[0]==1)) {
    error = 0.0;
    }
    else if ((LFSensor[0]==0)&&(LFSensor[1]==0)&&(LFSensor[2]==0)&&(LFSensor[3]==0)&&(LFSensor[0]==1)) {
    error = 0.4;
    }
    else if ((LFSensor[0]==0)&&(LFSensor[1]==0)&&(LFSensor[2]==0)&&(LFSensor[3]==1)&&(LFSensor[0]==1)) {
    error = 0.3;
    }
    else if ((LFSensor[0]==0)&&(LFSensor[1]==0)&&(LFSensor[2]==0)&&(LFSensor[3]==1)&&(LFSensor[0]==0)) {
    error = 0.2;
    }
    else if ((LFSensor[0]==0)&&(LFSensor[1]==0)&&(LFSensor[2]==1)&&(LFSensor[3]==1)&&(LFSensor[0]==0)) {
    error = 0.1;
    }
    else if ((LFSensor[0]==0)&&(LFSensor[1]==0)&&(LFSensor[2]==1)&&(LFSensor[3]==0)&&(LFSensor[0]==0)) {
    error = 0.0;
    }
    else if ((LFSensor[0]==0)&&(LFSensor[1]==1)&&(LFSensor[2]==1)&&(LFSensor[3]==0)&&(LFSensor[0]==0)) {
    error = -0.1;
    }
    else if ((LFSensor[0]==0)&&(LFSensor[1]==1)&&(LFSensor[2]==0)&&(LFSensor[3]==0)&&(LFSensor[0]==0)) {
    error = -0.2;
    }
    else if ((LFSensor[0]==1)&&(LFSensor[1]==1)&&(LFSensor[2]==0)&&(LFSensor[3]==0)&&(LFSensor[0]==0)) {
    error = -0.3;
    }
    else if ((LFSensor[0]==1)&&(LFSensor[1]==0)&&(LFSensor[2]==0)&&(LFSensor[3]==0)&&(LFSensor[0]==0)) {
    error = -0.4;
    }
    }
   
    // Enter here functions to send actuator commands, like:
    //  motor->setPosition(10.0);
    P = error;
    I += error;
    D = error - previousError;
    
    PIDvalue = (Kp*P)+(Ki*I)+(Kd*D);
    previousError = error;
    
    
    left_speed = 4-(MAX_SPEED * PIDvalue);
    right_speed = 4+(MAX_SPEED * PIDvalue);
    
    
    left_motor->setVelocity(left_speed);
    right_motor->setVelocity(right_speed);
   
    
  }
}
}








int main(int argc, char **argv) {

  // create the Robot instance.
  Robot *robot = new Robot();

  
  // You should insert a getDevice-like function in order to get the
  // instance of a device of the robot. Something like:
  //  Motor *motor = robot->getMotor("motorname");
  //  DistanceSensor *ds = robot->getDistanceSensor("dsname");
  //  ds->enable(timeStep);
  Motor *left_motor = robot->getMotor("left_motor");
  Motor *right_motor = robot->getMotor("right_motor");
  
  left_motor->setPosition(INFINITY);
  right_motor->setPosition(INFINITY);
  
  DistanceSensor *left_front = robot->getDistanceSensor("sonar_left_front");
  DistanceSensor *left_back = robot->getDistanceSensor("sonar_left_back");
  DistanceSensor *right_front = robot->getDistanceSensor("sonar_right_front");
  DistanceSensor *right_back = robot->getDistanceSensor("sonar_right_back");
  DistanceSensor *front_left = robot->getDistanceSensor("sonar_front_left");
  DistanceSensor *front_right = robot->getDistanceSensor("sonar_front_right");
  
  left_front->enable(TIME_STEP);
  left_back->enable(TIME_STEP);
  right_front->enable(TIME_STEP);
  right_back->enable(TIME_STEP);
  front_left->enable(TIME_STEP);
  front_right->enable(TIME_STEP);
  
  
  DistanceSensor *left_ir_2 = robot->getDistanceSensor("ir_2");
  DistanceSensor *left_ir_1 = robot->getDistanceSensor("ir_1");
  DistanceSensor *middle_ir = robot->getDistanceSensor("ir_0");
  DistanceSensor *right_ir_1 = robot->getDistanceSensor("ir_-1");
  DistanceSensor *right_ir_2 = robot->getDistanceSensor("ir_-2");
  
  left_ir_2->enable(TIME_STEP);
  left_ir_1->enable(TIME_STEP);
  middle_ir->enable(TIME_STEP);
  right_ir_1->enable(TIME_STEP);
  right_ir_2->enable(TIME_STEP);
  
  int ID_Sensor[5] = {0,0,0,0,0};
  
  //wall_following(robot,left_motor,right_motor,left_front,left_back,right_front,right_back,front_left,front_right);
 // dotted_line_following(robot,left_motor,right_motor,left_ir_2, left_ir_1, middle_ir,right_ir_1,right_ir_2);

while (robot->step(TIME_STEP) != -1) {

    ID_Sensor[0] = left_ir_2 -> getValue();
    ID_Sensor[1] = left_ir_1 -> getValue();
    ID_Sensor[2] = middle_ir -> getValue();
    ID_Sensor[3] = right_ir_1 -> getValue();
    ID_Sensor[4] = right_ir_2 -> getValue();
    
    
    
    for (int i = 0; i < 5; i++){
    if ( ID_Sensor[i]<=500) ID_Sensor[i] = 1;
    else ID_Sensor[i] = 0;
    
    for (int i = 0; i < 5; ++i) {
    std::cout << ID_Sensor[i] << "  " <<std::endl;
  }
    std::cout << "next" <<std::endl;
    
    
    if ((ID_Sensor[0]==1)&&(ID_Sensor[1]==1)&&(ID_Sensor[2]==1)&&(ID_Sensor[3]==1)&&(ID_Sensor[0]==1)) {
     left_motor->setVelocity(1.0);
     right_motor->setVelocity(1.0);
    }
    else{
    stage += 1;
    std::cout << "stage" << stage <<std::endl;
    
    if (stage == 1){
    std::cout << " line following state 1 " <<std::endl;
    dotted_line_following(robot, left_motor, right_motor, left_ir_2, left_ir_1, middle_ir, right_ir_1, right_ir_2, 1);
    }
    
    else if (stage == 2){
    std::cout << " wall following state 2 " <<std::endl;
    wall_following(robot, left_motor, right_motor, left_front, left_back, right_front, right_back, front_left, front_right, left_ir_2, left_ir_1, middle_ir, right_ir_1, right_ir_2, 2);
    }
    
    else if (stage == 3){
    std::cout << " line following state 3 " <<std::endl;
    dotted_line_following(robot, left_motor, right_motor, left_ir_2, left_ir_1, middle_ir, right_ir_1, right_ir_2, 3);
    }
    }
    
}
}
  // Main loop:
  // - perform simulation steps until Webots is stopping the controller
  
   

  // Enter here exit cleanup code.

  delete robot;
  return 0;
 
}
