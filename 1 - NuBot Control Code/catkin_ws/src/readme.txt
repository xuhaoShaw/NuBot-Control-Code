0/启动数梅派与arduino通信节点
rosrun rosserial_python serial_node.py /dev/ttyACM0

1/delta_III_ros是激光雷达的驱动程序，启动激光雷达的步骤
sudo chmod 777 /dev/ttyUSB0
rosrun delta_lidar delta_lidar_node or roslaunch  delta_lidar delta_lidar.launch

2/tf_publish
一个节点发布/odom话题和odom->baselink的tf变换
一个节点发布机器人的静态tf变换，比如laser->base_link,IMU->base_link

3/gmapping_slam
用于gmapping_slam建图，launch文件中包含了激光雷达的启动指令，不需要先启动激光雷达
roslaunch gmapping_slam gmapping_slam.launch

4/hector_slam
用于hector_slam建图,launch文件中包含了激光雷达的启动指令，不需要先启动激光雷达
roslaunch hector_slam hector_slam.launch

5/open_karto/slam_karto/sparse_bundle_adjustment/这三个功能包是开源的用于Karto_slam的功能包
karto_slam是我自己写的功能包，里面包含一个Karto_slam.launch文件，用于直接进行karto_slam操作。
launch文件中包含了激光雷达的启动指令，不需要先启动激光雷达
roslaunch karto_slam karto_slam.launch

