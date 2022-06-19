# NuBot-Control-Code

包含核电厂磁吸附小车的控制部分程序。

机器人控制系统包含两部分：上位机为树莓派，下位机为arduino。

上位机负责感知和决策，包括环境信息的感知（获取激光雷达点云信息）、规划运动路径；

下位机负责执行和反馈，包含下发运动指令给电机、反馈电机编码器信号给树莓派。



## NuBot Control Code

### Arduino

下位机arduino的控制程序，包含运动指令的设置、编码器信号的处理，及PID控制器。

### catkin_ws

- 启动数梅派与arduino通信节点

rosrun rosserial_python serial_node.py /dev/ttyACM0

- delta_III_ros

激光雷达的驱动程序，启动激光雷达的步骤

sudo chmod 777 /dev/ttyUSB0
rosrun delta_lidar delta_lidar_node or roslaunch  delta_lidar delta_lidar.launch

- tf_publish

一个节点发布/odom话题和odom->baselink的tf变换
一个节点发布机器人的静态tf变换，比如laser->base_link,IMU->base_link

- gmapping_slam

用于gmapping_slam建图，launch文件中包含了激光雷达的启动指令，不需要先启动激光雷达
roslaunch gmapping_slam gmapping_slam.launch

- hector_slam

用于hector_slam建图,launch文件中包含了激光雷达的启动指令，不需要先启动激光雷达
roslaunch hector_slam hector_slam.launch

- open_karto/slam_karto/sparse_bundle_adjustment/

这三个功能包是开源的用于Karto_slam的功能包

karto_slam是我自己写的功能包，里面包含一个Karto_slam.launch文件，用于直接进行karto_slam操作。
launch文件中包含了激光雷达的启动指令，不需要先启动激光雷达
roslaunch karto_slam karto_slam.launch



## 论文

已经发表的ICIRA会议论文，及尚在二次审稿的RCIM论文支撑材料。
