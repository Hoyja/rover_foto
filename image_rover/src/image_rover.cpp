#include <ros/ros.h>
#include <kobuki_msgs/BumperEvent.h>
#include <sensor_msgs/Image.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Accel.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
ros::Subscriber sub_bumper;

int i = 0;

void clbk_bumper(const kobuki_msgs::BumperEvent::ConstPtr& msg){
if(msg->state == kobuki_msgs::BumperEvent::PRESSED)
{

system("/home/jakob/catkin_ws/src/image_rover/src/abort.sh");

i++;
ros::Duration(0.5).sleep();

  ros::NodeHandle n;
  ros::Publisher pub_twist = n.advertise<geometry_msgs::Twist>("/cmd_vel_mux/input/teleop", 1000);
  ros::Publisher pub_accl = n.advertise<geometry_msgs::Accel>("/cmd_vel_mux/input/teleop", 1000);
  if(msg->bumper == 1){
    ROS_INFO("Center bumper activated!");

for (int a = 0; a < 7.15; a++){
    geometry_msgs::Twist msg;

      msg.linear.x = 0;
      msg.angular.z = 1.1;
      std::cout<< a << std::endl;
      pub_twist.publish(msg);
      ros::Duration(0.5).sleep();}

for (int b = 0; b < 5; b++){
    geometry_msgs::Accel msg;

      msg.linear.x = 0.3;
      msg.angular.z = 0;
      std::cout<< b << std::endl;
      pub_accl.publish(msg);
      ros::Duration(0.5).sleep();
}
for (int a = 0; a < 6.9; a++){
    geometry_msgs::Twist msg;

      msg.linear.x = 0;
      msg.angular.z = 1.1;
      std::cout<< a << std::endl;
      pub_twist.publish(msg);
      ros::Duration(0.5).sleep();}
  }
  else if(msg->bumper == 0){
    ROS_INFO("Left bumper activated!");

    for (int a = 0; a < 5.3625; a++){
    geometry_msgs::Twist msg;

      msg.linear.x = 0;
      msg.angular.z = -1.1;
      std::cout<< a << std::endl;
      pub_twist.publish(msg);
      ros::Duration(0.5).sleep();}

for (int b = 0; b < 5; b++){
    geometry_msgs::Accel msg;

      msg.linear.x = 0.3;
      msg.angular.z = 0;
      std::cout<< b << std::endl;
      pub_accl.publish(msg);
      ros::Duration(0.5).sleep();
  }
  for (int a = 0; a < 6.9; a++){
      geometry_msgs::Twist msg;

        msg.linear.x = 0;
        msg.angular.z = 1.1;
        std::cout<< a << std::endl;
        pub_twist.publish(msg);
        ros::Duration(0.5).sleep();}
}
  else if(msg->bumper == 2){
    ROS_INFO("Right bumper activated!");

for (int a = 0; a < 5.3625; a++){
    geometry_msgs::Twist msg;

      msg.linear.x = 0;
      msg.angular.z = 1.1;
      std::cout<< a << std::endl;
      pub_twist.publish(msg);
      ros::Duration(0.5).sleep();}

for (int b = 0; b < 5; b++){
    geometry_msgs::Accel msg;

      msg.linear.x = 0.3;
      msg.angular.z = 0;
      std::cout<< b << std::endl;
      pub_accl.publish(msg);
      ros::Duration(0.5).sleep();
  }
  for (int a = 0; a < 6.9; a++){
      geometry_msgs::Twist msg;

        msg.linear.x = 0;
        msg.angular.z = 1.1;
        std::cout<< a << std::endl;
        pub_twist.publish(msg);
        ros::Duration(0.5).sleep();}
}

}

}


static const std::string OPENCV_WINDOW = "Processed image";

char filename[80];

class ImageConverter
{
 ros::NodeHandle nh_;
 image_transport::ImageTransport it_;
 image_transport::Subscriber image_sub_;
 image_transport::Publisher image_pub_;

public:
 ImageConverter()
   : it_(nh_)
 {
    image_sub_ = it_.subscribe("/camera/rgb/image_raw", 1, &ImageConverter::imageCb, this);
    image_pub_ = it_.advertise("/camera/image_processed", 1);

   cv::namedWindow(OPENCV_WINDOW);
 }

 ~ImageConverter()
 {
   cv::destroyWindow(OPENCV_WINDOW);
 }

 void imageCb(const sensor_msgs::ImageConstPtr& msg)
 {
   cv_bridge::CvImagePtr cv_ptr;
   try
   {
     cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::RGB8);
   }
   catch (cv_bridge::Exception& e)
   {
     ROS_ERROR("cv_bridge exception: %s", e.what());
     return;
   }
   sprintf(filename,"obstacle_%d.png", i);
     cv::imwrite(filename,cv_ptr->image);
 }
};


int main(int argc, char **argv)
{
  ros::init(argc, argv, "image_rover");
  ros::NodeHandle n;

  sub_bumper = n.subscribe("/mobile_base/events/bumper", 1, clbk_bumper);
  ros::Publisher pub_twist = n.advertise<geometry_msgs::Twist>("/cmd_vel_mux/input/teleop", 1000);
  ros::Publisher pub_accl = n.advertise<geometry_msgs::Accel>("/cmd_vel_mux/input/teleop", 1000);

ros::Duration(15).sleep();

  ImageConverter ic;


ros::Duration(0.5).sleep();
ros::spin();
return 0;

}
