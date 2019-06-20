#include <ros/ros.h>
#include "kusv_msgs/LanePointData.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <string>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <eigen3/Eigen/Eigen>
#include <geometry_msgs/Point.h>

// input : image
// output : driving_way

static std::string OPENCV_WINDOW = "result_video";

class LaneDetector
{
private:
    ros::NodeHandle nh_;
    image_transport::ImageTransport it_;
    image_transport::Subscriber image_sub_;
    image_transport::Publisher image_pub_;
    ros::Publisher point_pub_;
    cv::Mat cameraMat;
    cv::Mat distCoeff;
public:
    LaneDetector() : it_(nh_) {
        // Subscrive to input video feed and publish output video feed
        image_sub_ = it_.subscribe("videofile/image_raw", 10, &LaneDetector::imageCb, this);
        image_pub_ = it_.advertise("image_converter/output_video", 1);
        point_pub_ = nh_.advertise<kusv_msgs::LanePointData>("lane/point", 100);
    }
    ~LaneDetector() {
        cv::destroyWindow(OPENCV_WINDOW);
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

        bool isProcessed;
        isProcessed = processingImage(img);
        if (isProcessed == false && cv::waitKey(1) == 27) //ESC키 누르면 종료
            return;

        // Output modified video stream
        image_pub_.publish(cv_ptr->toImageMsg());
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
        //********** TODO **********//
        // READ DISTORTION COEFFICIENTS (camera.yaml)
        fn = fs["distortion_coefficients"];
        fn >> tmpMat;
        this->distCoeff = tmpMat;

        ROS_INFO_STREAM("camera_matrix: " << cameraMat.size());
        ROS_INFO_STREAM("distortion_camera: " << distCoeff.size());
    }

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

        // Filter white pixels
        cv::inRange(img_bgr, cv::Scalar(100,100,100), cv::Scalar(255,255,255), filtered);
        cv::bitwise_and(img_bgr, img_bgr, white, filtered);

        // Filter yellow pixels
        cv::cvtColor(img_bgr, img_hls, cv::COLOR_BGR2HLS);
        split(img_hls, hls);
        cv::threshold(hls[0], hls[0], 140, 255, cv::THRESH_BINARY_INV);
        cv::threshold(hls[1], hls[1], 90, 255, cv::THRESH_BINARY);
        cv::threshold(hls[2], hls[2], 100, 255, cv::THRESH_BINARY);
        cv::bitwise_and(hls[0], hls[1], _combined);
        cv::bitwise_and(hls[1], hls[2], combined);
        cv::bitwise_and(_combined, combined, combined);
        cv::bitwise_and(img_bgr, img_bgr, yellow, combined);

        // White + Yellow
        //Combine the two above images
        cv::bitwise_or(white, yellow, img_filtered);
        return img_filtered;
    }

    cv::Mat sobelXY(const cv::Mat img, int thresh, int kernel_size=5) {
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
        addWeighted(abs_grad_x, 0.8, abs_grad_y, 0.5, 0, img_gradXY);
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
        //********** TODO **********//
        // CALCULATE THE MAGNITUDE OF GRADIENT (pow, add)
        cv::pow(abs_grad_y, 2.0, abs_grad_y);
        cv::add(abs_grad_x, abs_grad_y, img_gradMag);

        cv::normalize(img_gradMag, img_gradMag, 0, 255, cv::NORM_MINMAX);
        cv::threshold(img_gradMag, img_gradMag, thresh, 255, CV_THRESH_BINARY);
        return img_gradMag;
    }

    cv::Mat getGradientThresh(const cv::Mat img_gray_blur) {
        cv::Mat img_gradXY = sobelXY(img_gray_blur, 230);
        cv::Mat img_gradMag = sobelMag(img_gray_blur, 230);

        cv::Mat img_grad;
        cv::bitwise_and(img_gradXY, img_gradMag, img_grad);
        return img_grad;
    }

    cv::Mat regionOfInterest(const cv::Mat img_edges, int num, const cv::Point *points) {
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

    void applyPerspectiveTransform(const cv::Mat img_input, cv::Mat& img_output, const cv::Point2f* inputQuad, cv::Mat& lambda) {
        // Output Quadilateral or World plane coordinates
        cv::Point2f outputQuad[4];

        // Set the lambda matrix the same type and size as input
        lambda = cv::Mat::zeros(img_input.rows, img_input.cols, img_input.type());

        // The 4 points where the mapping is to be done , from bottom-left in clockwise order
        outputQuad[0] = cv::Point2f(0, img_input.rows-1);
        //********** TODO **********//
        // FILL OUT THE RSET OF POINTS
        outputQuad[1] = cv::Point2f(0, 0);
        outputQuad[2] = cv::Point2f(img_input.cols-1,0);
        outputQuad[3] = cv::Point2f(img_input.cols-1,img_input.rows-1);

        // Get the Perspective Transform Matrix i.e. lambda
        lambda = getPerspectiveTransform(inputQuad, outputQuad);
        // Apply the Perspective Transform just found to the src image
        warpPerspective(img_input, img_output, lambda, img_output.size());
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

    void findingPeak(const cv::Mat img, cv::Mat& img_hist, int& peaks, int top, int bottom, int left, int right) {
        sortWindow(img.size(), top, bottom, left, right);
        img_hist = img(cv::Range(top, bottom), cv::Range(left, right));

        cv::Mat colSum;
        cv::reduce(img_hist, colSum, 0, CV_REDUCE_SUM, CV_64F);
        cv::normalize(colSum, colSum, 0, img_hist.rows, cv::NORM_MINMAX, CV_8U);
        for(int i = 1; i < colSum.cols; i++) {
            cv::line(img_hist, cv::Point(i-1, (int)(*colSum.col(i-1).data)), cv::Point(i, (int)(*colSum.col(i).data)), cv::Scalar(255, 255, 255), 1, 8);
        }

        double minVal, maxVal;
        cv::Point minLoc, maxLoc;
        cv::minMaxLoc(colSum, &minVal, &maxVal, &minLoc, &maxLoc);
        peaks = left + maxLoc.x;
    }

    cv::Mat getSlidingWindow(const cv::Mat img, int division, int win_half_width, int* init_peaks=nullptr) {
        cv::Mat slidingMask = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
        cv::Mat mask(slidingMask);
        for (int j = 0; j < 2; j++) {
            cv::Mat img_hist;
            int peak, last_peak;
            peak = init_peaks[j];
            for (int i = 0; i < division; i++) {
                last_peak = peak;
                //********** TODO **********//
                // FIND THE NEXT PEAK AND MASK
                int top = (img.rows/division-1) * i;
                int bottom = (img.rows/division-1) * (i+1);
                int left = last_peak - win_half_width;
                int right = last_peak + win_half_width;
                sortWindow(img.size(), top, bottom, left, right);
                mask(cv::Rect(cv::Point(left, top), cv::Point(right, bottom))) = 255;
                findingPeak(mask, img_hist, peak, top, bottom, left, right);

                cv::bitwise_or(mask, slidingMask, slidingMask);
            }
        }
        return slidingMask;
    }

    void pubLinePixels(kusv_msgs::LanePointData lane) {
        point_pub_.publish(lane);
    }

    Eigen::Vector4d polyfitLane(const cv::Mat img_lane, int w_l, int w_r, int sample = 1) {
        int w_t = 0;
        int w_b = img_lane.rows;
        sortWindow(img_lane.size(), w_t, w_b, w_l, w_r);
        cv::Mat img_lane_ = img_lane(cv::Range(w_t, w_b), cv::Range(w_l, w_r));

        std::vector<cv::Point> line_pixels;
        kusv_msgs::LanePointData lane;
        geometry_msgs::Point pixel;

        for (int i = 0; i < img_lane_.size().width; i++) {
            for (int j = 0; j < img_lane_.size().height; j++) {
                if ((img_lane_.at<int>(i,j) != 0) && (j % sample == 0)) {
                    //********** TODO **********//
                    // FIND DETECTED LINE PIXELS
                }
            }
        }
        pubLinePixels(lane);
        Eigen::MatrixXd xMat(line_pixels.size(), 4);
        Eigen::VectorXd yVec(line_pixels.size());
        Eigen::Vector4d aVec;

        for (std::vector<cv::Point>::iterator it = line_pixels.begin(); it != line_pixels.end(); it++) {
            int i = 0;
            double x = (*it).x;
            double y = (*it).y;
            if (i % sample == 0) {
                xMat(i, 0) = 1;
                xMat(i, 1) = x;
                xMat(i, 2) = x * x;
                xMat(i, 3) = x * x * x;
                yVec(i) = y;
            }
            i++;
        }
        
        //********** TODO **********//
        // CALCULATE THE A_VECTOR

        ROS_WARN_STREAM("left: " << w_l << ", ["<< aVec.x() << ", " << aVec.y() << ", " << aVec.z() << ", " << aVec.w() << "]");
        return aVec;
    }

    bool processingImage(const cv::Mat img) {
        // 1. 원본 영상을 읽어옴
        if (img.empty())
            return false;


        // 2. 왜곡 보정
        cv::Mat img_undist;
        if (!cameraMat.empty() && !distCoeff.empty()) {
            cv::undistort(img, img_undist, cameraMat, distCoeff);
        }


        // 3. 미리 정해둔 노란색 범위 내에 있는 부분만 차선후보로 따로 저장함
        int kernel_size = 5;
        cv::Mat img_filtered, img_blur_;
        img_undist.copyTo(img_blur_);
        GaussianBlur(img_blur_, img_blur_, cv::Size(kernel_size, kernel_size), 0);
        img_filtered = filteringColor(img_blur_);


        // 4. 그레이스케일 영상으로 변환하여 에지 성분을 추출
        cv::Mat img_blur, img_edges;
        img_blur = grayAndBlur(img_filtered);
        img_edges = getGradientThresh(img_blur);


        // 5. 차선 검출할 영역을 제한함(진행방향 바닥에 존재하는 차선으로 한정)
        int width = img_filtered.cols;
        int height = img_filtered.rows;

        cv::Point points[4];
        points[0] = cv::Point(60, height-30);
        points[1] = cv::Point(width/2-60, height/2+50);
        points[2] = cv::Point(width/2+60, height/2+50);
        points[3] = cv::Point(width, height-30);

        cv::Mat img_roi;
        img_roi = regionOfInterest(img_edges, sizeof(points)/sizeof(cv::Point), points);


        // 6. 이미지 와핑 (Perspective Transform)
        cv::Mat warpMat(2, 4, CV_32FC1), _img_warp;

        cv::Point2f points2f[4];
        points2f[0] = cv::Point2f(60, height-30);
        points2f[1] = cv::Point2f(width/2-60, height/2+50);
        points2f[2] = cv::Point2f(width/2+60, height/2+50);
        points2f[3] = cv::Point2f(width, height-30);

        applyPerspectiveTransform(img_roi, _img_warp, points2f, warpMat);


        // 7. 하단의 절반에서의 히스토그램 Peak Point 찾기
        cv::Mat img_warp, img_hist;
        int peaks[2];
        _img_warp.copyTo(img_warp);
        findingPeak(img_warp, img_hist, peaks[0], 0, img_warp.rows/2, 0, img_warp.cols/2);
        findingPeak(img_warp, img_hist, peaks[1], 0, img_warp.rows/2, img_warp.cols/2, img_warp.cols);
        //ROS_INFO_STREAM("peaks: [" << peaks[0] << ", " << peaks[1] << "]");


        // 8. Sliding Window
        int division = 6;
        cv::Mat slidingMask = getSlidingWindow(img_warp, division, 60, peaks);
        cv::bitwise_and(slidingMask, _img_warp, _img_warp);


        // 9. Curve Fitting
        Eigen::Vector4d left_line, right_line;
        left_line = polyfitLane(_img_warp, 0, _img_warp.cols/2, 200);
        //right_line = polyfitLane(_img_warp, _img_warp.cols/2, _img_warp.cols-1, 200);
        //ROS_WARN_STREAM(*(left_line.data()) <<", "<<*(right_line.data()));


        // 10. 결과를 화면에 보여줌
        cv::Mat img_warp_, img_edges_;
        cv::Mat img_result;
        std::vector<cv::Mat> img_array;
        cv::cvtColor(img_roi, img_roi, CV_GRAY2RGB);
        cv::cvtColor(_img_warp, img_warp_, CV_GRAY2RGB);
        cv::cvtColor(slidingMask, slidingMask, CV_GRAY2RGB);
        cv::addWeighted(slidingMask, 0.4, img_warp_, 0.6, 0, img_warp_);
        cv::addWeighted(img_filtered, 1.0, img_roi, 0.5, 0, img_filtered);

        img_array.push_back(img_undist);
        img_array.push_back(img_filtered);
        img_array.push_back(img_warp_);

        cv::hconcat(img_array, img_result);
        cv::namedWindow(OPENCV_WINDOW, cv::WINDOW_AUTOSIZE);
        cv::imshow(OPENCV_WINDOW, img_result);
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
