//File: functions.cxx 02/14/2011
//Written by: Kyle Staub
//E-mail: kyle.staub@colorado.edu
//The main purpose of this program is to utilize functions to calculate
//various values for the position of a (several) planets and a sun and
//use a while loop to have them orbit the sun. 

//Directives:
#include<cstdlib>
#include<cmath>
#include<graphics.h>
#include<iostream>
#include<string>
using namespace std;
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//These are constants for the window size in pixel coordinates (S) and the
//window size in world coordinates (W).
int const S = 500;
int const W = 10000; //The bigger W, the smoother the orbits look.
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Function Prototypes
//draw_planet uses a, ecc, period, theta, and t to calculate orbit_dist and
//orbit_angle in order to draw the planet in the correct place. It also
//uses "color" to denote the color of the fillellipse, and size for the radius
//of the planet. 
void draw_planet(double a, double ecc, double period, double t, int color,
		 int size, double theta);

// A function to calculate the solution to Kepler's equation. The equation is
// x = ma + ecc*sin(x), and the answer returned is x, within EPSILON.
double kepler_solution(double ma, double ecc);

// orbit_angle(ecc, period, t) computes the angle of a planet's position
// in polar coordinates at time t.  ecc is the ecentricity of the orbit,
// and period is the time for one full orbit.  period and t must be in the
// same units.
double orbit_angle(double ecc, double period, double t);

// orbit_dist(a, ecc, theta) computes the distance of a planet
// from the sun when it is at angle theta in polar coordinates.
// a is the semi-major axis; ecc is the orbit's eccentricity.  The units
// of the answer are the same as the units of a.
double orbit_dist(double a, double ecc, double theta);

// pixel(v, v0, v1, pmax) converts v from an interval that ranges from v0 to v1
// into an integer interval that ranges from 0 to pmax. Note that when v = v0,
// the answer is always 0, and when v = v1, the answer is always pmax.
// Example 1: Suppose an x-coordinate called x ranges from -2 (on the left
// side of the screen) to +3 (on the right side of the screen), and
// we want to figure out the corresponding pixel x coordinate for a screen
// that is 400 pixel wide. Then call pixel(x, -2, 3, 400).
// Example 2: Suppose a y-coordinate called y ranges from +4 (on the top
// of the screen) to -2 (on the bottom of the screen), and
// we want to figure out the corresponding pixel y coordinate for a screen
// that is 300 pixels tall. Then call pixel(y, 4, -2, 300). Note that in this
// case v0 > v1 since we want bigger coordinates at the top of the screen.
int pixel(double v, double v0, double v1, int pmax);
//-----------------------------------------------------------------------------


//------------------------------------------------------------------------------
int main()
{
    double t = 0.0; //Initialization of time.
    
    
    initwindow(S, S, "Solar System", 0, 0, true);
    
    while (true)
    {
	cleardevice();
	draw_planet(0, 0, 0, 0, YELLOW, 45, 0); //Sun
	draw_planet(4000, .4, 10, t, BLUE, 10, M_PI/6);
	draw_planet(5900, .3, 25, t, RED, 15, M_PI/4);
	draw_planet(9001, .08, 100, t, GREEN, 20, 2*M_PI);
	draw_planet(8050, .6, 50, t, BROWN, 12, -M_PI/2);

	swapbuffers();
	
	
	t += .05;
       
	delay(5);    
    }
    
    return EXIT_SUCCESS;
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void draw_planet(double a, double ecc, double period, double t, int color,
		 int size, double theta)
{

    setfillstyle(SOLID_FILL, color);
    
    double r, angle; //Used to calculate px, and py. 
    
    r = orbit_dist(a, ecc, theta);
    angle = orbit_angle(ecc, period, t);

    int px, py;
    
    px = int(r*cos(angle));
    py = int(r*sin(angle));

    px = pixel(px, -W, W, S);
    py = pixel(py, W, -W, S);
    
    fillellipse(px, py, size, size);
    
}
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
double kepler_solution(double ma, double ecc)
{
    const double EPSILON = 0.001; //Criterion for accuracy in error.
    double x = 0.0;
    double rightside = ma + ecc*sin(x);
    double error = abs(x - rightside);

    while (error > EPSILON)
    {
	x = rightside;
	rightside = ma + ecc*sin(x);
	error = abs(x - rightside);
    }
    return x;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
int pixel(double v, double v0, double v1, int pmax)
{
    double ratio = (v - v0)/(v1 - v0);
    
    return int(ratio*pmax);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
double orbit_angle(double ecc, double period, double t)
{
    double ma = (2*M_PI*t)/period;
    double x = kepler_solution(ma, ecc);
    double answer = 2*atan(sqrt((1 + ecc)/(1 - ecc))*tan(x/2));
    
    return answer;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
double orbit_dist(double a, double ecc, double theta)
{
    double answer = a*(1 - ecc*ecc)/(1 + ecc*cos(theta));
    
    return answer;
}
//------------------------------------------------------------------------------
