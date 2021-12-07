#include <iostream>

#ifndef PP_PATH_H
#define PP_PATH_H

#include "point.hpp"
#include <vector>
#include <algorithm>
#include <math.h>

using std::vector;
// A path is basically a vector of waypoints

namespace PP {

    class Path {

        private:
        
            vector< Waypoint > points;

            void interpolate(double spacing){

                std::vector< Waypoint > interpolatedPoints;

                for(int loopCounter = 1; loopCounter < points.size(); loopCounter++ ){

                    Waypoint vector = points.at(loopCounter) - points.at(loopCounter - 1);

                    double mag = vector.magnitude();

                    int numPoints = ceil(mag / spacing);

                    vector = vector.normalize() * spacing;

                    for(int internalCounter = 0; internalCounter < numPoints; internalCounter++) {
                        interpolatedPoints.push_back( points.at(loopCounter - 1) + (vector * internalCounter) );
                    }

                }

                interpolatedPoints.push_back(points.at(points.size() - 1)); // add the final point

                points = interpolatedPoints;

            }

            void smooth(double a, double tol){

                std::vector< Waypoint > finalPath = std::vector<Waypoint>(points);

                double b = 1-a;
                double change = tol;

                while(change >= tol) {

                    change = 0.0;

                    for(int i = 1; i < points.size() - 1; i++){
                        Waypoint curPoint = finalPath.at(i);

                        Waypoint newPoint(curPoint);

                        double  xAux = curPoint.x;
                        newPoint.x += a * (points.at(i).x - curPoint.x ) + b * (finalPath.at(i - 1).x + finalPath.at(i + 1).x - (2.0 * curPoint.x));
                        change += fabs(newPoint.x - curPoint.x);

                        double yAux = curPoint.y;
                        newPoint.y += a * (points.at(i).y - curPoint.y ) + b * (finalPath.at(i - 1).y + finalPath.at(i + 1).y - (2.0 * curPoint.y));
                        change += fabs(newPoint.y - curPoint.y);

                        finalPath.at(i) = newPoint;
                    }
                    
                    //printf("%f\n", change);

                }

                points = finalPath;

            }

            
            double getCurveAtPoint(int index) {

                if(index == 0 || index == points.size()-1){
                    return 0.000000001;
                }

                double x1 = points.at(index).x + 0.0001;
                double x2 = points.at(index-1).x;
                double x3 = points.at(index+1).x;

                double y1 = points.at(index).y;
                double y2 = points.at(index-1).y;
                double y3 = points.at(index+1).y;

                double k1 = 0.5* ((x1*x1)+(y2*y2)-(x2*x2)-(y2*y2)) /(x1-x2);
                double k2 = (y1-y2)/(x1-x2);

                double center2 = 0.5 * ( (x2*x2)-(2*x2*k1)+(y2*y2)-(x3*x3)+(2*x3*k1)-(y3*y3) )  /  (((x3*k2)-y3+y2-(x2*k2)));
                double center1 = k1-k2*center2;

                double radius = sqrt( (x1-center1) *(x1-center1) + (y1-center1) * (y1-center1));

                return 1 / radius;
            }

            void calcVelocities(double max_vel, double max_accel, int k) {

                for(int index = 0; index < points.size(); index++) {

                    //std::cout << getCurveAtPoint(index) << std::endl;

                    double minSpeed = std::min(max_vel, (double) k/getCurveAtPoint(index) );
                    points.at(index).speed = minSpeed;

                }

                //std::cout << *this;

                points.at(points.size()-1).speed = 0.0;

                std::vector<double> distances;
                distances.push_back( 0.0 );

                for(int i = 1; i < points.size(); i++) {
                    distances.push_back( points.at(i).distanceFormula( points.at(i-1) ) );
                }

                for(int i = points.size()-2; i >=0; i--) {

                    double accelVal = sqrt( pow(points.at(i+1).speed, 2) + 2 * max_accel * distances.at(i+1) );

                    //std::cout << accelVal << std::endl;

                    points.at(i).speed = std::min(points.at(i).speed, accelVal);

                }

            }

        public:

            Path(const std::vector< Waypoint >& initialPoints, double spacing, double a, double tol, double max_vel, double max_accel, int turnK) : points(initialPoints) {
                interpolate(spacing);
                smooth(a, tol);
                calcVelocities(max_vel, max_accel, turnK);
            }

            Waypoint& getExactPoint(const int& index){
                return points[index];
            }

            Waypoint getInterpolatedPoint(const double& index){
                int flooredIndex = std::floor(index);
                double fraction = index - flooredIndex;
                Waypoint startPoint = points[flooredIndex];
                return startPoint + startPoint.interpolate(points[flooredIndex + 1], fraction);
            }

            friend std::ostream& operator<<(std::ostream& os, const Path& path){
                for(int i = 0; i < path.points.size(); i++){
                    std::cout << "Point " << i << ": " << path.points.at(i);
                }
            }

    };

};

#endif