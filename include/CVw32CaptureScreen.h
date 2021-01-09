/*
  CVw32CaptureScreen.h
*/

#ifndef __CVW32CAPTURESCREEN_H__
#define __CVW32CAPTURESCREEN_H__

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

namespace cvw32capturescreen {

using namespace std;

class DllExport CVw32CapScr {
private:
  cv::Rect rct;
  void *fakeptr;
public:
  CVw32CapScr(const cv::Rect &_rct) : rct(_rct), fakeptr(NULL) { Init(); }
  virtual ~CVw32CapScr(){ Dispose(); }
  void Init();
  void Dispose();
  cv::Mat cap(const cv::Size &sz);
};

DllExport cv::Mat pinp(cv::Mat &frm, const cv::Mat &pic);
DllExport cv::Mat pinp(cv::Mat &frm, cv::VideoCapture &cap);
DllExport cv::Mat pinp(cv::VideoCapture &cap, const cv::Mat &pic);

DllExport cv::Mat rotROI(const cv::Mat &im, const cv::RotatedRect &rrct,
  int mode=cv::BORDER_CONSTANT, const cv::Vec3b &bgr=cv::Vec3b(0, 0, 0));
DllExport cv::Mat rotRectPts(const cv::RotatedRect &rrct);
DllExport void drawRotRect(cv::Mat &im, const cv::RotatedRect &rrct, int th,
  const cv::Vec3b &bgr);
DllExport void drawRotRectFill(cv::Mat &im, const cv::RotatedRect &rrct,
  const cv::Vec3b &bgr);

DllExport vector<int> getHalfMoonROI(int r);
DllExport void drawCircularROI(cv::Mat &im, const cv::Point &o, int r,
  const cv::Vec3b &bgr);

class DllExport CVtickFPS {
private:
  double dur, dfreq;
  int64 ck, tck;
  int cnt, frdif;
  cv::TickMeter tm; // not support or changed spec on some OpenCV version
public:
  CVtickFPS(int frd=30) : dur(0), dfreq(1000.0 / cv::getTickFrequency()),
    ck(0), tck(cv::getTickCount()), cnt(0), frdif(frd) { Init(); }
  virtual ~CVtickFPS(){ Dispose(); }
  void Init(){ tm.start(); }
  void Dispose(){}
  void update(){
    if(cnt++ % frdif) return;
    int64 newtick = cv::getTickCount();
    ck = newtick - tck;
    dur = dfreq * ck;
    tck = newtick;
    tm.stop();
    // fps = frdif / tm.getTimeSec(); // divide by 0 ?
    tm.reset();
    tm.start();
  }
  int getCnt(){ return cnt; }
  double getFPS(){ return 1000 * ((cnt <= frdif) ? 1 : frdif) / dur; }
  void dspFPS(cv::Mat &frm, int r, int c,
    const cv::Scalar &col, double sz, int th,
    const vector<string> &s0, double fps,
    const vector<string> &s1, double ms,
    const vector<string> &s2, int64 tick){
    ostringstream oss;
    // oss.str("");
    // oss.clear(stringstream::goodbit);
    oss << s0[0] << fixed << setprecision(1) << fps << s0[1];
    oss << s1[0] << fixed << setprecision(1) << ms << s1[1];
    oss << s2[0] << hex << setw(8) << setfill('0') << tick << s2[1];
    cv::putText(frm, oss.str(),
      cv::Point(2 + 16 * c, 32 * (r + 1)), // top-left
      cv::FONT_HERSHEY_SIMPLEX, sz, col, th, // thickness=1
      cv::LINE_AA, false); // lineType=LINE_8, bottomLeftOrigin=false
  }
  void dsp(cv::Mat &frm){
    dspFPS(frm, 0, 0, cv::Scalar(255, 255, 0), 1.0, 2,
      vector<string>({"cv ", "FPS, "}), getFPS(),
      vector<string>({"", "ms, "}), dur,
      vector<string>({"Tick ", ""}), ck);
    dspFPS(frm, 1, 0, cv::Scalar(255, 0, 255), 1.0, 2,
      vector<string>({"tm ", "FPS, "}), tm.getFPS(),
      vector<string>({"T ", "s, "}), tm.getTimeSec(),
      vector<string>({"Tick ", ""}), tm.getTimeTicks());
  }
};

}

#endif // __CVW32CAPTURESCREEN_H__
