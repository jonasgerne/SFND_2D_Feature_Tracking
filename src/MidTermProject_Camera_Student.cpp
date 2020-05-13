/* INCLUDES FOR THIS PROJECT */
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <limits>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <list>

#include "dataStructures.h"
#include "matching2D.hpp"

// using namespace std;

/* MAIN PROGRAM */
int main(int argc, const char *argv[]) {

    /* INIT VARIABLES AND DATA STRUCTURES */

    // data location
    std::string dataPath = "../";

    // camera
    std::string imgBasePath = dataPath + "images/";
    std::string imgPrefix = "KITTI/2011_09_26/image_00/data/000000"; // left camera, color
    std::string imgFileType = ".png";
    int imgStartIndex = 0; // first file index to load (assumes Lidar and camera names have identical naming convention)
    int imgEndIndex = 9;   // last file index to load
    int imgFillWidth = 4;  // no. of digits which make up the file index (e.g. img-0001.png)

    // misc
    int dataBufferSize = 2;       // no. of images which are held in memory (ring buffer) at the same time
    std::vector<DataFrame> dataBuffer; // list of data frames which are held in memory at the same time
    bool bVis = false;            // visualize results

    // all possible detector / descriptor combinations
    std::vector<std::pair<std::string, std::string>> combinations{{"AKAZE", "AKAZE"},
                                                                  {"BRISK", "BRIEF"},
                                                                  {"BRISK", "BRISK"},
                                                                  {"BRISK", "FREAK"},
                                                                  {"BRISK", "ORB"},
                                                                  {"BRISK", "SIFT"},
                                                                  {"FAST", "BRIEF"},
                                                                  {"FAST", "BRISK"},
                                                                  {"FAST", "FREAK"},
                                                                  {"FAST", "ORB"},
                                                                  {"FAST", "SIFT"},
                                                                  {"HARRIS", "BRIEF"},
                                                                  {"HARRIS", "BRISK"},
                                                                  {"HARRIS", "FREAK"},
                                                                  {"HARRIS", "ORB"},
                                                                  {"HARRIS", "SIFT"},
                                                                  {"ORB", "BRIEF"},
                                                                  {"ORB", "BRISK"},
                                                                  {"ORB", "FREAK"},
                                                                  {"ORB", "ORB"},
                                                                  {"ORB", "SIFT"},
                                                                  {"SHITOMASI", "BRIEF"},
                                                                  {"SHITOMASI", "BRISK"},
                                                                  {"SHITOMASI", "FREAK"},
                                                                  {"SHITOMASI", "ORB"},
                                                                  {"SHITOMASI", "SIFT"},
                                                                  {"SIFT", "BRIEF"},
                                                                  {"SIFT", "BRISK"},
                                                                  {"SIFT", "FREAK"},
                                                                  {"SIFT", "SIFT"}};

    std::size_t total_matches;
    double t_keypoints;
    double t_descriptors;

    bool rectVis = false;

    std::string log_filename = dataPath + "results/test.csv";
    std::ofstream outfile;
    outfile.open(log_filename, std::ios_base::app);
    outfile << "Detector;Descriptor;NoFMatches;t_keypoints;t_descriptors;t_total" << std::endl;

    /* MAIN LOOP OVER ALL IMAGES */
    for (const auto& [detect, descrip]: combinations) {
            dataBuffer.clear();
            t_keypoints = 0.0;
            t_descriptors = 0.0;
            total_matches = 0;
            for (size_t imgIndex = 0; imgIndex <= imgEndIndex - imgStartIndex; imgIndex++) {
                /* LOAD IMAGE INTO BUFFER */

                // assemble filenames for current index
                std::ostringstream imgNumber;
                imgNumber << std::setfill('0') << std::setw(imgFillWidth) << imgStartIndex + imgIndex;
                std::string imgFullFilename = imgBasePath + imgPrefix + imgNumber.str() + imgFileType;

                // load image from file and convert to grayscale
                cv::Mat img, imgGray;
                img = cv::imread(imgFullFilename);
                cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);

                //// STUDENT ASSIGNMENT
                //// TASK MP.1 -> replace the following code with ring buffer of size dataBufferSize

                // push image into data frame buffer
                DataFrame frame;
                frame.cameraImg = imgGray;
                if (dataBuffer.size() >= dataBufferSize)
                    dataBuffer.erase(dataBuffer.begin());
                dataBuffer.push_back(frame);

                //// EOF STUDENT ASSIGNMENT
                std::cout << "#1 : LOAD IMAGE INTO BUFFER done" << std::endl;

                /* DETECT IMAGE KEYPOINTS */

                // extract 2D keypoints from current image
                std::vector<cv::KeyPoint> keypoints; // create empty feature list for current image
                const std::string& detectorType = detect;

                //// STUDENT ASSIGNMENT
                //// TASK MP.2 -> add the following keypoint detectors in file matching2D.cpp and enable string-based selection based on detectorType
                //// -> HARRIS, FAST, BRISK, ORB, AKAZE, SIFT

                auto t_keypoints_tmp = (double) cv::getTickCount();
                if (detectorType == "SHITOMASI") {
                    detKeypointsShiTomasi(keypoints, imgGray, false);
                } else if (detectorType == "HARRIS") {
                    detKeypointsHarris(keypoints, imgGray, false);
                } else {
                    detKeypointsModern(keypoints, imgGray, detectorType, false);
                }
                t_keypoints += ((double) cv::getTickCount() - t_keypoints_tmp) / cv::getTickFrequency();
                //// EOF STUDENT ASSIGNMENT

                //// STUDENT ASSIGNMENT
                //// TASK MP.3 -> only keep keypoints on the preceding vehicle
                // only keep keypoints on the preceding vehicle
                bool bFocusOnVehicle = true;
                cv::Rect vehicleRect(535, 180, 180, 150);
                if (bFocusOnVehicle) {
                    std::vector<cv::KeyPoint> keypoints_focused;
                    for (auto &keypoint : keypoints) {
                        if (vehicleRect.contains(keypoint.pt))
                            keypoints_focused.push_back(keypoint);
                    }
                    keypoints.swap(keypoints_focused);
                }
                if (rectVis) {
                    cv::Mat visImage = imgGray.clone();
                    cv::drawKeypoints(img, keypoints, visImage, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
                    cv::rectangle(visImage, vehicleRect, cv::Scalar(0, 0, 255));
                    std::string windowName = "Focus Rectangle with Remaining Keypoints";
                    cv::namedWindow(windowName, 6);
                    imshow(windowName, visImage);
                    cv::waitKey(0);
                }
                //// EOF STUDENT ASSIGNMENT
                /*float nof_keypoins(keypoints.size());
                float total_size = std::accumulate(keypoints.begin(), keypoints.end(), 0.0f,
                                                   [](float sum, const cv::KeyPoint &keypoint) { return sum + keypoint.size; });

                float mean = total_size / nof_keypoins;

                // Now calculate the variance
                auto variance_func = [&mean, &nof_keypoins](float sum, const cv::KeyPoint &kp) {
                    return sum + ((kp.size - mean) * (kp.size - mean) / (nof_keypoins - 1));
                };

                float variance = std::accumulate(keypoints.begin(), keypoints.end(), 0.0, variance_func);

                std::string log_filename = dataPath + "results/keypoints_with_var.csv";
                std::ofstream outfile;
                outfile.open(log_filename, std::ios_base::app);

                outfile << detectorType << "; " << imgIndex << "; " << keypoints.size() << "; " << std::to_string(mean) << "; "
                        << std::to_string(variance) << ";"
                        << std::endl;

                outfile.close();*/

                // optional : limit number of keypoints (helpful for debugging and learning)
                bool bLimitKpts = false;
                if (bLimitKpts) {
                    int maxKeypoints = 50;

                    if (detectorType ==
                        "SHITOMASI") { // there is no response info, so keep the first 50 as they are sorted in descending quality order
                        keypoints.erase(keypoints.begin() + maxKeypoints, keypoints.end());
                    }
                    cv::KeyPointsFilter::retainBest(keypoints, maxKeypoints);
                    std::cout << " NOTE: Keypoints have been limited!" << std::endl;
                }

                // push keypoints and descriptor for current frame to end of data buffer
                (dataBuffer.end() - 1)->keypoints = keypoints;
                std::cout << "#2 : DETECT KEYPOINTS done" << std::endl;

                /* EXTRACT KEYPOINT DESCRIPTORS */

                //// STUDENT ASSIGNMENT
                //// TASK MP.4 -> add the following descriptors in file matching2D.cpp and enable string-based selection based on descriptorType
                //// -> BRIEF, ORB, FREAK, AKAZE, SIFT

                cv::Mat descriptors;
                const std::string& descriptorName = descrip; // BRIEF, ORB, FREAK, AKAZE, SIFT
                auto t_descriptors_tmp = (double) cv::getTickCount();
                descKeypoints((dataBuffer.end() - 1)->keypoints, (dataBuffer.end() - 1)->cameraImg, descriptors,
                              descriptorName);
                t_descriptors += ((double) cv::getTickCount() - t_descriptors_tmp) / cv::getTickFrequency();

                //// EOF STUDENT ASSIGNMENT

                // push descriptors for current frame to end of data buffer
                (dataBuffer.end() - 1)->descriptors = descriptors;

                std::cout << "#3 : EXTRACT DESCRIPTORS done" << std::endl;

                if (dataBuffer.size() > 1) // wait until at least two images have been processed
                {

                    /* MATCH KEYPOINT DESCRIPTORS */

                    std::vector<cv::DMatch> matches;
                    std::string matcherType = "MAT_BF";                                                 // MAT_BF, MAT_FLANN
                    std::string descriptorType =
                            descriptorName == "SIFT" ? "DES_HOG" : "DES_BINARY";   // DES_BINARY, DES_HOG
                    std::string selectorType = "SEL_KNN";                                               // SEL_NN, SEL_KNN

                    //// STUDENT ASSIGNMENT
                    //// TASK MP.5 -> add FLANN matching in file matching2D.cpp
                    //// TASK MP.6 -> add KNN match selection and perform descriptor distance ratio filtering with t=0.8 in file matching2D.cpp

                    matchDescriptors((dataBuffer.end() - 2)->keypoints, (dataBuffer.end() - 1)->keypoints,
                                     (dataBuffer.end() - 2)->descriptors, (dataBuffer.end() - 1)->descriptors,
                                     matches, descriptorType, matcherType, selectorType);

                    //// EOF STUDENT ASSIGNMENT

                    // store matches in current data frame
                    (dataBuffer.end() - 1)->kptMatches = matches;
                    total_matches += matches.size();

                    std::cout << "#4 : MATCH KEYPOINT DESCRIPTORS done" << std::endl;

                    // visualize matches between current and previous image
                    bVis = false;
                    if (bVis) {
                        cv::Mat matchImg = ((dataBuffer.end() - 1)->cameraImg).clone();
                        cv::drawMatches((dataBuffer.end() - 2)->cameraImg, (dataBuffer.end() - 2)->keypoints,
                                        (dataBuffer.end() - 1)->cameraImg, (dataBuffer.end() - 1)->keypoints,
                                        matches, matchImg,
                                        cv::Scalar::all(-1), cv::Scalar::all(-1),
                                        std::vector<char>(), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

                        std::string windowName = "Matching keypoints between two camera images";
                        cv::namedWindow(windowName, 7);
                        cv::imshow(windowName, matchImg);
                        std::cout << "Press key to continue to next image" << std::endl;
                        cv::waitKey(0); // wait for key to be pressed
                    }
                    bVis = false;
                }

            } // eof loop over all images
            outfile << detect << ";" << descrip << ";" << total_matches << ";" << std::to_string(t_keypoints) << ";"
                    << std::to_string(t_descriptors) << ";" << std::to_string(t_keypoints + t_descriptors)
                    << std::endl;
    }
    outfile.close();

    return 0;
}
