#include <ros/ros.h>
#include "geometry_msgs/Point.h"
#include "geometry_msgs/PointStamped.h"
#include "tf/transform_listener.h"
#include "path_generator/LanePointData.h"
#include "path_generator/LanePointDataArray.h"
#include "path_generator/PolyfitLaneData.h"
#include "path_generator/PolyfitLaneDataArray.h"

#include <math.h>
#include <eigen3/Eigen/Dense>
#include <string>

#include <cstdlib>

class calc_path {
public:
    calc_path() {
        m_rosPubPolyLanes =
                m_rosNodeHandler.advertise<path_generator::PolyfitLaneDataArray>(
                    "waypoint_lane", 1000);

        m_rosSubCsvFile = m_rosNodeHandler.subscribe(
                    "localization/csv_local_map", 1000, &calc_path::csvCallback, this);

//        m_rosNodeHandler.param("lane_detection/ROIFront", m_ROIFront_param, 20.0);
//        m_rosNodeHandler.param("lane_detection/ROIRear", m_ROIRear_param, 10.0);
//        m_rosNodeHandler.param("lane_detection/ROILeft", m_ROILeft_param, 3.0);
//        m_rosNodeHandler.param("lane_detection/ROIRight", m_ROIRight_param, 3.0);
    }

    ~calc_path() {}

protected:
    ros::NodeHandle m_rosNodeHandler;
    ros::Subscriber m_rosSubCsvFile;
    ros::Publisher m_rosPubPolyLanes;

    double m_ROIFront_param = 15.0;
    double m_ROIRear_param = 3.0;
    double m_ROILeft_param = 1.0;
    double m_ROIRight_param = 1.0;

    tf::TransformListener m_rosTfListenr;

    path_generator::LanePointDataArray m_csvLanes;
    path_generator::LanePointDataArray m_ROILanes;
    path_generator::PolyfitLaneDataArray m_polyLanes;

public:
    void csvCallback(const path_generator::LanePointDataArray::ConstPtr &msg) {
        m_csvLanes = *msg;
    }

    void extractRegionOfInterest() {
        m_ROILanes.frame_id = "/body";
        m_ROILanes.id = m_csvLanes.id;
        m_ROILanes.lane.clear();

        for (auto i_lane = 0; i_lane < m_csvLanes.lane.size(); i_lane++) {
            path_generator::LanePointData lane;
            lane.frame_id = "/body";
            lane.id = m_csvLanes.lane[i_lane].id;
            int sample_size =
                    (m_csvLanes.lane[i_lane].point.size());  // down sample x4
            double m_dminDist = 10;
            int m_minIndx = 0;

            for (int i_point = 0; i_point < sample_size; i_point++) {
                geometry_msgs::PointStamped lanePoint_world;
                lanePoint_world.header.frame_id = "/world";
                lanePoint_world.header.stamp = ros::Time(0);
                lanePoint_world.point.x = m_csvLanes.lane[i_lane].point[i_point].x;
                lanePoint_world.point.y = m_csvLanes.lane[i_lane].point[i_point].y;
                geometry_msgs::PointStamped lanePoint_body;

                try {
                    m_rosTfListenr.transformPoint("/body",
                                                  lanePoint_world, lanePoint_body);
                    if ((lanePoint_body.point.x <= m_ROIFront_param) &&
                            (lanePoint_body.point.x >= -1 * m_ROIRear_param) &&
                            (lanePoint_body.point.y <= m_ROILeft_param) &&
                            (lanePoint_body.point.y >= -1 * m_ROIRight_param)) {
                        double m_dDist = sqrt(pow(lanePoint_body.point.x, 2) + pow(lanePoint_body.point.y, 2));
                        if (m_dDist < m_dminDist) {
                            m_dminDist = m_dDist;
                            m_minIndx = i_point;
                        }
                    }

                } catch (tf::TransformException &ex) {
                    // ROS_ERROR(ex.what());
                }


                if(i_point+1 == sample_size) {
                    for(int i = m_minIndx-2; i < m_minIndx+20; i++) {
                        geometry_msgs::PointStamped sample_world;
                        sample_world.header.frame_id = "/world";
                        sample_world.header.stamp = ros::Time(0);
                        sample_world.point.x = m_csvLanes.lane[i_lane].point[i].x;
                        sample_world.point.y = m_csvLanes.lane[i_lane].point[i].y;
                        geometry_msgs::PointStamped sample_body;

                        try {
                            m_rosTfListenr.transformPoint("/body",
                                                          sample_world, sample_body);
                            lane.point.push_back(sample_body.point);

                        } catch (tf::TransformException &ex) {
                            // ROS_ERROR(ex.what());
                        }
                    }
                }




//                try {
//                    m_rosTfListenr.transformPoint("/body",
//                                                  lanePoint_world, lanePoint_body);
//                    if ((lanePoint_body.point.x <= m_ROIFront_param) &&
//                            (lanePoint_body.point.x >= -1 * m_ROIRear_param) &&
//                            (lanePoint_body.point.y <= m_ROILeft_param) &&
//                            (lanePoint_body.point.y >= -1 * m_ROIRight_param)) {
//                        lane.point.push_back(lanePoint_body.point);
//                    }

//                } catch (tf::TransformException &ex) {
//                    // ROS_ERROR(ex.what());
//                }
            }
            m_ROILanes.lane.push_back(lane);
//            if (lane.point.size() >= 2) {
//                m_ROILanes.lane.push_back(lane);
//            }
        }
    }

    void polyfitLane() {
        m_polyLanes.frame_id = "/body";
        m_polyLanes.polyfitLanes.clear();

        for (auto i_lane = 0; i_lane < m_ROILanes.lane.size(); i_lane++) {
            // Eigen의 매트릭스를 차선의 포인트 갯수에 맞게 생성한다. down sample x1
            int down_size = (m_ROILanes.lane[i_lane].point.size());
            Eigen::MatrixXd X_Matrix(down_size, 4);
            Eigen::VectorXd y_Vector(down_size);
            Eigen::VectorXd a_Vector(4);

            // Eigen의 매트릭스에 포인트를 넣어준다.
            for (int i_point = 0; i_point < down_size; i_point++) {
                double x = m_ROILanes.lane[i_lane].point[i_point].x;
                double y = m_ROILanes.lane[i_lane].point[i_point].y;

                X_Matrix(i_point, 0) = 1;
                X_Matrix(i_point, 1) = x;
                X_Matrix(i_point, 2) = x * x;
                X_Matrix(i_point, 3) = x * x * x;
                y_Vector(i_point) = y;
            }

            a_Vector =
                    ((X_Matrix.transpose() * X_Matrix).inverse() * X_Matrix.transpose()) *
                    y_Vector;

            path_generator::PolyfitLaneData polyLane;
            polyLane.frame_id = "/body";
            polyLane.id = m_ROILanes.lane[i_lane].id;

            polyLane.a0 = a_Vector(0);
            polyLane.a1 = a_Vector(1);
            polyLane.a2 = a_Vector(2);
            polyLane.a3 = a_Vector(3);

            m_polyLanes.polyfitLanes.push_back(polyLane);
        }
        m_rosPubPolyLanes.publish(m_polyLanes);
    }
};

int main(int argc, char **argv) {
    ros::init(argc, argv, "path_generation");

    calc_path calc_path;

    // The approximate control time is 100 Hz
    ros::Rate loop_rate(100);
    while (ros::ok()) {
        calc_path.extractRegionOfInterest();
        calc_path.polyfitLane();

        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}
