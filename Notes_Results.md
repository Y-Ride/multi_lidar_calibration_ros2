# Calibration Notes

All of the following calibrations were done with the bags `calibration_top_of_ramp_angled_left_front` and `calibration_top_of_ramp_angled_right_back`. 

You can use `list_to_array.sh` to transform a list of inputs into an array of floats if needed.

Example:
``` bash
# Input
./list_to_array.sh 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1
```
``` bash
# Output
[1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0]
```
> Note: In vscode, you can use `ctl + shft + v` to preview this readme file. In the preview, the transformation matrices are printed as one line lists instead of in matrix format.

## Top

This is the main reference lidar
- Identity transform:
  1 0 0 0
  0 1 0 0
  0 0 1 0
  0 0 0 1


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
- The score is 6.49053
- Transformation matrix:
  -0.716498  -0.697582 0.00317738 -0.0903388
   0.445107  -0.460675  -0.767892  -0.732264
   0.537131  -0.548779   0.640572  -0.253779
          0          0          0          1
- This transformation can be replicated using:
ros2 run tf2_ros static_transform_publisher -0.0903388  -0.732264  -0.253779   2.58571 -0.567032 -0.708372 os_lidar_top os_lidar_left


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
- The score is 3.74825
- Transformation matrix:
  -0.706876    0.70714 -0.0167163 -0.0778711
  -0.490656  -0.473176   0.731683   0.706387
   0.509493   0.525411    0.68144  -0.286456
          0          0          0          1
- This transformation can be replicated using:
ros2 run tf2_ros static_transform_publisher -0.0778711   0.706387  -0.286456 0.606768   -2.607 -2.48477 os_lidar_top os_lidar_right


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
- The score is 1.65053
- Transformation matrix:
  0.550484  -0.572266  -0.607848   -1.74023
  0.711988   0.702006 -0.0161156 -0.0308126
  0.435936   -0.42391    0.79389  -0.229185
         0          0          0          1
- This transformation can be replicated using:
ros2 run tf2_ros static_transform_publisher   -1.74023 -0.0308126  -0.229185  0.912634 -0.451078 -0.490449 os_lidar_top os_lidar_front


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
- The score is 5.16496
- Transformation matrix:
  -0.61119   -0.60748    0.50736    1.74274
  0.708089  -0.706082 0.00758085  0.0221608
  0.353633   0.363889   0.861701  -0.173762
         0          0          0          1
- This transformation can be replicated using: 
ros2 run tf2_ros static_transform_publisher   1.74274 0.0221608 -0.173762   2.28288 -0.361452  0.399575 os_lidar_top os_lidar_back


