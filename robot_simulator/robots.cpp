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

int main(){
    cout<<M_PI<<endl;

    return 0;
}
