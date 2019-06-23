#include <ros/ros.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <string>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <ros/package.h>

class CameraInfo
{
private:
    ros::NodeHandle nh_;
    image_transport::ImageTransport it_;
    image_transport::Subscriber image_sub_;
    image_transport::Publisher image_pub_;

private:
    cv::Mat cameraMat;
    cv::Mat distCoeff;
    cv::Mat R = cv::Mat_<double>::eye(3, 3);
    cv::Mat T = cv::Mat_<double>::zeros(3, 1);
    cv::Mat R_inv = cv::Mat_<double>::eye(3, 3);
    cv::Mat C_w = cv::Mat_<double>::zeros(3, 1);
    double C_w_z = 0.0;

private:
    cv::Mat homograpyMat;

public:
    CameraInfo() : it_(nh_) {
        // Subscrive to input video feed and publish output video feed
        image_sub_ = it_.subscribe("videofile/image_raw", 10, &CameraInfo::imageCb, this);
    }
    ~CameraInfo() {}

    void imageCb(const sensor_msgs::ImageConstPtr& msg) {
        cv_bridge::CvImagePtr cv_ptr;
        cv::Mat img;
        try {
            cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
            img = cv_ptr->image;
        }
        catch (cv_bridge::Exception& e) {
            ROS_ERROR("cv_bridge exception: %s", e.what());
            return;
        }

        if (img.rows > 480 || img.cols > 640) {
            cv::Mat _img;
            img.copyTo(_img);
            cv::resize(_img, img, cv::Size(img.cols/2, img.rows/2), 0, 0, CV_INTER_AREA);
        }
        ROS_WARN_STREAM("image size: " << img.cols << ", " << img.rows);

        SolvePnP();
        FindHomography();
        dataWrite(img);
    }

    void dataRead(std::string filename) {
        cv::FileStorage fs;
        cv::Mat tmpMat;
        fs.open(filename, cv::FileStorage::READ);
        if (!fs.isOpened()) {
            ROS_ERROR_STREAM("Failed to open: " << filename << std::endl);
            return ;
        }
        cv::FileNode fn = fs["camera_matrix"];
        fn >> tmpMat;
        this->cameraMat = tmpMat;
        fn = fs["distortion_coefficients"];
        fn >> tmpMat;
        this->distCoeff = tmpMat;
        ROS_INFO_STREAM("camera_matrix: " << cameraMat.size());
        ROS_INFO_STREAM("distortion_camera: " << distCoeff.size());
    }

    void dataWrite(const cv::Mat img) { //write
        std::string filename = ros::package::getPath("final_lane_detector") + "/test_materials/calibration.yaml";
        cv::FileStorage fs(filename, cv::FileStorage::WRITE);
        fs << "image_width" << img.cols;
        fs << "image_height" << img.rows;
        fs << "camera_name" << "camera";
        fs << "camera_matrix" << cameraMat;
        fs << "distortion_model" << "plumb_bob";
        fs << "distortion_coefficients" << distCoeff;
        fs << "rotation_matrix" << R;
        fs << "rotation_matrix_inversion" << R_inv;
        fs << "translation_vector" << T;
        fs << "camera_origin_vector" << C_w;
        fs << "camera_origin_z_coordinate" << C_w_z;
        fs << "homograpy_matrix" << homograpyMat;
        fs.release();                                       // explicit close
    }

    /*

        ------------------------
        |                      |
        |A     B        C      |
        |                      |
        |                      |
        |                      |
        |                      |
        |                      |
        |                      |
        |                      |
      D |E                    F|
        ------------------------
      G                       H  I

    */
    void SolvePnP() {
        // 2D image points. If you change the image, you need to change vector
        //***************************** TUNING HERE *****************************//
        std::vector<cv::Point2d> image_points;
        image_points.push_back( cv::Point2d( 275, 372 ) );  // A
        image_points.push_back( cv::Point2d( 304, 372 ) );  // B
        image_points.push_back( cv::Point2d( 354, 372 ) );  // C
        image_points.push_back( cv::Point2d( 242, 393 ) );  // D
        image_points.push_back( cv::Point2d( 254, 393 ) );  // E
        image_points.push_back( cv::Point2d( 393, 393 ) );  // F
        image_points.push_back( cv::Point2d( 239, 395 ) );  // G
        image_points.push_back( cv::Point2d( 395, 395 ) );  // H
        image_points.push_back( cv::Point2d( 406, 394 ) );  // I


        // 3D model points.
        std::vector<cv::Point3d> model_points;
        model_points.push_back( cv::Point3d( 12.211 ,  1.285 ,  0.0 ) );  // A
        model_points.push_back( cv::Point3d( 12.211 ,  0.802 ,  0.0 ) );  // B
        model_points.push_back( cv::Point3d( 12.211 , -0.015 ,  0.0 ) );  // C
        model_points.push_back( cv::Point3d(  9.661 ,  1.446 ,  0.0 ) );  // D
        model_points.push_back( cv::Point3d(  9.661 ,  1.285 ,  0.0 ) );  // E
        model_points.push_back( cv::Point3d(  9.661 ,  -0.48 ,  0.0 ) );  // F
        model_points.push_back( cv::Point3d(  7.607 ,  1.446 ,  0.0 ) );  // G
        model_points.push_back( cv::Point3d(  7.607 ,  -0.48 ,  0.0 ) );  // H
        model_points.push_back( cv::Point3d(  7.607 , -0.634 ,  0.0 ) );  // I
        //***********************************************************************//

        // Solve for pose
        cv::Mat rotation_vector;
        cv::solvePnPRansac(model_points, image_points, cameraMat, distCoeff, rotation_vector, T);
        cv::Rodrigues(rotation_vector, R);
        R_inv = R.inv();
        C_w = - R_inv * T;
        C_w_z = ((double*)C_w.data)[2];
    }

    void FindHomography() {
        // 2D image points. If you change the image, you need to change vector
        //***************************** TUNING HERE *****************************//
        std::vector<cv::Point2f> image_points;
        image_points.push_back( cv::Point2f( 275, 372 ) );  // A
        image_points.push_back( cv::Point2f( 304, 372 ) );  // B
        image_points.push_back( cv::Point2f( 354, 372 ) );  // C
        image_points.push_back( cv::Point2f( 242, 393 ) );  // D
        image_points.push_back( cv::Point2f( 254, 393 ) );  // E
        image_points.push_back( cv::Point2f( 393, 393 ) );  // F
        image_points.push_back( cv::Point2f( 239, 395 ) );  // G
        image_points.push_back( cv::Point2f( 395, 395 ) );  // H
        image_points.push_back( cv::Point2f( 406, 394 ) );  // I


        // 3D model points.
        std::vector<cv::Point2f> model_points;
        model_points.push_back( cv::Point2f( 12.211f ,  1.285f ) );  // A
        model_points.push_back( cv::Point2f( 12.211f ,  0.802f ) );  // B
        model_points.push_back( cv::Point2f( 12.211f , -0.015f ) );  // C
        model_points.push_back( cv::Point2f(  9.661f ,  1.446f ) );  // D
        model_points.push_back( cv::Point2f(  9.661f ,  1.285f ) );  // E
        model_points.push_back( cv::Point2f(  9.661f ,  -0.48f ) );  // F
        model_points.push_back( cv::Point2f(  7.607f ,  1.446f ) );  // G
        model_points.push_back( cv::Point2f(  7.607f ,  -0.48f ) );  // H
        model_points.push_back( cv::Point2f(  7.607f , -0.634f ) );  // I
        //***********************************************************************//

        // Solve for pose
        homograpyMat = cv::findHomography(image_points, model_points, CV_RANSAC);
    }
};



int main(int argc, char** argv) {
    ros::init(argc, argv, "make_camera_info");
    CameraInfo info;
    if (argc != 2) {
        ROS_WARN("No calibration file!");
    }
    else {
        info.dataRead(argv[1]);
    }
    ros::spin();
    return 0;
}
