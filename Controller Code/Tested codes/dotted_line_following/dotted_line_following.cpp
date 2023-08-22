/// File:          test_robot.cpp
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
#define MAX_SPEED 4.25

// All the webots classes are defined in the "webots" namespace
using namespace std;
using namespace webots;

// This is the main program of your controller.
// It creates an instance of your Robot instance, launches its
// function(s) and destroys it at the end of the execution.
// Note that only one instance of Robot should be created in
// a controller program.
// The arguments of the main function can be specified by the
// "controllerArgs" field of the Robot node


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
  
  left_motor->setVelocity(0.0);
  right_motor->setVelocity(0.0);
  
  
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
  
  
  int LFSensor[5] = {0,0,0,0,0};
  
  

  double left_speed = MAX_SPEED;
  double right_speed = MAX_SPEED;
  
  float Kp = 1.0;
  float Ki = 0.3;
  float Kd = 0.03;
    
  float previousError = 0.0;
  float error = 0.0;
  float PIDvalue = 0.0;
  
  float P = 0;
  float I = 0;
  float D = 0;

  // Main loop:
  // - perform simulation steps until Webots is stopping the controller
  while (robot->step(TIME_STEP) != -1) {
    // Read the sensors:
    // Enter here functions to read sensor data, like:
    //  double val = ds->getValue();
    LFSensor[0] = left_ir_2 -> getValue();
    LFSensor[1] = left_ir_1 -> getValue();
    LFSensor[2] = middle_ir -> getValue();
    LFSensor[3] = right_ir_1 -> getValue();
    LFSensor[4] = right_ir_2 -> getValue();
    
    //red = 169  /86    blue = 363 / 184    white =  86
    
    for (int i = 0; i < 5; ++i) {
    std::cout << LFSensor[i] << "  " <<std::endl;
  }
    std::cout << "next" <<std::endl;
    
  
    for (int i = 0; i < 5; i++){
    if ( LFSensor[i]<=500) LFSensor[i] = 1;
    else LFSensor[i] = 0;
   
    // Process sensor data here.
    left_speed = MAX_SPEED;
    right_speed = MAX_SPEED;
    

    if ((LFSensor[0]==0)&&(LFSensor[1]==0)&&(LFSensor[2]==0)&&(LFSensor[3]==0)&&(LFSensor[0]==0)) {
    error = 0;
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
  }; 

  // Enter here exit cleanup code.

  delete robot;
  return 0;
}
