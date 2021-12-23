
#include <iostream>

#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <algorithm>
#include <math.h>

// Everything is in the PP namespace

namespace PP {

	struct Waypoint {

		double x;
		double y;
		double speed; // measured in inches per second

		Waypoint (const double& _x, const double& _y, const double& _speed = 0.0) : x(_x), y(_y), speed(_speed){
		}

		Waypoint () : Waypoint(0,0,0){}

		friend Waypoint operator+ (const Waypoint& lhs, const Waypoint& rhs) {
			return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.speed};
		}

		friend Waypoint operator- (const Waypoint& lhs, const Waypoint& rhs) {
			return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.speed };
		}

		friend Waypoint operator* (const Waypoint& point, const double& factor) {
			return { point.x * factor, point.y * factor, point.speed };
		}

		friend Waypoint operator* (const double& factor, const Waypoint& point) {
			return point * factor;
		}

		friend std::ostream& operator<< (std::ostream& os, const Waypoint& point) {
			os << "X: " << point.x << " Y: " << point.y  << " Speed: " << point.speed << std::endl;
			return os;
		}

		friend std::istream& operator>> (std::istream& is, Waypoint& point) {
			is >> point.x >> point.y >> point.speed;
			return is;
		}

		double magnitude(){
			return sqrt(x * x + y * y);
		}

		double distanceFormula(const Waypoint other){
			return (*this - other).magnitude();
		}

		Waypoint normalize() {
			double mag = magnitude();
  			return  { x / mag, y / mag, speed};
		}

		Waypoint interpolate(const Waypoint& other, const double& factor){
			// So this math is a bit harder
			// Subtract the other point from this
			// Multiply by factor to get the portion
			// Add this

			return *this + ( factor * (other - *this) );

		}

		double dotProduct(const Waypoint& other) const {
			return x * other.x + y * other.y;
		}

	};

}

#endif