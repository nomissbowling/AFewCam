/*
  AFewCam.cpp

  test with OpenCV 3.4.12 https://github.com/opencv/opencv/releases/tag/3.4.12
    x64/vc15/bin
      opencv_world3412.dll
      opencv_ffmpeg3412_64.dll

  with CVw32CapScr.dll compile from https://github.com/nomissbowling/RainbowArc
    bin/CVw32CapScr.dll
    lib/CVw32CapScr.lib
    include/CVw32CaptureScreen.h

  with CVDXVCapScr.dll compile from https://github.com/nomissbowling/RainbowArc
    bin/CVDXVCapScr.dll
    lib/CVDXVCapScr.lib
    include/CVDXVCaptureScreen.h

  use Microsoft Visual Studio 2017
  x64 compiler/linker
  "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.16.27023\bin\Hostx64\x64\cl.exe"
   -source-charset:utf-8 -execution-charset:utf-8
   -EHsc -Fe..\bin\AFewCam.exe ..\src\AFewCam.cpp
   -I..\include
   -IC:\OpenCV3\include
   -link
   /LIBPATH:..\lib
   /LIBPATH:C:\OpenCV3\x64\vc15\lib
   /LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.16.27023\lib\x64"
   /LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.17763.0\ucrt\x64"
   /LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.17763.0\um\x64"
   opencv_world3412.lib CVw32CapScr.lib CVDXVCapScr.lib

  del ..\bin\AFewCam.obj
  AFewCam
*/

#include "AFewCam.h"

#include <CVw32CaptureScreen.h>
using namespace cvw32capturescreen;
#include <CVDXVCaptureScreen.h>
using namespace cvdxvcapturescreen;
using namespace std;

namespace afewcam {

string test_capscr(int ac, char **av)
{
  vector<string> wn({"original", "gray", "Hue", "Alpha"});
  for(vector<string>::iterator i = wn.begin(); i != wn.end(); ++i)
    cv::namedWindow(*i, CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
  int cam_id = 0; // 1; // 0; // may be 'ManyCam Virtual Webcam'
  int width = 640, height = 480, fourcc;
  double fps = 30.0;
  cv::VideoCapture cap(cv::CAP_DSHOW + cam_id); // use DirectShow
  if(!cap.isOpened()) return "error: open camera";
  // cout << cv::getBuildInformation() << endl;
#if 1
  cout << hex << setw(8) << setfill('0')
    << (unsigned long)cap.get(cv::CAP_PROP_FOURCC) << endl; // e436eb7d ? 2YUY
  cout << cap.get(cv::CAP_PROP_FRAME_WIDTH) << endl;
  cout << cap.get(cv::CAP_PROP_FRAME_HEIGHT) << endl;
  cout << cap.get(cv::CAP_PROP_FPS) << endl;
#endif
#if 1
  if(!cap.set(cv::CAP_PROP_FRAME_WIDTH, width)) cout << "width err" << endl;
  if(!cap.set(cv::CAP_PROP_FRAME_HEIGHT, height)) cout << "height err" << endl;
  if(!cap.set(cv::CAP_PROP_FPS, fps)) cout << "fps err" << endl;
#endif
  // fourcc = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
  // fourcc = cv::VideoWriter::fourcc('m', 'p', '4', 'v');
  // fourcc = cv::VideoWriter::fourcc('X', 'V', 'I', 'D');
  // fourcc = cv::VideoWriter::fourcc('D', 'I', 'V', 'X');
  // fourcc = cv::VideoWriter::fourcc('X', '2', '6', '4');
  fourcc = 0x00000020; // fallback tag
  bool col = true;
  cv::VideoWriter wr("AFewCam.mp4", fourcc, fps, cv::Size(width, height), col);
  CVtickFPS tfps(10);
  cv::Mat frm, vc3, vc2;
#if 0
  while(cap.read(frm)){
#else // fake input from screen
  CVw32CapScr cvw32cs(cv::Rect(960, 512, 320, 240));
  CVDXVCapScr cvdxvcs3(3);
  CVDXVCapScr cvdxvcs2(2);
  while(true){
    frm = cvw32cs.cap(cv::Size(width, height));
    vc3 = cvdxvcs3.cap(cv::Size(width, height));
    vc2 = cvdxvcs2.cap(cv::Size(width, height));
#endif
    int cnt = tfps.getCnt();
    tfps.update();
    tfps.dsp(frm);
    cv::Point ct(300, 220);
    drawCircularROI(frm, ct, 32, cv::Vec3b(32, 192, 240));
    drawCircularROI(frm, cv::Point(320, 240), 8, cv::Vec3b(240, 32, 192));
    cv::imshow(wn[0], frm);
    cv::imshow(wn[1], vc3);
    cv::imshow(wn[2], vc2);
    cv::Mat im = pinp(cap, frm);
    wr << im;
    cv::imshow(wn[3], im);
    int k = cv::waitKey(1); // 1ms > 15ms ! on Windows
    if(k == 'q' || k == '\x1b') break;
  }
  wr.release();
  cap.release();
  cv::destroyAllWindows();
  // should be rewrite with true fps (re open writer and copy all frames)
  // but average fps may not be same for each frames
  return "ok";
}

}

using namespace afewcam;

int main(int ac, char **av)
{
  cout << fmt("sizeof(size_t): %zu", sizeof(size_t)) << endl;
  cout << test_capscr(ac, av) << endl;

  vector<char> buf({0x41, 0x42, 0x20, 0x43});
  stringstream ss;
  ss.write(&buf[0], buf.size());
#if 0
  cout << ss.str().c_str();
#else
  while(!ss.eof()){
    string s;
    ss >> s;
    cout << s << endl;
  }
#endif
  return 0;
}
