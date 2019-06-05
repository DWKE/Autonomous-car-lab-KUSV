//Polynomial Fit
#include <ros/ros.h>
#include "kusv_msgs/LanePointData.h"
#include <geometry_msgs/Point.h>
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

void Callback(const kusv_msgs::LanePointDataConstPtr& msg) {
    kusv_msgs::LanePointData lane;

    int degree, N;

    N = msg->point.size();
    double x[N],y[N];
    for (int i = 0; i < N; i++) {
        x[i] = msg->point.at(i).x;
    }
    for (int i = 0; i < N; i++) {
        y[i] = msg->point.at(i).y;
    }
    degree = 3;                                  // n is the degree of Polynomial
    double X[ 2*degree+1 ];                        //Array that will store the values of sigma(xi),sigma(xi^2),sigma(xi^3)....sigma(xi^2n)
    for (int i = 0; i < 2*degree+1; i++) {
        X[i]=0;
        for (int j = 0; j < N; j++)
            X[i]=X[i]+pow(x[j],i);        //consecutive positions of the array will store N,sigma(xi),sigma(xi^2),sigma(xi^3)....sigma(xi^2n)
    }
    double B[degree+1][degree+2], a[degree+1];            //B is the Normal matrix(augmented) that will store the equations, 'a' is for value of the final coefficients
    for (int i = 0; i <= degree;i++) {
        for (int j = 0; j <= degree; j++) {
            B[i][j]=X[i+j];                 //Build the Normal matrix by storing the corresponding coefficients at the right positions except the last column of the matrix
        }
    }
    double Y[degree+1];                    //Array to store the values of sigma(yi),sigma(xi*yi),sigma(xi^2*yi)...sigma(xi^n*yi)
    for (int i = 0; i < degree+1; i++) {
        Y[i]=0;
        for (int j = 0; j < N; j++) {
            Y[i]=Y[i]+pow(x[j],i)*y[j];        //consecutive positions will store sigma(yi),sigma(xi*yi),sigma(xi^2*yi)...sigma(xi^n*yi)
        }
    }
    for (int i = 0; i <= degree; i++) {
        B[i][degree+1]=Y[i];                //load the values of Y as the last column of B(Normal Matrix but augmented)
    }
    degree = degree+1;                //n is made n+1 because the Gaussian Elimination part below was for n equations, but here n is the degree of polynomial and for n degree we get n+1 equations
    ROS_INFO_STREAM("The Normal(Augmented Matrix):");
    for (int i = 0; i < degree; i++) {           //print the Normal-augmented matrix
        for (int j = 0; j <= degree; j++) {
            cout<<B[i][j]<<" "<<setw(6);
        }
        cout<<"\n";
    }
    for (int i = 0; i < degree; i++) {                   //From now Gaussian Elimination starts(can be ignored) to solve the set of linear equations (Pivotisation)
        for (int k = i+1 ; k < degree; k++) {
            if (B[i][i] < B[k][i]) {
                for (int j = 0; j <= degree; j++) {
                    double temp=B[i][j];
                    B[i][j] = B[k][j];
                    B[k][j] = temp;
                }
            }
        }
    }

    for (int i = 0; i < degree-1; i++) {           //loop to perform the gauss elimination
        for (int k = i+1; k < degree; k++) {
            double t=B[k][i]/B[i][i];
            for (int j = 0; j <= degree; j++)
                B[k][j]=B[k][j]-t*B[i][j];    //make the elements below the pivot elements equal to zero or elimnate the variables
        }
    }
    for (int i = degree-1; i >= 0; i--) {               //back-substitution
        //x is an array whose values correspond to the values of x,y,z..
        a[i]=B[i][degree];                //make the variable to be calculated equal to the rhs of the last equation
        for (int j = 0; j < degree; j++) {
            if (j!=i) {           //then subtract all the lhs values except the coefficient of the variable whose value                                   is being calculated
                a[i]=a[i]-B[i][j]*a[j];
            }
        }
        a[i] = a[i]/B[i][i];            //now finally divide the rhs by the coefficient of the variable to be calculated
    }
    cout<<"\n";
    ROS_INFO_STREAM("The values of the coefficients:");
    for (int i = 0; i < degree; i++) {
        ROS_INFO_STREAM("x^" << i << " = " << a[i]);            // Print the values of x^0,x^1,x^2,x^3,....
    }
    cout<<"\n";
    ROS_INFO_STREAM("The fitted Polynomial is given by: y=");
    for (int i = 0; i < degree; i++) {
        cout<<" + ("<<a[i]<<")"<<"x^"<<i;
    }
    cout<<"\n";
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "polinomial_regression");
    ros::NodeHandle nh_;
    ros::Subscriber point_sub_ = nh_.subscribe("lane/point", 100, &Callback);
    ros::spin();
    return 0;
}
