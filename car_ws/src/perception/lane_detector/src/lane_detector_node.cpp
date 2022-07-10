// Standard libs
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include "sensor_msgs/Image.h"

// OpenCV deps
#include <opencv4/opencv2/imgproc/imgproc.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>

// Msg types
#include "car_msgs/Lane.h"

class LaneDetector
{
    ros::NodeHandle nh;
    car_msgs::Lane lanes;
    image_transport::ImageTransport it;
    image_transport::Subscriber image_sub_;

public:
    LaneDetector() : it(nh)
    {
        image_sub_ = it.subscribe("/usb_cam/image_raw", 1, &LaneDetector::imageCb, this);
        ros::Publisher pub = nh.advertise<car_msgs::Lane>("/lane", 1);
    }

    ~LaneDetector(){}

    void imageCb(const sensor_msgs::ImageConstPtr &msg)
    {
        cv_bridge::CvImagePtr cv_ptr;
        namespace enc = sensor_msgs::image_encodings;

        try
        {
            cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        }
        catch (cv_bridge::Exception &e)
        {
            ROS_ERROR("cv_bridge exception: %s", e.what());
            return;
        }
        detect_edges(cv_ptr->image);
        // image_pub_.publish(cv_ptr->toImageMsg());

    }
    void detect_edges(cv::Mat img)
    {
        cv::Mat src, src_gray;
        cv::Mat dst, detected_edges;

        int edgeThresh = 1;
        int lowThreshold = 200;
        int highThreshold = 300;
        int kernel_size = 5;

        img.copyTo(src);

        cv::cvtColor(img, src_gray, CV_BGR2GRAY);
        cv::blur(src_gray, detected_edges, cv::Size(5, 5));
        cv::Canny(detected_edges, detected_edges, lowThreshold, highThreshold, kernel_size);

        dst = cv::Scalar::all(0);
        img.copyTo(dst, detected_edges);
        dst.copyTo(img);
    }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "lane_detector");
    LaneDetector ic;
    ros::spin();
    return 0;
}

//     ros::NodeHandle nh;
//     image_transport::ImageTransport it;

//     // Parameters & Attributes

//     LaneDetector : it(nh) {

//         ros::Publisher pub = nh.advertise<car_msgs::Lane>("/lane", 1);
//         image_transport::ImageTransport image_sub = it.subscribe<sensor_msgs::Image>("/front_image", 1, &LaneDetector::imageCB);
//     }
//     ~LaneDetector(){}

//     car_msgs::Lane laneDetection;

//     void imageCB(const sensor_msgs::ImageConstPtr &msg)
//     {
//         cv_bridge::CvImagePtr cv_ptr;

//         try
//         {
//             cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);

//             // image is cv_ptr->image;

//             // Find Lanes from Image

//             // Publish Lanes
//         }

//         catch (cv_bridge::Exception &e)
//         {
//             ROS_ERROR("cv_bridge exception: %s", e.what());
//             return;
//         }
//     }

//     // Methods for processing images & detecting lanes
// };

// int main(int argc, char **argv)
// {
//     ros::init(argc, argv, "3d_object_detector");

//     LaneDetector object_detector = LaneDetector();
//     ros::spin();

// }
