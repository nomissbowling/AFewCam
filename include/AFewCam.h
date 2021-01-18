/*
  AFewCam.h
*/

#ifndef __AFewCam_H__
#define __AFewCam_H__

// use CV_PI instead of M_PI
// #define _USE_MATH_DEFINES
#include <opencv2/opencv.hpp>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

#include <string>
#include <cstdio>
#include <cstdlib>

namespace afewcam {

using namespace std;

template <typename ... Args>
string fmt(const string &f, Args ... args)
{
  size_t l = snprintf(nullptr, 0, f.c_str(), args ...);
  vector<char> buf(l + 1);
  snprintf(&buf[0], l + 1, f.c_str(), args ...);
  return string(&buf[0], &buf[0] + l);
}

}

#endif // __AFewCam_H__
