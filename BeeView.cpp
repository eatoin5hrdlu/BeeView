//
// BeeView.cpp : Defines the entry point for the console application.
//
#define _ITERATOR_DEBUG_LEVEL 0
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE

#include "stdafx.h"
#include "windows.h"
#include <stdio.h>

#include <opencv2/opencv.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

//#include <iostream>
//#include <fstream>
#include "GUI.h"      // GUI.cpp (mouse,keyboard,signals)

using namespace std;
using namespace cv;

// INVESTIGATE THE ENVIRONMENT
void showenv(char **e)
{int i=0;while(e[i]!=(char *)NULL){printf("[%s]\n", e[i]);i++;}}

CvCapture* capture = NULL;
IplImage* gFrame = NULL;  // Main capture frame for live image
IplImage* gSFrame = NULL; // Sample frame for color capture

#include <process.h>
char *av[100];
char *ep[100];
void restart(void) { _execve("BeeView", av, ep); }

void cleanup(void) {
  if (capture != NULL) {
    cvReleaseCapture(&capture);
    capture = NULL;
  }
}

#include <signal.h>
void catcher(int signum)
{
  printf("caught %d\n",signum);
  loop = 0;
}

int
main(int argc, char **argv, char **envp)
{
  int i;
  for (i=0;i<100;i++) { av[i] = argv[i]; ep[i]=envp[i]; }
  setvbuf(stdout,NULL,_IONBF,0); // No buffering
  signal(SIGINT, catcher);       // cleanup if INTR
  system("C:/cygwin/bin/date");
  // showenv(envp);

  capture = cvCaptureFromCAM(0);
  if(!capture){ printf("No capture device.\n"); exit(3); }
	
  cvNamedWindow("video", CV_WINDOW_AUTOSIZE);
  int cxScreen = GetSystemMetrics(SM_CXSCREEN);
  int cyScreen = GetSystemMetrics(SM_CYSCREEN);
  //  printf("Screen is %d, %d\n", cxScreen, cyScreen);
  cvResizeWindow("video", cxScreen+50, cyScreen+130);
  cvMoveWindow( "video", -10, -30);
  int mouseParam = 5;
  cvSetMouseCallback("video", mouseHandler, &mouseParam);

  gFrame = cvQueryFrame(capture);
  gSFrame = cvCreateImage(cvSize(cxScreen+50,cyScreen+130), IPL_DEPTH_8U, 3);
  while(loop) {
    gFrame = cvQueryFrame(capture);
    if(!gFrame) break;	// Quit if we cannot grab a frame.
    cvResize(gFrame, gSFrame);
    cvShowImage("video", gSFrame);
    checkKeyboard();
  }
  printf("cleanup after loop\n");
  cleanup();
  printf("after cleanup\n");
  _execve("BeeView", argv, envp);
  //  restart();
}
