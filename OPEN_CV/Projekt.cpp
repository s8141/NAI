#include <opencv\cv.h>
#include <opencv\highgui.h>




using namespace cv;


int main(  ) {
	double w,h,fps;
	bool continueCapture = true;
	cv::namedWindow( "Okienko Prawie Inteligentne", CV_WINDOW_AUTOSIZE );
	// Tu zagadka dla Ciebie :)
	cv::VideoCapture cap( "C:\\Users\Public\Videos\Sample Videos\Zwierzêta.wmv" );
	 //cv::VideoCapture cap(0);
	
	if ( !cap.isOpened() ) return -1;
	w = cap.get( CV_CAP_PROP_FRAME_WIDTH ); //get the width of frames of the video
	h = cap.get( CV_CAP_PROP_FRAME_HEIGHT ); //get the height of frames of the video
	fps = cap.get( CV_CAP_PROP_FPS ); // ile mamy klatek na sekunde?
	std::cout << "Parametry wideo : " << w << " x " << h << " @ " << fps << " fps"<< std::endl;
	if ( fps < 0 ) fps = 30; // tak na wszelki wypadek :)
	while( continueCapture ) {
		
//		int key;
		
		cv::Mat frame;
		
		cv::Mat frame_new;

		if ( cap.read( frame ) ) {

			//cv::cvtColor( frame,frame_new,CV_RGB2HSV);
			//cv::blur( frame, frame_new, cv::Size(9,9) );
			cv::cvtColor( frame, frame_new, CV_BGR2GRAY );
			cv::Canny( frame_new, frame, 24, 24*3, 3 );
			cv::imshow( "Okienko Prawie Inteligentne", frame_new + frame);
		
			
			
		} else continueCapture = false;

		if( cv::waitKey( 1000.0/fps ) == 1048603 ) continueCapture = false;
	}
	return 0;

}

