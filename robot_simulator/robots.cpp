//#define _USE_MATH_DEFINES //For M_PI
#include<iostream>
#include<cmath>
#include<cstdio>
#include<iomanip>
#include<complex>
#include<vector>
#include<numeric>
#include<initializer_list>
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
        void operator=(robot);
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

void robot::operator=(robot r){
    set(r.x,r.y,r.orientation);
    set_noise(r.measurementnoise);
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

float follow_the_line(robot* robo, float param[]=NULL,bool print=false ) //float param1=0.3, float param2=0.1, float param3=0.001){ //PID Controller function
{
    robot newrobot=robot();
    newrobot.set(robo->x,robo->y,robo->orientation);
    newrobot.set_noise(robo->measurementnoise);
    int seed=time(NULL);
    if (sizeof(param)==0){
        float param[3]={0.2,0.0,0.0};
    }
    float speed=1.0,measurement,prevmeasure,sum,x2,error=0.0; //Equals distance assuming time interval of 1
    int iterations=1000;
    sum=0; //To integrate over the error
    measurement=newrobot.y;
    prevmeasure=newrobot.y;
    FILE* fin=fopen("robotpath.txt","w");
    for(int i=0; i<iterations*2; ++i){
        measurement=newrobot.sense(seed++);
        sum=sum+measurement;
        x2=-param[0]*measurement-param[1]*(measurement-prevmeasure)-param[2]*sum;
        newrobot.orientation= x2< -M_PI/2?-M_PI/2:(x2>M_PI/2?M_PI/2:x2);
        newrobot=newrobot.moverobot(speed);
        prevmeasure=measurement;
        //robot->print();
        if(i>=iterations){
            error+=abs(measurement);
        }
        if (print){
            newrobot.printtofile(fin,i);
            *robo=newrobot;
        }
    }
    fclose(fin);
    return error/float(iterations);
}

void gradient_descent(robot* robo,float tolerance=0.000001)
{
    int param_no=3;
    float diff_params[param_no];
    float params[param_no],sum;
    for (int i=0; i<param_no; ++i){
        diff_params[i]=1.0;
        params[i]=0.0;
    }
    robot newrobot=robot();
    newrobot.set(robo->x,robo->y,robo->orientation);
    newrobot.set_noise(robo->measurementnoise);
    float b_error=follow_the_line(&newrobot,params),error;
    int it=0;
    sum=tolerance*10;
    while(sum>=tolerance){
        for (int i=0; i<=param_no; ++i){
            params[i]+=diff_params[i];
            //robot newrobot=robot();
            //newrobot.set(robo->x,robo->y,robo->orientation);
            //newrobot.set_noise(robo->measurementnoise);
            error=follow_the_line(&newrobot,params);
            if (error<b_error){
                b_error=error;
                diff_params[i]*=1.1;
            }else{
                params[i]+=2.0*diff_params[i];
                //robot newrobot=robot();
                //newrobot.set(robo->x,robo->y,robo->orientation);
                //newrobot.set_noise(robo->measurementnoise);
                error=follow_the_line(&newrobot,params);
                if(error<b_error){
                    b_error=error;
                    diff_params[i]*=1.1;
                }else{
                    params[i]+=diff_params[i];
                    diff_params[i]*=0.9;
                }
            }
        }
        it=it+1;
        printf("Twiddle# %d %f %f %f %f %f %f %f %f\n",it,params[0],params[1],params[2],diff_params[0],diff_params[1],diff_params[2],b_error,sum);
        sum=0;
        for (int i=0; i<param_no; ++i){
            sum+=abs(diff_params[i]);
        }
    }
    error=follow_the_line(robo,params,true);
    cout<<error<<endl;
}

int main(){
    robot linefollower=robot();
    float initial_x,initial_y,initial_orientation,noise;
    cout<<"Give the inital x: ";
    cin>>initial_x;
    cout<<"Give the inital y: ";
    cin>>initial_y;
    cout<<"Give the intial orientation: ";
    cin>>initial_orientation;
    cout<<"Give the variance of gaussian noise in measurement :";
    cin>>noise;
    linefollower.set(initial_x,initial_y,initial_orientation);
    linefollower.set_noise(noise);
    gradient_descent(&linefollower);
    //follow_the_line(&linefollower);
    linefollower=linefollower.moverobot(10);
    //linefollower.orientation=0.01;
    //linefollower.moverobot(1);
    //linefollower.print();
    return 0;
}
