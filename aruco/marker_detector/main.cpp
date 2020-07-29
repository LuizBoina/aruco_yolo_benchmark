#include <opencv2/aruco.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <aruco/cameraparameters.h>

cv::aruco::Dictionary getDictionary() {
    cv::FileStorage fsr("./dictionary/dict_save.yml", cv::FileStorage::READ);
    if(!fsr.isOpened()) {
        std::cout << "Error when trying to read dict_save.yml. Check if file exists";
        exit (EXIT_FAILURE);
    }
    int markerSize, maxCorrectBits;
    cv::Mat bytes;
    // size of each marker dimension e.g. 5
    fsr["MarkerSize"] >> markerSize;
    // maximum number of erroneous bits that can be corrected during the marker detection.
    fsr["MaxCorrectionBits"] >> maxCorrectBits;
    // array that contains all the information about the marker codes
    fsr["ByteList"] >> bytes;
    fsr.release();
    cv::aruco::Dictionary dictionary = cv::aruco::Dictionary(bytes, markerSize, maxCorrectBits);
    return dictionary;
}

void readCamParams(cv::Mat &camMatrix, cv::Mat &distCoeffs) {
    cv::FileStorage fsr("./camera_calibration.yml", cv::FileStorage::READ);
    if(!fsr.isOpened()) {
        std::cout << "Error when trying to read camera_calibration.yml. Check if file exists";
        exit (EXIT_FAILURE);
    }
    fsr["camera_matrix"] >> camMatrix;
    fsr["distortion_coefficients"] >> distCoeffs;
    fsr.release();
}


int main() {

    cv::Mat followMarker, stopMarker;
    cv::aruco::Dictionary dictionary = getDictionary();
    cv::Mat camMatrix, distCoeffs;
    readCamParams(camMatrix, distCoeffs);
    std::cout << camMatrix << distCoeffs << std::endl;
    cv::VideoCapture inputVideo;
    inputVideo.open("http://192.168.15.3:4747/video?640x480");
    while (inputVideo.grab()) {
        cv::Mat image, imageCopy;
        inputVideo.retrieve(image);
        image.copyTo(imageCopy);
        std::vector<int> ids;
        std::vector<std::vector<cv::Point2f> > corners;
        cv::aruco::detectMarkers(image, dictionary, corners, ids);
        // if at least one marker detected
        if (ids.size() > 0) {
            cv::aruco::drawDetectedMarkers(imageCopy, corners, ids);
            std::vector<cv::Vec3d> rvecs, tvecs;
            cv::aruco::estimatePoseSingleMarkers(corners, 0.1301, camMatrix, distCoeffs, rvecs, tvecs);
            // draw axis for each marker
            for(int i=0; i<ids.size(); i++)
                cv::aruco::drawAxis(imageCopy, camMatrix, distCoeffs, rvecs[i], tvecs[i], 0.1);
        }
        cv::imshow("out", imageCopy);
        char key = (char) cv::waitKey(1);
        if (key == 27)
            break;
    }
    return 0;
}


//--- Routine to display image
//cv::Mat markers = cv::imread("./dictionary/markers.png", CV_LOAD_IMAGE_COLOR);
//namedWindow( "Display window", cv::WINDOW_AUTOSIZE );// Create a window for display.
//imshow( "Display window", markers );                   // Show our image inside it.
//cv::waitKey(0);

//--- Code to generate a custom dictionary
//int number= 2, dimension=5;
//cv::aruco::Dictionary dictionary = cv::aruco::generateCustomDictionary(number, dimension);
//cv::Mat store=dictionary.bytesList;
//cv::FileStorage fs("./dictionary/dic_save.yml", cv::FileStorage::WRITE);
//fs << "MarkerSize" << dictionary.markerSize;
//fs << "MaxCorrectionBits" << dictionary.maxCorrectionBits;
//fs << "ByteList" << dictionary.bytesList;
//fs.release();