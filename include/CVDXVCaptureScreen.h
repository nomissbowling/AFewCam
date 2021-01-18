/*
  CVDXVCaptureScreen.h
*/

#ifndef __CVDXVCAPTURESCREEN_H__
#define __CVDXVCAPTURESCREEN_H__

// use CV_PI instead of M_PI
// #define _USE_MATH_DEFINES
#include <opencv2/opencv.hpp>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

// #pragma comment(linker, "/export:<alias>=<decorated_name>")
#ifdef __MAKE_DLL__
#define DllExport __declspec(dllexport)
#else
#define DllExport __declspec(dllimport)
#endif

namespace cvdxvcapturescreen {

using namespace std;

class DllExport CVDXVCapScr {
private:
  int devId;
  void *fakeptr;
public:
  CVDXVCapScr(int _id) : devId(_id), fakeptr(NULL) { Init(); }
  virtual ~CVDXVCapScr(){ Dispose(); }
  void Init();
  void Dispose();
  cv::Mat cap(const cv::Size &sz);
};

}

#endif // __CVDXVCAPTURESCREEN_H__
