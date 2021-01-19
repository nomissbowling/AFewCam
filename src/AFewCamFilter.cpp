/*
  AFewCamFilter.cpp

  advapi32.lib for RegSetValueExW Reg[Enum|Open|Close|Create|Delete]KeyExW
  oleaut32.lib for SysFreeString SysAllocString
  legacy_stdio_definitions.lib for _vsnwprintf_s

  headers from https://github.com/ganboing/sdk71examples
    download and extract sdk71examples-master_ganboing_6779974_20150120.zip
    copy sdk71examples-master/multimedia/directshow/baseclasses
    to ../baseclasses

    compile ../baseclasses/*.cpp to static library with -DUNICODE
      ../lib/strmbase_u_x64.lib
      ../lib/strmbase_u_x86.lib

  needs AFewCamFilter.def (not use __declspec(dllexport))
    EXPORTS
        DllGetClassObject PRIVATE
        DllCanUnloadNow PRIVATE
        DllRegisterServer PRIVATE
        DllUnregisterServer PRIVATE
------------------------------------------------------------------------
  "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.16.27023\bin\Hostx64\x64\cl.exe" -source-charset:utf-8 -execution-charset:utf-8 -EHsc -LD -Fe..\bin\AFewCamFilter_x64.dll ..\src\AFewCamFilter.cpp -I..\include -I..\baseclasses -link /DEF:..\src\AFewCamFilter.def /LIBPATH:..\lib /LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.16.27023\lib\x64" /LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.17763.0\ucrt\x64" /LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.17763.0\um\x64" oleaut32.lib advapi32.lib kernel32.lib user32.lib gdi32.lib winmm.lib ole32.lib strmbase_u_x64.lib strmiids.lib legacy_stdio_definitions.lib

    to ..\bin\ AFewCamFilter_x64.dll AFewCamFilter_x64.lib

  "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.16.27023\bin\Hostx86\x86\cl.exe" -source-charset:utf-8 -execution-charset:utf-8 -EHsc -LD -Fe..\bin\AFewCamFilter_x86.dll ..\src\AFewCamFilter.cpp -I..\include -I..\baseclasses -link /MACHINE:x86 /DEF:..\src\AFewCamFilter.def /LIBPATH:..\lib /LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.16.27023\lib\x86" /LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.17763.0\ucrt\x86" /LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.17763.0\um\x86" oleaut32.lib advapi32.lib kernel32.lib user32.lib gdi32.lib winmm.lib ole32.lib strmbase_u_x86.lib strmiids.lib legacy_stdio_definitions.lib

    to ..\bin\ AFewCamFilter_x86.dll AFewCamFilter_x86.lib
------------------------------------------------------------------------
  run as administrator
  "C:\Windows\System32\regsvr32.exe" [/u] "<path>\AFewCamFilter_x64.dll"
  "C:\Windows\SysWOW64\regsvr32.exe" [/u] "<path>\AFewCamFilter_x86.dll"

  "C:\Program Files (x86)\Windows Kits\10\bin\10.0.17763.0\x64\graphedt.exe"
  "C:\Program Files (x86)\Windows Kits\10\bin\10.0.17763.0\x86\graphedt.exe"
*/

#include "AFewCamFilter.h"

#define LOGING_LEVEL 5
#define FILTER_NAME L"AFewCam_VCFilter"
#define OUTPUT_PIN_NAME L"OutPIN"
#if 0
#define CAM_WIDTH 640
#define CAM_HEIGHT 480
#define CAM_BITS 32
#else
#define CAM_WIDTH 640
#define CAM_HEIGHT 480
#define CAM_BITS 24
#endif

#define EWP_TITLE_DB "e:\\virtual\\camera_memo_utf-8.txt"

// {41466577-4361-6D5F-5643-46696C746572}
DEFINE_GUID(CLSID_AFewCamSrc,
  0x41466577, 0x4361, 0x6D5F, 0x56, 0x43, 0x46, 0x69, 0x6C, 0x74, 0x65, 0x72);

const AMOVIESETUP_MEDIATYPE sudPinTypes[] = {{
#if CAM_BITS == 32
  &MEDIATYPE_Video, &MEDIASUBTYPE_RGB32}};
#else
  &MEDIATYPE_Video, &MEDIASUBTYPE_RGB24}};
#endif
const AMOVIESETUP_PIN sudPins[] = {{
  OUTPUT_PIN_NAME, FALSE, TRUE, FALSE, FALSE,
  &CLSID_NULL, NULL, 1, sudPinTypes}};
const AMOVIESETUP_FILTER afFilterInfo = {
  &CLSID_AFewCamSrc, FILTER_NAME,
#if 0
  MERIT_DO_NOT_USE,
#else
  MERIT_NORMAL,
#endif
#if 0
  0, NULL // nop filter
#else
  1, sudPins // make out PINs
#endif
};

REGFILTER2 rf2FilterReg = {
  1,
#if 0
  MERIT_DO_NOT_USE,
#else
  MERIT_NORMAL,
#endif
#if 0
  0, NULL // nop filter
#else
  1, sudPins // make out PINs
#endif
};

CFactoryTemplate g_Templates[] = {
  {FILTER_NAME, &CLSID_AFewCamSrc, CAFewCamSrc::CreateInstance, NULL, &afFilterInfo}};
int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);

STDAPI DllRegisterServer()
{
#if 0
  return AMovieDllRegisterServer2(TRUE);
#else
  HRESULT hr = AMovieDllRegisterServer2(TRUE);
  if(FAILED(hr)) return hr;
  IFilterMapper2 *pFM2 = NULL;
  hr = CoCreateInstance(CLSID_FilterMapper2, NULL, CLSCTX_INPROC_SERVER,
    IID_IFilterMapper2, (void **)&pFM2);
  if(FAILED(hr)) return hr;
  if(pFM2){
    IMoniker *pMoniker = NULL;
    hr = pFM2->RegisterFilter(CLSID_AFewCamSrc, FILTER_NAME, &pMoniker,
      &CLSID_VideoInputDeviceCategory, FILTER_NAME, &rf2FilterReg);
    pFM2->Release();
  }
  return hr;
#endif
}

STDAPI DllUnregisterServer()
{
#if 0
  return AMovieDllRegisterServer2(FALSE);
#else
  HRESULT hr = AMovieDllRegisterServer2(FALSE);
  if(FAILED(hr)) return hr;
  IFilterMapper2 *pFM2 = NULL;
  hr = CoCreateInstance(CLSID_FilterMapper2, NULL, CLSCTX_INPROC_SERVER,
    IID_IFilterMapper2, (void **)&pFM2);
  if(FAILED(hr)) return hr;
  if(pFM2){
    hr = pFM2->UnregisterFilter(
      &CLSID_VideoInputDeviceCategory, FILTER_NAME, CLSID_AFewCamSrc);
    pFM2->Release();
  }
  return hr;
#endif
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved)
{
  DbgSetModuleLevel(LOG_TRACE, LOGING_LEVEL);
  // DbgSetModuleLevel(LOG_ERROR, LOGING_LEVEL);
  return DllEntryPoint((HINSTANCE)hModule, dwReason, lpReserved);
}

BOOL EnumWndTitle(EWPParams *pewp, char *fn)
{
  FILE *fp = fopen(fn, "rb");
  if(fp){
    char tmp[_countof(EWPParams::title) * 4];
    fgets(tmp, sizeof(tmp), fp);
    fclose(fp);
    int l = strlen(tmp);
    if(l > 0 && tmp[l - 1] == 0x0A){ tmp[l - 1] = '\0'; --l; }
#if 0
    for(int i = 0; i < l; ++i) pewp->title[i] = (wchar_t)tmp[i];
    if(l >= 0) pewp->title[l] = L'\0';
#else
    int len = MultiByteToWideChar(CP_UTF8, 0, tmp, -1, NULL, 0);
    MultiByteToWideChar(CP_UTF8, 0, tmp, -1, pewp->title, len); // -1 add L'\0'
    // MB_PRECOMPOSED MB_COMPOSITE MB_ERR_INVALID_CHARS MB_USEGLYPHCHARS
    // if(GetLastError() == ERROR_NO_UNICODE_TRANSLATION) ...
#endif
  }
  return TRUE;
}

BOOL CALLBACK EnumWndProc(HWND hWnd, LPARAM lp)
{
  EWPParams *pewp = (EWPParams *)lp;
  wchar_t buf[_countof(EWPParams::title)] = L"";
  SendMessageTimeoutW(hWnd, WM_GETTEXT, sizeof(buf), (LPARAM)buf,
    SMTO_BLOCK, 100, NULL);
  if(!wcsncmp(buf, pewp->title, wcslen(pewp->title))){
    pewp->hWnd = hWnd;
    return FALSE;
  }
  return TRUE;
}

CUnknown *WINAPI CAFewCamSrc::CreateInstance(IUnknown *pUnk, HRESULT *phr)
{
  CAFewCamSrc *pNewFilter = new CAFewCamSrc(pUnk, phr);
  if(!pNewFilter) *phr = E_OUTOFMEMORY;
  return dynamic_cast<CUnknown *>(pNewFilter);
}

CAFewCamSrc::CAFewCamSrc(IUnknown *pUnk, HRESULT *phr) :
  CSource(FILTER_NAME, pUnk, CLSID_AFewCamSrc)
{
  CPushPin *pPin = new CPushPin(phr, this); // will be deleted by BaseClasses
  if(!pPin) *phr = E_OUTOFMEMORY;
}

CAFewCamSrc::~CAFewCamSrc()
{
}

HRESULT CAFewCamSrc::QueryInterface(REFIID riid, void **ppv)
{
  if(riid == _uuidof(IAMStreamConfig) || riid == _uuidof(IKsPropertySet))
    return m_paStreams[0]->QueryInterface(riid, ppv);
  else
    return CSource::QueryInterface(riid, ppv);
}

CPushPin::CPushPin(HRESULT *phr, CAFewCamSrc *pFilter) :
  CSourceStream(NAME("CPushPin"), phr, pFilter, OUTPUT_PIN_NAME),
  m_pFilter(pFilter), m_Count(0), m_rtFrameLength(666666), // 100 nanosec 15fps
  m_BmpData(NULL), m_Bitmap(NULL), m_Hdc(NULL)
{
  GetMediaType(&m_mt);
}

CPushPin::~CPushPin()
{
  Dispose();
}

void CPushPin::Dispose()
{
  if(m_Bitmap){ DeleteObject(m_Bitmap); m_Bitmap = NULL; }
  if(m_Hdc){ DeleteDC(m_Hdc); m_Hdc = NULL; }
  if(m_BmpData){ delete m_BmpData; m_BmpData = NULL; }
}

BITMAPINFOHEADER *CPushPin::PreSetupHDCandBMI(VIDEOINFO *pvi, BOOL flg)
{
  Dispose();
  BITMAPINFOHEADER *pBmi = &(pvi->bmiHeader);
  if(flg){
    pBmi->biSize = sizeof(BITMAPINFOHEADER);
    pBmi->biWidth = CAM_WIDTH;
    pBmi->biHeight = CAM_HEIGHT;
    pBmi->biPlanes = 1;
    pBmi->biBitCount = CAM_BITS;
    pBmi->biCompression = BI_RGB;
    pBmi->biSizeImage = DIBSIZE(pvi->bmiHeader);
    // biXPelsPerMeter, biYPelsPerMeter, biClrUsed
    pBmi->biClrImportant = 0;
  }
  return pBmi;
}

HRESULT CPushPin::SetupHDCandBMI(BITMAPINFOHEADER *pBmi, size_t sz)
{
  m_BmpData = new DWORD[pBmi->biWidth * pBmi->biHeight];
  memset(m_BmpData, 0, sz);
  HDC dwhdc = GetDC(GetDesktopWindow());
  m_Bitmap = CreateDIBitmap(dwhdc, pBmi, CBM_INIT, m_BmpData, (BITMAPINFO *)pBmi, DIB_RGB_COLORS);
  m_Hdc = CreateCompatibleDC(dwhdc);
  SelectObject(m_Hdc, m_Bitmap);
  ReleaseDC(GetDesktopWindow(), dwhdc);
  return NOERROR;
}

HRESULT CPushPin::CompleteConnect(IPin *pReceivePin)
{
  VIDEOINFO *pvi = (VIDEOINFO *)m_mt.Format();
  BITMAPINFOHEADER *pBmi = PreSetupHDCandBMI(pvi, FALSE);
  SetupHDCandBMI(pBmi, m_mt.GetSampleSize());
  return __super::CompleteConnect(pReceivePin);
}

STDMETHODIMP CPushPin::Disconnect()
{
  Dispose();
  return __super::Disconnect();
}

STDMETHODIMP CPushPin::Notify(IBaseFilter *pSelf, Quality q)
{
#if 0
  return E_FAIL;
#else
  return E_NOTIMPL;
#endif
}

HRESULT CPushPin::GetMediaType(CMediaType *pMediaType)
{
  VIDEOINFO *pvi = (VIDEOINFO *)pMediaType->AllocFormatBuffer(sizeof(VIDEOINFO));
  ZeroMemory(pvi, sizeof(VIDEOINFO));
  pvi->AvgTimePerFrame = m_rtFrameLength;
  // call before use pvi->bmiHeader after ZeroMemory
  BITMAPINFOHEADER *pBmi = PreSetupHDCandBMI(pvi, TRUE);
  SetRectEmpty(&(pvi->rcSource));
  SetRectEmpty(&(pvi->rcTarget));
  pMediaType->SetType(&MEDIATYPE_Video);
  pMediaType->SetFormatType(&FORMAT_VideoInfo);
  const GUID subtype = GetBitmapSubtype(&pvi->bmiHeader);
  pMediaType->SetSubtype(&subtype);
  pMediaType->SetTemporalCompression(FALSE);
  pMediaType->SetSampleSize(DIBSIZE(*pBmi));
  SetupHDCandBMI(pBmi, pMediaType->GetSampleSize());
  return S_OK;
}

HRESULT CPushPin::CheckMediaType(const CMediaType *pMediaType)
{
  HRESULT hr = S_OK;
  CheckPointer(pMediaType, E_POINTER);
  CMediaType mt;
  GetMediaType(&mt);
  if(mt != *pMediaType) hr = E_FAIL;
  FreeMediaType(mt);
  return hr;
}

HRESULT CPushPin::DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *pRequest)
{
#if 1
  VIDEOINFO *pvi = reinterpret_cast<VIDEOINFO *>(m_mt.Format());
#else
  VIDEOINFO *pvi = (VIDEOINFO *)m_mt.Format();
#endif
  ASSERT(pvi != NULL);
  pRequest->cBuffers = 1;
  if(pvi->bmiHeader.biSizeImage > (DWORD)pRequest->cbBuffer)
    pRequest->cbBuffer = pvi->bmiHeader.biSizeImage;
  ALLOCATOR_PROPERTIES Actual;
  HRESULT hr = pAlloc->SetProperties(pRequest, &Actual);
  if(FAILED(hr)) return hr;
  if(Actual.cbBuffer < pRequest->cbBuffer) return E_FAIL;
  return S_OK;
}

HRESULT CPushPin::FillBuffer(IMediaSample *pSample)
{
  CheckPointer(pSample, E_POINTER);
  // check format changed by downstreamfilter
  ASSERT(m_mt.formattype == FORMAT_VideoInfo);
  ASSERT(m_mt.cbFormat >= sizeof(VIDEOINFOHEADER));
  LPBYTE pSampleData = NULL;
  const long size = pSample->GetSize();
  pSample->GetPointer(&pSampleData);
  CRefTime ref;
  m_pFilter->StreamTime(ref);
  PatBlt(m_Hdc, 0, 0, CAM_WIDTH, CAM_HEIGHT, BLACKNESS);
  HFONT fnt = CreateFont(44, 0, 3570, 3570, FW_BOLD, TRUE, TRUE, FALSE,
    SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
    PROOF_QUALITY, VARIABLE_PITCH | FF_ROMAN , L"Migu 1M");
  SelectObject(m_Hdc, fnt);
  TCHAR buf[_countof(EWPParams::title)];
#if 1
  IReferenceClock *clock;
  m_pFilter->GetSyncSource(&clock);
  REFERENCE_TIME stime;
  clock->GetTime(&stime);
#endif
#if 1
  EWPParams ewp = {NULL, L""};
  EnumWndTitle(&ewp, EWP_TITLE_DB);
  EnumWindows(EnumWndProc, (LPARAM)&ewp);
  if(ewp.hWnd){
    HDC wDC = GetDC(ewp.hWnd);
    BitBlt(m_Hdc, 0, 0, CAM_WIDTH, CAM_HEIGHT, wDC, 0, 0, SRCCOPY);
    ReleaseDC(ewp.hWnd, wDC);
  }
  _snwprintf_s(buf, _countof(buf), _TRUNCATE, TEXT("%s"), ewp.title);
#else
  _snwprintf_s(buf, _countof(buf), _TRUNCATE, TEXT("%08x"), ref.Millisecs());
#endif
  TextOut(m_Hdc, 0, 0, buf, lstrlen(buf));
#if 1
  clock->Release();
#endif
  SelectObject(m_Hdc, GetStockObject(SYSTEM_FONT));
  DeleteObject(fnt);
  VIDEOINFO *pvi = (VIDEOINFO *)m_mt.Format();
  GetDIBits(m_Hdc, m_Bitmap, 0, CAM_HEIGHT,
    pSampleData, (BITMAPINFO *)&pvi->bmiHeader, DIB_RGB_COLORS);
  const REFERENCE_TIME delta = m_rtFrameLength;
  REFERENCE_TIME start_time = ref;
  FILTER_STATE state;
  m_pFilter->GetState(0, &state);
  if(state == State_Paused) start_time = 0;
  REFERENCE_TIME end_time = start_time + delta;
  pSample->SetTime(&start_time, &end_time);
  pSample->SetActualDataLength(size);
  pSample->SetSyncPoint(TRUE); // TRUE: key frame, FALSE: delta frame
  // Sleep(CRefTime(m_rtFrameLength).Millisecs()); // wait
#if 1
  ++m_Count;
#endif
  return S_OK;
}

HRESULT CPushPin::QueryInterface(REFIID riid, LPVOID *ppvObj)
{
  if(riid == _uuidof(IAMStreamConfig)) *ppvObj = (IAMStreamConfig *)this;
  else if(riid == _uuidof(IKsPropertySet)) *ppvObj = (IKsPropertySet *)this;
  else return CSourceStream::QueryInterface(riid, ppvObj);
  AddRef();
  return S_OK;
}

ULONG CPushPin::AddRef()
{
  return GetOwner()->AddRef();
}

ULONG CPushPin::Release()
{
  return GetOwner()->Release();
}

HRESULT CPushPin::Get(REFGUID PropSet, ULONG Id, LPVOID InstanceData, ULONG InstanceLength, LPVOID PropertyData, ULONG DataLength, ULONG *BytesReturned)
{
  if(PropSet != AMPROPSETID_Pin) return E_PROP_SET_UNSUPPORTED;
  if(Id != AMPROPERTY_PIN_CATEGORY) return E_PROP_ID_UNSUPPORTED;
  if(PropertyData == NULL && BytesReturned == NULL) return E_POINTER;
  if(BytesReturned) *BytesReturned = sizeof(GUID);
  if(PropertyData == NULL) return S_OK; // Caller just wants to know the size.
  if(DataLength < sizeof(GUID)) return E_UNEXPECTED; // The buffer is too small.
  *(GUID *)PropertyData = PIN_CATEGORY_CAPTURE;
  return S_OK;
}

HRESULT CPushPin::Set(REFGUID PropSet, ULONG Id, LPVOID InstanceData, ULONG InstanceLength, LPVOID PropertyData, ULONG DataLength)
{
  return E_NOTIMPL;
}

HRESULT CPushPin::QuerySupported(REFGUID PropSet, ULONG Id, ULONG *TypeSupport)
{
  if(PropSet != AMPROPSETID_Pin) return E_PROP_SET_UNSUPPORTED;
  if(Id != AMPROPERTY_PIN_CATEGORY) return E_PROP_ID_UNSUPPORTED;
  // We support getting this property, but not setting it.
  if(TypeSupport) *TypeSupport = KSPROPERTY_SUPPORT_GET;
  return S_OK;
}

HRESULT CPushPin::GetFormat(AM_MEDIA_TYPE **ppmt)
{
  *ppmt = CreateMediaType(&m_mt);
  return S_OK;
}

HRESULT CPushPin::GetNumberOfCapabilities(int *piCount, int *piSize)
{
  *piCount = 1;
  *piSize = sizeof(VIDEO_STREAM_CONFIG_CAPS);
  return S_OK;
}

HRESULT CPushPin::GetStreamCaps(int iIndex, AM_MEDIA_TYPE **ppmt, BYTE *pSCC)
{
  *ppmt = CreateMediaType(&m_mt);
  VIDEOINFOHEADER *pvi = (VIDEOINFOHEADER *)(*ppmt)->pbFormat;

  BITMAPINFOHEADER *pBmi = &(pvi->bmiHeader);
  pBmi->biSize = sizeof(BITMAPINFOHEADER);
  pBmi->biWidth = CAM_WIDTH;
  pBmi->biHeight = CAM_HEIGHT;
  pBmi->biPlanes = 1;
  pBmi->biBitCount = CAM_BITS;
  pBmi->biCompression = BI_RGB;
  pBmi->biSizeImage = GetBitmapSize(&pvi->bmiHeader);
  // biXPelsPerMeter, biYPelsPerMeter, biClrUsed
  pBmi->biClrImportant = 0;

  SetRectEmpty(&(pvi->rcSource));
  SetRectEmpty(&(pvi->rcTarget));

  (*ppmt)->majortype = (const GUID)(*sudPinTypes[0].clsMajorType);
  (*ppmt)->subtype = (const GUID)(*sudPinTypes[0].clsMinorType);
  (*ppmt)->formattype = FORMAT_VideoInfo;
  (*ppmt)->bTemporalCompression = FALSE;
  (*ppmt)->bFixedSizeSamples = TRUE;
  (*ppmt)->lSampleSize = pBmi->biSizeImage;
  (*ppmt)->cbFormat = sizeof(VIDEOINFOHEADER);

  VIDEO_STREAM_CONFIG_CAPS *pvscc = (VIDEO_STREAM_CONFIG_CAPS *)pSCC;
  pvscc->guid = FORMAT_VideoInfo;
  pvscc->VideoStandard = AnalogVideo_None;
  pvscc->InputSize.cx = CAM_WIDTH;
  pvscc->InputSize.cy = CAM_HEIGHT;
  pvscc->MinCroppingSize.cx = CAM_WIDTH;
  pvscc->MinCroppingSize.cy = CAM_HEIGHT;
  pvscc->MaxCroppingSize.cx = CAM_WIDTH;
  pvscc->MaxCroppingSize.cy = CAM_HEIGHT;
  pvscc->CropGranularityX = 80;
  pvscc->CropGranularityY = 60;
  pvscc->CropAlignX = 0;
  pvscc->CropAlignY = 0;

  pvscc->MinOutputSize.cx = CAM_WIDTH;
  pvscc->MinOutputSize.cy = CAM_HEIGHT;
  pvscc->MaxOutputSize.cx = CAM_WIDTH;
  pvscc->MaxOutputSize.cy = CAM_HEIGHT;
  pvscc->OutputGranularityX = 0;
  pvscc->OutputGranularityY = 0;
  pvscc->StretchTapsX = 0;
  pvscc->StretchTapsY = 0;
  pvscc->ShrinkTapsX = 0;
  pvscc->ShrinkTapsY = 0;
  pvscc->MinFrameInterval = 200000; // 50fps
  pvscc->MaxFrameInterval = 50000000; // 0.2fps
  pvscc->MinBitsPerSecond = (CAM_WIDTH * CAM_HEIGHT * CAM_BITS) / 5;
  pvscc->MaxBitsPerSecond = (CAM_WIDTH * CAM_HEIGHT * CAM_BITS) * 50;

  return S_OK;
}

HRESULT CPushPin::SetFormat(AM_MEDIA_TYPE *pmt)
{
  VIDEOINFOHEADER *pvi = (VIDEOINFOHEADER *)m_mt.pbFormat;
  m_mt = *pmt;
  IPin *pin;
  ConnectedTo(&pin);
  if(pin){
    IFilterGraph *pGraph = m_pFilter->GetGraph();
    pGraph->Reconnect(this);
  }
  return S_OK;
}

ULONG CPushPin::GetMiscFlags()
{
  return AM_FILTER_MISC_FLAGS_IS_SOURCE;
}
