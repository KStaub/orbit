//File: field.cxx 02/22/2011
//Written by: Kyle Staub
//E-mail: kyle.staub@colorado.edu
//The main purpose of this program is to draw an acceleration vector
//field with three massive bodies acting on each individual point of the
//field and to see the magnitude and direction of these accelerations
//resulting from the forces of the massive bodies.

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
double const WMAX = 2.5e11;
double const G = 6.67e-11;
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// The parameters x and y give the location of a spaceship in a
// two-dimensional star field. The parameters sx and sy give the
// location of a star of mass m in this field. The return value
// is the acceleration along the x-axis that the ship feels from
// the star. All distances are in meters and the mass
// is in kilograms. The answer is in m/sec^2.
double accx(double x, double y, double sx, double sy, double m);

// The parameters x and y give the location of a spaceship in a
// two-dimensional star field. The parameters sx and sy give the
// location of a star of mass m in this field. The return value
// is the acceleration along the y-axis that the ship feels from
// the star. All distances are in meters and the mass
// is in kilograms. The answer is in m/sec^2.
double accy(double x, double y, double sx, double sy, double m);

//This function draws the vector field using draw_vector and draw_star,
//and because it's only doing these things, it doesn't need parameters.
//It's useful to have this and simply call it in the main program so further
//changes can be made without having to change the main program. 
void draw_field(double sx_1, double sx_2, double sx_3,
		double sy_1, double sy_2, double sy_3);

//This function uses the world coordinates (sx, sy) of a planet/star and draws
//it at that location with a specified color (COLOR) and size (radius).
void draw_star(double sx, double sy, int COLOR, int radius);

//This function draws a vector of acceleration on a starship given it's initial
// x and y coordinates in world coordinates. 
void draw_vector(double x, double y, double sx_1, double sx_2,
		 double sx_3,double sy_1, double sy_2, double sy_3);

//Length uses current x and y values to calculate the length of the
//vector that would be drawn at those points using accx and accy.
//To have this function separate from draw_vector allows for easy removal
//in case we ever want to not restrict the lengths in any way. 
double length(double x, double y, double sx_1, double sx_2,
	      double sx_3,double sy_1, double sy_2, double sy_3);

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
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
int main()
{
    double sx_1 = 3.0e10;
    double sx_2 = -4.5e10;
    double sx_3 = 4.5e10;
    double sy_1 = -7.5e10; 
    double sy_2 = 3.0e10;
    double sy_3 = 9.0e10;

    initwindow(S, S, "Field of Dreams", 0, 0, true);

    double t = 0.0;

    while(true)
    {
	cleardevice();
	draw_field(sx_1, sx_2, sx_3, sy_1, sy_2, sy_3);
    
	draw_star(sx_1, sy_1, YELLOW, 5);
	draw_star(sx_2, sy_2, BLUE, 7);
	draw_star(sx_3, sy_3, RED, 9);

	sx_1 += sx_1*.0005*cos(t)*t;
	sx_2 += sx_2*.001*cos(t)*t;
	sx_3 += sx_3*.0005*cos(t)*t;
	
	sy_1 += sy_1*.0005*sin(t)*t;
	sy_2 += sy_2*.001*sin(t)*t;
	sy_3 += sy_3*.0005*sin(t)*t;

	swapbuffers();

	t += .01;
	delay(1);

    }
    
    delay(60000);
    return EXIT_SUCCESS;
}
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
double accx(double x, double y, double sx, double sy, double m)
{
    double dx = (sx - x);
    double dy = (sy - y);
    double denominator = pow(dx*dx + dy*dy, 1.5);

    return G*m*dx/denominator;
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
double accy(double x, double y, double sx, double sy, double m)
{
    double dx = (sx - x);
    double dy = (sy - y);
    double denominator = pow(dx*dx + dy*dy, 1.5);
    
    return G*m*dy/denominator;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void draw_field(double sx_1, double sx_2, double sx_3,
		double sy_1, double sy_2, double sy_3)
{
    double x, y;
    
    for(x = -WMAX; x <= WMAX; x += WMAX/6)
    {
	for(y = -WMAX; y <= WMAX; y += WMAX/6)
	{
	    
	    if(length(x, y, sx_1, sx_2, sx_3,
		      sy_1, sy_2, sy_3) < (WMAX/7.07107))
	    {
		draw_vector(x, y, sx_1, sx_2, sx_3,
			    sy_1, sy_2, sy_3);
	    }
	}
    }

}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void draw_star(double sx, double sy, int COLOR, int radius)
{
    //These turn the given world coordinates of the star into pixel
    //coordinates to be drawn.
    sx = pixel(sx, -WMAX, WMAX, S);
    sy = pixel(sy, WMAX, -WMAX, S);
    
    setfillstyle(SOLID_FILL, COLOR);
    fillellipse(int(sx), int(sy), radius, radius);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void draw_vector(double x, double y, double sx_1, double sx_2,
		 double sx_3,double sy_1, double sy_2, double sy_3)
{
    //These lines calculate each acceleration in the x from each star
    //at each x and y. Then, gives a total.
    double vx_1 = accx(x, y, sx_1, sy_1, 2.5e30);
    double vx_2 = accx(x, y, sx_2, sy_2, 9.5e30);
    double vx_3 = accx(x, y, sx_3, sy_3, 1.5e31);
    double vx_total = (vx_1 + vx_2 + vx_3) * (5.0e10);

    //These lines calculate each acceleration in the y from each star
    //at each x and y. Then gives a total. 
    double vy_1 = accy(x, y, sx_1, sy_1, 2.5e30);
    double vy_2 = accy(x, y, sx_2, sy_2, 9.5e30);
    double vy_3 = accy(x, y, sx_3, sy_3, 1.5e31);
    double vy_total = (vy_1 + vy_2 + vy_3) * (5.0e10);
    
    //end_x and end_y are the final coordinates of the vector line.
    double end_x = pixel(x + vx_total, -WMAX, WMAX, S);
    double end_y = pixel(y + vy_total, WMAX, -WMAX, S);

    x = pixel(x, -WMAX, WMAX, S);
    y = pixel(y, WMAX, -WMAX, S);

    setfillstyle(SOLID_FILL, WHITE);
    fillellipse(int(end_x), int(end_y), 2, 2);
    line(int(x), int(y), int(end_x), int(end_y));
}
//----------------------------------------------------------------------------

//------------------------------------------------------------------------------
double length(double x, double y, double sx_1, double sx_2,
	      double sx_3,double sy_1, double sy_2, double sy_3)
{
    //These lines calculate each acceleration in the x from each star
    //at each x and y. Then, gives a total.
    double vx_1 = accx(x, y, sx_1, sy_1, 2.5e30);
    double vx_2 = accx(x, y, sx_2, sy_2, 9.5e30);
    double vx_3 = accx(x, y, sx_3, sy_3, 1.5e31);
    double vx_total = (vx_1 + vx_2 + vx_3) * (5.0e10);

    //These lines calculate each acceleration in the y from each star
    //at each x and y. Then gives a total. 
    double vy_1 = accy(x, y, sx_1, sy_1, 2.5e30);
    double vy_2 = accy(x, y, sx_2, sy_2, 9.5e30);
    double vy_3 = accy(x, y, sx_3, sy_3, 1.5e31);
    double vy_total = (vy_1 + vy_2 + vy_3) * (5.0e10);

    //This gives the distance of the line.  
    double distance = pow((pow((vx_total), 2) +
			   pow((vy_total), 2)), 0.5);
    

    return (distance);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
int pixel(double v, double v0, double v1, int pmax)
{
    double ratio = (v - v0)/(v1 - v0);
    
    return int(ratio*pmax);
}
//------------------------------------------------------------------------------
