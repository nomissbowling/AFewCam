/*
  AFewCamFilter.h
*/

#ifndef __AFEWCAMFILTER_H__
#define __AFEWCAMFILTER_H__

#define UNICODE
#define _UNICODE
#include <wchar.h>

#define _USE_MATH_DEFINES
#include <cmath>

#include <string>
#include <cstdio>
#include <cstdlib>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

#define WIN32_LEAN_AND_MEAN
#define _WIN32_DCOM
#define _CRT_SECURE_NO_DEPRECATE
#include <windows.h>
//#pragma warning(disable : 4995)
//#pragma comment(lib, "strmiids.lib")
//#pragma comment(lib, "strmbase.lib")
// now use strmbase_u_x64.lib strmbase_u_x86.lib for UNICODE
// headers from https://github.com/ganboing/sdk71examples
#include <streams.h>
#include <initguid.h>

extern "C" {
STDAPI DllRegisterServer();
STDAPI DllUnregisterServer();
BOOL WINAPI DllEntryPoint(HINSTANCE, ULONG, LPVOID);
};

typedef struct _EWPParams {
  HWND hWnd;
  wchar_t title[256];
} EWPParams;

class CAFewCamSrc : public CSource {
protected:
public:
  static CUnknown *WINAPI CreateInstance(LPUNKNOWN pUnk, HRESULT *phr);
  CAFewCamSrc(LPUNKNOWN pUnk, HRESULT *phr);
  virtual ~CAFewCamSrc();
  // inherit IUnknown
  STDMETHODIMP QueryInterface(REFIID riid, void **ppv); // HRESULT
  IFilterGraph *GetGraph(){ return m_pGraph; }
};

class CPushPin : public CSourceStream,
  public IAMFilterMiscFlags, public IAMStreamConfig, public IKsPropertySet {
protected:
  CAFewCamSrc *m_pFilter; // this pin belongs to the filter
  unsigned __int64 m_Count; // frame counter
  const REFERENCE_TIME m_rtFrameLength; // time per frame
  LPDWORD m_BmpData;
  HBITMAP m_Bitmap;
  HDC m_Hdc;
public:
  CPushPin(HRESULT *phr, CAFewCamSrc *pFilter);
  virtual ~CPushPin();
  virtual void Dispose();
  BITMAPINFOHEADER *PreSetupHDCandBMI(VIDEOINFO *pvi, BOOL flg);
  HRESULT SetupHDCandBMI(BITMAPINFOHEADER *pBmi, size_t sz);
  HRESULT CompleteConnect(IPin *pReceivePin);
  STDMETHODIMP Disconnect();
  STDMETHODIMP Notify(IBaseFilter *pSelf, Quality q);
  // inherit CSourceStream
  HRESULT GetMediaType(CMediaType *pMediaType);
  HRESULT CheckMediaType(const CMediaType *pMediaType);
  HRESULT DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *pRequest);
  HRESULT FillBuffer(IMediaSample *pSample);
  // inherit IUnknown
  STDMETHODIMP QueryInterface(REFIID riid, LPVOID *ppvObj); // HRESULT
  STDMETHODIMP_(ULONG) AddRef();
  STDMETHODIMP_(ULONG) Release();
  // inherit IKsPropertySet
  HRESULT STDMETHODCALLTYPE Get(REFGUID PropSet, ULONG Id, LPVOID InstanceData, ULONG InstanceLength, LPVOID PropertyData, ULONG DataLength, ULONG *BytesReturned);
  HRESULT STDMETHODCALLTYPE Set(REFGUID PropSet, ULONG Id, LPVOID InstanceData, ULONG InstanceLength, LPVOID PropertyData, ULONG DataLength);
  HRESULT STDMETHODCALLTYPE QuerySupported(REFGUID PropSet, ULONG Id, ULONG *TypeSupport);
  // inherit IAMStreamConfig
  HRESULT STDMETHODCALLTYPE GetFormat(AM_MEDIA_TYPE **ppmt);
  HRESULT STDMETHODCALLTYPE GetNumberOfCapabilities(int *piCount, int *piSize);
  HRESULT STDMETHODCALLTYPE GetStreamCaps(int iIndex, AM_MEDIA_TYPE **ppmt, BYTE *pSCC);
  HRESULT STDMETHODCALLTYPE SetFormat(AM_MEDIA_TYPE *pmt);
  // inherit IAMFilterMiscFlags
  ULONG STDMETHODCALLTYPE GetMiscFlags();
};

#endif // __AFEWCAMFILTER_H__
