UNAME := $(shell uname -s)

INCDIR = -I. -I./usr/local/include/opencv -I/usr/local/include

DBG    = -g
OPT    = -O3
PTH    = -pthread
CPP    = g++
CFLAGS = -c $(DBG) $(INCDIR)

LINK   = -lm -L/usr/local/lib/ -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_video
LINKMATLAB = -L/usr/local/MATLAB/R2012a/bin/glnxa64/ -lmat -lmx

help: 
	@echo $(UNAME)

video_seg: video_segmentation.o
	$(CPP) -o video_seg video_segmentation.o $(LINK)

video_segmentation.o: video_segmentation.cpp
	$(CPP) $(CFLAGS) video_segmentation.cpp



clean:
	/bin/rm -f video_seg *.o 

