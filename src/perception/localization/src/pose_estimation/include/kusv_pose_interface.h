
#ifndef __KUSV_COMMON_H__
#define __KUSV_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------
** Includes
*/
#include <string.h> // memset
#include <stdint.h>

/*---------------------------------------------------------------------------
** Defines and Macros
*/
#ifdef _MSC_VER
  #define INLINE __forceinline /**< Use __forceinline (VC++ specific). */
#else
  #define INLINE inline       /**< Use standard inline. */
#endif
#ifndef DBL_EPSILON 
	#define DBL_EPSILON 2.2204460492503131e-16 /**< Double epsilon value. */
#endif
#ifndef DBL_MAX
	#define DBL_MAX 1.7976931348623158e+308 /**< Double maximum positive value. */
#endif
#ifndef DBL_MIN
	#define DBL_MIN 2.2250738585072014e-308 /**< Double minimum positive value. */
#endif
#ifndef FLT_MAX
	#define FLT_MAX 3.402823466e+38F /**< Float maximum positive value. */
#endif
#ifndef FLT_MIN
	#define FLT_MIN 1.175494351e-38F /**< Float minimum positive value. */
#endif
#ifndef TRUE
	#define TRUE 1
#endif
#ifndef FALSE
	#define FALSE 0
#endif
/** Approximation of the mathematical constant \a Pi. */
#define KUSV_PI						3.1415926535897932384626433832795
/** Returns the minimum between \a a and \a b. */
#define KUSV_MIN(a, b)				((a) > (b) ? (b) : (a))
/** Returns the maximum between \a a and \a b. */
#define KUSV_MAX(a, b)				((a) < (b) ? (b) : (a))
/** Returns the maximum between \a min and the minumum between \a a and \a max. */
#define KUSV_MIN_MAX(min, max, a)	(KUSV_MAX((min), KUSV_MIN((a), (max))))
/** Returns the absolute value of \a a. */
#define KUSV_ABS(a)					((a) >= 0  ? (a) : -(a))
/** Returns the sign value of \a a. */
#define KUSV_SIGN(a)				((a) > 0 ? 1 : -1)
/** Returns the integral value that is nearest to \a a, with halfway cases rounded away from zero. */
#define KUSV_ROUND(a)				(int)((a) > 0 ? ((a) + 0.5) : ((a) - 0.5))
/** Converts decimal degrees to radians. */
#define KUSV_DEG2RAD				(KUSV_PI/180.0)
/** Converts radians to decimal degrees. */
#define KUSV_RAD2DEG				(180.0/KUSV_PI)
/** Returns true if value is odd, or false otherwise. */
#define KUSV_ODD(a)					((a)&1)
/** Returns true if value is even, or false otherwise. */
#define KUSV_EVEN(a)				(!((a)&1))
/** Earth's mean radius in meters. */
#define KUSV_EARTH_RADIUS 			6371000.0

/*---------------------------------------------------------------------------
** Typedefs
*/
/**
 * @defgroup DataTypes Data types definition
 * @{
 */

/** Boolean */
typedef unsigned char			bool_t;
/** plain 8 bit character */
typedef char					char_t;
// /** Unsigned 8 bit integer */
// typedef unsigned char 			uint8_t;
// /** Unsigned 16 bit integer */
// typedef unsigned short 			uint16_t;
// /** Unsigned 32 bit integer */
// typedef unsigned int 			uint32_t;
// /** Unsigned 64 bit integer */
// typedef unsigned long long		uint64_t;
// /** Signed 8 bit integer */
// typedef signed char				int8_t;
// /** Signed 16 bit integer */
// typedef signed short 			int16_t;
// /** Signed 32 bit integer */
// typedef signed int 				int32_t;
// /** Signed 64 bit integer */
// typedef signed long long 		int64_t;
/** 32 bit floating-point */
typedef float					float32_t;
/** 64 bit floating-point */
typedef double					float64_t;

/** @} */

/**
 * @enum KUSV_SURFACE_CLASS
 */
typedef enum {
        KUSV_SCL_UNKNOWN = 0, /**< Unknown. */
        KUSV_SCL_TARMAC, /**< Tarmac. */
        KUSV_SCL_CONCRETE, /**< Concrete. */
        KUSV_SCL_PAVING, /**< Paving. */
        KUSV_SCL_GRAVEL, /**< Gravel. */
        KUSV_SCL_SOIL, /**< Soil. */
        KUSV_SCL_SAND, /**< Sand. */
        KUSV_SCL_WATER /**< Water. */
} KUSV_SURFACE_CLASS;

/**
 * @enum KUSV_TURN_SIGNAL
 */
typedef enum {
        KUSV_TSI_UNKNOWN = 0, /**< Unknown. */
        KUSV_TSI_OFF, /**< Off. */
        KUSV_TSI_RIGHT, /**< Right turn signal is on. */
        KUSV_TSI_LEFT, /**< Left turn signal is on. */
        KUSV_TSI_HAZARD /**< Left and right turn signal are on. */
} KUSV_TURN_SIGNAL;

/** @enum KUSV_STATUS */
typedef enum {
        KUSV_STA_OFF = 0, /**< Off. */
        KUSV_STA_ON, /**< On. */
        KUSV_STA_FAILURE, /**< Failure. */
        KUSV_STA_UNKNOWN /**< Unknown. */
} KUSV_STATUS;

/** @enum KUSV_GNSS_QUALITY */
typedef enum {
        KUSV_NO_FIX = 0, /**< Fix not available. */
        KUSV_FIX = 1, /**< GNSS fix. */
        KUSV_DIFF_FIX = 2, /**< Differential GPS fix (values above 2 are 2.3 features). */
        KUSV_PPS_FIX = 3, /**< PPS fix. */
        KUSV_RTK_INT = 4, /**< Real Time Kinematic. */
        KUSV_RTK_FLOAT = 5, /**< Float RTK. */
        KUSV_DR = 6, /**< Estimated (dead reckoning) . */
        KUSV_MANUAL = 7, /**< Manual input mode. */
        KUSV_SIMULATION = 8, /**< Simulation mode. */
        KUSV_UNKNOWN = 9 /**< Unknown. */
} KUSV_GNSS_QUALITY;

#pragma pack(push)  /* Push current alignment to stack */
#pragma pack(1)     /* Set alignment to 1 byte boundary */

/**
 * @struct KUSV_Gnss
 * @brief Global navigation satellite system data
 */
typedef struct {
	uint16_t			version; /**< Version. */
        KUSV_STATUS			status; /**< Status. */
	uint32_t			timestamp; /**< Timestamp [ms]. */
	float64_t 			latitude; /**< Latitude [deg]. */ 
	float64_t 			longitude; /**< Longitude [deg]. */
	float32_t 			altitude; /**< Orthometric height, mean-sea-level reference when available or 0 [m]. */
	float32_t 			heading; /**< Heading to North [deg]. */
	float32_t 			latitude_sigma; /**< Standard deviation of latitude [m]. */
	float32_t 			longitude_sigma; /**< Standard deviation of longitude [m]. */
	float32_t 			altitude_sigma; /**< Standard deviation of altitude [m]. */
	float32_t 			heading_sigma; /**< Standard deviation of heading [deg]. */
        KUSV_GNSS_QUALITY	quality; /**< Quality indicator. */
	uint32_t 			number_of_satellites; /**< Number of satellites. */
	float32_t 			HDOP; /**< Horizontal Dilution of Precision [m]. */
} KUSV_Gnss;

/**
 * @struct KUSV_Motion
 * @brief Motion data
 */
typedef struct {
	uint16_t		version; /**< Version. */
        KUSV_STATUS		status; /**< Status. */
	uint32_t		timestamp; /**< Timestamp [ms]. */
	float32_t		speed_x; /**< Longitudinal speed [m/s]. */
	float32_t		speed_y; /**< Lateral speed [m/s]. */
	float32_t		speed_z; /**< Vertical speed [m/s]. */
	float32_t		acceleration_x; /**< Longitudinal acceleration  [m/s²]. */
	float32_t		acceleration_y; /**< Lateral acceleration [m/s²]. */
	float32_t		acceleration_z; /**< Vertical acceleration [m/s²]. */
	float32_t		roll_rate; /**< Roll rate. [deg/s]. */
	float32_t		pitch_rate; /**< Pitch rate. [deg/s]. */
	float32_t		yaw_rate; /**< Yaw rate. [deg/s]. */
	float32_t		speed_x_sigma; /**< Standard deviation of speed_x_rel [m/s]. */
	float32_t		speed_y_sigma; /**< Standard deviation of speed_y_rel [m/s]. */
	float32_t		speed_z_sigma; /**< Standard deviation of speed_z_rel [m/s]. */
	float32_t		acceleration_x_sigma; /**< Standard deviation of acceleration_x_rel [m/s^2]. */
	float32_t		acceleration_y_sigma; /**< Standard deviation of acceleration_y_rel [m/s^2]. */	
	float32_t		acceleration_z_sigma; /**< Standard deviation of acceleration_z_rel [m/s^2]. */
	float32_t		roll_rate_sigma; /**< Standard deviation of roll rate. [deg/s]. */
	float32_t		pitch_rate_sigma; /**< Standard deviation of pitch rate. [deg/s]. */
	float32_t		yaw_rate_sigma; /**< Standard deviation of yaw rate. [deg/s]. */
} KUSV_Motion;

#pragma pack(pop)

/*---------------------------------------------------------------------------
** Functions
*/
INLINE void
initGnss(
        KUSV_Gnss *gnss
){
        memset(gnss, 0, sizeof(KUSV_Gnss));
	gnss->version = 1;
	return;
}

INLINE void
initMotion(
        KUSV_Motion *motion
){
        memset(motion, 0, sizeof(KUSV_Motion));
	motion->version = 1;
	return;
}

#ifdef __cplusplus
}
#endif

#endif /* __KUSV_COMMON_H__ */
/*---------------------------------------------------------------------------
** End of File
*/
