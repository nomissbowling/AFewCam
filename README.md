AFewCam
=======

virtual-camera for streaming OBS NDC TDL etc

https://github.com/nomissbowling/AFewCam


requirements
------------

- [DX SDK 7.1 Examples](https://github.com/ganboing/sdk71examples)
- [OpenCV 3.4.12](https://github.com/opencv/opencv/releases/tag/3.4.12)
- with CVw32CapScr.dll CVDXVCapScr.dll compile from [RainbowArc](https://github.com/nomissbowling/RainbowArc)


reference
---------

- [DX SDK 2010-10 full 599,452,800](SDK_DirectX9-11_DXSDK_Jun10.exe)
- [DX SDK 2010-10 redist 100,271,992](DirectX90c_directx_Jun2010_redist.exe)
- [DirectShow](https://qiita.com/tomoki0sanaki/items/fec5cb057b9872c0664c)
- [Qiita HexagramNM](https://qiita.com/HexagramNM/items/2311f025f3af758c83a0)
- [NMVCamFilter](https://github.com/HexagramNM/NMVCamFilter)
- [mysourcefilter](https://github.com/syu5-gh/mysourcefilter)
- [svcam](https://github.com/aricatamoy/svcam)
- [(firefly-vj) OutPIN](https://blog.firefly-vj.net/2007/10/08/push-model-source-filter.html)
- [(firefly-vj) Video Renderer](http://blog.firefly-vj.net/2007/10/25/directshow-show-dvcam.html)
- [(firefly-vj) DllRegisterServer](https://blog.firefly-vj.net/2008/05/11/directshow-filter-development-project-setting.html)
- [(MS) DirectShow Filter](https://docs.microsoft.com/ja-jp/windows/win32/directshow/building-directshow-filters)
- [(MS) DllRegisterServer Assignment Category](https://docs.microsoft.com/en-us/windows/win32/directshow/implementing-dllregisterserver)
- [COM Interface Camera Filter IAMFilterMiscFlags IAMStreamConfig IKsPropertySet](https://community.osr.com/discussion/245023/virtual-camera-source-filter-directshow)
- [COM](https://ichigopack.net/win32com/)
- [(geek) Grabber](https://www.geekpage.jp/programming/directshow/capture-samplegrabber-1.php)
- [(geek) Grabber](https://www.geekpage.jp/programming/directshow/samplegrabber.php)
- [(geek) Grabber](https://www.geekpage.jp/programming/directshow/video-capture.php)
- [videoInput](https://github.com/ofTheo/videoInput)
- [(OBSproject) libDShowCapture](https://github.com/obsproject/libdshowcapture/blob/master/source/dshow-encoded-device.cpp)
- [GraphStudioNext](https://github.com/cplussharp/graph-studio-next)
- [DirectShow](https://interaction-lab.org/kawashima/firewire/directshow.html)
- [DirectShow](http://tarodev.web.fc2.com/dshow2.html)
- [Filter Property Page](http://mibc.blog.fc2.com/blog-category-25.html)


filter
------

64bit

- "C:\Windows\System32\regsvr32.exe" [/u] "<path>\AFewCamFilter_x64.dll"
- "C:\Program Files (x86)\Windows Kits\10\bin\10.0.17763.0\x64\graphedt.exe"
-   proppage.dll

32bit

- "C:\Windows\SysWOW64\regsvr32.exe" [/u] "<path>\AFewCamFilter_x86.dll"
- "C:\Program Files (x86)\Windows Kits\10\bin\10.0.17763.0\x86\graphedt.exe"
-   proppage.dll


license
-------

MIT license
