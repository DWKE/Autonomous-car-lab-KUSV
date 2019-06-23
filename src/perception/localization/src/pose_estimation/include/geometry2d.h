 /**
 * @copyright Valeo Driving Assistance Research (c) 2014.
 * <br>All rights reserved. Subject to limited distribution and restricted disclosure only.
 * @author paulo.resende@valeo.com
 * @file geometry2d.h
 * @brief Geometry 2D header file.
 * @version 1.0
 * @date 03-03-2014
 * @bug	No known bugs
 * @warning	No warnings
 */

#ifndef __GEOMETRY2D_H__
#define __GEOMETRY2D_H__

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------
** Includes
*/
#include <stdio.h> /* printf */
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <float.h>

#ifdef WIN32
#include "stdbool.h"
#else
#include <stdbool.h>
#endif

/*---------------------------------------------------------------------------
** Defines and Macros
*/
#ifndef max
	/**
	 * @def max
	 * Calculates the maximum of a and b
	 */
	#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
	/**
	 * @def min
	 * Calculates the minimum of a and b
	 */
	#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef PI
	/**
	 * @def PI
	 * Approximation of the mathematical constant Pi
	 */
	#define PI 3.1415926535897932384626433832795
#endif

#ifndef R
	/**
	 * @def R
	 * Earth's mean radius in meters
	 */
	#define R 6367449.1 //6371009.0  
#endif

/*---------------------------------------------------------------------------
** Typedefs
*/
/**
 * @struct GeoPoint2D
 * @brief GeoPoint2D structure
 */
typedef struct {
	double latitude;  /** @var double latitude */
	double longitude;  /** @var double longitude */
} GeoPoint2D;

/**
 * @struct Point2D
 * @brief Point2D structure
 */
typedef struct {
	double x;  /** @var double x */
	double y;  /** @var double y */
} Point2D;

/**
 * @struct Segment2D
 * @brief Segment2D structure
 */
typedef struct {
	Point2D point[2];
} Segment2D;

/**
 * @struct Circle
 * @brief Circle structure
 */
typedef struct {
	Point2D point;
	/**
	 * @var double radius
	 * Circle radius
	 */
	double radius;
} Circle;

/*---------------------------------------------------------------------------
** Functions
*/
/**
 * @fn void rotatePointAroundOrigin(Point2D *point_p, double rad)
 * @brief Rotate a vertex using the origin as pivot
 * @param point_p Pointer to a Point2D structure
 * @param rad Angle expressed in radians
 */
void
rotatePointAroundOrigin(
	Point2D	*point_p,
	double	rad
);

/**
 * @fn void rotatePointAroundPivot(Point2D *point_p, Point2D *pivot_point_p, double rad)
 * @brief Rotate a point using another point as pivot
 * @param point_p Pointer to a Point2D structure
 * @param pivot_point_p Pivot Point2D pointer
 * @param rad Angle expressed in radians
 */
void
rotatePointAroundPivot(
	Point2D	*point_p,
	Point2D	*pivot_point_p,
	double	rad
);

/**
 * @fn double pointsSquareDistance(Point2D *point1_p, Point2D *point2_p)
 * @brief Calculates the square distance between two points
 * @param p1_p Point2D 1
 * @param p2_p Point2D 2
 * @return dist
 */
double
pointsSquareDistance(
	Point2D *p1_p,
	Point2D *p2_p
);

/**
 * @fn double pointsDistance(Point2D *p1_p, Point2D *p2_p)
 * @brief Calculates the distance between two points
 * @param p1_p Point2D 1
 * @param p2_p Point2D 2
 * @return dist
 */
double
pointsDistance(
	Point2D	*p1_p,
	Point2D	*p2_p
);

/**
 * @fn double dotProduct(Segment2D *s1_p, Segment2D *s2_p)
 * @brief Determines the dot product between 2 line segments 
 * @param s1_p Segment2D 1
 * @param s2_p Segment2D 2
 * @return product
 */
double
dotProduct(
	Segment2D *s1_p,
	Segment2D *s2_p
);

/**
 * @fn double crossProduct(Segment2D *s1_p, Segment2D *s2_p)
 * @brief Determines the cross product between 2 line segments 
 * @param s1_p Segment2D 1
 * @param s2_p Segment2D 2
 * @return product
 */
double
crossProduct(
	Segment2D *s1_p,
	Segment2D *s2_p
);

/**
 * @fn double segmentsAngle(Segment2D *s1_p, Segment2D *s2_p)
 * @brief Determines the angle in radians between 2 line segments 
 * @param s1_p Segment2D 1
 * @param s2_p Segment2D 2
 * @return angle
 */
double
segmentsAngle(
	Segment2D *s1_p,
	Segment2D *s2_p
);

/**
 * @fn double distancePointSegment(Point2D *point_p, Segment2D *segment_p, Point2D *closest_point_p)
 * @brief Calculates the distance between a point and a line segment
 * @param point_p Point2D
 * @param segment_p Line
 * @return closest_point_p Closest point in the line segment
 */
double
distancePointSegment(
	Point2D		*point_p,
	Segment2D	*segment_p,
	Point2D		*closest_point_p
);

/**
 * @fn double segmentsDistance(Segment2D *s1_p, Segment2D *s2_p, Point2D *point_p)
 * @brief Calculates the minimum distance between 2 line segments 
 * @param s1_p Segment2D 1
 * @param s2_p Segment2D 2
 * @return dist
 * @return point_p Intersection point
 */
double
segmentsDistance(
	Segment2D	*s1_p,
	Segment2D	*s2_p,
	Point2D		*point_p
);

/**
 * @fn bool segmentsIntersection(Segment2D *s1_p, Segment2D *s2_p, Point2D *point_p)
 * @brief Computes the intersection between two line segments
 * @param s1_p Segment2D 1
 * @param s2_p Segment2D 2
 * @return point_p Intersection point
 */
bool
segmentsIntersection(
	Segment2D	*s1_p,
	Segment2D	*s2_p,
	Point2D		*point_p
);

/**
 * @fn double distancePointLine(Point2D *point_p, Segment2D *segment_p, Point2D *closest_point_p)
 * @brief Calculates the distance between a point and a line
 * @param point_p Point2D
 * @return segment_p Line
 * @return closest_point_p Closest point in the line
 */
double
distancePointLine(
	Point2D		*point_p,
	Segment2D	*segment_p,
	Point2D		*closest_point_p
);

/**
 * @fn bool isPerpendicular(Point2D *point1_p, Point2D *point2_p, Point2D *point3_p)
 * @brief Checks if the given points are perpendicular to x or y axis 
 * @param point1_p Point2D 1
 * @param point2_p Point2D 2
 * @param point3_p Point2D 3
 */
bool
isPerpendicular(
	Point2D *point1_p,
	Point2D *point2_p,
	Point2D *point3_p
);

/**
 * @fn void circleFit(Point2D *point1_p, Point2D *point2_p, Point2D *point3_p, Circle *circle_p)
 * @brief Determine a circle from 3 Points
 * @param point1_p Point2D 1
 * @param point2_p Point2D 2
 * @param point3_p Point2D 3
 * @return circle_p Circle
 */
void
circleFit(
	Point2D *point1_p,
	Point2D *point2_p,
	Point2D *point3_p,
	Circle *circle_p
);

/**
 * @fn double interiorAngle(Point2D *point1_p, Point2D *point2_p, Point2D *point3_p)
 * @brief Determines the interior angle between the vector (point1_p -> point2_p) and the vector (point1_p -> point3_p)
 * @param point1_p Point2D point 1
 * @param point2_p Point2D point 2
 * @param point3_p Point2D point 3
 * @return angle
 */
double
interiorAngle(
	Point2D *point1_p,
	Point2D *point2_p,
	Point2D *point3_p
);

/**
 * @fn double polygonArea(Point2D *points_p, unsigned int number_of_points)
 * @brief Determine area of a polygon
 * @param points_p Array of Point2D points
 * @param number_of_points Number of points
 * @return area Area
 */
double
polygonArea(
	Point2D *points_p,
	unsigned int number_of_points
);

/**
 * @fn Point2D polygonCentroid(Point2D *points_p, unsigned int number_of_points)
 * @brief Determine centroid of a polygon
 * @detail Points must be in clockwise or counterclockwise order.
           The area of the polygon must be non-zero.
		   The polygon must not be self-intersecting (simple).
		   The polygon may or may not be convex.
 * @param points_p Array of Point2D points of polygon
 * @param number_of_points Number of points of polygon
 * @return centroid Centroid
 */
Point2D
polygonCentroid(
	Point2D *points_p,
	unsigned int number_of_points
);

/**
 * @fn bool pointInPolygon(Point2D *points_p, unsigned int number_of_points, Point2D *point_p)
 * @brief Determine if a point is inside a polygon
 * @param points_p Array of Point2D points of polygon
 * @param number_of_points Number of points of polygon
 * @param point_p Point2D point to be tested
 * @return is_inside True is is inside or false otherwise. May return true or false if the point is exactly on the edge of the polygon
 */
bool
pointInPolygon(
	Point2D *points_p,
	unsigned int number_of_points,
	Point2D *point_p
);

/**
 * @fn double polygonsDistance(Point2D *points1_p, unsigned int number_of_points1, Point2D *points2_p, unsigned int number_of_points2)
 * @brief Determine distance between two polygons
 * @param points1_p Array of Point2D points of polygon 1
 * @param number_of_points1 Number of points of polygon 1
 * @param points2_p Array of Point2D points of polygon 2
 * @param number_of_points2 Number of points of polygon 2
 * @return minimum_distance Distance
 */
double
polygonsDistance(
	Point2D *points1_p,
	unsigned int number_of_points1,
	Point2D *points2_p,
	unsigned int number_of_points2
);

/**
 * @fn double deg2Rad(double angle)
 * Convert decimal degrees to radians
 * @param angle Angle in decimal degrees
 * @return angle Angle in radians
 */
double
deg2Rad(
	double angle
);

/**
 * @fn double deg2Rad(double angle)
 * Convert radians to decimal degrees
 * @param angle Angle in radians
 * @return angle Angle in decimal degrees
 */
double
rad2Deg(
	double angle
);

/**
 * @fn double normalizeAngle(double angle)
 * Normalize angle to interval [-PI, PI[ by adding/subtracting multiples of 2*PI
 * @param angle Angle in radians
 * @return angle Normalized angle in radians
 */
double
normalizeAngle(
	double angle
);

/**
 * @fn double lengthOfDegreeLatitude(double ref_latitude)
 * Determine the length of a latitude degree in meters
 * @param ref_latitude Reference latitude in degrees
 * @return latlen
 */
double
lengthOfDegreeLatitude(
	double ref_latitude
);

/**
 * @fn double lengthOfDegreeLongitude(double ref_latitude)
 * Determine the length of a longitude degree in meters
 * @param ref_latitude Reference latitude in degrees
 * @return longlen
 */
double
lengthOfDegreeLongitude(
	double ref_latitude
);

/**
 * @fn double haversineDistance(GeoPoint2D *geopoint1_p, GeoPoint2D *geopoint2_p)
 * Calculates great-circle distances in meters between the two geographical points ?that is, the shortest distance over the earth’s surface ?using the Haversine formula.
 * It assumes a spherical earth, ignoring ellipsoidal effects, which is accurate enough for most purposes.
 * @param geopoint1_p First point in decimal degrees
 * @param geopoint2_p Second point in decimal degrees
 * @returns (Number} dist
 */
double
haversineDistance(
	GeoPoint2D *geopoint1_p,
	GeoPoint2D *geopoint2_p
);

/**
 * @fn double vincentyDistance(GeoPoint2D *geopoint1_p, GeoPoint2D *geopoint2_p)
 * Calculates geodetic distance between two points specified by latitude/longitude using 
 * Vincenty inverse formula for ellipsoids
 * @param geopoint1_p First point in decimal degrees
 * @param geopoint2_p Second point in decimal degrees
 * @returns (Number} dist
 */
double
vincentyDistance(
	GeoPoint2D *geopoint1_p,
	GeoPoint2D *geopoint2_p
);

/**
 * @fn void wgs84ToCartesian(GeoPoint2D *geopoint_ref_p, GeoPoint2D *geopoint_p, Point2D *point_p)
 * @param geopoint_p Reference point in decimal degrees
 * @param geopoint_p Point in decimal degrees to be transformed 
 * @returns point_p
 */
void
wgs84ToCartesian(
	GeoPoint2D	*geopoint_ref_p,
	GeoPoint2D	*geopoint_p,
	Point2D		*point_p
);

/**
 * @fn void cartesianToWgs84(GeoPoint2D *geopoint_ref_p, Point2D *point_p, GeoPoint2D *geopoint_p)
 * @param geopoint_p Reference point in decimal degrees
 * @param point_p Point in meters to be transformed 
 * @returns geopoint_p
 */
void
cartesianToWgs84(
	GeoPoint2D	*geopoint_ref_p,
	Point2D		*point_p,
	GeoPoint2D	*geopoint_p
);

/**
 * @fn void wgs84ToCartesianHaversine(GeoPoint2D *geopoint_ref_p, GeoPoint2D *geopoint_p, Point2D *point_p)
 * @param geopoint_p Reference point in decimal degrees
 * @param geopoint_p Point in decimal degrees to be transformed 
 * @returns point_p
 */
void
wgs84ToCartesianHaversine(
	GeoPoint2D	*geopoint_ref_p,
	GeoPoint2D	*geopoint_p,
	Point2D		*point_p
);

/**
 * @fn void cartesianToWgs84(GeoPoint2D *geopoint_ref_p, Point2D *point_p, GeoPoint2D *geopoint_p)
 * @param geopoint_p Reference point in decimal degrees
 * @param point_p Point in meters to be transformed 
 * @returns geopoint_p
 */
void
cartesianToWgs84Haversine(
	GeoPoint2D	*geopoint_ref_p,
	Point2D		*point_p,
	GeoPoint2D	*geopoint_p
);

/**
 * @fn void wgs84ToClarke1880(GeoPoint2D *geopoint_p, Point2D *point_p)
 * @param geopoint_p Reference point in decimal degrees
 * @returns point_p
 */
void
wgs84ToClarke1880(
	GeoPoint2D	*geopoint_p,
	Point2D		*point_p
);

/**
 * @fn void clarke1880ToWgs84(Point2D *point_p, GeoPoint2D *geopoint_p)
 * @param point_p Point in meters
 * @returns geopoint_p
 */
void
clarke1880ToWgs84(
	Point2D *point_p,
	GeoPoint2D *geopoint_p
);

/**
 * @fn Point2D bezierPoint2D(Point2D* points_p, unsigned int number_of_points, double t);
 * @brief Determine a generalized Bezier curve point given a particular parameter value t
 * @param points_p Array of Point2D control points
 * @param number_of_points Number of control points
 * @param t parameter value: 0 <= t <= 1
 * @returns point Point on curve at parameter t
 */
Point2D 
bezierPoint2D(
	Point2D* points_p,
	unsigned int number_of_points,
	double t
);

/**
 * @fn Point2D cubicBezierPoint2D(Point2D* points_p, unsigned int number_of_points, double t);
 * @brief Determine a cubic Bezier curve point given a particular parameter value t
 * @param points_p Array of Point2D control points
 * @param t parameter value: 0 <= t <= 1
 * @returns point Point on curve at parameter t
 */
Point2D 
cubicBezierPoint2D(
	Point2D* points_p,
	double t
);

/**
 * @fn Point2D cubicBezierDerivative(Point2D* points_p, unsigned int, double t);
 * @brief Determine a Bezier curve point derivative given a particular parameter value t
 * @param points_p Array of 4 Point2D control points 
 * @param t parameter value: 0 <= t <= 1
 * @returns point Point derivative on curve at parameter t
 */
Point2D 
cubicBezierDerivative(
	Point2D* points_p,
	double t
);

/**
 * @fn Point2D cubicBezierSecondDerivative(Point2D* points_p, double t);
 * @brief Determine a Bezier curve point second derivative given a particular parameter value t
 * @param points_p Array of 4 Point2D control points 
 * @param t parameter value: 0 <= t <= 1
 * @returns point Point second derivative on curve at parameter t
 */
Point2D 
cubicBezierSecondDerivative(
	Point2D* points_p,
	double t
);

/**
 * @fn double cubicBezierCurvature(Point2D* points_p, double t);
 * @brief Determine a Bezier curve curvature given a particular parameter value t
 * @param points_p Array of 4 Point2D control points 
 * @param t parameter value: 0 <= t <= 1
 * @returns point Point curvature on curve at parameter t
 */
double 
cubicBezierCurvature(
	Point2D* points_p,
	double t
);

/**
 * @fn double cubicBezierHeading(Point2D* points_p, unsigned int, double t);
 * @brief Determine a Bezier curve point heading given a particular parameter value t
 * @param points_p Array of 4 Point2D control points 
 * @param t parameter value: 0 <= t <= 1
 * @returns point Point heading on curve at parameter t
 */
double 
cubicBezierHeading(
	Point2D* points_p,
	double t
);

/**
 * @fn bool normalizedCurveParametrization(Point2D* points_p, unsigned int number_of_points, double *t_p, bool use_chord_length);
 * @brief Normalized curve parameterization, domain is between 0 and 1 inclusive
 * @param points_p Array of Point2D points
 * @param number_of_points Number of points
 * @param t_p Parametrized values of t
 * @returns bool True is success and false if failed
 */
bool
normalizedCurveParametrization(
	Point2D* points_p,
	unsigned int number_of_points,
	double *t_p,
	bool use_chord_length
);

/**
 * @fn bool cubicBezierFit(Point2D* points_p, unsigned int number_of_points, Point2D* bezier_p);
 * @brief Fits a 3rd order Bezier curve to a given set of input points
 * @param points_p Array of Point2D sample points
 * @param number_of_points Number of points
 * @param bezier_p Array of Point2D control points
 * @returns bool True is success and false if failed
 */
bool
cubicBezierFit(
	Point2D* points_p,
	unsigned int number_of_points,
	Point2D* bezier_p
);

#ifdef __cplusplus
}
#endif

#endif  /* __GEOMETRY2D_H__ */
/*---------------------------------------------------------------------------
** End of File
*/