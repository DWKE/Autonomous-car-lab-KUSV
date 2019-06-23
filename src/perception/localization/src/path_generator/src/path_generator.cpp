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

//        m_rosNodeHandler.param("lane_detection/ns", m_vehicle_namespace_param,
//                               std::string(""));
//        m_rosNodeHandler.param("lane_detection/path", m_path_param,
//                               std::string(""));
//        if (m_path_param == std::string("")) {
//            ROS_ERROR_STREAM("Empty path!!");
//        }
    }

    ~calc_path() {}

protected:
    ros::NodeHandle m_rosNodeHandler;
    ros::Subscriber m_rosSubCsvFile;
    ros::Publisher m_rosPubPolyLanes;
    std::string m_vehicle_namespace_param;

    double m_ROIFront_param = 10.0;
    double m_ROIRear_param = 3.0;
    double m_ROILeft_param = 1.0;
    double m_ROIRight_param = 1.0;

    tf::TransformListener m_rosTfListenr;

    std::string m_path_param;
    path_generator::LanePointDataArray m_csvLanes;
    path_generator::LanePointDataArray m_ROILanes;
    path_generator::PolyfitLaneDataArray m_polyLanes;

    path_generator::LanePointData m_rightLane;
    path_generator::PolyfitLaneData m_rightPoly;

public:
    void csvCallback(const path_generator::LanePointDataArray::ConstPtr &msg) {
        m_csvLanes = *msg;
    }
    /*
  void loadLaneData() {
    std::vector<SKusvLane> csvLaneImport;
    csvLaneImport.ImportKusvLaneCsvFile(m_path_param);
    m_csvLanes.frame_id = "/world";
    m_csvLanes.lane.clear();
    for (auto i_lane = 0; i_lane < csvLaneImport.m_vecKusvLanes.size();
         i_lane++) {
      path_generator::LanePointData lane;
      lane.frame_id = "world";
      lane.id = std::to_string(csvLaneImport.m_vecKusvLanes[i_lane].m_nLaneID);
      for (auto i_point = 0;
           i_point <
           csvLaneImport.m_vecKusvLanes[i_lane].m_vecKusvLanePoint.size();
           i_point++) {
        geometry_msgs::Point point;
        point.x = csvLaneImport.m_vecKusvLanes[i_lane]
                      .m_vecKusvLanePoint[i_point]
                      .m_dPtX_m;
        point.y = csvLaneImport.m_vecKusvLanes[i_lane]
                      .m_vecKusvLanePoint[i_point]
                      .m_dPtY_m;
        lane.point.push_back(point);
      }
      m_csvLanes.lane.push_back(lane);
    }
  }
  */

    void extractRegionOfInterest() {
        m_ROILanes.frame_id = m_vehicle_namespace_param + "/body";
        m_ROILanes.id = m_csvLanes.id;
        m_ROILanes.lane.clear();

        for (auto i_lane = 0; i_lane < m_csvLanes.lane.size(); i_lane++) {
            path_generator::LanePointData lane;
            lane.frame_id = m_vehicle_namespace_param + "/body";
            lane.id = m_csvLanes.lane[i_lane].id;
            int down_size =
                    (m_csvLanes.lane[i_lane].point.size() + 3) / 4;  // down sample x4

            for (int i_point = 0; i_point < down_size; i_point++) {
                geometry_msgs::PointStamped lanePoint_world;
                lanePoint_world.header.frame_id = "/world";
                lanePoint_world.header.stamp = ros::Time(0);
                lanePoint_world.point.x = m_csvLanes.lane[i_lane].point[i_point * 4].x;
                lanePoint_world.point.y = m_csvLanes.lane[i_lane].point[i_point * 4].y;
                geometry_msgs::PointStamped lanePoint_body;

                try {
                    m_rosTfListenr.transformPoint(m_vehicle_namespace_param + "/body",
                                                  lanePoint_world, lanePoint_body);
                    if ((lanePoint_body.point.x <= m_ROIFront_param) &&
                            (lanePoint_body.point.x >= -1 * m_ROIRear_param) &&
                            (lanePoint_body.point.y <= m_ROILeft_param) &&
                            (lanePoint_body.point.y >= -1 * m_ROIRight_param)) {
                        lane.point.push_back(lanePoint_body.point);
                    }

                } catch (tf::TransformException &ex) {
                    // ROS_ERROR(ex.what());
                }
            }
            if (lane.point.size() >= 2) {
                m_ROILanes.lane.push_back(lane);
            }
        }
    }

    void polyfitLane() {
        m_polyLanes.frame_id = m_vehicle_namespace_param + "/body";
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
            polyLane.frame_id = m_vehicle_namespace_param + "/body";
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
