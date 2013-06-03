#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <cstdio>
#include <cstdlib>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{

	VideoCapture cap;
	VideoWriter RawVideo;
	VideoWriter FgVideo;
	VideoWriter BgVideo;
	BackgroundSubtractorMOG2 bg_model; //(100, 3, 0.3, 5);
	Mat img, fgmask, fgimg;

	// default: capture video from camera
	if(argc < 2)
        cap.open(0);
    else
        cap.open("breakfast.avi");

    if(!cap.isOpened())
    {
        printf("\nCan not open camera or video file\n");
		system("PAUSE");
        return -1;
    }

    double fps = cap.get(CV_CAP_PROP_FPS);
	fps = fps <= 0? 10.0 : fps;
	int width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	int height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	printf("video frame rate: %f fps, width: %d, height: %d\n", fps, width, height);
	Size frameSize(width, height);
    if(false == FgVideo.open("raw.avi", CV_FOURCC('M', 'P', 'E', 'G'), fps, frameSize, true))
	{
		printf("Can not open video writer for raw video\n");
		system("PAUSE");
		return -1;
	}
	if(false == FgVideo.open("fg.avi", CV_FOURCC('M', 'P', 'E', 'G'), fps, frameSize, true))
	{
		printf("Can not open video writer for foregound video\n");
		system("PAUSE");
		return -1;
	}
	if(false == BgVideo.open("bg.avi", CV_FOURCC('M', 'P', 'E', 'G'), fps, frameSize, true))
	{
		printf("Can not open video writer for background video\n");
		system("PAUSE");
		return -1;
	}

	namedWindow( "Capture ", CV_WINDOW_AUTOSIZE);
    namedWindow( "Foreground ", CV_WINDOW_AUTOSIZE );

	// loop for video fore/background segmentation of each frame
    for(;;)
    {
        // read one frame
        cap >> img;        
        if(img.empty())
            break;

		RawVideo.write(img);
		imshow("Capture", img);
        
        if(fgimg.empty())
          fgimg.create(img.size(), img.type());		

        //update the model and get foreground mask as binary image
        bg_model(img, fgmask, -1);

        fgimg = Scalar::all(0);
        img.copyTo(fgimg, fgmask);

		//background image
        Mat bgimg;
        bg_model.getBackgroundImage(bgimg);

        // write segmented frame to video
		FgVideo << fgimg;		
        BgVideo << bgimg;
		imshow("Foreground", fgimg);

		char c = (char)waitKey(1000/fps);
        if(c == 27)   // quit after ESC
            break;
    }

	return 0;
}

/*   C code
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
     //load the video file to the memory
     CvCapture *capture =  cvCaptureFromAVI("D:/Private/opencv/MyProjects/Video_Segmentation/Debug/breakfast.avi");

     if( !capture ) 
	 {
        printf("\nCan not open video file\n");
		system("PAUSE");
        return -1;
    }

     //obtain the frames per seconds of that video
     int fps = ( int )cvGetCaptureProperty( capture, CV_CAP_PROP_FPS );

    //create a window with the title "Video"
    cvNamedWindow("Video");

    while(true) {
             //grab and retrieve each frames of the video sequencially 
             IplImage* frame = cvQueryFrame( capture );

             if( !frame ) break;

             //show the retrieved frame in the "Video" window
             cvShowImage( "Video", frame );

             int c; 


             if(fps!=0){  

                     //wait for 1000/fps milliseconds
                     c = cvWaitKey(1000/fps);
            }else{
                     //wait for 40 milliseconds
                      c = cvWaitKey(40);
            } 



          //exit the loop if user press "Esc" key  (ASCII value of "Esc" is 27) 
            if((char)c==27 ) break;
   }

   //destroy the opened window
   cvDestroyWindow("Video");   
   //release memory
   cvReleaseCapture( &capture );

    return 0;

}*/