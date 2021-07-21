#include "ros/ros.h"
#include "visualization_msgs/Marker.h"
#include "geometry_msgs/Twist.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

ros::Publisher pub;
int sleep_cnt = 0;

void msgCallback(const visualization_msgs::Marker::ConstPtr&msg)
{   
	ros::NodeHandle nh;    
    pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel",100);
    //pub = nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel",100);
    geometry_msgs::Twist pub_msg;
    ros::Rate r(1);

    int id = msg->id;
    double point_x = msg->pose.position.x;
	double point_y = msg->pose.position.y;
    double point_z = msg->pose.position.z;
    double ori_x = msg->pose.orientation.x;
    double ori_y = msg->pose.orientation.y;
    double ori_z = msg->pose.orientation.z;
    double ori_w = msg->pose.orientation.w;

    ROS_INFO("id = %d", id);
    ROS_INFO("point_x = %f", point_x);
    ROS_INFO("distance = %f", point_z);

    if ( (id == 3) || (id == 6) || (id == 9) || (id == 12) )
    {
        if (point_x > 0.1)
        {
            ROS_INFO("turn right");
            pub_msg.linear.x = 0;
            pub_msg.angular.z = -0.1;
            pub.publish(pub_msg);
        }
        else if (point_x < -0.1)
        {
            ROS_INFO("turn left");
            pub_msg.linear.x = 0;
            pub_msg.angular.z = 0.1;
            pub.publish(pub_msg);
        }
        else // if ((point_x <= 0.02) && (point_x >= -0.02))
        {
            if (point_z > 0.77)
            {
                ROS_INFO("go");
                pub_msg.linear.x = 0.2;
                pub.publish(pub_msg);
            }
            else
            {
                ROS_INFO("stop");
                ros::Duration(2.0).sleep();
                pub_msg.linear.x = 0;
                pub_msg.angular.z = 0;
                pub.publish(pub_msg);
                if ( (id == 3) || (id == 6) || (id == 9) || (id == 12) )
                {
                    pub_msg.angular.z = -1.5707963268/4.0;
                    pub.publish(pub_msg);
                    sleep_cnt++;
                    if (sleep_cnt > 4)
                    {
                        ROS_INFO("turn at edge");
                        pub_msg.angular.z = 0;
                        pub.publish(pub_msg);
                        ros::Duration(2.0).sleep();
                        pub_msg.linear.x = 0.2;
                        pub.publish(pub_msg);
                        ros::Duration(2.0).sleep();
                        pub_msg.angular.z = 0;
                        pub.publish(pub_msg);
                        sleep_cnt = 0;   
                    }
                }
                // pub_msg.linear.x = 0.2;
                // pub.publish(pub_msg);
                // ros::Duration(1.0).sleep();
                // if (msg->pose.position.z > 0.77)
                // {
                //     pub_msg.linear.x = 0.2;
                //     pub.publish(pub_msg);
                // }
            }
        }
    }
    else
    {
        ROS_INFO("Go foward");
        pub_msg.linear.x = 0.1;
        pub.publish(pub_msg);
    }
    // r.sleep();
}

int main(int argc, char **argv)
{
    ros::init(argc,argv,"go_stop");
	
	ros::NodeHandle nh;

    ros::Subscriber sub = nh.subscribe("/aruco_single/marker",10,msgCallback);	

	ros::spin();

	return 0;
}