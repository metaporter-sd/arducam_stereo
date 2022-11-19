#include <cstdio>
#include <iostream> // for standard I/O
#include <string>   // for strings
#include <iomanip>  // for controlling float print precision
#include <sstream>  // string to number conversion

//#include "/usr/include/opencv4/opencv2/core/core.hpp"
//#include "/usr/include/opencv4/videoio/videoio.hpp"
//#include "/usr/include/opencv4/highgui/highgui.hpp"

#include <opencv2/core.hpp>     // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>  // OpenCV window I/O

std::string create_gst_pipeline(int deviceID, int captureWidth, int captureHeight, int framerate, int orientation, int displayWidth, int displayHeight);

int main(int argc, char ** argv) 
{
	(void) argc;
	(void) argv;

	printf("hello world arducam_stereo package\n");

 	int deviceID = 0;             // 0 = open default camera, TODO: change to reading from ROS param
    int apiID = cv::CAP_GSTREAMER; // TODO: get from ROS param

	// TODO: get from ROS params
	int captureWidth = 2592;
	int captureHeight = 1944; 
	int framerate = 10;
	int orientation = 2;
	
	int displayWidth = 1440;
	int displayHeight = 1080; 

	
	std::string gstPipeline = create_gst_pipeline(deviceID, captureWidth, captureHeight, framerate, orientation, displayWidth, displayHeight);
	std::cout<<"\n"<<gstPipeline<<"\n"<<std::endl; // print gstreamer pipeline for review
	
	// execute pipeline as system command for testing
	/*	
	std::string gstLaunch = "gst-launch-1.0 " + gstPipeline; 
	const char* cmd = gstLaunch.c_str();
	std::system(cmd);
	*/

	
	cv::VideoCapture cap("nvarguscamerasrc sensor-id=0 ! video/x-raw(memory:NVMM), width=(int)2592, height=(int)1944, format=(string)NV12, framerate=(fraction)10/1 ! nvvidconv flip-method=2 ! video/x-raw, width=(int)1440, height=(int)1080, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink", apiID);
	if (!cap.isOpened()) {
        std::cout  << "Could not open the input video: " << deviceID << std::endl;
        return -1;
    }
	
	cv::Mat frame;
	while (true) {
		// Capture frame-by-frame
		cap >> frame; 
		// if frame is read correctly ret is True
		if (frame.empty()) {
			std::cout<<"Can't receive frame (stream end?). Exiting ..."<<std::endl;
			break;
		} 
		
		// Our operations on the frame come here
		// gray = cv2.cvtColor(frame, cv.COLOR_BGR2GRAY)
	
		// Display the resulting frame
		cv::imshow("Frame", frame);
		
		int keyCode = cv::waitKey(30) & 0xFF;
        // Stop the program on the ESC key
		if (keyCode == 27) {
			break;
		}
	}
	cap.release();
	cv::destroyAllWindows();
	
	return 0;
}

std::string create_gst_pipeline(int deviceID=0, int captureWidth=2592, int captureHeight=1944, int framerate=10, int orientation=2, int displayWidth=1440, int displayHeight=1080) 
{

	std::string gstPipeline = "nvarguscamerasrc sensor-id=" + std::to_string(deviceID) + 
		
		" ! " +

		"\"" + 
		"video/x-raw(memory:NVMM), " + 
        "width=(int)" + std::to_string(captureWidth) + ", " + 
		"height=(int)" + std::to_string(captureHeight) + ", " +
		"format=(string)NV12, " +
		"framerate=(fraction)" + std::to_string(framerate) + "/1" +
		"\"" + 
		
		" ! " +
        
		"nvvidconv " +
		"flip-method=" + std::to_string(orientation) +
		
		" ! " +

		"\"" + 
        //"video/x-raw(memory:NVMM), " +
		"video/x-raw, " + 
		"width=(int)" + std::to_string(displayWidth) + ", " + 
		"height=(int)" + std::to_string(displayHeight) +  ", " +
		"format=(string)BGRx"
		"\"" + 
		
		" ! " +
		
		"videoconvert" +
		
		" ! " +
		/*
		"\"" + 
		"video/x-raw, " + 
		"format=(string)BGR" + 
		"\"" +
		*/
		" ! " + 
		
		//"appsink";
		"nvoverlaysink";






	/*
	std::string gstPipeline = "nvarguscamerasrc sensor-id=" + std::to_string(deviceID) + 
		
		" ! " +

		"\"" + 
		"video/x-raw(memory:NVMM), " + 
		"format=(string)NV12, " +
        "width=(int)" + std::to_string(captureWidth) + ", " + 
		"height=(int)" + std::to_string(captureHeight) + ", " +
		"framerate=(fraction)" + std::to_string(framerate) + "/1" +
		"\"" + 
		
		" ! " +
        
		"nvvidconv flip-method=" + std::to_string(orientation) +

		" ! " +

		"\"" + 
        "video/x-raw, " +
		"format=(string)BGRx, " + 
		"width=(int)" + std::to_string(displayWidth) + ", " + 
		"height=(int)" + std::to_string(displayHeight) +  ", " +
		"framerate=(fraction)" + std::to_string(framerate) + "/1" +
		"\"" + 

		" ! " +
	
		"videoconvert" +

		" ! " +
			
        "\"" + 
		"video/x-raw, " + 
		"format=(string)BGR" + 
		"\"" +
		
		" ! " + 
		
		"appsink";
		*/

	return gstPipeline;
}
