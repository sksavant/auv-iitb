//#define _USE_MATH_DEFINES //For M_PI
#include<iostream>
#include<cmath>
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
        robot(); //constructor
        void set(float, float, float);//sets the position and orientation to given values
        //void set_noise(measurementnoise); //Sets the measurement noise: Later!
        robot move(float , float ,float , float);
        void sense(); // Sense the position and update belief

    private:

};

void robot::set(float nx, float ny, float nor)//sets the position and orientation to given values
{
    x=nx;
    y=ny;
    orientation=nor;
}

robot robot::move(float steering, float distance, float tol=0.0001, float max_steering=M_PI_4)//float tol=0.0001, float max_steering=M_PI_4)
{
  float turn,center_x,center_y,radius;
  //Some checks to do : if distance is positive, steering angle is not high
    if (steering>max_steering){
        steering=max_steering;
    }
    if(steering<-max_steering){
        steering=-max_steering;
    }
    if(distance<0.0){
        distance=0.0;
    }
    robot newrobot=robot();
    //set steering noise here if there or distance noise
    turn=tan(steering);
    if (abs(turn)<tol){
        newrobot.x=x+ distance*cos(orientation);
        newrobot.y=y+ distance*sin(orientation);
        newrobot.orientation= fmod(orientation+turn,float(2.0*M_PI));
    }else{
        radius=distance/turn;
        center_x=x-sin(orientation)*radius;
        center_y=y+cos(orientation)*radius;
        newrobot.orientation= fmod(orientation+turn,float(2.0*M_PI));
        newrobot.x=center_x+sin(orientation)*radius;
        newrobot.y=center_y-cos(orientation)*radius;
    }
    return newrobot;
}

int main(){
    cout<<M_PI<<endl;

    return 0;
}
