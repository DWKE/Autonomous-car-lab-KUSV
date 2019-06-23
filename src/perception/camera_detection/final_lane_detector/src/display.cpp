#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>
#include <math.h>
#include <ros/ros.h>
#include <string>
#include <tf/tf.h>
#include <tf/transform_broadcaster.h>

#include "kusv_msgs/LanePointData.h"
#include "kusv_msgs/LanePointDataArray.h"
#include "kusv_msgs/PolyfitLaneData.h"
#include "kusv_msgs/PolyfitLaneDataArray.h"

#define PI 3.1415926579

class Display {
private:
    ros::NodeHandle m_nh;

    ros::Publisher m_rosPubVehiclesMarker;

    ros::Publisher m_rosPubLanesMarkerArray;
    ros::Subscriber m_rosSubLanePointsArray;

    ros::Publisher m_rosPubLeftPointsMarkerArray;
    ros::Publisher m_rosPubRightPointsMarkerArray;
    ros::Subscriber m_rosSubLanePointLeft;
    ros::Subscriber m_rosSubLanePointRight;

    ros::Publisher m_rosPubPolyMarkerArray;
    ros::Subscriber m_rosSubPolyLaneArray;

    ros::Publisher m_rosPubDrivingWay;
    ros::Subscriber m_rosSubDrivingWay;

    tf::TransformBroadcaster m_rosTfBroadcaster;


private:
    std::string m_sVehicle_id = "vehicle";
    double m_dVehicleX = 0.0;
    double m_dVehicleY = 0.0;
    double m_dVehicleYaw = 0.0;
    bool m_isVehicleExist = true;


private:
    kusv_msgs::LanePointData m_point_left;
    kusv_msgs::LanePointData m_point_right;
    kusv_msgs::LanePointDataArray m_lanePoints;
    kusv_msgs::PolyfitLaneDataArray m_polyLanes;
    kusv_msgs::PolyfitLaneData m_drivingWay;



public:
    Display() {
        m_rosPubVehiclesMarker =
                m_nh.advertise<visualization_msgs::Marker>("vehicle_marker", 10);

        m_rosPubLanesMarkerArray =
                m_nh.advertise<visualization_msgs::MarkerArray>("lanePoints_marker", 10);
        m_rosSubLanePointsArray =
                m_nh.subscribe("points/left_and_right", 10, &Display::lanePointsCallback, this);

        m_rosPubLeftPointsMarkerArray =
                m_nh.advertise<visualization_msgs::MarkerArray>("pointsLeft_marker", 10);
        m_rosSubLanePointLeft =
                m_nh.subscribe("points/left", 10, &Display::pointsLeftCallback, this);

        m_rosPubRightPointsMarkerArray =
                m_nh.advertise<visualization_msgs::MarkerArray>("pointsRight_marker", 10);
        m_rosSubLanePointRight =
                m_nh.subscribe("points/right", 10, &Display::pointsRightCallback, this);

        m_rosPubPolyMarkerArray =
                m_nh.advertise<visualization_msgs::MarkerArray>("polyfit_lanes_marker", 10);
        m_rosSubPolyLaneArray =
                m_nh.subscribe("lane/left_and_right", 10, &Display::polyLanesCallback, this);

        m_rosPubDrivingWay =
                m_nh.advertise<visualization_msgs::MarkerArray>("driving_way_marker", 10);
        m_rosSubDrivingWay =
                m_nh.subscribe("vision_lane", 10, &Display::drivingWayCallback, this);
    }
    ~Display() {}


public:
    void broadcasting_tf_vehicle() {
        tf::Quaternion q;
        q.setRPY(0, 0, 0);

        tf::Transform transform;
        transform.setOrigin(tf::Vector3(0.0, 0.0, 0.0));
        transform.setRotation(q);

        // broadcasting the vehicle's body coordinate system
        // The parent coordinate is world, child coordinate is body.
        m_rosTfBroadcaster.sendTransform(tf::StampedTransform(
                                             transform, ros::Time::now(), "world", m_sVehicle_id));
    }


public:
    void mark_vehicle() {
        if (m_isVehicleExist == true) {
            tf::Quaternion q_temp;
            tf::Matrix3x3 m(q_temp);
            q_temp.setRPY(90.0 / 180.0 * PI, 0, 180.0 / 180.0 * PI + m_dVehicleYaw);
            tf::Quaternion q(q_temp.getX(), q_temp.getY(), q_temp.getZ(),
                             q_temp.getW());

            visualization_msgs::Marker vehicle_marker;
            vehicle_marker.header.frame_id = m_sVehicle_id;
            vehicle_marker.header.stamp = ros::Time::now();
            vehicle_marker.ns = m_sVehicle_id;
            vehicle_marker.id = 0;
            // Set the marker type
            vehicle_marker.type = visualization_msgs::Marker::MESH_RESOURCE;
            vehicle_marker.mesh_resource =
                    "package://final_lane_detector/test_materials/meshes/BMW_X5_4.dae";
            vehicle_marker.mesh_use_embedded_materials = false;

            vehicle_marker.pose.position.x = m_dVehicleX;
            vehicle_marker.pose.position.y = m_dVehicleY;
            vehicle_marker.pose.position.z = 0.0;
            vehicle_marker.pose.orientation.x = q.getX();
            vehicle_marker.pose.orientation.y = q.getY();
            vehicle_marker.pose.orientation.z = q.getZ();
            vehicle_marker.pose.orientation.w = q.getW();
            // Set the scale of the marker
            vehicle_marker.scale.x = 1.0;
            vehicle_marker.scale.y = 1.0;
            vehicle_marker.scale.z = 1.0;

            vehicle_marker.color.r = 1.0;
            vehicle_marker.color.g = 1.0;
            vehicle_marker.color.b = 1.0;
            vehicle_marker.color.a = 1.0;

            vehicle_marker.lifetime = ros::Duration(0.1);

            m_rosPubVehiclesMarker.publish(vehicle_marker);
        }
    }


public:
    void pointsLeftCallback(const kusv_msgs::LanePointData::ConstPtr &msg) {
        m_point_left.id = msg->id;
        m_point_left.point = msg->point;
    }
    void pointsRightCallback(const kusv_msgs::LanePointData::ConstPtr &msg) {
        m_point_right.id = msg->id;
        m_point_right.point = msg->point;
    }
    void lanePointsCallback(const kusv_msgs::LanePointDataArray::ConstPtr &msg) {
        m_lanePoints.id = msg->id;
        m_lanePoints.lane = msg->lane;
    }

    void mark_Points() {
        visualization_msgs::MarkerArray markerArray;

        int id = 0;

        for (auto i_point = 0; i_point < m_point_left.point.size(); i_point++) {
            visualization_msgs::Marker marker;
            marker.header.frame_id = m_sVehicle_id;
            marker.header.stamp = ros::Time::now();

            marker.ns = m_point_left.id;
            marker.id = id++;

            marker.type = visualization_msgs::Marker::SPHERE;
            marker.action = visualization_msgs::Marker::ADD;

            marker.color.r = 1.0f;
            marker.color.g = 1.0f;
            marker.color.b = 0.0f;

            marker.color.a = 0.7;
            marker.scale.x = 0.2;
            marker.scale.x = 0.2;
            marker.scale.x = 0.2;
            marker.lifetime = ros::Duration(0.1);

            markerArray.markers.push_back(marker);
        }
        m_rosPubLeftPointsMarkerArray.publish(markerArray);
    }
    void mark_Lanes() {
        visualization_msgs::MarkerArray markerArray;

        int id = 0;

        for (auto i_lane = 0; i_lane < m_lanePoints.lane.size(); i_lane++) {
            visualization_msgs::Marker marker;
            marker.header.frame_id = m_sVehicle_id;
            marker.header.stamp = ros::Time::now();

            marker.ns = m_lanePoints.lane[i_lane].id;
            marker.id = id++;

            marker.type = visualization_msgs::Marker::SPHERE_LIST;
            marker.action = visualization_msgs::Marker::ADD;

            if (i_lane == 0) {
                marker.color.r = 1.0f;
                marker.color.g = 0.0f;
                marker.color.b = 0.0f;
            }
            else {
                marker.color.r = 0.0f;
                marker.color.g = 1.0f;
                marker.color.b = 0.0f;
            }
            marker.color.a = 0.7;
            marker.scale.x = 0.1;
            marker.scale.x = 0.1;
            marker.scale.x = 0.1;
            marker.lifetime = ros::Duration(0.2);

            geometry_msgs::Point prevPoint;
            bool first = true;

            for (auto i_point = 0; i_point < m_lanePoints.lane[i_lane].point.size();
                 i_point++) {
                geometry_msgs::Point currPoint;
                currPoint.x = m_lanePoints.lane[i_lane].point[i_point].x;
                currPoint.y = m_lanePoints.lane[i_lane].point[i_point].y;
                currPoint.z = 0.0;

                if (first == true) {
                    first = false;
                } else {
                    double dx = currPoint.x - prevPoint.x;
                    double dy = currPoint.y - prevPoint.y;
                    if ((dx * dx + dy * dy) <= 2.0 * 2.0) {
                        marker.points.push_back(prevPoint);
                        marker.points.push_back(currPoint);
                    } else {
                        markerArray.markers.push_back(marker);
                        marker.points.clear();
                        marker.id = id++;
                    }
                }
                prevPoint = currPoint;
            }
            markerArray.markers.push_back(marker);
        }
        m_rosPubLanesMarkerArray.publish(markerArray);
    }


public:
    void polyLanesCallback(const kusv_msgs::PolyfitLaneDataArray::ConstPtr &msg) {
        m_polyLanes.id = msg->id;
        m_polyLanes.polyfitLanes = msg->polyfitLanes;
    }

    void mark_polyLanes(double interval = 0.1, double ROILength = 30.0) {
        visualization_msgs::MarkerArray markerArray;
        for (auto i_lane = 0; i_lane < m_polyLanes.polyfitLanes.size(); i_lane++) {

            double x = 0.0;
            double y = m_polyLanes.polyfitLanes[i_lane].d;

            double distance_square = x * x + y * y;
            ;
            int id = 0;
            while (distance_square < ROILength * ROILength) {
                double a0 = m_polyLanes.polyfitLanes[i_lane].d;
                double a1 = m_polyLanes.polyfitLanes[i_lane].c;
                double a2 = m_polyLanes.polyfitLanes[i_lane].b;
                double a3 = m_polyLanes.polyfitLanes[i_lane].a;

                y = a0 + a1 * x + a2 * x * x + a3 * x * x * x;
                distance_square = x * x + y * y;

                visualization_msgs::Marker marker;
                marker.header.frame_id = m_sVehicle_id;
                marker.header.stamp = ros::Time::now();

                marker.ns = m_polyLanes.polyfitLanes[i_lane].id;
                marker.id = id;

                marker.type = visualization_msgs::Marker::CYLINDER;
                marker.action = visualization_msgs::Marker::ADD;

                marker.pose.position.x = x;
                marker.pose.position.y = y;
                marker.pose.position.z = 0.0;
                marker.pose.orientation.x = 0.0;
                marker.pose.orientation.y = 0.0;
                marker.pose.orientation.z = 0.1;
                marker.pose.orientation.w = 1.0;
                marker.color.r = 0.0f;
                marker.color.g = 1.0f;
                marker.color.b = 0.0f;
                marker.color.a = 1.0;
                marker.scale.x = 0.1;
                marker.scale.y = 0.1;
                marker.scale.z = 0.1;
                marker.lifetime = ros::Duration(0.2);

                markerArray.markers.push_back(marker);
                x += interval;
                id++;
            }
        }
        m_rosPubPolyMarkerArray.publish(markerArray);
    }


public:
    void drivingWayCallback(const kusv_msgs::PolyfitLaneData::ConstPtr &msg) {
        m_drivingWay = *msg;
    }

    void mark_drivingWay(double interval = 0.1, double ROILength = 30.0) {
        double a0 = m_drivingWay.d;
        double a1 = m_drivingWay.c;
        double a2 = m_drivingWay.b;
        double a3 = m_drivingWay.a;

        double x = 0.0;
        double y = a0;

        double distance_square = x * x + y * y;
        int id = 0;
        visualization_msgs::MarkerArray markerArray;
        while (distance_square < ROILength * ROILength) {

            y = a0 + a1 * x + a2 * x * x + a3 * x * x * x;
            distance_square = x * x + y * y;

            visualization_msgs::Marker marker;
            marker.header.frame_id = m_sVehicle_id;
            marker.header.stamp = ros::Time::now();

            marker.ns = m_drivingWay.id;
            marker.id = id;

            marker.type = visualization_msgs::Marker::CYLINDER;
            marker.action = visualization_msgs::Marker::ADD;

            marker.pose.position.x = x;
            marker.pose.position.y = y;
            marker.pose.position.z = 0.0;
            marker.pose.orientation.x = 0.0;
            marker.pose.orientation.y = 0.0;
            marker.pose.orientation.z = 0.1;
            marker.pose.orientation.w = 1.0;
            marker.color.r = 1.0f;
            marker.color.g = 1.0f;
            marker.color.b = 0.0f;
            marker.color.a = 1.0;
            marker.scale.x = 0.1;
            marker.scale.y = 0.1;
            marker.scale.z = 0.1;
            marker.lifetime = ros::Duration(0.2);

            markerArray.markers.push_back(marker);
            x += interval;
            id++;
        }

        m_rosPubDrivingWay.publish(markerArray);
    }
};







int main(int argc, char **argv) {
    ros::init(argc, argv, "display");
    // Vehicle vehicle;
    Display display;

    double prev_vehiclesMarkTime = ros::Time::now().toSec();
    double prev_lanesMarkTime = ros::Time::now().toSec();
    // The approximate control time is 100 Hz
    ros::Rate loop_rate(100);
    while (ros::ok()) {

        if ((ros::Time::now().toSec() - prev_vehiclesMarkTime) > 0.1) {
            prev_vehiclesMarkTime = ros::Time::now().toSec();
            display.mark_vehicle();
        }
        if ((ros::Time::now().toSec() - prev_lanesMarkTime) > 0.1) {
            prev_lanesMarkTime = ros::Time::now().toSec();
            display.mark_Lanes();
            display.broadcasting_tf_vehicle();
            display.mark_polyLanes();
            display.mark_drivingWay();
        }

        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}

