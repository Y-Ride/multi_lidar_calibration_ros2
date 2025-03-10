#include <multi_lidar_calibration/multi_lidar_calibration_ndt.hpp>

namespace calibration
{
namespace multi_lidar_calibration_ndt
{

MultiLidarCalibrationNdt::MultiLidarCalibrationNdt()
: Node("multi_lidar_calibration_ndt"),
  pointcloud_source_sub_(this, "input/source_pointcloud", rmw_qos_profile_sensor_data),
  pointcloud_target_sub_(this, "input/target_pointcloud", rmw_qos_profile_sensor_data),
  sync_(SyncPolicy(10), pointcloud_source_sub_, pointcloud_target_sub_)
{
  sync_.setMaxIntervalDuration(rclcpp::Duration(1.0, 0));
  // initialize parameters of node
  param_.initial_pose =
    declare_parameter<std::vector<double>>("initial_pose", {0.0, 0.0, 0.0, 0.0, 1.57, 0.0});
  param_.leaf_size = declare_parameter<double>("leaf_size", 0.1);
  param_.max_iteration = declare_parameter<int>("max_iteration", 100);
  param_.transform_epsilon = declare_parameter<double>("transform_epsilon", 0.01);
  param_.step_size = declare_parameter<double>("step_size", 0.1);
  param_.resolution = declare_parameter<double>("resolution", 0.5);

  approximate_voxel_filter_.setLeafSize(param_.leaf_size, param_.leaf_size, param_.leaf_size);

  ndt_.setMaximumIterations(param_.max_iteration);
  ndt_.setTransformationEpsilon(param_.transform_epsilon);
  ndt_.setStepSize(param_.step_size);
  ndt_.setResolution(param_.resolution);

  Eigen::Translation3f initial_translation(
    param_.initial_pose.at(0), param_.initial_pose.at(1), param_.initial_pose.at(2));

  Eigen::AngleAxisf initial_rotation_x(param_.initial_pose.at(3), Eigen::Vector3f::UnitX());
	Eigen::AngleAxisf initial_rotation_y(param_.initial_pose.at(4), Eigen::Vector3f::UnitY());
	Eigen::AngleAxisf initial_rotation_z(param_.initial_pose.at(5), Eigen::Vector3f::UnitZ());
  current_transform_mtraix_ =
    (initial_translation * initial_rotation_x * initial_rotation_y * initial_rotation_z)
      .matrix();
  std::cout << "initial guess: " << std::endl << current_transform_mtraix_ << std::endl;

  // tf2 broadcaster
  tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);

  sync_.registerCallback(std::bind(
    &MultiLidarCalibrationNdt::callbackLidars, this, std::placeholders::_1,
    std::placeholders::_2));

  // Print out the topic names
  // std::cout << "Subscribed to: " << pointcloud_source_sub_.getTopic() << std::endl;
  // std::cout << "Subscribed to: " << pointcloud_target_sub_.getTopic() << std::endl;
}

MultiLidarCalibrationNdt::~MultiLidarCalibrationNdt()
{
}

void MultiLidarCalibrationNdt::callbackLidars(const sensor_msgs::msg::PointCloud2::ConstSharedPtr & point_1,
    const sensor_msgs::msg::PointCloud2::ConstSharedPtr & point_2)
{
  // std::cout << "Inside of callback. Got point clouds" << std::endl;
  rclcpp::Time start_time = this->now();
  pcl::PointCloud<pcl::PointXYZI>::Ptr source_pointcloud(new pcl::PointCloud<pcl::PointXYZI>);
  pcl::PointCloud<pcl::PointXYZI>::Ptr target_pointcloud (new pcl::PointCloud<pcl::PointXYZI>);
  pcl::PointCloud<pcl::PointXYZI>::Ptr final_pointcloud (new pcl::PointCloud<pcl::PointXYZI>);

  pcl::PointCloud<pcl::PointXYZI>::Ptr filtered_source_pointcloud(
    new pcl::PointCloud<pcl::PointXYZI>);
  pcl::PointCloud<pcl::PointXYZI>::Ptr filtered_target_pointcloud(
    new pcl::PointCloud<pcl::PointXYZI>);

  pcl::fromROSMsg(*point_1, *source_pointcloud);
  pcl::fromROSMsg(*point_2, *target_pointcloud);

  // for (const auto& point : source_pointcloud->points) {
  //   if (!pcl::isFinite(point)) {
  //       std::cerr << "NaN or Inf detected in source point cloud!" << std::endl;
  //   }
  // }

  // for (const auto& point : target_pointcloud->points) {
  //     if (!pcl::isFinite(point)) {
  //         std::cerr << "NaN or Inf detected in target point cloud!" << std::endl;
  //     }
  // }

  // // Print first 20 points from source point cloud
  // std::cout << "Source PointCloud (first 20 points):" << std::endl;
  // for (size_t i = 0; i < std::min<size_t>(20, source_pointcloud->points.size()); ++i) {
  //     std::cout << "Point " << i << ": ("
  //               << source_pointcloud->points[i].x << ", "
  //               << source_pointcloud->points[i].y << ", "
  //               << source_pointcloud->points[i].z << ")" << std::endl;
  // }

  // // Print first 20 points from target point cloud
  // std::cout << "Target PointCloud (first 20 points):" << std::endl;
  // for (size_t i = 0; i < std::min<size_t>(20, target_pointcloud->points.size()); ++i) {
  //     std::cout << "Point " << i << ": ("
  //               << target_pointcloud->points[i].x << ", "
  //               << target_pointcloud->points[i].y << ", "
  //               << target_pointcloud->points[i].z << ")" << std::endl;
  // }

  // Print the size of the source and target point clouds
  // std::cout << "Source cloud size: " << source_pointcloud->size() << std::endl;
  // std::cout << "Target cloud size: " << target_pointcloud->size() << std::endl;

  // Remove NaNs from source point cloud
  pcl::PointCloud<pcl::PointXYZI>::Ptr filtered_source(new pcl::PointCloud<pcl::PointXYZI>());
  std::vector<int> indices;
  pcl::removeNaNFromPointCloud(*source_pointcloud, *filtered_source, indices);

  // Remove NaNs from target point cloud
  pcl::PointCloud<pcl::PointXYZI>::Ptr filtered_target(new pcl::PointCloud<pcl::PointXYZI>());
  pcl::removeNaNFromPointCloud(*target_pointcloud, *filtered_target, indices);

  // std::cout << "Filtered source cloud size: " << filtered_source->size() << std::endl;
  // std::cout << "Filtered target cloud size: " << filtered_target->size() << std::endl;

  // return;
  approximate_voxel_filter_.setInputCloud(filtered_target);
  approximate_voxel_filter_.filter(*filtered_target_pointcloud);

  ndt_.setInputSource(filtered_target_pointcloud);
  ndt_.setInputTarget(filtered_source);

  ndt_.align(*final_pointcloud, current_transform_mtraix_);

  if (ndt_.hasConverged())
  {
    current_transform_mtraix_ = ndt_.getFinalTransformation();
    std::cout << "NDT converged." << std::endl
              << "The score is " << ndt_.getFitnessScore() << std::endl;
    std::cout << "Transformation matrix:" << std::endl;
    std::cout << current_transform_mtraix_ << std::endl;
    Eigen::Matrix3f rotation_matrix = current_transform_mtraix_.block(0, 0, 3, 3);
    Eigen::Vector3f translation_vector = current_transform_mtraix_.block(0, 3, 3, 1);
    std::cout << "This transformation can be replicated using:" << std::endl;
    std::cout << "ros2 run tf2_ros static_transform_publisher " << translation_vector.transpose()
              << " " << rotation_matrix.eulerAngles(2,1,0).transpose() << " " << point_1->header.frame_id.c_str() 
              << " " << point_2->header.frame_id.c_str() << std::endl;

    Eigen::Quaternionf q(rotation_matrix);
    geometry_msgs::msg::TransformStamped t;
    t.header.stamp = point_1->header.stamp;
    t.header.frame_id = point_1->header.frame_id;
    t.child_frame_id = point_2->header.frame_id;
    t.transform.translation.x = translation_vector[0];
    t.transform.translation.y = translation_vector[1];
    t.transform.translation.z = translation_vector[2];
    t.transform.rotation.x = q.x();
    t.transform.rotation.y = q.y();
    t.transform.rotation.z = q.z();
    t.transform.rotation.w = q.w();
    tf_broadcaster_->sendTransform(t);
  }
  rclcpp::Time end_time = this->now();
  std::cout << "process time: " << (end_time - start_time).seconds() * 1000.0 << "ms.\n\n";
}

} // namespace multi_lidar_calibration_ndt
} // calibration 