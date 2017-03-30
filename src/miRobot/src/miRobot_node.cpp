#include "ros/ros.h"
#include "std_msgs/String.h"
#include <tf/transform_listener.h>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "miRobot_node");
  ros::NodeHandle node;
  //create publisher to write to whereInTheWorldIsMyHand topic
  ros::Publisher rPalmTransform = 
    node.advertise<geometry_msgs::Transform>("whereInTheWorldIsMyHand", 100);

  //object to recieve tf transformations
  tf::TransformListener listener;
  //rate to run loop, get hand position every second
  ros::Rate rate(1);

  //loop to get and publish hand position
  while (ros::ok()) {
    tf::StampedTransform transform;
    try {
      //store transform of right palm with respect to world in transform
      listener.lookupTransform("rightPalm", "world", ros::Time(0), transform);
    }
    //write error if exception thrown
    catch (tf::TransformException &ex) {
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
      continue;
    }
    //convert StampedTransform to geometry_msg and publish
    geometry_msgs::Transform rightPalm; 
    tf::transformTFToMsg(transform.getIdentity(), rightPalm);
    rPalmTransform.publish(rightPalm);

    //only find and publish every second
    rate.sleep();
  }
  return 0;
};
