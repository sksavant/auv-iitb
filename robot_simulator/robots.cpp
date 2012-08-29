//#define _USE_MATH_DEFINES //For M_PI
#include<iostream>
#include<cmath>
#include<cstdio>
using namespace std;

// Controlling the robot i.e, try to move and then use PID Controller to control the robot so that it will follow the X-axis.


// robot class:
// *need to initialise robot with a given position, orientation (Constructor)
// *Need to move robot as a function of steering and distance(assume some initial)
// *Need to update the belief of robot based on sensor measurement of the distance from x axis
//
class robot {
    public:
        float x,y,orientation; //x and y are measure of distance in some units; orientation is in radians

    public:
        void set(float, float, float);//sets the position and orientation to given values
        //void set_noise(measurementnoise); //Sets the measurement noise: Later!
        robot moverobot(float );
        float sense(); // Sense the position and update belief
        void print();

    private:

};

void robot::set(float nx, float ny, float nor)//sets the position and orientation to given values
{
    x=nx;
    y=ny;
    orientation=nor;
}

robot robot::moverobot(float distance) //float tol=0.0001, float max_steering=M_PI_4)
{
  float turn,center_x,center_y,radius;
  //Some checks to do : if distance is positive, steering angle is not high
    if(distance<0.0){
        distance=0.0;
    }
    robot newrobot=robot();
    //set steering noise here if there or distance noise
    newrobot.x=x+ distance*cos(orientation);
    newrobot.y=y+ distance*sin(orientation);
    newrobot.orientation= fmod(orientation+turn,float(2.0*M_PI));
    return newrobot;
}

float robot::sense()
{
    return y+;//To give errored measurement
    //If error in sensing/measurement of position: return x,.y.
}

void robot::print()
{
    printf("[x=%f y=%f orient=%f]\n",x,y,orientation);
}

void follow_the_line(robot* robot, float param1=0.1, float param2=0.0, float param3=0.000){ //PID Controller function
    float speed=1.0,measurement,prevmeasure,sum; //Equals distance assuming time interval of 1
    int iterations=100;
    sum=0; //To integrate over the error
    measurement=robot->y;
    prevmeasure=robot->y;
    for(int i=0; i<iterations; ++i){
        measurement=robot->sense();
        sum=sum+measurement;
        robot->orientation=-param1*measurement-param2*(measurement-prevmeasure)-param3*sum;
        *robot=robot->moverobot(speed);
        prevmeasure=measurement;
        robot->print();
    }
}

int main(){
    robot linefollower=robot();
    linefollower.set(0,1,0);
    follow_the_line(&linefollower);
    linefollower=linefollower.moverobot(10);
    linefollower.orientation=0.01;
    linefollower.moverobot(1);
    linefollower.print();
    return 0;
}
