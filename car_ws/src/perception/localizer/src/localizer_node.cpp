// Standard libs
#include <ros/ros.h>
#include <pcl_ros/point_cloud.h>
#include "pcl-1.10/pcl/point_types.h"

// Msg types
#include "car_msgs/LidarObjectDetect.h"
#include "sensor_msgs/NavSatFix.h"


using namespace std;

typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;

class Localizer {

    public:

    ros::Subscriber cloudSub;
    ros::Subscriber gpsSub;
    ros::Publisher pub;

    // Parameters & Attributes
    PointCloud cloud;
    sensor_msgs::NavSatFix gps;
    car_msgs::LidarObjectDetect globalPose;
    
    void cloudCB(const PointCloud::ConstPtr &msg)
    {
        this->cloud = *msg;
    }

    void gpsCB(const sensor_msgs::NavSatFix::ConstPtr &msg){
        this->gps = *msg;
    }

    Localizer (ros::NodeHandle &nh){

        cloudSub = nh.subscribe<PointCloud>("/pointcloud", 1, &Localizer::cloudCB, this);
        gpsSub = nh.subscribe<sensor_msgs::NavSatFix>("/gps", 1, &Localizer::gpsCB, this);
        pub = nh.advertise<car_msgs::LidarObjectDetect> ("/globalPose", 1);
    }

    // Methods for processing point clouds & gps and estimation of global pose

};

    

int main(int argc, char **argv)
{
    ros::init(argc, argv, "3d_object_detector");
    ros::NodeHandle nh;
    Localizer object_detector = Localizer(nh);
    ros::spin();
}
