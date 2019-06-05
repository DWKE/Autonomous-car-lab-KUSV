/**
 * @copyright Valeo Driving Assistance Research (c) 2014.
 * <br>All rights reserved. Subject to limited distribution and restricted disclosure only.
 * @author paulo.resende@valeo.com
 * @file geometry2d.c
 * @brief Geometry 2D source file.
 * @version 1.0
 * @date 03-03-2014
 * @bug	No known bugs
 * @warning	No warnings
 */

#include "geometry2d.h"

void
rotatePointAroundOrigin(
	Point2D *point_p,
	double rad
){
	// rad = (deg * PI / 180);
	double x = point_p->x;
	double y = point_p->y;

	point_p->x = (x * (double)cos(rad)) - (y * (double)sin(rad));
	point_p->y = (x * (double)sin(rad)) + (y * (double)cos(rad));
}  // end of rotatePointAroundOrigin

void
rotatePointAroundPivot(
	Point2D *point_p,
	Point2D *pivot_point_p,
	double	rad
){
	// Subtract pivot from Point2D point_p
	point_p->x -= pivot_point_p->x;
	point_p->y -= pivot_point_p->y;

	// Rotate Point2D point_p around the origin
	rotatePointAroundOrigin(point_p, rad);

	// Add pivot to the Point2D point_p
	point_p->x += pivot_point_p->x;
	point_p->y += pivot_point_p->y;
}  //end of rotate_around_pivot

double
pointsSquareDistance(
	Point2D *p1_p,
	Point2D *p2_p
){	
	// Determine the square distance from Point2D 1 (x1, y1) to Point2D 2 (x2, y2)
	//const double delta_x =  p1_p->x - p2_p->x;
	//const double delta_y =  p1_p->y - p2_p->y;
	//const double dist = delta_x * delta_x + delta_y * delta_y;
	//return dist;
	return (p1_p->x - p2_p->x) * (p1_p->x - p2_p->x) + (p1_p->y - p2_p->y) * (p1_p->y - p2_p->y);
}  // end of square_pointsDistance

double
pointsDistance(
	Point2D *p1_p,
	Point2D *p2_p
){
	const double dist = sqrt(pointsSquareDistance(p1_p, p2_p));
	return dist;
}  // end of pointsDistance

double
dotProduct(
	Segment2D *s1_p,
	Segment2D *s2_p
){
	//Compute the dot product s1 . s2
	double product;
	double xx1 = s1_p->point[1].x - s1_p->point[0].x;
	double yy1 = s1_p->point[1].y - s1_p->point[0].y;
	double xx2 = s2_p->point[1].x - s2_p->point[0].x;
	double yy2 = s2_p->point[1].y - s2_p->point[0].y;

	return product = xx1 * xx2 + yy1 * yy2;
}  // end of dotProduct

double crossProduct(Segment2D *s1_p, Segment2D *s2_p)
{
	//Compute the cross product s1 x s2
	double product;
	double xx1 = s1_p->point[1].x - s1_p->point[0].x;
	double yy1 = s1_p->point[1].y - s1_p->point[0].y;
	double xx2 = s2_p->point[1].x - s2_p->point[0].x;
	double yy2 = s2_p->point[1].y - s2_p->point[0].y;

	return product = xx1 * yy2 - yy1 * xx2;
}  // end of crossProduct

double
segmentsAngle(
	Segment2D *s1_p,
	Segment2D *s2_p
){
	double angle;
	double s1_length = pointsDistance(&s1_p->point[0], &s1_p->point[1]);
	double s2_length = pointsDistance(&s2_p->point[0], &s2_p->point[1]);

	return angle = acos(dotProduct(s1_p, s2_p)/(s1_length * s2_length));
}  // end of segment_angle

double
distancePointSegment(
	Point2D		*point_p,
	Segment2D	*segment_p,
	Point2D		*closest_point_p
){
	double A = point_p->x - segment_p->point[0].x;
	double B = point_p->y - segment_p->point[0].y;
	double C = segment_p->point[1].x - segment_p->point[0].x;
	double D = segment_p->point[1].y - segment_p->point[0].y;

	double dot = A * C + B * D;
	double len_sq = C * C + D * D;
	
	double param, dist;

	if (len_sq == 0.0) {
#ifdef _DEBUG
		printf("WARNING: P1 and P2 are coincident (denominator in the equation for param is 0)\n");
#endif
		closest_point_p->x = segment_p->point[0].x;
		closest_point_p->y = segment_p->point[0].y;
	} 
	else {
		param = dot / len_sq;
		if(param < 0.0) {
			closest_point_p->x = segment_p->point[0].x;
			closest_point_p->y = segment_p->point[0].y;
		}
		else if(param > 1.0) {
			closest_point_p->x = segment_p->point[1].x;
			closest_point_p->y = segment_p->point[1].y;
		}
		else {
			closest_point_p->x = segment_p->point[0].x + param * C;
			closest_point_p->y = segment_p->point[0].y + param * D;
		}
	}

	return dist = pointsDistance(point_p, closest_point_p); // Calls distance function;
}  // end of distancePointSegment

double
segmentsDistance(
	Segment2D	*s1_p,
	Segment2D	*s2_p,
	Point2D		*point_p
){
	Point2D closest_point;
	double dist, temp_dist;

	// Find the minimum distance between the segments --
	if (segmentsIntersection(s1_p, s2_p, point_p) == true) { 
    // The line segments intersect --
		dist = 0.0;
	}
	else {
		// If there is no intersection, then the distance between
		// a line endpoint and a line segment is the minimum distance
		temp_dist = distancePointSegment(&s1_p->point[0], s2_p, &closest_point);
		dist = temp_dist;
		temp_dist = distancePointSegment(&s1_p->point[1], s2_p, &closest_point);
		if (temp_dist < dist)
			dist = temp_dist;
		temp_dist = distancePointSegment(&s2_p->point[0], s1_p, &closest_point);
		if (temp_dist < dist)
			dist = temp_dist;
		temp_dist = distancePointSegment(&s2_p->point[1], s1_p, &closest_point);
		if (temp_dist < dist)
			dist = temp_dist;
	}

	return dist;
}  // end of segmentsDistance

bool
segmentsIntersection(
	Segment2D	*s1_p,
	Segment2D	*s2_p,
	Point2D		*point_p
){
	unsigned int i, j;

	bool segments_intersect = false;

	/* Point2D P1 from line segment 1 */
	const double x1 = s1_p->point[0].x;
	const double y1 = s1_p->point[0].y;	

	/* Point2D P2 from line segment 1 */
	const double x2 = s1_p->point[1].x;
	const double y2 = s1_p->point[1].y;

	/* Point2D P3 from line segment 2 */
	const double x3 = s2_p->point[0].x;
	const double y3 = s2_p->point[0].y;

	/* Point2D P4 from line segment 2 */
	const double x4 = s2_p->point[1].x;
	const double y4 = s2_p->point[1].y;
	
	double u_a, u_b, denominator;

	const double u_a_numerator = ((x4 - x3) * (y1 - y3)) - ((y4 - y3) * (x1 - x3));
	const double u_b_numerator = ((x2 - x1) * (y1 - y3)) - ((y2 - y1) * (x1 - x3));

	// The denominators for the equations for u_a and u_b are the same
	denominator = ((y4 - y3) * (x2 - x1)) - ((x4 - x3) * (y2 - y1));

	// If the denominator for the equations for u_a and u_b is 0 then the two lines are parallel.
	// Have the same slope.
	if (denominator == 0) {
		if ((u_a_numerator == 0.0) && (u_b_numerator == 0.0)) { // Lines are coincident
			double segment1_length = pointsDistance(&s1_p->point[0], &s1_p->point[1]);
			double segment2_length = pointsDistance(&s2_p->point[0], &s2_p->point[1]);
			double max_points_distance = 0.0;
			for (i = 0; i < 2; i++) {
				for (j = 0; j < 2; j++) {
					max_points_distance = max(max_points_distance, pointsDistance(&s1_p->point[i], &s2_p->point[j]));
				}
			}
			if (max_points_distance <= (segment1_length + segment2_length)) {
				segments_intersect = true;
			}
		}
		else { 
      // Lines aren't coincident
		}
	}
	else { // denominator != 0
		u_a = u_a_numerator / denominator;
		u_b = u_b_numerator / denominator;

		// If the denominator and numerator for the equations for u_a
		// and u_b are 0 then the two lines are coincident.
		if ((u_a > 0) && (u_a < 1) && (u_b > 0) && (u_b < 1)) {
			// Intersection point 
			point_p->x = x1 + (u_a * (x2 - x1));
			point_p->y = y1 + (u_a * (y2 - y1));
			segments_intersect = true;
			return segments_intersect;
		}
	}

	return segments_intersect;
}  // end of check_segmentsIntersection

double
distancePointLine(
	Point2D		*point_p,
	Segment2D	*segment_p,
	Point2D		*closest_point_p
){
	double A = point_p->x - segment_p->point[0].x;
	double B = point_p->y - segment_p->point[0].y;
	double C = segment_p->point[1].x - segment_p->point[0].x;
	double D = segment_p->point[1].y - segment_p->point[0].y;

	double dot = A * C + B * D;
	double len_sq = C * C + D * D;
	
	double param, dist;

	if (len_sq == 0.0) {
#ifdef _DEBUG
		printf("WARNING: P1 and P2 are coincident (denominator in the equation for param is 0)\n");
#endif
		closest_point_p->x = segment_p->point[0].x;
	    closest_point_p->y = segment_p->point[0].y;
	}
	else {
		param = dot / len_sq;
		closest_point_p->x = segment_p->point[0].x + param * C;
		closest_point_p->y = segment_p->point[0].y + param * D;
	}

	return dist = pointsDistance(point_p, closest_point_p); // Calls distance function;
}  // end of distancePointLine

bool
isPerpendicular(
	Point2D *point1_p,
	Point2D *point2_p,
	Point2D *point3_p
){ 
	// Checks if the given points are perpendicular to x or y axis 
	double yDelta_a = point2_p->y - point1_p->y;
	double xDelta_a = point2_p->x - point1_p->x;
	double yDelta_b = point3_p->y - point2_p->y;
	double xDelta_b = point3_p->x - point2_p->x;

	// Checking whether the lines of the two points are vertical
	if (fabs(xDelta_a) <= 0.000000001 && fabs(yDelta_b) <= 0.000000001) return false; // The points are perpendicular and parallel to x-y axis
	else if (fabs(yDelta_a) <= 0.0000001) return true; // A line of two point are perpendicular to x-axis 1
	else if (fabs(yDelta_b) <= 0.000000001) return true; // A line of two point are perpendicular to x-axis 2
	else if (fabs(xDelta_a)<= 0.000000001) return true; // A line of two point are perpendicular to y-axis 1
	else if (fabs(xDelta_b)<= 0.000000001) return true; // A line of two point are perpendicular to y-axis 2
	else return false ;
}  // end of isPerpendicular

void
circleFit(
	Point2D *point1_p,
	Point2D *point2_p,
	Point2D *point3_p,
	Circle *circle_p
){
	Point2D *p1_p, *p2_p, *p3_p;
	
	double aSlope, bSlope;
	double yDelta_a, xDelta_a, yDelta_b, xDelta_b;
	
	circle_p->radius = -1; // error checking 
	if (!isPerpendicular(point1_p, point2_p, point3_p)) { p1_p = point1_p; p2_p = point2_p; p3_p = point3_p; }//if
	else if (!isPerpendicular(point1_p, point3_p, point2_p)) { p1_p = point1_p; p2_p = point3_p; p3_p = point2_p; }//else if
	else if (!isPerpendicular(point2_p, point1_p, point3_p)) { p1_p = point2_p; p2_p = point1_p; p3_p = point3_p; }//else if
	else if (!isPerpendicular(point2_p, point3_p, point1_p)) { p1_p = point2_p; p2_p = point3_p; p3_p = point1_p; }//else if
	else if (!isPerpendicular(point3_p, point2_p, point1_p)) { p1_p = point3_p; p2_p = point2_p; p3_p = point1_p; }//else if
	else if (!isPerpendicular(point3_p, point1_p, point2_p)) { p1_p = point3_p; p2_p = point1_p; p3_p = point2_p; }//else if
	else return; // The three points are perpendicular to axis

	// Find the circle
	yDelta_a = p2_p->y - p1_p->y;
	xDelta_a = p2_p->x - p1_p->x;
	yDelta_b = p3_p->y - p2_p->y;
	xDelta_b = p3_p->x - p2_p->x;

	if (fabs(xDelta_a) <= 0.000000001 && fabs(yDelta_b) <= 0.000000001) {
		circle_p->point.x = 0.5f*(p2_p->x + p3_p->x);
		circle_p->point.y = 0.5f*(p1_p->y + p2_p->y);
		circle_p->radius = pointsDistance(&circle_p->point, p1_p);
		return;
	}
	if (fabs(xDelta_b) <= 0.000000001 && fabs(yDelta_a) <= 0.000000001) {
		circle_p->point.x = 0.5f*(p1_p->x + p2_p->x);
		circle_p->point.y = 0.5f*(p2_p->y + p3_p->y);
		circle_p->radius = pointsDistance(&circle_p->point, p1_p);
		return;
	}
	
	// isPerpendicular() assures that xDelta(s) are not zero
	aSlope = yDelta_a/xDelta_a;
	bSlope = yDelta_b/xDelta_b;
	if (fabs(aSlope-bSlope) <= 0.000000001) return; //The three pts are colinear

	// Calculate center
	circle_p->point.x = (double)((aSlope*bSlope*(p1_p->y - p3_p->y) + bSlope*(p1_p->x + p2_p->x) -
		aSlope*(p2_p->x + p3_p->x)) / (2.0*(bSlope-aSlope)));
	circle_p->point.y = (double)(-1.0*(circle_p->point.x - (p1_p->x+p2_p->x)/2.0)/aSlope +
		(p1_p->y+p2_p->y)/2.0);
	circle_p->radius = pointsDistance(&circle_p->point, p1_p);
	
	return;
}  // end of circleFit

double
interiorAngle(
	Point2D *point1_p,
	Point2D *point2_p,
	Point2D *point3_p
) {
	double slope[2];
	slope[0] = (point2_p->y - point1_p->y)/(point2_p->x - point1_p->x);
	slope[1] = (point3_p->y - point1_p->y)/(point3_p->x - point1_p->x);
	return atan(fabs((slope[1]-slope[0])/(1.0+(slope[1]*slope[0]))));
}  // end of interiorAngle

double
polygonArea(
	Point2D *points_p,
	unsigned int number_of_points
){
	double area = 0.0;
	unsigned int i, j = number_of_points-1;
	for (i = 0; i < number_of_points; i++) {
		area += (points_p[j].x + points_p[i].x) * (points_p[j].y - points_p[i].y);
		j = i;
	}
	return area*0.5;
} // end of polygonArea

Point2D
polygonCentroid(
	Point2D *points_p,
	unsigned int number_of_points
) {
  double area, sum;
  Point2D centroid;
  unsigned int i, j;

  centroid.x = centroid.y = sum = 0.0;  
  for(i = number_of_points-1, j = 0; j < number_of_points; i = j++) {
    sum += area = points_p[i].x * points_p[j].y - points_p[i].y * points_p[j].x;
    centroid.x += (points_p[i].x + points_p[j].x)*area;
    centroid.y += (points_p[i].y + points_p[j].y)*area;
  }
  sum *= 3.0;
  centroid.x /= sum;
  centroid.y /= sum;
  return centroid;
} // end of polygonCentroid

bool
pointInPolygon(
	Point2D *points_p,
	unsigned int number_of_points,
	Point2D *point_p
){
	unsigned int i, j = number_of_points-1;
	bool is_inside = false;

	for (i = 0; i < number_of_points; i++) {
		if ((points_p[i].y < point_p->y && points_p[j].y >= point_p->y ||
			points_p[j].y < point_p->y && points_p[i].y >= point_p->y) &&
			(points_p[i].x <= point_p->x || points_p[j].x <= point_p->x)) {
			if (points_p[i].x + (point_p->y - points_p[i].y) / (points_p[j].y - points_p[i].y) * (points_p[j].x - points_p[i].x) < point_p->x) {
				is_inside =! is_inside;
			}
		}
		j = i;
	}
	return is_inside;
} // end of pointInPolygon

double
polygonsDistance(
	Point2D *points1_p,
	unsigned int number_of_points1,
	Point2D *points2_p,
	unsigned int number_of_points2
){
	unsigned int i, j;
	double segments_distance;
	double minimum_distance = HUGE;
	Segment2D segment1, segment2;
	Point2D point;

	// Verify if any of the vertices of polygon 1 are inside polygon 2
	// or if any of the vertices of polygon 2 are inside polygon 1
	for (i = 0; i < number_of_points1; i++) {
		if (pointInPolygon(points2_p, number_of_points2, &points1_p[i]) == true) {
			return 0.0;
		}
	}
	for (i = 0; i < number_of_points2; i++) {
		if (pointInPolygon(points1_p, number_of_points1, &points2_p[i]) == true) {
			return 0.0;
		}
	}

	for (i = 0; i < number_of_points1; i++) {
		for (j = 0; j < number_of_points2; j++) {
			segment1.point[0].x = points1_p[i].x;
			segment1.point[0].y = points1_p[i].y;
			if (i == number_of_points1-1) {
				segment1.point[1].x = points1_p[0].x;
				segment1.point[1].y = points1_p[0].y;
			} 
			else {
				segment1.point[1].x = points1_p[i+1].x;
				segment1.point[1].y = points1_p[i+1].y; 
			}
			segment2.point[0].x = points2_p[j].x;
			segment2.point[0].y = points2_p[j].y;
			if (j == number_of_points2-1) {
				segment2.point[1].x = points2_p[0].x;
				segment2.point[1].y = points2_p[0].y;
			} 
			else {
				segment2.point[1].x = points2_p[j+1].x;
				segment2.point[1].y = points2_p[j+1].y; 
			}
			segments_distance = segmentsDistance(&segment1, &segment2, &point);
			if (segments_distance == 0.0) {
				return 0.0;
			} 
			else {
				if (segments_distance < minimum_distance) {
					minimum_distance = segments_distance;
				}
			}
		}
	}

	return minimum_distance;
} // end of polygonsDistance

double
deg2Rad(
	double angle
){
  return (angle * PI / 180.0);
}  // end of deg2Rad(double angle)

double
rad2Deg(
	double angle
){
  return (angle * 180.0 /PI);
}  // end of rad2Deg(double angle)

double
normalizeAngle(
	double angle
){
	while (angle < -PI) angle += 2.0*PI;
	while (angle >= PI) angle -= 2.0*PI;
	return angle;
}  // normalizeAngle(double angle)

double
lengthOfDegreeLatitude(
	double ref_latitude
){
	double latlen;
	// Convert latitude to radians
	double lat = deg2Rad(ref_latitude);

	// Set up "Constants"
	double m1 = 111132.92;		// Latitude calculation term 1
	double m2 = -559.82;		// Latitude calculation term 2
	double m3 = 1.175;			// Latitude calculation term 3
	double m4 = -0.0023;		// Latitude calculation term 4

	// Calculate the length of a degree of latitude and longitude in meters
	return latlen = (double)(m1 + (m2 * cos(2 * lat)) + (m3 * cos(4 * lat)) + (m4 * cos(6 * lat)));
}  // lengthOfDegreeLatitude

double
lengthOfDegreeLongitude(
	double ref_latitude
){
	double lonlen;
	// Convert latitude to radians
	double lat = deg2Rad(ref_latitude);

	// Set up "Constants"
	double p1 = 111412.84;		// Longitude calculation term 1
	double p2 = -93.5;			// Longitude calculation term 2
	double p3 = 0.118;			// Longitude calculation term 3

	return lonlen = (double)((p1 * cos(lat)) + (p2 * cos(3 * lat)) + (p3 * cos(5 * lat)));
}  // end of lengthOfDegreeLongitude

double
haversineDistance(
	GeoPoint2D *geopoint1_p,
	GeoPoint2D *geopoint2_p
){
	// Haversine formula to calculate distance (in meters) between two points specified by latitude/longitude (in numeric degrees)
  double dist;
  double delta_lat = deg2Rad(geopoint2_p->latitude - geopoint1_p->latitude);
  double delta_lon = deg2Rad(geopoint2_p->longitude - geopoint1_p->longitude);
  double a = (pow(sin(delta_lat/2), 2) + (cos(deg2Rad(geopoint1_p->latitude)) * cos(deg2Rad(geopoint2_p->latitude)) * pow(sin(delta_lon/2), 2)));
	double c = 2 * asin(sqrt(a)); // Inverse haversine
  return dist = R * c;
}  // end of haversineDistance

double
vincentyDistance(
	GeoPoint2D *geopoint1_p,
	GeoPoint2D *geopoint2_p
){
  // WGS-84 ellipsoid params
  double a = 6378137;
  double b = 6356752.314245;
  double f = 1/298.257223563;
  double L = deg2Rad(geopoint2_p->longitude-geopoint1_p->longitude);
  double U1 = atan((1-f) * tan(deg2Rad(geopoint1_p->latitude)));
  double U2 = atan((1-f) * tan(deg2Rad(geopoint2_p->latitude)));
  double sinU1 = sin(U1);
  double cosU1 = cos(U1);
  double sinU2 = sin(U2);
  double cosU2 = cos(U2);
  
  double lambda = L;
  double lambdaP;
  unsigned int iterLimit = 100;
  double sinLambda, cosLambda, sinSigma, cosSigma, sigma, sinAlpha, cosSqAlpha, cos2SigmaM, C;
  double uSq, A, B, deltaSigma, s;
  do {
    sinLambda = sin(lambda);
    cosLambda = cos(lambda);
    sinSigma = sqrt((cosU2*sinLambda) * (cosU2*sinLambda) + 
      (cosU1*sinU2-sinU1*cosU2*cosLambda) * (cosU1*sinU2-sinU1*cosU2*cosLambda));
    if (sinSigma==0) { // co-incident points
      return 0;
    }
    cosSigma = sinU1*sinU2 + cosU1*cosU2*cosLambda;
    sigma = atan2(sinSigma, cosSigma);
    sinAlpha = cosU1 * cosU2 * sinLambda / sinSigma;
    cosSqAlpha = 1 - sinAlpha*sinAlpha;
    cos2SigmaM = cosSigma - 2*sinU1*sinU2/cosSqAlpha;
    // if (_isnan(cos2SigmaM)) cos2SigmaM = 0;  // equatorial line: cosSqAlpha=0 (?)
    C = f/16*cosSqAlpha*(4+f*(4-3*cosSqAlpha));
    lambdaP = lambda;
    lambda = L + (1-C) * f * sinAlpha *
      (sigma + C*sinSigma*(cos2SigmaM+C*cosSigma*(-1+2*cos2SigmaM*cos2SigmaM)));
  } while (fabs(lambda-lambdaP) > 1e-12 && --iterLimit>0);

  if (iterLimit==0) return -1;  // formula failed to converge

  uSq = cosSqAlpha * (a*a - b*b) / (b*b);
  A = 1 + uSq/16384*(4096+uSq*(-768+uSq*(320-175*uSq)));
  B = uSq/1024 * (256+uSq*(-128+uSq*(74-47*uSq)));
  deltaSigma = B*sinSigma*(cos2SigmaM+B/4*(cosSigma*(-1+2*cos2SigmaM*cos2SigmaM)-
    B/6*cos2SigmaM*(-3+4*sinSigma*sinSigma)*(-3+4*cos2SigmaM*cos2SigmaM)));
  s = b*A*(sigma-deltaSigma);

  return s;
  // Note: to return initial/final bearings in addition to distance, use something like:
  //double initialBearing = rad2Deg(atan2(cosU2*sinLambda, cosU1*sinU2-sinU1*cosU2*cosLambda));
  //double finalBearing = rad2Deg(atan2(cosU1*sinLambda, -sinU1*cosU2+cosU1*sinU2*cosLambda));
}

void
wgs84ToCartesian(
	GeoPoint2D	*geopoint_ref_p,
	GeoPoint2D	*geopoint_p,
	Point2D		*point_p
){
  point_p->x = (geopoint_p->longitude - geopoint_ref_p->longitude)*lengthOfDegreeLongitude(geopoint_ref_p->latitude);
  point_p->y = (geopoint_p->latitude - geopoint_ref_p->latitude)*lengthOfDegreeLatitude(geopoint_ref_p->latitude);
}

void
cartesianToWgs84(
GeoPoint2D	*geopoint_ref_p,
Point2D		*point_p,
GeoPoint2D	*geopoint_p
){
  geopoint_p->latitude = geopoint_ref_p->latitude + point_p->y/lengthOfDegreeLatitude(geopoint_ref_p->latitude);
  geopoint_p->longitude = geopoint_ref_p->longitude + point_p->x/lengthOfDegreeLongitude(geopoint_ref_p->latitude);
}

void
wgs84ToCartesianHaversine(
	GeoPoint2D	*geopoint_ref_p,
	GeoPoint2D	*geopoint_p,
	Point2D		*point_p
){
	GeoPoint2D tmp_point;
	tmp_point.latitude = geopoint_ref_p->latitude;
	tmp_point.longitude = geopoint_p->longitude;
	point_p->x = haversineDistance(geopoint_ref_p, &tmp_point); // Haversine
	if (geopoint_ref_p->longitude > geopoint_p->longitude) point_p->x = -point_p->x;
	tmp_point.latitude = geopoint_p->latitude;
	tmp_point.longitude = geopoint_ref_p->longitude;
	point_p->y = haversineDistance(geopoint_ref_p, &tmp_point); // Haversine
	if (geopoint_ref_p->latitude > geopoint_p->latitude) point_p->y = -point_p->y;
}

void
cartesianToWgs84Haversine(
	GeoPoint2D	*geopoint_ref_p,
	Point2D		*point_p,
	GeoPoint2D	*geopoint_p
){
	double dist = sqrt(pow(point_p->x, 2) + pow(point_p->y, 2));
	double theta = deg2Rad(90 - rad2Deg(atan2(point_p->y, point_p->x)));
	// Reverse Haversine
	geopoint_p->latitude = asin(sin(deg2Rad(geopoint_ref_p->latitude)) * cos(dist/R) + cos(deg2Rad(geopoint_ref_p->latitude)) * sin(dist/R) * cos(theta));
	geopoint_p->latitude = rad2Deg(geopoint_p->latitude);
	geopoint_p->longitude = deg2Rad(geopoint_ref_p->longitude) + atan2((sin(theta) * sin(dist/R) * cos(deg2Rad(geopoint_ref_p->latitude))), (cos(dist/R) - sin(deg2Rad(geopoint_ref_p->latitude)) * sin(deg2Rad(geopoint_p->latitude))));
	geopoint_p->longitude = rad2Deg(geopoint_p->longitude);
}

void
wgs84ToClarke1880(
	GeoPoint2D	*geopoint_p,
	Point2D		*point_p
){
	// Molodensky Datum transformation: geodetic to cartesian
	// Ellipsoid of Clarke (1880) -> see parameters here: http://en.wikipedia.org/wiki/Figure_of_the_Earth 
	double a = 6378249.145; // Equatorial radius -> semi-major earth axis [m]
	double b = 6356514.870; // b = a*(1-f); // Polar radius -> semi-minor earth radius [m]
	double f = (a - b) / a; // f = 1/293.465; // Flattening
	double e1 = (2 * f) - pow(f, 2); // e1 = (a*a-b*b)/(a*a); // First eccentricity of the ellipsoid squared

	double lat = deg2Rad(geopoint_p->latitude);
	double lon = deg2Rad(geopoint_p->longitude);
	double height = 0.0; // Height above the ellipsoid

	double N = a/sqrt(1 - e1 * pow(sin(lat), 2)); // Radius of curvature in prime vertical
	double z;
	point_p->x = (N + height) * cos(lat) * cos(lon);
	point_p->y = (N + height) * cos(lat) * sin(lon);
	z = (N * (1 - e1) + height) * sin(lat);
}

void
clarke1880ToWgs84(
	Point2D		*point_p,
	GeoPoint2D	*geopoint_p
){
	// Molodensky Datum transformation: cartesian to geodetic
	// Ellipsoid of Clarke (1880) -> see parameters here: http://en.wikipedia.org/wiki/Figure_of_the_Earth 
	double a = 6378249.145; // Equatorial radius -> semi-major earth axis [m]
	double b = 6356514.870; // b = a*(1-f); // Polar radius -> semi-minor earth radius [m]
	double f = (a - b) / a; // f = 1/293.465; // Flattening
	double e1 = (2 * f) - pow(f, 2); // e1 = (a*a-b*b)/(a*a); // First eccentricity of the ellipsoid squared

	double p = sqrt(pow(point_p->x, 2) + pow(point_p->y, 2));
	double height, z = 4832647.89238; // meters?
	double theta = 0.0; //atan(z*a/p*b);
	double e2 = (a*a-b*b)/(b*b); // Second eccentricity of the ellipsoid squared

	geopoint_p->latitude = rad2Deg(atan((z + e2 * b * pow(sin(theta), 3)) / (p - e1 * a * pow(cos(theta), 3))));
	geopoint_p->longitude = rad2Deg(atan2(point_p->y, point_p->x)); 
	height = (p / cos(deg2Rad(geopoint_p->latitude))) - deg2Rad(geopoint_p->longitude);
}

Point2D 
bezierPoint2D(
	Point2D* points_p,
	unsigned int number_of_points,
	double t
){
	Point2D point; // Point on curve at parameter t
	// By default the point is at (0,0)
	point.x = 0.0;
	point.y = 0.0;
	
	if (number_of_points > 2) {
		unsigned int degree, i, j;
		Point2D *tmp_points_p; // Local copy of control points
		// Copy array
		tmp_points_p = (Point2D*)malloc((unsigned)((number_of_points)*sizeof(Point2D)));
		
		degree = number_of_points-1;
		for (i = 0; i <= degree; i++) {
			tmp_points_p[i].x = points_p[i].x;
			tmp_points_p[i].y = points_p[i].y;
		}
		
		// Triangle computation
		for (i = 1; i <= degree; i++) {	
			for (j = 0; j <= degree-i; j++) {
				tmp_points_p[j].x = (1.0 - t) * tmp_points_p[j].x + t * tmp_points_p[j+1].x;
				tmp_points_p[j].y = (1.0 - t) * tmp_points_p[j].y + t * tmp_points_p[j+1].y;
			}
		}
		
		point.x = tmp_points_p[0].x;
		point.y = tmp_points_p[0].y;
		free((void *)tmp_points_p);
	}
	return point;
}

Point2D 
cubicBezierPoint2D(
	Point2D* points_p,
	double t
){
	Point2D point; // Point on curve at parameter t

	double cx = 3 * (points_p[1].x - points_p[0].x);
	double bx = 3 * (points_p[2].x - points_p[1].x) - cx;
	double ax = points_p[3].x - points_p[0].x - cx - bx;
	double cy = 3 * (points_p[1].y - points_p[0].y);
	double by = 3 * (points_p[2].y - points_p[1].y) - cy;
	double ay = points_p[3].y - points_p[0].y - cy - by;
	double t_squared = t * t;
	double t_cubed = t_squared * t;
	point.x = (ax * t_cubed) + (bx * t_squared) + (cx * t) + points_p[0].x;
	point.y = (ay * t_cubed) + (by * t_squared) + (cy * t) + points_p[0].y;

	return point;
}

Point2D 
cubicBezierDerivative(
	Point2D* points_p,
	double t
){
	Point2D point; // Point on curve at parameter t

	double t_squared = t * t;
	double s0 = -3 + 6 * t - 3 * t_squared;
	double s1 = 3 - 12 * t + 9 * t_squared;
	double s2 = 6 * t - 9 * t_squared;
	double s3 = 3 * t_squared;
	point.x = points_p[0].x * s0 + points_p[1].x * s1 + points_p[2].x * s2 + points_p[3].x * s3;
	point.y = points_p[0].y * s0 + points_p[1].y * s1 + points_p[2].y * s2 + points_p[3].y * s3;

	return point;
}

Point2D 
cubicBezierSecondDerivative(
	Point2D* points_p,
	double t
){
	Point2D point; // Point on curve at parameter t

	double s0 = 6 - 6 * t;
	double s1 = -12 + 18 * t;
	double s2 = 6 - 18 * t;
	double s3 = 6 * t;
	point.x = points_p[0].x * s0 + points_p[1].x * s1 + points_p[2].x * s2 + points_p[3].x * s3;
	point.y = points_p[0].y * s0 + points_p[1].y * s1 + points_p[2].y * s2 + points_p[3].y * s3;

	return point;
}

double 
cubicBezierCurvature(
	Point2D* points_p,
	double t
){
	Point2D d1 = cubicBezierDerivative(points_p, t);
	Point2D d2 = cubicBezierSecondDerivative(points_p, t);
	double r1 = sqrt(pow(d1.x * d1.x + d1.y * d1.y, 3));
	double r2 = fabs(d1.x * d2.y - d2.x * d1.y);
	return r2/r1;
}

double 
cubicBezierHeading(
	Point2D* points_p,
	double t
){
	Point2D d1 = cubicBezierDerivative(points_p, t);
	return atan2(d1.y, d1.x);
}


bool
normalizedCurveParametrization(
	Point2D* points_p,
	unsigned int number_of_points,
	double *t_p,
	bool use_chord_length
){
	unsigned int i;
	double t, step;

	if (use_chord_length == true) {
		// ChordLength
		t_p[0] = 0.0;
		for (i = 1; i < number_of_points; i++) {
			t_p[i] = pointsDistance(&points_p[i-1], &points_p[i]) + t_p[i-1];
		}
		if (t_p[number_of_points-1] <= 0.0) {
			return false;
		}
		for (i = 0; i < number_of_points; i++) {
			t_p[i] /= t_p[number_of_points-1];
		}
	}
	else {	
		// Uniform
		i = 1;
		step = 1.0/(number_of_points - 1);
		for (t = step; t <= 1.0+FLT_EPSILON; t+=step) {
			t_p[i] = t;
			i++;
		}
	}

	return true;
}

bool
cubicBezierFit(
	Point2D* points_p,
	unsigned int number_of_points,
	Point2D* bezier_p
){
	unsigned int i;
	double* t_p;

	for (i = 0; i < 4; i++) {
		bezier_p[i].x = 0.0;
		bezier_p[i].y = 0.0;
	}

	t_p = (double*)malloc((unsigned)((number_of_points)*sizeof(double)));
	if (normalizedCurveParametrization(points_p, number_of_points, t_p, false) == true) {
		bezier_p[0].x = points_p[0].x;
		bezier_p[0].y = points_p[0].y;
		bezier_p[3].x = points_p[number_of_points-1].x;
		bezier_p[3].y = points_p[number_of_points-1].y;
		if (number_of_points == 1) { // if only one value in p
			bezier_p[1].x = bezier_p[0].x;
			bezier_p[1].y = bezier_p[0].y;
			bezier_p[2].x = bezier_p[0].x;
			bezier_p[2].y = bezier_p[0].y;
		}
		else if (number_of_points == 2) { // if only two values in p
			bezier_p[1].x = bezier_p[0].x;
			bezier_p[1].y = bezier_p[0].y;
			bezier_p[2].x = bezier_p[3].x;
			bezier_p[2].y = bezier_p[3].y;
		}
		else if (number_of_points == 3) { // if only two values in p
			bezier_p[1].x = points_p[1].x;
			bezier_p[1].y = points_p[1].y;
			bezier_p[2].x = points_p[1].x;
			bezier_p[2].y = points_p[1].y;
		}
		else {
			double DENOM;
			double A1 = 0.0, A2 = 0.0, A12 = 0.0; // Initialization
			Point2D C1, C2;
			C1.x = 0.0;
			C1.y = 0.0;
			C2.x = 0.0;
			C2.y = 0.0;
			for (i = 1; i < number_of_points-1; i++) {
				double B0 = pow((1-t_p[i]), 3); // Bezier Basis
				double B1 = (3*t_p[i]*pow((1-t_p[i]), 2));
				double B2 = (3*pow(t_p[i], 2)*(1-t_p[i]));
				double B3 = pow(t_p[i], 3);
				A1  = A1 + pow(B1,2);
				A2  = A2 + pow(B2,2);
				A12 = A12 + B1*B2;
				C1.x = C1.x + B1*(points_p[i].x - B0*bezier_p[0].x - B3*bezier_p[3].x);
				C1.y = C1.y + B1*(points_p[i].y - B0*bezier_p[0].y - B3*bezier_p[3].y);	
				C2.x = C2.x + B2*(points_p[i].x - B0*bezier_p[0].x - B3*bezier_p[3].x);
				C2.y = C2.y + B2*(points_p[i].y - B0*bezier_p[0].y - B3*bezier_p[3].y);
			}
			DENOM = ((A1*A2)-(A12*A12)); // Common denominator for all points
			if (DENOM == 0) {
				bezier_p[1].x = bezier_p[0].x;
				bezier_p[1].y = bezier_p[0].y;
				bezier_p[2].x = bezier_p[3].x;
				bezier_p[2].y = bezier_p[3].y;
			}
			else {
				bezier_p[1].x = (A2*C1.x - A12*C2.x)/DENOM;
				bezier_p[1].y = (A2*C1.y - A12*C2.y)/DENOM;
				bezier_p[2].x = (A1*C2.x - A12*C1.x)/DENOM;
				bezier_p[2].y = (A1*C2.y - A12*C1.y)/DENOM;
			}
		}
	}
	else {
		return false;
	}

	free((void *)t_p);
	return true;
}

