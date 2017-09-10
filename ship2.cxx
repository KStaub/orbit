//File: ship2.cxx 03/02/2011
//Written by: Kyle Staub
//E-mail: kyle.staub@colorado.edu
//This program draws a trajectory for a ship in the acceleration field
//drawn last week, and adds the ability to move the stars. 

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
//window size in world coordinates (W), the strength of gravity (G), the
//simulation time (SIMTIME), and how many frames per second (FPS).
int const S = 500;
double const WMAX = 2.5e11;
double const G = 6.67e-11;
double SIMTIME = 50000;
double FPS = 30;
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

//This function draws the vector field using draw_vector and draw_star.
//It's useful to have this and simply call it in the main program so further
//changes can be made without having to change the main program. The parameters
//are used in draw_vector, and if these parameters are changed in the main
//program, the parameters in draw_vector are as well. 
void draw_field(double sx_1, double sx_2, double sx_3,
		double sy_1, double sy_2, double sy_3);

//This function uses the world coordinates (sx, sy) of a planet/star/ship
//and draws it at that location with a specified color (COLOR)
//and size (radius).
void draw_star(double sx, double sy, int COLOR, int radius);

//This function draws a vector of acceleration on a starship given it's initial
// x and y coordinates in world coordinates, and also the locations of all
//three stars given by (sx, sy).
void draw_vector(double x, double y, double sx_1, double sx_2,
		 double sx_3,double sy_1, double sy_2, double sy_3);

//Length uses current x and y values, and locations of the stars
//to calculate the length of the vector that would be drawn at those
//points using accx and accy.
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

// world(p, v0, v1, pmax) is a coordinate transformation, similar to the
// pixel function, but it goes in the other direction (converting a pixel
// coordinate p (ranging from 0 to pmax) into a world coordinate (ranging
// from v0 to v1). Borrowed from Mr. Michael Main.
double world(int p, double v0, double v1, int pmax);
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
int main()
{

    initwindow(S, S, "Field of Dreams", 0, 0, true);
    
    //Control for how many right clicks there have been.
    int rclicks;

    //Pixel coordinates of a click.
    int px, py;
    
    // Starting (x,y) and (vx, vy) of the ship.
    double x = -6.0e10;
    double y = -1.3e11;
    double vx = 0, vy = 0;

    //Coordinates of the stars.
    double sx_1 = 3.0e10;
    double sx_2 = -4.5e10;
    double sx_3 = 4.5e10;
    double sy_1 = -7.5e10; 
    double sy_2 = 3.0e10;
    double sy_3 = 9.0e10;

    //Calculations for acceleration.
    double total_ax, ax_1, ax_2, ax_3;
    double total_ay, ay_1, ay_2, ay_3;

    
    while(true)
    {
	//Initializing variables.    
	x = x;
	y = y;
	vx = vx;
	vy = vy;
	rclicks = rclicks;
	
	cleardevice();
	
	//Acceleration field given those star positions.
	draw_field(sx_1, sx_2, sx_3, sy_1, sy_2, sy_3);
	//The stars in their starting positions.
	draw_star(sx_1, sy_1, YELLOW, 5);
	draw_star(sx_2, sy_2, BLUE, 7);
	draw_star(sx_3, sy_3, RED, 9);
	//Ship (using draw_star).
	draw_star(x, y, GREEN, 4);

	swapbuffers();
	delay(int(1000/FPS));
	
	//Calculations for acceleration.
	ax_1 = accx(x, y, sx_1, sy_1, 2.5e30);
	ax_2 = accx(x, y, sx_2, sy_2, 9.5e30);
	ax_3 = accx(x, y, sx_3, sy_3, 1.5e31);
	total_ax = (ax_1 + ax_2 + ax_3);

	ay_1 = accy(x, y, sx_1, sy_1, 2.5e30);
	ay_2 = accy(x, y, sx_2, sy_2, 9.5e30);
	ay_3 = accy(x, y, sx_3, sy_3, 1.5e31);
	total_ay = (ay_1 + ay_2 + ay_3);

	//New (x,y) and new (vx, vy).
	x = x + vx*SIMTIME;
	y = y + vy*SIMTIME;
	
	vx = vx + total_ax*SIMTIME;
	vy = vy + total_ay*SIMTIME;

	//This is how we calculate left clicks and the new ship (x,y).
	if(ismouseclick(WM_LBUTTONDOWN))
	{
	    getmouseclick(WM_LBUTTONDOWN, px, py);
	    x = world(px, -WMAX, WMAX, S);
	    y = world(py, WMAX, -WMAX, S);
	    vx = vy = 0;
	}

	//This is how we calculate right clicks and star positions
	//based on them.
	if(ismouseclick(WM_RBUTTONDOWN))
	{
	    ++rclicks;
	    
	    if(rclicks%3 == 1)
	    {
		getmouseclick(WM_RBUTTONDOWN, px, py);
		sx_1 = world(px, -WMAX, WMAX, S);
		sy_1 = world(py, WMAX, -WMAX, S);
	    }
	    if(rclicks%3 == 2)
	    {
		getmouseclick(WM_RBUTTONDOWN, px, py);
		sx_2 = world(px, -WMAX, WMAX, S);
		sy_2 = world(py, WMAX, -WMAX, S);
	    }
	    if(rclicks%3 == 0)
	    {
		getmouseclick(WM_RBUTTONDOWN, px, py);
		sx_3 = world(px, -WMAX, WMAX, S);
		sy_3 = world(py, WMAX, -WMAX, S);
	    }
	}

    }
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
    double x, y; //Loop controls (also, coordinates).
    
    for(x = -WMAX; x <= WMAX; x += WMAX/6)
    {
	for(y = -WMAX; y <= WMAX; y += WMAX/6)
	{

	    //Condition on whether or not to draw vectors. 
	    if(length(x, y, sx_1, sx_2, sx_3, sy_1, sy_2, sy_3)
	       < (WMAX/7.07107))
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

    //Converts into pixel coordinates so they can be drawn.
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

//-----------------------------------------------------------------------------
double world(int p, double v0, double v1, int pmax)
{
    double fraction = double(p)/double(pmax);
    double distance_from_v0 = fraction*(v1 - v0);
    
    return v0 + distance_from_v0;
}
//-----------------------------------------------------------------------------
