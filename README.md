# SFND 2D Feature Tracking

<img src="images/keypoints.png" width="820" height="248" />

The idea of the camera course is to build a collision detection system - that's the overall goal for the Final Project. As a preparation for this, you will now build the feature tracking part and test various detector / descriptor combinations to see which ones perform best. This mid-term project consists of four parts:

* First, you will focus on loading images, setting up data structures and putting everything into a ring buffer to optimize memory load. 
* Then, you will integrate several keypoint detectors such as HARRIS, FAST, BRISK and SIFT and compare them with regard to number of keypoints and speed. 
* In the next part, you will then focus on descriptor extraction and matching using brute force and also the FLANN approach we discussed in the previous lesson. 
* In the last part, once the code framework is complete, you will test the various algorithms in different combinations and compare them with regard to some performance measures. 

See the classroom instruction and code comments for more details on each of these parts. Once you are finished with this project, the keypoint matching part will be set up and you can proceed to the next lesson, where the focus is on integrating Lidar points and on object detection using deep-learning. 

## Dependencies for Running Locally
* cmake >= 2.8
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* OpenCV >= 4.1
  * This must be compiled from source using the `-D OPENCV_ENABLE_NONFREE=ON` cmake flag for testing the SIFT and SURF detectors.
  * The OpenCV 4.1.0 source code can be found [here](https://github.com/opencv/opencv/tree/4.1.0)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./2D_feature_tracking`.

## Results
#### MP.1 Databuffer
I opted for a simple implementation using a `std::vector`, deleting the first element if the size is greater or equal than two. Other options would be the actual databuffer from the Boost library or using a queue.

#### MP.2 Detectors
Implemented using the functions defined in the header

#### MP.3 Removing outliers
Using the `cv::Rect::contains` function outliers can be easily removed.
<img src="images/rect_filter.png" />

#### MP.4, MP.5 and MP.6
These were implemented following the exercises in the lessons.


#### MP.7 Number of keypoints and neighborhood size
Number of keypoints:
<img src="images/number_of_keypoints.png" />

Keypoint size:
<img src="images/size_of_keypoints.png" />

#### MP.8 Tested combinations of detectors and descriptors
AKAZE descriptors can only be used with AKAZE detectors. SIFT with ORB gave me an out of memory error, so this combination was not tested.
The most matches are obtained when using ORB detector with SIFT descriptor. 

| Detector  | Descriptor | NoFMatches | t_keypoints | t_descriptors | t_total  |
|-----------|------------|------------|-------------|---------------|----------|
| ORB       | SIFT       | 17174      | 0,0421      | 0,155143      | 0,197243 |
| ORB       | FREAK      | 16411      | 0,04169     | 0,201233      | 0,242923 |
| ORB       | ORB        | 15990      | 0,040569    | 0,069039      | 0,109607 |
| ORB       | BRIEF      | 15229      | 0,106922    | 0,003929      | 0,11085  |
| BRISK     | SIFT       | 14684      | 1,628167    | 0,146596      | 1,774763 |
| BRISK     | FREAK      | 13038      | 1,613648    | 0,205103      | 1,818751 |
| BRISK     | ORB        | 11512      | 1,606406    | 0,06912       | 1,675526 |
| BRISK     | BRIEF      | 10002      | 1,616963    | 0,00672       | 1,623684 |
| FAST      | SIFT       | 8298       | 0,005616    | 0,081616      | 0,087232 |
| FAST      | FREAK      | 7252       | 0,005495    | 0,203483      | 0,208978 |
| FAST      | ORB        | 6371       | 0,005546    | 0,019083      | 0,02463  |
| FAST      | BRIEF      | 5290       | 0,005489    | 0,004335      | 0,009824 |
| HARRIS    | SIFT       | 4191       | 0,050935    | 0,069998      | 0,120933 |
| HARRIS    | FREAK      | 4027       | 0,049465    | 0,196853      | 0,246318 |
| HARRIS    | ORB        | 3882       | 0,05068     | 0,017864      | 0,068544 |
| HARRIS    | BRIEF      | 3718       | 0,050415    | 0,001948      | 0,052363 |
| SHITOMASI | SIFT       | 3544       | 0,058508    | 0,073326      | 0,131834 |
| SHITOMASI | FREAK      | 2617       | 0,062519    | 0,201279      | 0,263798 |
| SIFT      | SIFT       | 2098       | 0,499573    | 0,400291      | 0,899864 |
| SHITOMASI | ORB        | 1851       | 0,081659    | 0,021665      | 0,103324 |
| SIFT      | FREAK      | 1298       | 0,545766    | 0,205627      | 0,751392 |
| AKAZE     | AKAZE      | 1259       | 0,381787    | 0,289511      | 0,671298 |
| SHITOMASI | BRIEF      | 944        | 0,076951    | 0,005831      | 0,082782 |
| SIFT      | BRIEF      | 702        | 0,562185    | 0,004695      | 0,56688  |


#### MP.9 Performance

`t_total` gives the combined time of keypoint and descriptor extraction.

| Detector  | Descriptor | NoFMatches | t_keypoints | t_descriptors | t_total  |
|-----------|------------|------------|-------------|---------------|----------|
| FAST      | BRIEF      | 5290       | 0,005489    | 0,004335      | 0,009824 |
| FAST      | ORB        | 6371       | 0,005546    | 0,019083      | 0,02463  |
| HARRIS    | BRIEF      | 3718       | 0,050415    | 0,001948      | 0,052363 |
| HARRIS    | ORB        | 3882       | 0,05068     | 0,017864      | 0,068544 |
| SHITOMASI | BRIEF      | 944        | 0,076951    | 0,005831      | 0,082782 |
| FAST      | SIFT       | 8298       | 0,005616    | 0,081616      | 0,087232 |
| SHITOMASI | ORB        | 1851       | 0,081659    | 0,021665      | 0,103324 |
| ORB       | ORB        | 15990      | 0,040569    | 0,069039      | 0,109607 |
| ORB       | BRIEF      | 15229      | 0,106922    | 0,003929      | 0,11085  |
| HARRIS    | SIFT       | 4191       | 0,050935    | 0,069998      | 0,120933 |
| SHITOMASI | SIFT       | 3544       | 0,058508    | 0,073326      | 0,131834 |
| ORB       | SIFT       | 17174      | 0,0421      | 0,155143      | 0,197243 |
| FAST      | FREAK      | 7252       | 0,005495    | 0,203483      | 0,208978 |
| ORB       | FREAK      | 16411      | 0,04169     | 0,201233      | 0,242923 |
| HARRIS    | FREAK      | 4027       | 0,049465    | 0,196853      | 0,246318 |
| SHITOMASI | FREAK      | 2617       | 0,062519    | 0,201279      | 0,263798 |
| SIFT      | BRIEF      | 702        | 0,562185    | 0,004695      | 0,56688  |
| AKAZE     | AKAZE      | 1259       | 0,381787    | 0,289511      | 0,671298 |
| SIFT      | FREAK      | 1298       | 0,545766    | 0,205627      | 0,751392 |
| SIFT      | SIFT       | 2098       | 0,499573    | 0,400291      | 0,899864 |
| BRISK     | BRIEF      | 10002      | 1,616963    | 0,00672       | 1,623684 |
| BRISK     | ORB        | 11512      | 1,606406    | 0,06912       | 1,675526 |
| BRISK     | SIFT       | 14684      | 1,628167    | 0,146596      | 1,774763 |
| BRISK     | FREAK      | 13038      | 1,613648    | 0,205103      | 1,818751 |
