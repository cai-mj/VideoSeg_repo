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
        cap.open(string(argv[1]));

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
		waitKey(0);
		return -1;
	}
	if(false == FgVideo.open("fg.avi", CV_FOURCC('M', 'P', 'E', 'G'), fps, frameSize, true))
	{
		printf("Can not open video writer for foregound video\n");
		waitKey(0);
		return -1;
	}
	if(false == BgVideo.open("bg.avi", CV_FOURCC('M', 'P', 'E', 'G'), fps, frameSize, true))
	{
		printf("Can not open video writer for background video\n");
		system("PAUSE");
		return -1;
	}

	// loop for video fore/background segmentation of each frame
    for(;;)
    {
        // read one frame
        cap >> img;        
        if(img.empty())
            break;	
        
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
        RawVideo << img;
		FgVideo << fgimg;		
        BgVideo << bgimg;

		char c = (char)waitKey(1000/fps);
        if(c == 27)   // quit after ESC
            break;
    }

	return 0;
}
