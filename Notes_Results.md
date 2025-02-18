# Calibration Notes

## **__ TODO:__**

Redo the calibration but use `all_lidars_all_topics_down_ramp` instead of `all_lidars_360_outside`

## Top

This is the main reference lidar


## Left

**Initial:**
- "initial_pose"="[-0.0889, -0.73025, -0.2413, 0.853466, 0.0, 2.35619]"
- initial guess: 
 -0.707104  -0.70711         0   -0.0889
  0.464837 -0.464832 -0.753563  -0.73025
  0.532852 -0.532847  0.657375   -0.2413
         0         0         0         1

**After NDT Calibration:**
- NDT converged.
- The score is 8.89009
- Transformation matrix:
 -0.714313  -0.699821 0.00274721 -0.0769035
  0.442218  -0.454411  -0.773275  -0.761647
  0.542402  -0.551145   0.634065  -0.233036
         0          0          0          1
- This transformation can be replicated using:
ros2 run tf2_ros static_transform_publisher -0.0769035  -0.761647  -0.233036   2.58726 -0.573294  -0.71555 os_lidar_top os_lidar_left


## Right

**Initial:**
- "initial_pose"="[-0.0889, 0.73025, -0.2413, -0.8307767, 0.0, -2.35619]"
- initial guess: 
 -0.707104   0.70711         0   -0.0889
 -0.476806 -0.476802  0.738455   0.73025
  0.522169  0.522164  0.674302   -0.2413
         0         0         0         1

**After NDT Calibration:**
- NDT converged.
- The score is 0.930197
- Transformation matrix:
 -0.708694   0.705374 -0.0141393 -0.0733905
 -0.487661  -0.475277   0.732324    0.72309
  0.509843   0.525889   0.680809   -0.25894
         0          0          0          1
- This transformation can be replicated using:
ros2 run tf2_ros static_transform_publisher -0.0733905    0.72309   -0.25894 0.602703 -2.60659 -2.48388 os_lidar_top os_lidar_right


## Front

**Initial:**
- "initial_pose"="[-1.7526, 0.0, -0.2032, 0.0, -0.6562438, 0.785398]"
- initial guess: 
  0.560233 -0.560233 -0.610145   -1.7526
  0.707107  0.707107         0         0
  0.431438 -0.431438   0.79229   -0.2032
         0         0         0         1


**After NDT Calibration:**
- NDT converged.
- The score is 0.794342
- Transformation matrix:
  0.551509  -0.570191  -0.608868   -1.77807
  0.711277   0.702775 -0.0138622 -0.0260497
  0.435801  -0.425428   0.793151  -0.220312
         0          0          0          1
- This transformation can be replicated using:
ros2 run tf2_ros static_transform_publisher   -1.77807 -0.0260497  -0.220312  0.911249 -0.450928 -0.492325 os_lidar_top os_lidar_front


## Back

**Initial:**
- "initial_pose"="[1.7526, 0.0, -0.2032, 0.0, 0.5148721, 2.35619]"
- initial guess: 
 -0.615432 -0.615437  0.492423    1.7526
   0.70711 -0.707104         0         0
  0.348194  0.348198  0.870356   -0.2032
         0         0         0         1

**After NDT Calibration:**
- NDT converged.
- The score is 7.64531
- Transformation matrix:
 -0.611508  -0.605518   0.509318     1.7702
   0.70655  -0.707628 0.00702698 0.00465821
  0.356153   0.364156    0.86055  -0.175391
         0          0          0          1
- This transformation can be replicated using:
ros2 run tf2_ros static_transform_publisher     1.7702 0.00465821  -0.175391   2.28421 -0.364148  0.400316 os_lidar_top os_lidar_back


