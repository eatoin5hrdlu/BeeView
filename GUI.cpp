// GUI.cpp

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#include "stdafx.h"

#include <stdio.h>
#include <signal.h>
#include "windows.h"
#include <iostream>
#include <fstream>
using namespace std;

#include "globals.h"
#include "GUI.h"

void help(){cout<<"Reliably present a full-screen live video image.\n";}

int occasionally = 1;
int loop = 1;

void checkKeyboard(void)
{
	unsigned char c = cvWaitKey(4);
	if (occasionally++ % 100 == 0) downtime();
	switch(c) {
	    case 'x':    loop = 0;
    	    default:     return;
	}
}

void mouseHandler(int event, int x, int y, int flags, void* param)
{
    switch(event){
      case CV_EVENT_LBUTTONDOWN:
	printf("Left button down ( %d,%d ) ",x,y);
        break;

      case CV_EVENT_LBUTTONUP:
	printf("Left button up\n");
        break;
    }
}

static int displayOn = 1;
static int offtime = 0;

void downtime(void)
{
    SYSTEMTIME st;
    GetSystemTime(&st);
	if (st.wHour > 26 || st.wHour < 3) // Greenwich time
    // if (st.wHour > 17 || st.wHour < 8) // AFTER HOURS
      {
	if (displayOn)
	  {
	    displayOn = 0;
	    SendMessageW(HWND_BROADCAST,
			 WM_SYSCOMMAND,
			 SC_MONITORPOWER,
			 (LPARAM) 2 );
	    printf("After hours: (hr=%d) display needs to be off\n",st.wHour);
		system("C:/cygwin/bin/date");
	  }
      }
    else if (displayOn == 0) {
      displayOn = 1;
      SendMessageW(HWND_BROADCAST,
		   WM_SYSCOMMAND,
		   SC_MONITORPOWER,
		   (LPARAM)-1);
      printf("Working hours: turning display on\n");
	  system("C:/cygwin/bin/date");
    }
}

