#include <ros/ros.h>
#include <string>
#include <vector>
#include <iomanip>
#include <cmath>
#include <math.h>
#include <array>
#include <iostream>
#include <fstream>
#include <eigen3/Eigen/Dense>
#include <algorithm>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/eigen.hpp>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>

#include <geometry_msgs/Point.h>
#include <sensor_msgs/image_encodings.h>

#include "kusv_msgs/LanePointData.h"
#include "kusv_msgs/LanePointDataArray.h"
#include "kusv_msgs/PolyfitLaneData.h"
#include "kusv_msgs/PolyfitLaneDataArray.h"

#define BUFFER 5


static std::string RESULT_WINDOW = "result_video";
//static std::string TEST_WINDOW = "test_video";

class LaneDetector
{
private:
    ros::NodeHandle m_nh;
    image_transport::ImageTransport m_it;

private:
    image_transport::Subscriber m_imageSub;
    image_transport::Publisher m_imagePub;

private:
    ros::Publisher m_pointsPub;
    ros::Publisher m_pointPub_left;
    ros::Publisher m_pointPub_right;

private:
    ros::Publisher m_lanesPub;
    ros::Publisher m_lanePub_left;
    ros::Publisher m_lanePub_right;
    ros::Publisher m_lanePub_middle;


private:
    double size_x;
    double size_y;

private:
    cv::Mat cameraMat;
    cv::Mat distCoeff;
    cv::Mat R;
    cv::Mat T;
    cv::Mat R_inv;
    cv::Mat C_w;
    double C_w_z;
    cv::Mat homograpyMat;


private:
    Eigen::Matrix3d cameraMat_eigen;
    Eigen::Matrix3d R_eigen;
    Eigen::Vector3d T_eigen;
    Eigen::Matrix3d R_inv_eigen;
    Eigen::Vector3d C_w_eigen;
    Eigen::Matrix3f homograpyMat_eigen;


private:
    cv::Scalar RED   = cv::Scalar(0, 0, 255);
    cv::Scalar GREEN = cv::Scalar(0, 255, 0);
    cv::Scalar BLUE  = cv::Scalar(255, 0, 0);

private:
    int param_h_low;
    int param_h_high;
    int param_l_low;
    int param_s_low;
    int param_division;
    int param_window_halfWidth;
    bool param_useFakeLane;

private:
    float ROAD_WIDTH = 3.035;

private:
    double param_a_thresh;
    double param_b_thresh;
    double param_c_thresh;
    double param_d_thresh;
private:
    int param_color_merge;
    int param_abcd_selection;

private:
    double m_dBuffer[BUFFER];
    double m_dMovingAverage;

public:
    LaneDetector() : m_it(m_nh) {
        m_imageSub =
                m_it.subscribe("stream", 1, &LaneDetector::imageCb, this);
        m_imagePub = m_it.advertise("videofile/output_video", 1);

        m_pointsPub =
                m_nh.advertise<kusv_msgs::LanePointDataArray>(
                    "points/left_and_right", 10);
        m_pointPub_left =
                m_nh.advertise<kusv_msgs::LanePointData>(
                    "points/left", 10);
        m_pointPub_right =
                m_nh.advertise<kusv_msgs::LanePointData>(
                    "points/right", 10);

        m_lanesPub =
                m_nh.advertise<kusv_msgs::PolyfitLaneDataArray>(
                    "lane/left_and_right", 10);
        m_lanePub_left =
                m_nh.advertise<kusv_msgs::PolyfitLaneData>(
                    "lane/left", 10);
        m_lanePub_right =
                m_nh.advertise<kusv_msgs::PolyfitLaneData>(
                    "lane/right", 10);
        m_lanePub_middle =
                m_nh.advertise<kusv_msgs::PolyfitLaneData>(
                    "vision_lane", 10);

        m_nh.param("lane_detector/h_low", param_h_low, 0);
        m_nh.param("lane_detector/h_high", param_h_high, 0);
        m_nh.param("lane_detector/l_low", param_l_low, 0);
        m_nh.param("lane_detector/s_low", param_s_low, 0);
        m_nh.param("lane_detector/division", param_division, 0);
        m_nh.param("lane_detector/window_halfWidth", param_window_halfWidth, 0);
        m_nh.param("lane_detector/useFakeLane", param_useFakeLane, true);
        m_nh.param("lane_detector/a_thresh", param_a_thresh, 1.0);
        m_nh.param("lane_detector/b_thresh", param_b_thresh, 1.0);
        m_nh.param("lane_detector/c_thresh", param_c_thresh, 1.0);
        m_nh.param("lane_detector/d_thresh", param_d_thresh, 1.0);
        m_nh.param("lane_detector/color_merge", param_color_merge, 1);
        m_nh.param("lane_detector/abcd_selection", param_abcd_selection, 1);

        for (uint32_t i = 0; i < BUFFER; i++)
            m_dBuffer[i] = 0.0;
    }

    ~LaneDetector() {
        cv::destroyWindow(RESULT_WINDOW);
        //cv::destroyWindow(TEST_WINDOW);
    }

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
        //ROS_WARN_STREAM(img.size() << ", " << img.size().height);

        bool isProcessed;
        cv::Mat img_output;
        size_x = img.cols;
        size_y = img.rows;
        isProcessed = processingImage(img, img_output);
        if (isProcessed == false && cv::waitKey(1) == 27) //ESC키 누르면 종료
            return;

        cv_ptr->image = img_output;
        // Output modified video stream
        m_imagePub.publish(cv_ptr->toImageMsg());
    }



public:
    void dataRead(std::string filename) {
        cv::FileStorage fs;
        cv::Mat tmpMat;
        double k;
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
        fn = fs["rotation_matrix"];
        fn >> tmpMat;
        this->R = tmpMat;
        fn = fs["rotation_matrix_inversion"];
        fn >> tmpMat;
        this->R_inv = tmpMat;
        fn = fs["translation_vector"];
        fn >> tmpMat;
        this->T = tmpMat;
        fn = fs["camera_origin_vector"];
        fn >> tmpMat;
        this->C_w = tmpMat;
        fn = fs["camera_origin_z_coordinate"];
        fn >> k;
        this->C_w_z = k;
        fn = fs["homograpy_matrix"];
        fn >> tmpMat;
        this->homograpyMat = tmpMat;

        cv::cv2eigen(this->cameraMat, cameraMat_eigen);
        cv::cv2eigen(this->R, R_eigen);
        cv::cv2eigen(this->R_inv, R_inv_eigen);
        cv::cv2eigen(this->T, T_eigen);
        cv::cv2eigen(this->C_w, C_w_eigen);
        cv::cv2eigen(this->homograpyMat, homograpyMat_eigen);

        ROS_INFO_STREAM("camera_matrix: " << cameraMat.size());
        ROS_INFO_STREAM("distortion_camera: " << distCoeff.size());
        ROS_INFO_STREAM("rotation_matrix: " << R.size());
        ROS_INFO_STREAM("translation_vector: " << T.size());
        ROS_INFO_STREAM("homograpy_matrix: " << homograpyMat.size());
    }

public:
    cv::Mat grayAndBlur(const cv::Mat _img_bgr, int kernel_size=5) {
        cv::Mat img_fixed;
        cvtColor(_img_bgr, img_fixed, CV_BGR2GRAY);
        GaussianBlur(img_fixed, img_fixed, cv::Size(kernel_size, kernel_size), 0);
        return img_fixed;
    }

    cv::Mat filteringColor(const cv::Mat _img_bgr) {
        // Filter the image to include only yellow and white pixels
        cv::Mat img_bgr, img_filtered;
        cv::Mat white, yellow;
        _img_bgr.copyTo(img_bgr);
        cv::Mat img_hls, hls[3], _combined, combined, filtered;

        //***************************** TUNING HERE *****************************//
        // Filter white pixels
        cv::inRange(img_bgr, cv::Scalar(130,130,100), cv::Scalar(255,255,255), filtered);
        cv::bitwise_and(img_bgr, img_bgr, white, filtered);
        cv::bitwise_xor(white, img_bgr, white);

        // Filter yellow pixels
        cv::cvtColor(img_bgr, img_hls, cv::COLOR_BGR2HLS);
        split(img_hls, hls);
        switch (param_color_merge) {
        case 0:
            cv::threshold(hls[2], hls[2],  param_s_low, 255, cv::THRESH_BINARY);
            cv::bitwise_and(img_bgr, img_bgr, img_filtered, hls[2]);
            break;

        case 1:
            cv::threshold(hls[0], hls[0],  param_h_high, 255, cv::THRESH_BINARY_INV);
            cv::threshold(hls[0], hls[0],  param_h_low, 255, cv::THRESH_BINARY);
            cv::threshold(hls[1], hls[1],  param_l_low, 255, cv::THRESH_BINARY);
            cv::threshold(hls[2], hls[2],  param_s_low, 255, cv::THRESH_BINARY);
            cv::bitwise_and(  hls[0],    hls[2],  _combined);
            cv::bitwise_and(  hls[1],    hls[2],   combined);
            cv::bitwise_or(_combined,  combined,  _combined);
            cv::bitwise_and(  hls[2], _combined,   combined);
            cv::bitwise_and(img_bgr, img_bgr, img_filtered, combined);
            break;

        case 2:
            cv::threshold(hls[0], hls[0],  param_h_high, 255, cv::THRESH_BINARY_INV);
            cv::threshold(hls[0], hls[0],  param_h_low, 255, cv::THRESH_BINARY);
            cv::threshold(hls[1], hls[1],  param_l_low, 255, cv::THRESH_BINARY);
            cv::threshold(hls[2], hls[2],  param_s_low, 255, cv::THRESH_BINARY);
            cv::bitwise_or(hls[0],  hls[2],  _combined);
            cv::bitwise_or(hls[1],  hls[2],  combined);
            cv::bitwise_and(_combined,  combined,  combined);
            cv::bitwise_and(  hls[2], combined,   combined);
            cv::bitwise_and(img_bgr, img_bgr, img_filtered, combined);
            break;
        }

        // White + Yellow
        //Combine the two above images
        //cv::bitwise_or(white, yellow, img_filtered);
        //***********************************************************************//
        return img_filtered;
    }

    cv::Mat sobelXY(const cv::Mat img, int thresh, double x_ratio=0.8, double y_ratio=0.5, int kernel_size=5) {
        cv::Mat img_gradXY;
        int ddepth = CV_16S;
        int scale = 1;
        int delta = 0;

        // Generate grad_x and grad_y
        cv::Mat grad_x, grad_y;
        cv::Mat abs_grad_x, abs_grad_y;

        // Gradient X
        //Scharr(src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT);
        cv::Sobel(img, grad_x, ddepth, 1, 0, kernel_size, scale, delta, cv::BORDER_DEFAULT);
        convertScaleAbs(grad_x, abs_grad_x);

        // Gradient Y
        //Scharr(src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT);
        cv::Sobel(img, grad_y, ddepth, 0, 1, kernel_size, scale, delta, cv::BORDER_DEFAULT);
        convertScaleAbs(grad_y, abs_grad_y);

        // Total Gradient (approximate)
        addWeighted(abs_grad_x, x_ratio, abs_grad_y, y_ratio, 0, img_gradXY);

        cv::threshold(img_gradXY, img_gradXY, thresh, 255, CV_THRESH_BINARY);
        return img_gradXY;
    }

    cv::Mat sobelMag(const cv::Mat img, int thresh, int kernel_size=5) {
        cv::Mat img_gradMag;
        int ddepth = CV_16S;
        int scale = 1;
        int delta = 0;

        // Generate grad_x and grad_y
        cv::Mat grad_x, grad_y;
        cv::Mat abs_grad_x, abs_grad_y;

        // Gradient X
        //Scharr(src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT);
        cv::Sobel(img, grad_x, ddepth, 1, 0, kernel_size, scale, delta, cv::BORDER_DEFAULT);
        convertScaleAbs(grad_x, abs_grad_x);

        // Gradient Y
        //Scharr(src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT);
        cv::Sobel(img, grad_y, ddepth, 0, 1, kernel_size, scale, delta, cv::BORDER_DEFAULT);
        convertScaleAbs(grad_y, abs_grad_y);

        cv::pow(abs_grad_x, 2.0, abs_grad_x);
        cv::pow(abs_grad_y, 2.0, abs_grad_y);

        cv::add(abs_grad_x, abs_grad_y, img_gradMag);
        cv::normalize(img_gradMag, img_gradMag, 0, 255, cv::NORM_MINMAX);
        cv::threshold(img_gradMag, img_gradMag, thresh, 255, CV_THRESH_BINARY);
        return img_gradMag;
    }

    cv::Mat getGradientThresh(const cv::Mat img_gray_blur, int xy_thresh=70, int mag_thresh=170, double x_ratio=0.8, double y_ratio=0.5, int kernel_size_xy=5, int kernel_size_mag=5) {
        cv::Mat img_gradXY = sobelXY(img_gray_blur, xy_thresh, x_ratio, y_ratio, kernel_size_xy);
        cv::Mat img_gradMag = sobelMag(img_gray_blur, mag_thresh, kernel_size_mag);
        //cv::Mat img_canny;
        //cv::Canny(img_gray_blur, img_canny, 50, 100);
        //cv::imshow("canny", img_canny);

        cv::Mat _img_grad, img_grad;
        cv::bitwise_and(img_gradXY, img_gradMag, img_grad);
        //cv::bitwise_and(img_canny, _img_grad, img_grad);

        //cv::imshow("grad", img_grad);
        return img_grad;
    }

    cv::Mat regionOfInterest(const cv::Mat img_edges, int num, const cv::Point *points,  cv::Point *points_) {
        cv::Mat img_mask = cv::Mat::zeros(img_edges.rows, img_edges.cols, CV_8UC1);

        cv::Scalar mask_color = cv::Scalar(255, 255, 255);
        const cv::Point* ppt[1] = {points};
        int npt[] = {num};

        // filling pixels inside the polygon defined by "vertices" with the fill color
        fillPoly(img_mask, ppt, npt, 1, mask_color, cv::LINE_8);

        // returning the image only where mask pixels are nonzero
        cv::Mat img_masked;
        cv::bitwise_and(img_edges, img_mask, img_masked);
        return img_masked;
    }

    void calcPerspectiveTransform(const cv::Mat img_input, const cv::Point2f* inputQuad, cv::Mat& lambda, cv::Mat& lambda_inv) {
        // Output Quadilateral or World plane coordinates
        cv::Point2f outputQuad[4];

        // Set the lambda matrix the same type and size as input
        lambda = cv::Mat::zeros(img_input.rows, img_input.cols, img_input.type());

        // The 4 points where the mapping is to be done , from bottom-left in clockwise order
        outputQuad[0] = cv::Point2f(50, img_input.rows-1);
        outputQuad[1] = cv::Point2f(50, 1);
        outputQuad[2] = cv::Point2f(img_input.cols-50, 1);
        outputQuad[3] = cv::Point2f(img_input.cols-50, img_input.rows-1);

        // Get the Perspective Transform Matrix i.e. lambda
        lambda = getPerspectiveTransform(inputQuad, outputQuad);
        lambda_inv = lambda.inv();
    }

    void sortWindow(cv::Size size, int& w_t, int& w_b, int& w_l, int& w_r) {
        int window_top[3] = {0, w_t, size.height};
        std::sort(window_top, window_top+3);
        int window_bottom[3] = {0, w_b, size.height};
        std::sort(window_bottom, window_bottom+3);
        int window_left[3] = {0, w_l, size.width};
        std::sort(window_left, window_left+3);
        int window_right[3] = {0, w_r, size.width};
        std::sort(window_right, window_right+3);
        w_t = window_top[1];
        w_b = window_bottom[1];
        w_l = window_left[1];
        w_r = window_right[1];
    }

    bool findingPeak(const cv::Mat img, int& peaks, int top, int bottom, int left, int right) {
        sortWindow(img.size(), top, bottom, left, right);
        cv::Mat img_hist = img(cv::Range(top, bottom), cv::Range(left, right));

        cv::Mat colSum;
        cv::reduce(img_hist, colSum, 0, CV_REDUCE_SUM, CV_64F);
        cv::normalize(colSum, colSum, 0, img_hist.rows, cv::NORM_MINMAX, CV_8U);

        // For Visualizating a histogram
        //for(int i = 1; i < colSum.cols; i++) {
        //    cv::line(img_hist, cv::Point(i-1, (int)(*colSum.col(i-1).data)), cv::Point(i, (int)(*colSum.col(i).data)), cv::Scalar(255, 255, 255), 1, 8);
        //}
        //cv::imshow("img_hist", img_hist);

        cv::Scalar sum = cv::sum(colSum);
        if (sum == cv::Scalar(0)) {
            peaks = (left + right)/2;
            //peaks = 0;
            return false;
        }

        double minVal, maxVal;
        cv::Point minLoc, maxLoc;
        cv::minMaxLoc(colSum, &minVal, &maxVal, &minLoc, &maxLoc);
        peaks = left + maxLoc.x;
        return true;
    }

    cv::Mat getSlidingWindow(const cv::Mat img, int division, int win_half_width, std::vector<cv::Point2f>& points, bool isLeft, int* arr_peaks=nullptr, bool useFakeLane=false) {
        cv::Mat slidingMask = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
        cv::Mat mask(slidingMask);

        int peak, last_peak;
        peak = arr_peaks[isLeft? 0: 1];
        for (int i = 0; i < division; i++) {
            last_peak = peak;
            int top    = img.rows - ((img.rows/division)*(i+1));
            int bottom = img.rows - ((img.rows/division)*(i));
            int left   = last_peak - win_half_width;
            int right  = last_peak + win_half_width;
            sortWindow(img.size(), top, bottom, left, right);   // sortWindow(cv::Size(img.cols, img.rows), top, bottom, left, right);

            mask(cv::Rect(cv::Point(left, top), cv::Point(right, bottom))) = 255;
            cv::bitwise_or(mask, slidingMask, slidingMask);

            bool hasPeak = findingPeak(img, peak, top, bottom, left, right);

            if (hasPeak == true) {
                cv::Point point;
                point.x = (left + right)/2;
                point.y = bottom;
                points.push_back(point);
            }
            else if (useFakeLane == true) {
                cv::Point point;
                point.x = (left + right)/2;
                point.y = bottom;
                points.push_back(point);
            }
        }
        return slidingMask;
    }

    std::vector<cv::Point2f> findingLanePixels(const cv::Mat img_lane, cv::Mat& img_unwarp, const cv::Mat lamda_inv, bool isLeft, const std::vector<cv::Point2f> points) {
        int w_t = 0;
        int w_b = img_lane.rows;
        int w_l, w_r;
        cv::Mat img_lane_;
        img_lane_ = img_lane.clone();

        if (isLeft == true) {
            w_l = 0;
            w_r = img_lane.cols/2;
            sortWindow(img_lane.size(), w_t, w_b, w_l, w_r);
            img_lane_(cv::Range(w_t, w_b), cv::Range(w_r, img_lane_.cols)) = 0;
        }
        else {
            w_l = img_lane.cols/2;
            w_r = img_lane.cols;
            sortWindow(img_lane.size(), w_t, w_b, w_l, w_r);
            img_lane_(cv::Range(w_t, w_b), cv::Range(0, w_l)) = 0;
        }
        cv::warpPerspective(img_lane_, img_unwarp, lamda_inv, img_unwarp.size());
        std::vector<cv::Point2f> points_2d;
        if (!points.empty()) {
            cv::perspectiveTransform(points, points_2d, lamda_inv);
        }
        return points_2d;
    }

public:
    std::vector<cv::Point3f> twoDim2threeDim(const std::vector<cv::Point2f> points_2d) {
        std::vector<cv::Point3f> points_3d;
        for (uint i = 0; i < points_2d.size(); i++) {
            float scale = homograpyMat_eigen(2,0)*points_2d[i].x + homograpyMat_eigen(2,1)*points_2d[i].y + homograpyMat_eigen(2,2)*1.0f;
            scale = 1.0f / scale;

            Eigen::Vector3f P_c, P_w, P;
            P_c(0) = scale * points_2d[i].x;
            P_c(1) = scale * points_2d[i].y;
            P_c(2) = scale * 1.0f;

            P = homograpyMat_eigen * P_c;

            cv::Point3f point_3d;
            point_3d.x = P(0);
            point_3d.y = P(1);
            point_3d.z = 0.0f;
            points_3d.push_back(point_3d);
        }
        return points_3d;
    }

public:
    kusv_msgs::LanePointData vector2lanePoints(const std::vector<cv::Point3f> points_3d) {
        kusv_msgs::LanePointData lanePoints;
        for (uint i = 0; i < points_3d.size(); i++) {
            geometry_msgs::Point point;
            point.x = points_3d[i].x;
            point.y = points_3d[i].y;
            point.z = points_3d[i].z;
            lanePoints.point.push_back(point);
        }
        return lanePoints;
    }

    kusv_msgs::LanePointData vector2lanePoints(const std::vector<cv::Point2f> points_2d) {
        kusv_msgs::LanePointData lanePoints;
        for (uint i = 0; i < points_2d.size(); i++) {
            geometry_msgs::Point point;
            point.x = points_2d[i].x;
            point.y = points_2d[i].y;
            point.z = 0.0;
            lanePoints.point.push_back(point);
        }
        return lanePoints;
    }

public:
    std::vector<double> polyLaneFitting(const std::vector<cv::Point3f> points_3d, std::string lane_id) {
        std::vector<double> aVec;

        uint degree, N;
        N = points_3d.size();

        std::vector<double> x, y, X, a, Y;
        for (uint i = 0; i < N; i++) {
            x.push_back(points_3d[i].x);
            //ROS_INFO_STREAM(x[i]<<", "<<msg->point.at(i).x);
            y.push_back(points_3d[i].y);
        }

        // n is the degree of Polynomial
        // X is an array that will store the values of sigma(xi),sigma(xi^2),sigma(xi^3)....sigma(xi^2n)
        // consecutive positions of the array will store N,sigma(xi),sigma(xi^2),sigma(xi^3)....sigma(xi^2n)
        degree = 3;
        for (uint i = 0; i < 2*degree+1; i++) {
            X.push_back(0);
            for (uint j = 0; j < N; j++)
                X[i]=X[i]+pow(x[j],i);
        }

        std::vector<std::vector<double>> B(degree+1, std::vector<double>(degree+2, 0));
        // B is the Normal matrix(augmented) that will store the equations, 'a' is for value of the final coefficients
        // Build the Normal matrix by storing the corresponding coefficients at the right positions except the last column of the matrix
        for (uint i = 0; i <= degree; i++) {
            for (uint j = 0; j <= degree; j++) {
                B[i][j]=X[i+j];
            }
        }

        // Y is an array to store the values of sigma(yi),sigma(xi*yi),sigma(xi^2*yi)...sigma(xi^n*yi)
        // consecutive positions will store sigma(yi),sigma(xi*yi),sigma(xi^2*yi)...sigma(xi^n*yi)
        for (uint i = 0; i < degree+1; i++) {
            a.push_back(0);
            Y.push_back(0);
            for (uint j = 0; j < N; j++) {
                Y[i]=Y[i]+pow(x[j],i)*y[j];
            }
        }
        // load the values of Y as the last column of B(Normal Matrix but augmented)
        // n is made n+1 because the Gaussian Elimination part below was for n equations, but here n is the degree of polynomial and for n degree we get n+1 equations
        for (uint i = 0; i <= degree; i++) {
            B[i][degree+1]=Y[i];
        }

        degree = degree+1;
        //ROS_WARN_STREAM(msg->id);

        for (uint i = 0; i < degree; i++) {                   //From now Gaussian Elimination starts(can be ignored) to solve the set of linear equations (Pivotisation)
            for (uint k = i+1 ; k < degree; k++) {
                if (B[i][i] < B[k][i]) {
                    for (uint j = 0; j <= degree; j++) {
                        double temp = B[i][j];
                        B[i][j] = B[k][j];
                        B[k][j] = temp;
                    }
                }
            }
        }
        // loop to perform the gauss elimination
        // make the elements below the pivot elements equal to zero or elimnate the variables
        for (uint i = 0; i < degree-1; i++) {
            for (uint k = i+1; k < degree; k++) {
                double t = B[k][i]/B[i][i];
                for (uint j = 0; j <= degree; j++)
                    B[k][j] = B[k][j] - (t * B[i][j]);
            }
        }
        // back-substitution
        for (int i = degree-1; i >= 0; i--) {
            // x is an array whose values correspond to the values of x,y,z..
            // make the variable to be calculated equal to the rhs of the last equation
            a[i] = B[i][degree];
            for (uint j = 0; j < degree; j++) {
                // then subtract all the lhs values except the coefficient of the variable whose value is being calculated
                if (j != i) {
                    a[i]=a[i]-B[i][j]*a[j];
                }
            }
            // now finally divide the rhs by the coefficient of the variable to be calculated
            a[i] = a[i]/B[i][i];
        }

        //std::cout << "The fitted Polynomial is given by: y=\n\n";
        for (uint i = 0; i < degree; i++) {
            //std::cout << " + ("<<a[i]<<")"<<"x^"<<i;
            aVec.push_back(a[i]);
            //a[i] = 0;
        }
        kusv_msgs::PolyfitLaneData lane;

        if (!aVec.empty()) {
            lane.id = lane_id;
            lane.a = aVec[3];
            lane.b = aVec[2];
            lane.c = aVec[1];
            lane.d = aVec[0];
            //ROS_INFO_STREAM("0: " << aVec[0] << ", 1: " << aVec[1] << ", 2: " << aVec[2] << ", 3: " << aVec[3]);
            if (lane.id == "left_lane") {
                m_lanePub_left.publish(lane);
            }
            else {
                m_lanePub_right.publish(lane);
            }
        }
        return aVec;
    }

public:
    void average(double data) {
        for (uint32_t i = BUFFER - 1; i > 0; i--)
            m_dBuffer[i] = m_dBuffer[i-1];

        m_dBuffer[0] = data;

        m_dMovingAverage = 0.0;
        for (uint32_t i = 0; i < BUFFER; i++)
            m_dMovingAverage += m_dBuffer[i];

        m_dMovingAverage /= BUFFER;
    }

    std::vector<double> findDrivingWay(const std::vector<double> left_line, const std::vector<double> right_line) {
        double prev_a, prev_b, prev_c, prev_d;

        bool l_lane_exist = false;
        bool r_lane_exist = false;
        std::vector<double> middle(4);


        if(std::isnan(left_line[0])){
            l_lane_exist = false;
        }
        else{
            l_lane_exist = true;
        }

        if(std::isnan(right_line[0])){
            r_lane_exist = false;
        }
        else{
            r_lane_exist = true;
        }


        if(l_lane_exist && r_lane_exist){
            middle[0] = (left_line[0] + right_line[0])/2; //d
            middle[1] = (left_line[1] + right_line[1])/2; //c
            middle[2] = (left_line[2] + right_line[2])/2; //b
            middle[3] = (left_line[3] + right_line[3])/2; //a

            prev_d = middle[0];
            prev_c = middle[1];
            prev_b = middle[2];
            prev_a = middle[3];
            ROS_INFO_STREAM("BOTH");
        }

        if(l_lane_exist && !r_lane_exist){
            middle[0] = left_line[0] - (ROAD_WIDTH/2); //d
            middle[1] = left_line[1]; //c
            middle[2] = left_line[2]; //b
            middle[3] = left_line[3]; //a

            prev_d = middle[0];
            prev_c = middle[1];
            prev_b = middle[2];
            prev_a = middle[3];
            ROS_INFO_STREAM("ONLY LEFT");
        }

        if(!l_lane_exist && r_lane_exist){
            middle[0] = right_line[0] + (ROAD_WIDTH/2); //d
            middle[1] = right_line[1]; //c
            middle[2] = right_line[2]; //b
            middle[3] = right_line[3]; //a

            prev_d = middle[0];
            prev_c = middle[1];
            prev_b = middle[2];
            prev_a = middle[3];
            ROS_INFO_STREAM("ONLY RIGHT");
        }

        if(!l_lane_exist && !r_lane_exist){
            middle[0] = prev_d;
            middle[1] = prev_c;
            middle[2] = prev_b;
            middle[3] = prev_a;
            ROS_INFO_STREAM("NONE");
        }

        switch (param_abcd_selection) {
        case 0:
            if((std::abs(middle[3]) > param_a_thresh) || (std::abs(middle[2]) > param_b_thresh) || (std::abs(middle[1]) > param_c_thresh)|| (std::abs(middle[0]) > param_d_thresh)) {
                middle[0] = prev_d;
                middle[1] = prev_c;
                middle[2] = prev_b;
                middle[3] = prev_a;
                ROS_INFO_STREAM("!!!!!LANE IS WRONG!!!!!");
            }
            break;

        case 1:
            break;

        case 2:
            break;
        }


        //ROS_INFO_STREAM("left(abcd):    " << left_line[3] << "   " << left_line[2] <<"   " << left_line[1] << "   " << left_line[0]);
        //ROS_INFO_STREAM("right(abcd):   " << right_line[3] << "   " << right_line[2] <<"   " << right_line[1] << "   " << right_line[0]);
        ROS_INFO_STREAM("middle(abcd):  " << middle[3] << "   " << middle[2] << "   " << middle[1] << "   " << middle[0]);

        return middle;
    }

public:
    std::vector<cv::Point3f> getSamplingPoint(std::vector<double> coeff, std::vector<cv::Point3f> points_3d_prev){
        double d = coeff[0];
        double c = coeff[1];
        double b = coeff[2];
        double a = coeff[3];

        std::vector<cv::Point3f> points_3d;

        for (uint i = 0; i < points_3d_prev.size(); i++) {
            cv::Point3f point_3d;
            point_3d.x = points_3d_prev[i].x;
            point_3d.y = a * (point_3d.x*point_3d.x*point_3d.x) + b * (point_3d.x*point_3d.x) + c * (point_3d.x) + d;
            point_3d.z = 0.0f;

            points_3d.push_back(point_3d);
        }
        return points_3d;
    }

    std::vector<cv::Point> threeDim2twoDim(std::vector<cv::Point3f> points_3d) {
        std::vector<cv::Point> points_2d;
        Eigen::Matrix3f homoM_inv = homograpyMat_eigen.inverse();

        for (uint i = 0; i < points_3d.size(); i++){
            float scale_inv = homoM_inv(2,0)*points_3d[i].x + homoM_inv(2,1)*points_3d[i].y + homoM_inv(2,2)*1.0f;
            scale_inv = 1.0f / scale_inv;
            Eigen::Vector3f P_w, P_c;
            P_w(0) = scale_inv * points_3d[i].x;
            P_w(1) = scale_inv * points_3d[i].y;
            P_w(2) = scale_inv * 1.0f;

            P_c = homoM_inv * P_w;

            cv::Point point_2d;
            point_2d.x = P_c(0);
            point_2d.y = P_c(1);
            //ROS_WARN_STREAM(P_c(2));
            points_2d.push_back(point_2d);
        }
        return points_2d;
    }

    void makeFinalImage(const cv::Mat img, cv::Mat& img_modified, const std::vector<cv::Point> points_2d, cv::Scalar lineColor=cv::Scalar(0, 0, 255)){
        if (img.empty())
            return;
        img_modified = img.clone();

        const cv::Point *pts = (const cv::Point*)cv::Mat(points_2d).data;
        int npts = cv::Mat(points_2d).rows;

        cv::polylines(img_modified, &pts, &npts, 1, false, lineColor, 3);
    }



public:
    bool processingImage(const cv::Mat img, cv::Mat& img_output) {
        // 1. 원본 영상을 읽어옴
        if (img.empty())
            return false;


        // 2. 왜곡 보정
        cv::Mat img_undist;
        if (!cameraMat.empty() && !distCoeff.empty()) {
            cv::undistort(img, img_undist, cameraMat, distCoeff);
        }


        // 3. 미리 정해둔 노란색 범위 내에 있는 부분만 차선후보로 따로 저장함
        cv::Mat img_filtered, img_blur_;
        img_undist.copyTo(img_blur_);
        //***************************** TUNING HERE *****************************//
        int kernel_size_blur = 5;
        //***********************************************************************//
        GaussianBlur(img_blur_, img_blur_, cv::Size(kernel_size_blur, kernel_size_blur), 0);
        img_filtered = filteringColor(img_blur_);


        // 4. 그레이스케일 영상으로 변환하여 에지 성분을 추출
        cv::Mat img_blur, img_edges;
        //***************************** TUNING HERE *****************************//
        kernel_size_blur = 5;
        int kernel_size_xy = 5;
        int kernel_size_mag = 3;
        int xy_thresh = 70;
        int mag_thresh = 170;
        double x_ratio = 0.8;
        double y_ratio = 0.5;
        //***********************************************************************//
        img_blur = grayAndBlur(img_filtered, kernel_size_blur);
        img_edges = getGradientThresh(img_blur, xy_thresh, mag_thresh, x_ratio, y_ratio, kernel_size_xy, kernel_size_mag);


        // 5. 차선 검출할 영역을 제한함(진행방향 바닥에 존재하는 차선으로 한정)
        //***************************** TUNING HERE *****************************//
        cv::Point points[4];
        points[0] = cv::Point( 56, 418);
        points[1] = cv::Point(225, 338);
        points[2] = cv::Point(460, 338);
        points[3] = cv::Point(660, 418);
        //points[4] = cv::Point(489, 418);
        //points[5] = cv::Point(382, 339);
        //points[6] = cv::Point(303, 339);
        //points[7] = cv::Point(227, 418);
        //***********************************************************************//
        cv::Mat img_roi;
        img_roi = regionOfInterest(img_edges, sizeof(points)/sizeof(cv::Point), points, points);


        // 6. 이미지 와핑 (Perspective Transform)
        cv::Mat warpMat, warpMat_inv;
        cv::Mat _img_warp;
        //***************************** TUNING HERE *****************************//
        cv::Point2f points2f[4];
        points2f[0] = cv::Point2f( 56, 418);
        points2f[1] = cv::Point2f(225, 338);
        points2f[2] = cv::Point2f(460, 338);
        points2f[3] = cv::Point2f(660, 418);
        //***********************************************************************//
        // Apply the Perspective Transform just found to the src image
        calcPerspectiveTransform(img_roi, points2f, warpMat, warpMat_inv);
        cv::warpPerspective(img_roi, _img_warp, warpMat, _img_warp.size());


        // 7. 하단의 절반에서의 히스토그램 Peak Point 찾기
        cv::Mat img_warp;
        int peaks[2];
        _img_warp.copyTo(img_warp);

        bool hasLeftPeak  = findingPeak(img_warp,  peaks[0],  img_warp.rows/2,  img_warp.rows,                0,  img_warp.cols/2);
        bool hasRightPeak = findingPeak(img_warp,  peaks[1],  img_warp.rows/2,  img_warp.rows,  img_warp.cols/2,    img_warp.cols);

        if (hasLeftPeak == false)
            peaks[0] = img_warp.cols/4;
        if (hasRightPeak == false)
            peaks[1] = 3*img_warp.cols/4;


        // 8. Sliding Window
        //***************************** TUNING HERE *****************************//
        int division = param_division;
        int window_halfWidth = param_window_halfWidth;
        bool useFakeLane = param_useFakeLane;
        //***********************************************************************//
        std::vector<cv::Point2f> samplePoints_left, samplePoints_right;
        cv::Mat slidingMask = cv::Mat::zeros(img_warp.rows, img_warp.cols, CV_8UC1);
        cv::Mat slidingMask_left = getSlidingWindow(img_warp, division, window_halfWidth, samplePoints_left, true, peaks, useFakeLane);
        cv::Mat slidingMask_right = getSlidingWindow(img_warp, division, window_halfWidth, samplePoints_right, false, peaks, useFakeLane);
        cv::bitwise_or(slidingMask_left, slidingMask, slidingMask);
        cv::bitwise_or(slidingMask_right, slidingMask, slidingMask);
        cv::bitwise_and(slidingMask, _img_warp, _img_warp);


        // 9-1. Lane에 해당하는 pixel들을 찾음
        cv::Mat img_unwarp_left, img_unwarp_right;
        std::vector<cv::Point2f> points2d_left, points2d_right;

        points2d_left = findingLanePixels(_img_warp, img_unwarp_left, warpMat_inv, true, samplePoints_left);
        points2d_right = findingLanePixels(_img_warp, img_unwarp_right, warpMat_inv, false, samplePoints_right);


        // 9-2. Publishing an image for debugging
        cv::Mat img_debugging;
        img_debugging = img.clone();
        cv::cvtColor(img_unwarp_left, img_unwarp_left, CV_GRAY2RGB);
        cv::cvtColor(img_unwarp_right, img_unwarp_right, CV_GRAY2RGB);
        cv::addWeighted(img_unwarp_left, 0.4, img, 1.0, 0, img_debugging);
        cv::addWeighted(img_unwarp_right, 0.4, img_debugging, 1.0, 0, img_debugging);
        //cv::imshow("img_debugging", img_debugging);
        img_debugging.copyTo(img_output);


        // 10. 2d pixel point에서 3d vehicle coordinage point로 변환

        // debugging
        //std::vector<cv::Point2f> image_points;
        //image_points.push_back( cv::Point2f( 275, 372 ) );  // A
        //image_points.push_back( cv::Point2f( 304, 372 ) );  // B
        //image_points.push_back( cv::Point2f( 354, 372 ) );  // C
        //image_points.push_back( cv::Point2f( 242, 393 ) );  // D
        //image_points.push_back( cv::Point2f( 254, 393 ) );  // E
        //image_points.push_back( cv::Point2f( 393, 393 ) );  // F
        //image_points.push_back( cv::Point2f( 239, 395 ) );  // G
        //image_points.push_back( cv::Point2f( 395, 395 ) );  // H
        //image_points.push_back( cv::Point2f( 406, 394 ) );  // I
        //std::vector<cv::Point3f> points3d = twoDim2threeDim(image_points);
        //ROS_WARN_STREAM(std::endl << points3d);

        std::vector<cv::Point3f> points3d_left, points3d_right, points3d_middle;
        //ROS_WARN_STREAM("2d_left\n" << points2d_left);
        //ROS_WARN_STREAM("2d_right\n" << points2d_right);
        points3d_left = twoDim2threeDim(points2d_left);
        points3d_right = twoDim2threeDim(points2d_right);
        //ROS_WARN_STREAM("3d_left\n" << points3d_left);
        //ROS_WARN_STREAM("3d_right\n" << points3d_right);


        // 11. 3d vehicle coordinate points로 lane(a, b, c, d) 계산
        std::vector<double> laneCoeff_left, laneCoeff_right;
        laneCoeff_left = polyLaneFitting(points3d_left, "left_lane");
        laneCoeff_right = polyLaneFitting(points3d_right, "right_lane");

        //ROS_WARN_STREAM("abcd_left\n" << *laneCoeff_left.data());
        //ROS_WARN_STREAM("abcd_right\n" << *laneCoeff_right.data());

        // 12. 중앙 lane(a, b, c, d) 계산
        std::vector<double> laneCoeff_middle;
        laneCoeff_middle = findDrivingWay(laneCoeff_left, laneCoeff_right);

        kusv_msgs::PolyfitLaneData vision_lane;
        if (!laneCoeff_middle.empty()) {
            vision_lane.id = "vision_lane";
            vision_lane.a = laneCoeff_middle[3];
            vision_lane.b = laneCoeff_middle[2];
            vision_lane.c = laneCoeff_middle[1];
            vision_lane.d = laneCoeff_middle[0];
            m_lanePub_middle.publish(vision_lane);
        }


        // 13-1. lane(a, b, c, d) 결과 검증
        std::vector<cv::Point3f> testPoints3d_left, testPoints3d_right, testPoints3d_middle;
        std::vector<cv::Point> testPoints2d_left, testPoints2d_right, testPoints2d_middle;
        cv::Mat img_modified_left, img_modified_right, img_modified_combined;

        std::vector<cv::Point2f> points2d_middle;
        points2d_middle.push_back( cv::Point2f( 0, 343 ) );
        points2d_middle.push_back( cv::Point2f( 0, 353 ) );
        points2d_middle.push_back( cv::Point2f( 0, 363 ) );
        points2d_middle.push_back( cv::Point2f( 0, 373 ) );
        points2d_middle.push_back( cv::Point2f( 0, 383 ) );
        points2d_middle.push_back( cv::Point2f( 0, 393 ) );
        points2d_middle.push_back( cv::Point2f( 0, 403 ) );
        points2d_middle.push_back( cv::Point2f( 0, 413 ) );
        points3d_middle = twoDim2threeDim(points2d_middle);

        testPoints3d_left = getSamplingPoint(laneCoeff_left, points3d_left);
        testPoints3d_right = getSamplingPoint(laneCoeff_right, points3d_right);
        testPoints3d_middle = getSamplingPoint(laneCoeff_middle, points3d_middle);                                  // 중앙 lane(a, b, c, d)

        //ROS_WARN_STREAM("3d_left_test\n" << testPoints3d_left);
        //ROS_WARN_STREAM("3d_right_test\n" << testPoints3d_right);

        //ROS_WARN_STREAM("2d_middle" << points2d_middle);
        //ROS_WARN_STREAM("3d_middle" << points3d_middle);
        //ROS_WARN_STREAM("3d_middle_test" << testPoints3d_middle);

        testPoints2d_left = threeDim2twoDim(testPoints3d_left);
        testPoints2d_right = threeDim2twoDim(testPoints3d_right);
        testPoints2d_middle = threeDim2twoDim(testPoints3d_middle);                                // 중앙 lane(a, b, c, d)

        //ROS_WARN_STREAM("2d_left_test" << testPoints2d_left);
        //ROS_WARN_STREAM("2d_right_test" << testPoints2d_right);
        //ROS_WARN_STREAM("2d_middle_test" << testPoints2d_middle);

        makeFinalImage(               img,     img_modified_left,   testPoints2d_left,   RED);
        makeFinalImage( img_modified_left,    img_modified_right,  testPoints2d_right, GREEN);
        makeFinalImage(img_modified_right, img_modified_combined, testPoints2d_middle,  BLUE);     // 중앙 lane(a, b, c, d)

        //cv::namedWindow(TEST_WINDOW, cv::WINDOW_AUTOSIZE);
        //cv::imshow(TEST_WINDOW, img_modified_combined);


        // 13-2. Publishing lanes for debugging
        kusv_msgs::LanePointDataArray lanePoints;
        kusv_msgs::LanePointData lanePoint_left;
        kusv_msgs::LanePointData lanePoint_right;
        lanePoint_left = vector2lanePoints(points3d_left);
        lanePoint_right = vector2lanePoints(points3d_right);
        lanePoints.id = "lanes";
        //lanePoints.lane.push_back(lanePoint_left);
        //lanePoints.lane.push_back(lanePoint_right);
        lanePoints.lane.push_back(lanePoint_left);
        lanePoints.lane.push_back(lanePoint_right);
        m_pointsPub.publish(lanePoints);


        // 14. 결과를 화면에 보여줌
        cv::Mat img_warp_, img_edges_;
        cv::Mat img_result;
        std::vector<cv::Mat> img_array;
        cv::cvtColor(img_roi, img_roi, CV_GRAY2RGB);
        cv::cvtColor(_img_warp, img_warp_, CV_GRAY2RGB);
        cv::cvtColor(slidingMask, slidingMask, CV_GRAY2RGB);
        cv::addWeighted(slidingMask, 0.4, img_warp_, 0.6, 0, img_warp_);
        cv::addWeighted(img_undist, 1.0, img_roi, 0.5, 0, img_filtered);

        img_array.push_back(img_modified_combined);
        img_array.push_back(img_filtered);
        img_array.push_back(img_warp_);

        cv::hconcat(img_array, img_result);
        cv::namedWindow(RESULT_WINDOW, cv::WINDOW_AUTOSIZE);
        cv::imshow(RESULT_WINDOW, img_result);
    }
};







int main(int argc, char** argv) {
    ros::init(argc, argv, "lane_detector");
    LaneDetector detector;
    if (argc != 2) {
        ROS_WARN("No calibration file");
    }
    else {
        detector.dataRead(argv[1]);
    }
    ros::spin();
    return 0;
}
