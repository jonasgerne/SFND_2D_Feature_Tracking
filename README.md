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
Set `rectVis` to true for the following visualization.

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
The most matches are obtained when using BRISK detector and BRIEF descriptor.

| Detector  | Descriptor | NoFMatches | t_keypoints | t_descriptors | t_total  |
|-----------|------------|------------|-------------|---------------|----------|
| BRISK     | BRIEF      | 1704       | 1.633042    | 0.006727      | 1.639769 |
| BRISK     | SIFT       | 1646       | 1.631513    | 0.145356      | 1.77687  |
| BRISK     | BRISK      | 1570       | 1.622238    | 1.399032      | 3.02127  |
| BRISK     | FREAK      | 1526       | 1.620089    | 0.208242      | 1.828331 |
| BRISK     | ORB        | 1510       | 1.622184    | 0.069174      | 1.691358 |
| AKAZE     | AKAZE      | 1259       | 0.38018     | 0.291637      | 0.671817 |
| FAST      | BRIEF      | 1099       | 0.005653    | 0.004295      | 0.009948 |
| FAST      | ORB        | 1081       | 0.005621    | 0.018966      | 0.024587 |
| FAST      | SIFT       | 1046       | 0.005802    | 0.081838      | 0.08764  |
| SHITOMASI | BRIEF      | 944        | 0.058567    | 0.003775      | 0.062342 |
| SHITOMASI | SIFT       | 927        | 0.058046    | 0.071148      | 0.129194 |
| SHITOMASI | ORB        | 907        | 0.058056    | 0.018794      | 0.07685  |
| FAST      | BRISK      | 899        | 0.005654    | 1.398523      | 1.404177 |
| FAST      | FREAK      | 881        | 0.005655    | 0.203262      | 0.208917 |
| SIFT      | SIFT       | 800        | 0.533202    | 0.45117       | 0.984372 |
| SHITOMASI | BRISK      | 767        | 0.05822     | 1.386309      | 1.444529 |
| SHITOMASI | FREAK      | 766        | 0.057796    | 0.201478      | 0.259274 |
| ORB       | SIFT       | 763        | 0.042424    | 0.158751      | 0.201176 |
| ORB       | ORB        | 761        | 0.04263     | 0.071302      | 0.113931 |
| ORB       | BRISK      | 751        | 0.04382     | 1.382738      | 1.426558 |
| SIFT      | BRIEF      | 702        | 0.561426    | 0.00422       | 0.565646 |
| SIFT      | FREAK      | 596        | 0.5479      | 0.210281      | 0.75818  |
| SIFT      | BRISK      | 592        | 0.559791    | 1.411377      | 1.971168 |
| ORB       | BRIEF      | 545        | 0.103089    | 0.003688      | 0.106777 |
| ORB       | FREAK      | 421        | 0.043338    | 0.201429      | 0.244767 |
| HARRIS    | BRIEF      | 174        | 0.052716    | 0.001997      | 0.054712 |
| HARRIS    | ORB        | 164        | 0.049771    | 0.018715      | 0.068486 |
| HARRIS    | SIFT       | 164        | 0.050603    | 0.069967      | 0.12057  |
| HARRIS    | FREAK      | 145        | 0.04972     | 0.198383      | 0.248103 |
| HARRIS    | BRISK      | 142        | 0.050419    | 1.385083      | 1.435502 |


#### MP.9 Performance

`t_total` gives the combined time of keypoint and descriptor extraction. All results were obtained using all ten images.
Regarding only the time for keypoint detection and descriptor extraction, based on my experiments I recommend either a 
combination of FAST detection with BRIEF or ORB descriptors. While ORB descriptors lead to a little bit more matches, 
they take a lot more time. Third comes the HARRIS detector with BRIEF descriptors, however, the number of matches is already quite lower. 

| Detector  | Descriptor | NoFMatches | t_keypoints | t_descriptors | t_total  |
|-----------|------------|------------|-------------|---------------|----------|
| FAST      | BRIEF      | 1099       | 0.005653    | 0.004295      | 0.009948 |
| FAST      | ORB        | 1081       | 0.005621    | 0.018966      | 0.024587 |
| HARRIS    | BRIEF      | 174        | 0.052716    | 0.001997      | 0.054712 |
| SHITOMASI | BRIEF      | 944        | 0.058567    | 0.003775      | 0.062342 |
| HARRIS    | ORB        | 164        | 0.049771    | 0.018715      | 0.068486 |
| SHITOMASI | ORB        | 907        | 0.058056    | 0.018794      | 0.07685  |
| FAST      | SIFT       | 1046       | 0.005802    | 0.081838      | 0.08764  |
| ORB       | BRIEF      | 545        | 0.103089    | 0.003688      | 0.106777 |
| ORB       | ORB        | 761        | 0.04263     | 0.071302      | 0.113931 |
| HARRIS    | SIFT       | 164        | 0.050603    | 0.069967      | 0.12057  |
| SHITOMASI | SIFT       | 927        | 0.058046    | 0.071148      | 0.129194 |
| ORB       | SIFT       | 763        | 0.042424    | 0.158751      | 0.201176 |
| FAST      | FREAK      | 881        | 0.005655    | 0.203262      | 0.208917 |
| ORB       | FREAK      | 421        | 0.043338    | 0.201429      | 0.244767 |
| HARRIS    | FREAK      | 145        | 0.04972     | 0.198383      | 0.248103 |
| SHITOMASI | FREAK      | 766        | 0.057796    | 0.201478      | 0.259274 |
| SIFT      | BRIEF      | 702        | 0.561426    | 0.00422       | 0.565646 |
| AKAZE     | AKAZE      | 1259       | 0.38018     | 0.291637      | 0.671817 |
| SIFT      | FREAK      | 596        | 0.5479      | 0.210281      | 0.75818  |
| SIFT      | SIFT       | 800        | 0.533202    | 0.45117       | 0.984372 |
| FAST      | BRISK      | 899        | 0.005654    | 1.398523      | 1.404177 |
| ORB       | BRISK      | 751        | 0.04382     | 1.382738      | 1.426558 |
| HARRIS    | BRISK      | 142        | 0.050419    | 1.385083      | 1.435502 |
| SHITOMASI | BRISK      | 767        | 0.05822     | 1.386309      | 1.444529 |
| BRISK     | BRIEF      | 1704       | 1.633042    | 0.006727      | 1.639769 |
| BRISK     | ORB        | 1510       | 1.622184    | 0.069174      | 1.691358 |
| BRISK     | SIFT       | 1646       | 1.631513    | 0.145356      | 1.77687  |
| BRISK     | FREAK      | 1526       | 1.620089    | 0.208242      | 1.828331 |
| SIFT      | BRISK      | 592        | 0.559791    | 1.411377      | 1.971168 |
| BRISK     | BRISK      | 1570       | 1.622238    | 1.399032      | 3.02127  |
