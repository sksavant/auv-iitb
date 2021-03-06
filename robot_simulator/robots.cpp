//#define _USE_MATH_DEFINES //For M_PI
#include<iostream>
#include<cmath>
#include<cstdio>
#include<iomanip>
#include<complex>
using namespace std;
#include"normal.cpp"

// Controlling the robot i.e, try to move and then use PID Controller to control the robot so that it will follow the X-axis.


// robot class:
// *need to initialise robot with a given position, orientation (Constructor)
// *Need to move robot as a function of steering and distance(assume some initial)
// *Need to update the belief of robot based on sensor measurement of the distance from x axis
//
class robot {
    public:
        float x,y,orientation,measurementnoise; //x and y are measure of distance in some units; orientation is in radians

    public:
        void set(float, float, float);//sets the position and orientation to given values
        void set_noise(float); //Sets the measurement noise: Later!
        robot moverobot(float );
        float sense(int); // Sense the position and update belief
        void print();
        void printtofile(FILE*,int);

    private:

};

void robot::set(float nx, float ny, float nor)//sets the position and orientation to given values
{
    x=nx;
    y=ny;
    orientation=nor;
}

void robot::set_noise(float noise)
{
    measurementnoise=noise;
}

robot robot::moverobot(float distance) //float tol=0.0001, float max_steering=M_PI_4)
{
  float center_x,center_y,radius;
  //Some checks to do : if distance is positive, steering angle is not high
    if(distance<0.0){
        distance=0.0;
    }
    robot newrobot=robot();
    //set steering noise here if there or distance noise
    newrobot.x=x+ distance*cos(orientation);
    newrobot.y=y+ distance*sin(orientation);
    newrobot.orientation= fmod(orientation,float(2.0*M_PI));
    newrobot.measurementnoise=measurementnoise;
    return newrobot;
}

float robot::sense(int seed)
{
    float x2= r4_normal(y,measurementnoise,seed);
    return x2;
    //To give errored measurement
    //If error in sensing/measurement of position: return x,.y.
}

void robot::print()
{
    printf("[x=%f y=%f orient=%f]\n",x,y,orientation);
}

void robot::printtofile(FILE* f,int i)
{
    fprintf(f,"%d %f\n",i,y);
}

void follow_the_line(robot* robot, float param1=0.3, float param2=0.1, float param3=0.001){ //PID Controller function
    int seed=time(NULL);
    float speed=1.0,measurement,prevmeasure,sum,x2; //Equals distance assuming time interval of 1
    int iterations=1000;
    sum=0; //To integrate over the error
    measurement=robot->y;
    prevmeasure=robot->y;
    FILE* fin=fopen("robotpath.txt","w");
    for(int i=0; i<iterations; ++i){
        measurement=robot->sense(seed++);
        sum=sum+measurement;
        x2=-param1*measurement-param2*(measurement-prevmeasure)-param3*sum;
        robot->orientation= x2< -M_PI/2?-M_PI/2:(x2>M_PI/2?M_PI/2:x2);
        *robot=robot->moverobot(speed);
        prevmeasure=measurement;
        //robot->print();
        robot->printtofile(fin,i);
    }
    fclose(fin);
}

int main(){
    robot linefollower=robot();
    float initial_x,initial_y,initial_orientation;
    cout<<"Give the inital x: ";
    cin>>initial_x;
    cout<<"Give the inital y: ";
    cin>>initial_y;
    cout<<"Give the intial orientation: ";
    cin>>initial_orientation;
    linefollower.set(initial_x,initial_y,initial_orientation);
    linefollower.set_noise(2.0);
    follow_the_line(&linefollower);
    linefollower=linefollower.moverobot(10);
    //linefollower.orientation=0.01;
    //linefollower.moverobot(1);
    //linefollower.print();
    return 0;
}
