#include "201001622.h"


float cloudX=0;		//달걀 움직임 좌표
float cloudX2=0;
float cloudX3=0;
float cloudX4=0;
float cloudX5=0;

float jumpValue1 = 0;         //달걀 점수 변수
float jumpValue2 = 0;
float jumpValue3 = 0;
float jumpValue4 = 0;
float jumpValue5 = 0;


float blackHoleX = 0;         //블랙홀 좌표
float blackHoleY = 0;
float tempX = 0;
float tempY = 0;


float t1 = 0;
float tToggle1 = 0.07;

float t2 = 0;
float tToggle2 = 0.07;

float t3 = 0;
float tToggle3 = 0.07;

float t4 = 0;
float tToggle4 = 0.07;

float t5 = 0;
float tToggle5 = 0.07;



int WingX=0;	//파리 날개 좌표

int start = 0;            // 시작판별 변수
int score = 0;            //점수 (달걀 깬 갯수)
float angle = 0;          //칼날의 회전각도
float angleValue = 10;     //칼날의 회전속도
float chainAngle1 = 0;    // 체인1 회전
float chainAngle2 = 0;    // 체인2 회전


float fanXpos  = 0.f;
float fanYpos  = 0.f;
float eggAngle = 0.f;

float characterMove = 0.f;
float characterMoveValue = -5.f;

float accumulatedTime=0;	//경과 시간

bool toggle = false;		//토글키
bool bossTrigger = false;
bool bossSound = true;

int volumeShow = 10;

LONG scaledVolume = DSBVOLUME_MAX - 1000;

CSound* sound = NULL;
CSoundManager* soundManager = NULL;
IDirectSoundBuffer* pDSBuffer = NULL;        //효과음 볼륨컨트롤 가능
IDirectSoundBuffer* pDSBPrimary = NULL;      //배경음악 볼륨컨트롤 가능

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif






int WINAPI WinMain(
    HINSTANCE /* hInstance */,
    HINSTANCE /* hPrevInstance */,
    LPSTR /* lpCmdLine */,
    int /* nCmdShow */
    )
{
    // Ignoring the return value because we want to continue running even in the
    // unlikely event that HeapSetInformation fails.
    HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

    if (SUCCEEDED(CoInitialize(NULL)))
    {
        {
            DemoApp app;

            if (SUCCEEDED(app.Initialize()))
            {
                app.RunMessageLoop();
            }
        }
        CoUninitialize();
    }

    return 0;
}

DemoApp::DemoApp() :
    m_hwnd(NULL),
    m_pD2DFactory(NULL),
	m_pWICFactory(NULL),

    m_pRT(NULL),
    m_pPathGeometry(NULL),
	m_pEgg1(NULL),
	m_pEgg2(NULL),
    m_pEgg3(NULL),
	m_pEgg4(NULL),
	m_pEgg5(NULL),
	m_pMovingEgg(NULL),
	m_blackHole(NULL),

	m_wheelPath(NULL),
	m_flyPath(NULL),
	
	m_pFlyGeometry(NULL),
	m_pRightEyeGeometry(NULL),
	m_pLeftEyeGeometry(NULL),
	m_pMiddleWingGeometry(NULL),
	m_pUpWingGeometry(NULL),
	m_pDownWingGeometry(NULL),

	m_pRectGeo(NULL),

	m_pBitmap(NULL),
	m_pMinionBitmap(NULL),
    m_pJoodyBitmap(NULL),
	m_pElsaBitmap(NULL),
	m_pMaxBitmap(NULL),

	m_pWheel(NULL),
	m_pChain(NULL),
	m_pChain2(NULL),

    m_pBrush(NULL),
    m_pBlackBrush(NULL),
	m_pRedBrush(NULL),
	m_pMinionBrush(NULL),
	m_pJoodyBrush(NULL),
	m_pElsaBrush(NULL),
	m_pMaxBrush(NULL),
	m_pRadialGradientBrush(NULL),
	m_pLinearGradientBrush(NULL),


    m_Animation(),
	m_linearAnimation()
{
}

DemoApp::~DemoApp()
{
    SafeRelease(&m_pD2DFactory);
	SafeRelease(&m_pWICFactory);

    SafeRelease(&m_pRT);
    SafeRelease(&m_pPathGeometry);
	
    SafeRelease(&m_pBrush);
    SafeRelease(&m_pBlackBrush);
	SafeRelease(&m_pRedBrush);
	SafeRelease(&m_pMinionBrush);
	SafeRelease(&m_pJoodyBrush);
	SafeRelease(&m_pElsaBrush);
	SafeRelease(&m_pMaxBrush);


	SafeRelease(&m_pEgg1);
	SafeRelease(&m_pEgg2);
	SafeRelease(&m_pEgg3);
	SafeRelease(&m_pEgg4);
	SafeRelease(&m_pEgg5);
	SafeRelease(&m_pMovingEgg);
	SafeRelease(&m_blackHole);


	SafeRelease(&m_wheelPath);
	SafeRelease(&m_flyPath);

	SafeRelease(&m_pFlyGeometry);
	SafeRelease(&m_pRightEyeGeometry);
	SafeRelease(&m_pLeftEyeGeometry);
	SafeRelease(&m_pMiddleWingGeometry);
	SafeRelease(&m_pUpWingGeometry);
	SafeRelease(&m_pDownWingGeometry);

	SafeRelease(&m_pBitmap);
	SafeRelease(&m_pMinionBitmap);
	SafeRelease(&m_pJoodyBitmap);
	SafeRelease(&m_pElsaBitmap);
	SafeRelease(&m_pMaxBitmap);
	SafeRelease(&m_pRadialGradientBrush);
	SafeRelease(&m_pLinearGradientBrush);
	SafeRelease(&m_pRectGeo);

	SafeRelease(&m_pWheel);
	
	SafeRelease(&m_pChain);
	SafeRelease(&m_pChain2);


}

HRESULT DemoApp::Initialize()
{
    HRESULT hr;

    //register window class
    WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = DemoApp::WndProc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = sizeof(LONG_PTR);
    wcex.hInstance     = HINST_THISCOMPONENT;
    wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName  = NULL;
    wcex.lpszClassName = L"App";

    RegisterClassEx(&wcex);

    hr = CreateDeviceIndependentResources();
    if (SUCCEEDED(hr))
    {
        // Create the application window.
        m_hwnd = CreateWindow(
            L"App",
            L"달걀 채색 공장(201001622 이진용)",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            1200,
			600,
            NULL,
            NULL,
            HINST_THISCOMPONENT,
            this
            );
        hr = m_hwnd ? S_OK : E_FAIL;

        if (SUCCEEDED(hr))
        {
            float length = 0;
            hr = m_wheelPath->ComputeLength(
                NULL, //no transform
                &length
                );

            if (SUCCEEDED(hr) )
            {
                m_Animation.SetStart(0);        //start at beginning of path
                m_Animation.SetEnd(length);     //length at end of path
                m_Animation.SetDuration(5.f );  //seconds

                ZeroMemory(&m_DwmTimingInfo, sizeof(m_DwmTimingInfo));
                m_DwmTimingInfo.cbSize = sizeof(m_DwmTimingInfo);

                // Get the composition refresh rate. If the DWM isn't running,
                // get the refresh rate from GDI -- probably going to be 60Hz
                if (FAILED(DwmGetCompositionTimingInfo(NULL, &m_DwmTimingInfo)))
                {
                    HDC hdc = GetDC(m_hwnd);
                    m_DwmTimingInfo.rateCompose.uiDenominator = 1;
                    m_DwmTimingInfo.rateCompose.uiNumerator = GetDeviceCaps(hdc, VREFRESH);
                    ReleaseDC(m_hwnd, hdc);
                }

                ShowWindow(m_hwnd, SW_SHOWNORMAL);

                UpdateWindow(m_hwnd);
            }
        }



		if (SUCCEEDED(hr))
        {
            float pLength = 0;
			hr = m_flyPath->ComputeLength(
                NULL, //no transform
                &pLength
                );

            if (SUCCEEDED(hr) )
            {
                m_linearAnimation.SetStart(0);        //start at beginning of path
                m_linearAnimation.SetEnd(pLength);     //length at end of path
                m_linearAnimation.SetDuration(10.f);  //seconds

                ZeroMemory(&m_DwmTimingInfo, sizeof(m_DwmTimingInfo));
                m_DwmTimingInfo.cbSize = sizeof(m_DwmTimingInfo);

                // Get the composition refresh rate. If the DWM isn't running,
                // get the refresh rate from GDI -- probably going to be 60Hz
                if (FAILED(DwmGetCompositionTimingInfo(NULL, &m_DwmTimingInfo)))
                {
                    HDC hdc = GetDC(m_hwnd);
                    m_DwmTimingInfo.rateCompose.uiDenominator = 1;
                    m_DwmTimingInfo.rateCompose.uiNumerator = GetDeviceCaps(hdc, VREFRESH);
                    ReleaseDC(m_hwnd, hdc);
                }

                ShowWindow(m_hwnd, SW_SHOWNORMAL);

                UpdateWindow(m_hwnd);
            }
        }
    }

	
	//장치 객체 생성
	IDirectSound8* directSound;
	DirectSoundCreate8(NULL,&directSound, NULL);

	
	//협력레벨 지정
	directSound->SetCooperativeLevel(m_hwnd,DSSCL_PRIORITY);

	//주버퍼 포맷 지정
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd,sizeof(DSBUFFERDESC) );
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	dsbd.dwBufferBytes = 0;
	dsbd.lpwfxFormat = NULL;
	

	//IDirectSoundBuffer* pDSBPrimary = NULL;
	directSound->CreateSoundBuffer(&dsbd, &pDSBPrimary, NULL);
	
	WAVEFORMATEX wfx;
	ZeroMemory(&wfx, sizeof(WAVEFORMATEX) );
	wfx.wFormatTag = (WORD)WAVE_FORMAT_PCM;
	wfx.nChannels =(WORD)2;
	wfx.nSamplesPerSec = (DWORD)22050;
	wfx.wBitsPerSample = (WORD)16;
	wfx.nBlockAlign = (WORD)(wfx.wBitsPerSample / 8 * wfx.nChannels );
	wfx.nAvgBytesPerSec = (DWORD)(wfx.nSamplesPerSec * wfx.nBlockAlign );

	pDSBPrimary->SetFormat(&wfx);

	pDSBPrimary->Release();

	//2차 버퍼 생성  BGM
	CWaveFile* pWaveFile = NULL;
	pWaveFile = new CWaveFile();
	pWaveFile->Open( L".\\pokemonCenter.wav", NULL, WAVEFILE_READ );
	
	
	//DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC) );
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRLVOLUME;
	dsbd.guid3DAlgorithm = GUID_NULL;
	dsbd.lpwfxFormat = pWaveFile->m_pwfx;
	dsbd.dwBufferBytes = pWaveFile->GetSize();
	

	
	directSound->CreateSoundBuffer(&dsbd, &pDSBuffer, NULL);



	//이차버퍼에 데이터 채우기
	sound = new CSound(&pDSBuffer, dsbd.dwBufferBytes, 1 , pWaveFile, 0 );  // pDSBuffer 위에 전역변수임


	pDSBPrimary->SetVolume(scaledVolume);

	sound->Play(0,DSBPLAY_LOOPING);
	


	soundManager = new CSoundManager;
	if( ! soundManager->init(m_hwnd) )
		return FALSE;

	int id;
	if (! soundManager->add(L".\\boss.wav", &id)) //id=0부터 시작함.
		return FALSE;
	if (! soundManager->add(L".\\storm.wav",&id));




    return hr;
}

HRESULT DemoApp::CreateDeviceIndependentResources()
{



	static const WCHAR msc_fontName[] = L"Verdana";   //사용할 폰트 이름
    static const FLOAT msc_fontSize = 20;			  //폰트 크기
	

    HRESULT hr = S_OK;
    ID2D1GeometrySink *pSink = NULL;

    // Create a Direct2D factory.
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
	
	if (SUCCEEDED(hr))
    {
        // Create WIC factory.
        hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pWICFactory));
    }

	if (SUCCEEDED(hr))
	{
        // Create a DirectWrite factory.
        hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(m_pDWriteFactory),
            reinterpret_cast<IUnknown **>(&m_pDWriteFactory)
            );
    }
	 if (SUCCEEDED(hr))
    {
        // Create a DirectWrite text format object.
        hr = m_pDWriteFactory->CreateTextFormat(
            msc_fontName,
            NULL,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            msc_fontSize,
            L"", //locale
            &m_pTextFormat
            );
    }

		if (SUCCEEDED(hr))  //  바퀴벌레 기하
			{
				hr = m_pD2DFactory->CreateEllipseGeometry(D2D1::Ellipse(D2D1::Point2F(0,0), 10, 5),&m_pWheel);

			}

	
		if (SUCCEEDED(hr))  //  칼날 반원 기하
		{
			hr = m_pD2DFactory->CreatePathGeometry(&m_pPathGeometry);

			if (SUCCEEDED(hr))
			{
				// Write to the path geometry using the geometry sink.
				hr = m_pPathGeometry->Open(&pSink);

				if (SUCCEEDED(hr))
				{
					pSink->BeginFigure(
						D2D1::Point2F(200 , 100),  // 200 ,100
						D2D1_FIGURE_BEGIN_FILLED
						);
         

					pSink->AddBezier(D2D1::BezierSegment( D2D1::Point2F(220, 300), D2D1::Point2F(380, -100) , D2D1::Point2F(400, 100) ) );   // 220, 300    380 -100     400 100
					
			

					pSink->EndFigure(D2D1_FIGURE_END_CLOSED);


					hr = pSink->Close();
				}

				   SafeRelease(&pSink);
			}
		}


			if (SUCCEEDED(hr))  //  바퀴 경로 기하
			{
			hr = m_pD2DFactory->CreatePathGeometry(&m_wheelPath);

			if (SUCCEEDED(hr))
			{
				// Write to the path geometry using the geometry sink.
				hr = m_wheelPath->Open(&pSink);

				if (SUCCEEDED(hr))
				{
					pSink->BeginFigure(D2D1::Point2F(200,0),D2D1_FIGURE_BEGIN_FILLED);
          
				pSink->AddBezier(D2D1::BezierSegment(
					D2D1::Point2F(250,-50),D2D1::Point2F(80,-80),D2D1::Point2F(0,-50)));
				pSink->AddBezier(D2D1::BezierSegment(
					D2D1::Point2F(-50,-10),D2D1::Point2F(-180,-80),D2D1::Point2F(-200,0)));
				pSink->AddBezier(D2D1::BezierSegment(
					D2D1::Point2F(-230,60),D2D1::Point2F(-80,70),D2D1::Point2F(0,50)));
				pSink->AddBezier(D2D1::BezierSegment(
					D2D1::Point2F(50,10),D2D1::Point2F(180,80),D2D1::Point2F(200,0)));
					
					
		
					pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

					hr = pSink->Close();
				}

				   SafeRelease(&pSink);
			}
		}



		
			if (SUCCEEDED(hr))  // 파리 경로 기하
			{
				hr = m_pD2DFactory->CreatePathGeometry(&m_flyPath);

			if (SUCCEEDED(hr))
			{
				// Write to the path geometry using the geometry sink.
				hr = m_flyPath->Open(&pSink);

				if (SUCCEEDED(hr))
				{
					pSink->BeginFigure(D2D1::Point2F(200,0),D2D1_FIGURE_BEGIN_FILLED);
          
				pSink->AddBezier(D2D1::BezierSegment(
					D2D1::Point2F(250,-50),D2D1::Point2F(80,-80),D2D1::Point2F(-100,-50)));
				pSink->AddBezier(D2D1::BezierSegment(
					D2D1::Point2F(-50,-10),D2D1::Point2F(-180,-80),D2D1::Point2F(-100,0)));
				pSink->AddBezier(D2D1::BezierSegment(
					D2D1::Point2F(-230,60),D2D1::Point2F(-80,70),D2D1::Point2F(50,50)));
				pSink->AddBezier(D2D1::BezierSegment(
					D2D1::Point2F(50,10),D2D1::Point2F(180,80),D2D1::Point2F(300,60)));
					
					
		
					pSink->EndFigure(D2D1_FIGURE_END_CLOSED);

					hr = pSink->Close();
				}

				   SafeRelease(&pSink);
			}
		}

			
	if(SUCCEEDED(hr)) //파리 몸통 기하
    {
        hr = m_pD2DFactory->CreateEllipseGeometry(
			D2D1::Ellipse(D2D1::Point2F(0,0), 20, 10), &m_pFlyGeometry);
    }
	if(SUCCEEDED(hr)) //파리 오른쪽 눈알 기하
    {
        hr = m_pD2DFactory->CreateEllipseGeometry(
			D2D1::Ellipse(D2D1::Point2F(20,-5), 5, 5), &m_pRightEyeGeometry);
    }
	if(SUCCEEDED(hr)) //파리 왼쪽 눈알 기하
    {
        hr = m_pD2DFactory->CreateEllipseGeometry(
			D2D1::Ellipse(D2D1::Point2F(20,5), 5, 5), &m_pLeftEyeGeometry);
    }

	if(SUCCEEDED(hr)) //파리 날개 가운데 기하
    {
        hr = m_pD2DFactory->CreatePathGeometry(&m_pMiddleWingGeometry);
        if(SUCCEEDED(hr))
        {
            ID2D1GeometrySink *pSink = NULL;

            hr = m_pMiddleWingGeometry->Open(&pSink);
            if (SUCCEEDED(hr))
            {
                pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

                pSink->BeginFigure(
                    D2D1::Point2F(10, -9),
                    D2D1_FIGURE_BEGIN_FILLED
                    );

				pSink->AddLine(D2D1::Point2F(10, -15));
				pSink->AddArc(D2D1::ArcSegment(D2D1::Point2F(-10,-15),D2D1::SizeF(10,5),0,
					D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE,D2D1_ARC_SIZE_SMALL));
				pSink->AddLine(D2D1::Point2F(-10, -9));
				pSink->AddLine(D2D1::Point2F(-10, 9));
				pSink->AddLine(D2D1::Point2F(-10, 15));
				pSink->AddArc(D2D1::ArcSegment(D2D1::Point2F(10,15),D2D1::SizeF(10,5),0,
					D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE,D2D1_ARC_SIZE_SMALL));

                pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
            }
            hr = pSink->Close();

            SafeRelease(&pSink);
       }
    }
	if(SUCCEEDED(hr)) //파리 날개 위쪽 기하
    {
        hr = m_pD2DFactory->CreatePathGeometry(&m_pUpWingGeometry);
        if(SUCCEEDED(hr))
        {
            ID2D1GeometrySink *pSink = NULL;

            hr = m_pUpWingGeometry->Open(&pSink);
            if (SUCCEEDED(hr))
            {
                pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

                pSink->BeginFigure(
                    D2D1::Point2F(10, -9),
                    D2D1_FIGURE_BEGIN_FILLED
                    );

				pSink->AddLine(D2D1::Point2F(10, -15));
				pSink->AddArc(D2D1::ArcSegment(D2D1::Point2F(-10,-15),D2D1::SizeF(10,5),45,
					D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE,D2D1_ARC_SIZE_SMALL));
				pSink->AddLine(D2D1::Point2F(-10, -9));
				pSink->AddLine(D2D1::Point2F(-10, 9));
				pSink->AddLine(D2D1::Point2F(-10, 15));
				pSink->AddArc(D2D1::ArcSegment(D2D1::Point2F(10,15),D2D1::SizeF(10,5),-45,
					D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE,D2D1_ARC_SIZE_SMALL));

                pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
            }
            hr = pSink->Close();

            SafeRelease(&pSink);
       }
    }
	if(SUCCEEDED(hr)) //파리 날개 아래쪽 기하
    {
        hr = m_pD2DFactory->CreatePathGeometry(&m_pDownWingGeometry);
        if(SUCCEEDED(hr))
        {
            ID2D1GeometrySink *pSink = NULL;

            hr = m_pDownWingGeometry->Open(&pSink);
            if (SUCCEEDED(hr))
            {
                pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

                pSink->BeginFigure(
                    D2D1::Point2F(10, -9),
                    D2D1_FIGURE_BEGIN_FILLED
                    );

				pSink->AddLine(D2D1::Point2F(10, -15));
				pSink->AddArc(D2D1::ArcSegment(D2D1::Point2F(-10,-15),D2D1::SizeF(10,5),-45,
					D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE,D2D1_ARC_SIZE_SMALL));
				pSink->AddLine(D2D1::Point2F(-10, -9));
				pSink->AddLine(D2D1::Point2F(-10, 9));
				pSink->AddLine(D2D1::Point2F(-10, 15));
				pSink->AddArc(D2D1::ArcSegment(D2D1::Point2F(10,15),D2D1::SizeF(10,5),45,
					D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE,D2D1_ARC_SIZE_SMALL));

                pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
            }
            hr = pSink->Close();

            SafeRelease(&pSink);
       }
    }


	m_pWingGeometry[0] = m_pMiddleWingGeometry;	//중간위치의 날개
	m_pWingGeometry[1] = m_pUpWingGeometry;		//위쪽 위치의 날개
	m_pWingGeometry[2] = m_pMiddleWingGeometry;	//중간위치의 날개
	m_pWingGeometry[3] = m_pDownWingGeometry;	//아래쪽 위치의 날개

    return hr;
}

HRESULT DemoApp::CreateDeviceResources()
{
    HRESULT hr = S_OK;

    if (!m_pRT)
    {
        RECT rc;
        GetClientRect(m_hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(
            (rc.right - rc.left),
            (rc.bottom - rc.top)
            );

        // Create a Direct2D render target
        hr = m_pD2DFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(m_hwnd, size),
            &m_pRT
            );



	  if (SUCCEEDED(hr))
      {
            // Create a bitmap by loading it from a file.
			hr = LoadBitmapFromFile(m_pRT, m_pWICFactory, L".\\eggFactory.png", 300, 179, &m_pBitmap);   //m_pBitmap
      }
		

	  if (SUCCEEDED(hr))
      {
                  // Create a bitmap by loading it from a file.
				hr = LoadBitmapFromFile(m_pRT, m_pWICFactory, L".\\minion.png", 150, 150, &m_pMinionBitmap); 
      }

	  if (SUCCEEDED(hr))
      {
                  // Create a bitmap by loading it from a file.
				hr = LoadBitmapFromFile(m_pRT, m_pWICFactory, L".\\joody.png", 150, 150, &m_pJoodyBitmap); 
      }


	  if (SUCCEEDED(hr))
      {
                  // Create a bitmap by loading it from a file.
				hr = LoadBitmapFromFile(m_pRT, m_pWICFactory, L".\\elsa.png", 150, 150, &m_pElsaBitmap); 
      }

	  if (SUCCEEDED(hr))
      {
                  // Create a bitmap by loading it from a file.
				hr = LoadBitmapFromFile(m_pRT, m_pWICFactory, L".\\max.png", 150, 150, &m_pMaxBitmap); 
      }

	  if( SUCCEEDED(hr))
	  {

		  hr = LoadBitmapFromFile(m_pRT,m_pWICFactory, L".\\blackHole.png",200,150,&m_pBlackHoleBitmap); //640 ,360
		  hr = m_pRT->CreateBitmapBrush(m_pBlackHoleBitmap,&m_pBlackHoleBrush);
	  }


			


	     if (SUCCEEDED(hr))
            {
                D2D1_BITMAP_BRUSH_PROPERTIES propertiesXClampYClamp = D2D1::BitmapBrushProperties(
                    D2D1_EXTEND_MODE_CLAMP, D2D1_EXTEND_MODE_CLAMP, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);

				 hr = m_pRT->CreateBitmapBrush(m_pMinionBitmap, propertiesXClampYClamp, &m_pMinionBrush);
				 hr = m_pRT->CreateBitmapBrush(m_pJoodyBitmap, propertiesXClampYClamp, &m_pJoodyBrush);
				 hr = m_pRT->CreateBitmapBrush(m_pElsaBitmap, propertiesXClampYClamp, &m_pElsaBrush);
				 hr = m_pRT->CreateBitmapBrush(m_pMaxBitmap, propertiesXClampYClamp, &m_pMaxBrush);
				 //hr = m_pRT->CreateBitmapBrush(m_pBlackHoleBitmap, propertiesXClampYClamp, &m_pBlackHoleBrush);
		 }

				if (SUCCEEDED(hr))
				{
						ID2D1GradientStopCollection *pGradientStops = NULL;

						static const D2D1_GRADIENT_STOP gradientStops[] =
						{
							{   0.f,  D2D1::ColorF(D2D1::ColorF::Black, 1.0f)  },
							{   1.f,  D2D1::ColorF(D2D1::ColorF::White, 0.0f)  },
						};

						hr = m_pRT->CreateGradientStopCollection(gradientStops, 2, &pGradientStops);

						if (SUCCEEDED(hr))
						{
							hr = m_pRT->CreateRadialGradientBrush(
								D2D1::RadialGradientBrushProperties(D2D1::Point2F(75, 75), D2D1::Point2F(0, 0), 75, 75),
								pGradientStops, &m_pRadialGradientBrush);
						}

						if (SUCCEEDED(hr))
						{
							hr = m_pRT->CreateLinearGradientBrush(
								D2D1::LinearGradientBrushProperties(D2D1::Point2F(0, 0), D2D1::Point2F(150, 150)),
								pGradientStops, &m_pLinearGradientBrush);
						}

				}







        if (SUCCEEDED(hr))
        {
            hr = m_pRT->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Black),
                &m_pBrush
                );
        }
        if (SUCCEEDED(hr))
        {
            hr = m_pRT->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Black),
                &m_pBlackBrush
                );
        }

		   if (SUCCEEDED(hr))
        {
            hr = m_pRT->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Gold),
                &m_pRedBrush
                );
        }
    }

	
	

	  


    return hr;
}

void DemoApp::DiscardDeviceResources()
{
    SafeRelease(&m_pRT);
   
	SafeRelease(&m_pRectGeo);
}

void DemoApp::RunMessageLoop()
{
    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

HRESULT DemoApp::OnRender()
{
    HRESULT hr;

    hr = CreateDeviceResources();
    if (SUCCEEDED(hr))
    {


        D2D1_POINT_2F point;
        D2D1_POINT_2F tangent;

		D2D1_MATRIX_3X2_F wheelMatrix;
		D2D1_MATRIX_3X2_F flyMatrix;
	
 

        D2D1_SIZE_F rtSize = m_pRT->GetSize();	//렌더타겟의 사이즈
        float minWidthHeightScale = min(rtSize.width, rtSize.height) / 600;
		
		char tmp[20];	//임시 텍스트를 담을 배열
		
		static float float_time = 0.0f;
		static float fly_time   = 0.0f;

        float length  = m_Animation.GetValue(float_time);
		float fLength = m_linearAnimation.GetValue(fly_time);
		
	
		::ZeroMemory(tmp, 20);	//경과시간을 문자열로 저장하기
		::sprintf(tmp, "경과시간 : %.1f", accumulatedTime);
		int sz1 = ::MultiByteToWideChar(CP_ACP,0,tmp,-1,NULL,0);
		LPWSTR buf1 = new WCHAR[sz1];
		::MultiByteToWideChar(CP_ACP,0,tmp,-1,buf1,sz1);
		
		
		
		::ZeroMemory(tmp, 20);	//이동한 누적거리를 문자열로 저장하기
		::sprintf(tmp, "음향볼륨 : %d", volumeShow);
		int sz2 = ::MultiByteToWideChar(CP_ACP,0,tmp,-1,NULL,0);
		LPWSTR buf2 = new WCHAR[sz2];
		::MultiByteToWideChar(CP_ACP,0,tmp,-1,buf2,sz2);
		

		::ZeroMemory(tmp, 20);	//양품달걀을 문자열로 저장하기
		::sprintf(tmp, "양품달걀 : %d", score);
		int sz3= ::MultiByteToWideChar(CP_ACP,0,tmp,-1,NULL,0);
		LPWSTR buf3 = new WCHAR[sz3];
		::MultiByteToWideChar(CP_ACP,0,tmp,-1,buf3,sz3);

		//바닥을 그릴 사각형
		D2D1_RECT_F Rect = D2D1::RectF(0,rtSize.height/1.4,rtSize.width,rtSize.height);

		//윈도우 창 사이즈에 따라 크기 변환
        D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(
            minWidthHeightScale,
            minWidthHeightScale
            );
		//중앙으로 이동시킬 변환 매트릭스
        D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation(
            rtSize.width / 2,
            rtSize.height / 2
            );

        m_pRT->BeginDraw();

        // Reset to identity transform
        m_pRT->SetTransform(D2D1::Matrix3x2F::Identity());

        //배경 하늘색으로 클리어
		m_pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));
	
		
        // 창그리기
		m_pRT->DrawBitmap(m_pBitmap, D2D1::RectF(0.0f, 0.0f, rtSize.width, rtSize.height/1.4));

	  

		//달걀 1
		hr = m_pD2DFactory->CreateEllipseGeometry(D2D1::Ellipse(D2D1::Point2F(rtSize.width,50),60,40),&m_pEgg1);
		m_pRT->SetTransform(D2D1::Matrix3x2F::Translation(cloudX,300 - jumpValue1 ));
		m_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Beige,1));
		m_pRT->FillGeometry(m_pEgg1, m_pBrush );
		m_pRT->DrawGeometry(m_pEgg1, m_pBlackBrush, 2);

		//달걀 2
		hr = m_pD2DFactory->CreateEllipseGeometry(D2D1::Ellipse(D2D1::Point2F(rtSize.width,50),60,40),&m_pEgg2);
		m_pRT->SetTransform(D2D1::Matrix3x2F::Translation(cloudX2,300 - jumpValue2 ));
		m_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::DeepSkyBlue,1));
		m_pRT->FillGeometry(m_pEgg2, m_pBrush );
		m_pRT->DrawGeometry(m_pEgg2, m_pBlackBrush, 2);


		//달걀 3
		hr = m_pD2DFactory->CreateEllipseGeometry(D2D1::Ellipse(D2D1::Point2F(rtSize.width,50),60,40),&m_pEgg3);
		m_pRT->SetTransform(D2D1::Matrix3x2F::Translation(cloudX3,300 - jumpValue3 ));
		m_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Green,1));
		m_pRT->FillGeometry(m_pEgg3, m_pBrush );
		m_pRT->DrawGeometry(m_pEgg3, m_pBlackBrush, 2);


		//달걀 4
		hr = m_pD2DFactory->CreateEllipseGeometry(D2D1::Ellipse(D2D1::Point2F(rtSize.width,50),60,40),&m_pEgg4);
		m_pRT->SetTransform(D2D1::Matrix3x2F::Translation(cloudX4,300 - jumpValue4 ));
		m_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Yellow,1));
		m_pRT->FillGeometry(m_pEgg4, m_pBrush );
		m_pRT->DrawGeometry(m_pEgg4, m_pBlackBrush, 2);


		//달걀 5
		hr = m_pD2DFactory->CreateEllipseGeometry(D2D1::Ellipse(D2D1::Point2F(rtSize.width,50),60,40),&m_pEgg5);
		m_pRT->SetTransform(D2D1::Matrix3x2F::Translation(cloudX5,300 - jumpValue5 ));
		m_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red,1));
		m_pRT->FillGeometry(m_pEgg5, m_pBrush );
		m_pRT->DrawGeometry(m_pEgg5, m_pBlackBrush, 2);

		m_pRT->SetTransform(D2D1::Matrix3x2F::Identity());

	
		//////////////////////////////////////////////////////////////////////////////////
		// 달걀 뺏어가는 블랙홀


		
		
		if( score != 0 && (score % 20) == 0 )
		{
			bossTrigger = true;

			if( bossSound == true )
				soundManager->play(0,FALSE);

			tempX = 80 + rand() % 1000;
			tempY = 80 + rand() % 200;
		}

		
		if( bossTrigger == true )
		{
			
			blackHoleX = tempX;
			blackHoleY = tempY;
		
			hr = m_pD2DFactory->CreateEllipseGeometry(D2D1::Ellipse(D2D1::Point2F(80,80),80,80),&m_blackHole);
			
			m_pRT->SetTransform(D2D1::Matrix3x2F::Translation(blackHoleX - 80 , blackHoleY - 80) );
		
			m_pRT->FillGeometry(m_blackHole, m_pBlackHoleBrush);
			m_pRT->DrawGeometry(m_blackHole, m_pBlackBrush,1);

			m_pRT->SetTransform(D2D1::Matrix3x2F::Identity());

			int x = rtSize.width/2 + fanXpos;
			int y = 200 + fanYpos;

		
			if( blackHoleX - 60 < x && x < blackHoleX + 60 && blackHoleY - 60 < y && y < blackHoleY + 60 )
			{
				bossTrigger == false;
				
				if( bossSound == true )
					soundManager->play(1,FALSE);
			}
			
			else 
			{ 
				static int count;

				count++;

				if( score > 0 && ( count % 20 ) == 0 )
					score = score - 2;


			}

		}
		

		m_pRT->SetTransform(D2D1::Matrix3x2F::Identity());


		/////////////////////////////////////////////////////////////////////////

		// 내가 조종하는 달걀
		hr = m_pD2DFactory->CreateEllipseGeometry(D2D1::Ellipse(D2D1::Point2F(rtSize.width/2 + fanXpos ,200 + fanYpos),60,40),&m_pMovingEgg);
		
		m_pRT->SetTransform(D2D1::Matrix3x2F::Rotation( eggAngle, D2D1::Point2F( rtSize.width/2 + fanXpos, 200 + fanYpos ) ) ); 
		
		m_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::HotPink,1));
		m_pRT->FillGeometry(m_pMovingEgg, m_pBrush );
		m_pRT->DrawGeometry(m_pMovingEgg, m_pBlackBrush, 2);

		m_pRT->SetTransform(D2D1::Matrix3x2F::Identity());
		//////////////////////////////////////////////////////////////////////////////////////
		
	
	
		//칼날
		D2D1_MATRIX_3X2_F translateEllipse = D2D1::Matrix3x2F::Translation( 200 ,115 ); //200 115
		D2D1_MATRIX_3X2_F rotateEllipse = D2D1::Matrix3x2F::Rotation( angle, D2D1::Point2F( 300 , 100  ) );  //300 ,100

		m_pRT->SetTransform( rotateEllipse * translateEllipse );
	
        m_pRT->DrawGeometry(m_pPathGeometry, m_pBlackBrush, 5.f, 0);
		m_pRT->FillGeometry(m_pPathGeometry, m_pRedBrush, 0);

		//////////////////////////////////////////////////////
	


		if( start != 0 )
		{
			if( cloudX == 0 )
				score++;
	
			if( cloudX2 == 0 )
				score++;

			if( cloudX3 == 0 )
				score++;

			if( cloudX4 == 0 )
				score++;

			if( cloudX5 == 0 )
				score++;

			if( float_time == 0 )
			{
				if(score > 0)
					score -= 1;
			}
		}
			start = 1;

		//매트릭스 초기화
		m_pRT->SetTransform(D2D1::Matrix3x2F::Identity());

		


		//컨베이어벨트 그리기
		m_pRT->DrawRectangle(Rect, m_pBlackBrush,30);
		m_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::LightGray,1));
		m_pRT->FillRectangle(Rect,m_pBrush);

		m_pRT->SetTransform(D2D1::Matrix3x2F::Identity());

		
		//미니언
	    hr = m_pD2DFactory->CreateRectangleGeometry( D2D1::RectF(0,0,150,150), &m_pRectGeo );
		m_pRT->SetTransform(D2D1::Matrix3x2F::Translation(300 + characterMove,405) );
		m_pRT->FillGeometry(m_pRectGeo, m_pMinionBrush,m_pRadialGradientBrush);
		

		//엘사
	
		m_pRT->SetTransform(D2D1::Matrix3x2F::Translation(450 + characterMove,405) );
		m_pRT->FillGeometry(m_pRectGeo, m_pElsaBrush,m_pRadialGradientBrush);
		

		//주디 홉스

		m_pRT->SetTransform(D2D1::Matrix3x2F::Translation(600 + characterMove,405) );
		m_pRT->FillGeometry(m_pRectGeo, m_pJoodyBrush,m_pLinearGradientBrush);


		//베이 맥스
	
		m_pRT->SetTransform(D2D1::Matrix3x2F::Translation(750 + characterMove,405) );
		m_pRT->FillGeometry(m_pRectGeo, m_pMaxBrush,m_pLinearGradientBrush);
		
		m_pRT->SetTransform(D2D1::Matrix3x2F::Identity());
		
///////////////////////////////////////////////////////////////////////////////////////////////////

		//움직이는 바퀴벌레 그리기
		D2D1_MATRIX_3X2_F translateWheel = D2D1::Matrix3x2F::Translation(600,400);

		if(toggle){
		m_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::DarkOrange,1));
		m_pRT->SetTransform( translateWheel );
        m_pRT->DrawGeometry(m_wheelPath, m_pBrush);
		}

	
			hr = m_wheelPath->ComputePointAtLength(
				length, 
				NULL, 
				&point, 
				&tangent); 

			 Assert(SUCCEEDED(hr));

			wheelMatrix = D2D1::Matrix3x2F(
				tangent.x, tangent.y,
				-tangent.y, tangent.x,
				point.x, point.y
            );

				m_pRT->SetTransform(  wheelMatrix * translateWheel );

				//파리 날개 그리기
				m_pRT->DrawGeometry(m_pWingGeometry[WingX], m_pBlackBrush);
				m_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::LightBlue,1));
				m_pRT->FillGeometry(m_pWingGeometry[WingX], m_pBrush);
				//파리 몸통 그리기
				m_pRT->DrawGeometry(m_pFlyGeometry, m_pBlackBrush);
				m_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::DarkCyan,1));
				m_pRT->FillGeometry(m_pFlyGeometry, m_pBrush);
				//파리 눈알 그리기
				m_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black,1));
				m_pRT->FillGeometry(m_pRightEyeGeometry, m_pBrush);
				m_pRT->FillGeometry(m_pLeftEyeGeometry, m_pBrush);

				m_pRT->SetTransform(D2D1::Matrix3x2F::Identity());

///////////////////////////////////////////////////////////////////////////////////

				//움직이는 파리 그리기

		D2D1_MATRIX_3X2_F translateFly = D2D1::Matrix3x2F::Translation(600,200);

		if(toggle){
		m_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::DarkOrange,1));
		m_pRT->SetTransform( translateFly );
        m_pRT->DrawGeometry(m_flyPath, m_pBrush);
		}

	
			hr = m_flyPath->ComputePointAtLength(
				fLength, 
				NULL, 
				&point, 
				&tangent); 

			 Assert(SUCCEEDED(hr));

			flyMatrix = D2D1::Matrix3x2F(
				tangent.x, tangent.y,
				-tangent.y, tangent.x,
				point.x, point.y
            );

		m_pRT->SetTransform(  flyMatrix * translateFly );
				

		//파리 날개 그리기
		m_pRT->DrawGeometry(m_pWingGeometry[WingX], m_pBlackBrush);
		m_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::LightBlue,1));
		m_pRT->FillGeometry(m_pWingGeometry[WingX], m_pBrush);
		//파리 몸통 그리기
		m_pRT->DrawGeometry(m_pFlyGeometry, m_pBlackBrush);
		m_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::ForestGreen,1));
        m_pRT->FillGeometry(m_pFlyGeometry, m_pBrush);
		//파리 눈알 그리기
		m_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black,1));
		m_pRT->FillGeometry(m_pRightEyeGeometry, m_pBrush);
		m_pRT->FillGeometry(m_pLeftEyeGeometry, m_pBrush);


		m_pRT->SetTransform(D2D1::Matrix3x2F::Identity());


/////////////////////////////////////////////////////////////////////////////////////////////////



		//왼쪽 기계 체인 바깥쪽 타원
		hr = m_pD2DFactory->CreateEllipseGeometry(D2D1::Ellipse(D2D1::Point2F(rtSize.width,50),30,50),&m_pChain);
		D2D1_MATRIX_3X2_F translateChain3 = D2D1::Matrix3x2F::Translation(-1100, 400);
		D2D1_MATRIX_3X2_F rotateChain3 = D2D1::Matrix3x2F::Rotation( chainAngle1, D2D1::Point2F(rtSize.width-1100, 450 ) );
	
		m_pRT->SetTransform( translateChain3 * rotateChain3 );
		m_pRT->DrawGeometry(m_pChain, m_pBlackBrush, 2);

		//왼쪽 기계 체인 안쪽 타원
		hr = m_pD2DFactory->CreateEllipseGeometry(D2D1::Ellipse(D2D1::Point2F(rtSize.width, 50),10,20),&m_pChain2);
		D2D1_MATRIX_3X2_F translateChain4 = D2D1::Matrix3x2F::Translation(-1100, 400);
		D2D1_MATRIX_3X2_F rotateChain4 = D2D1::Matrix3x2F::Rotation( chainAngle2 , D2D1::Point2F(rtSize.width-1100, 450 ) );
	
		m_pRT->SetTransform( translateChain4 * rotateChain4 );
		m_pRT->DrawGeometry(m_pChain2, m_pBlackBrush, 2);



		//오른쪽 기계 체인 바깥쪽 타원
		hr = m_pD2DFactory->CreateEllipseGeometry(D2D1::Ellipse(D2D1::Point2F(rtSize.width,50),30,50),&m_pChain);
		D2D1_MATRIX_3X2_F translateChain = D2D1::Matrix3x2F::Translation(-100, 400);
		D2D1_MATRIX_3X2_F rotateChain = D2D1::Matrix3x2F::Rotation( chainAngle1  , D2D1::Point2F(rtSize.width-100, 450 ) );
	
		m_pRT->SetTransform( translateChain * rotateChain );
		m_pRT->DrawGeometry(m_pChain, m_pBlackBrush, 2);

		//오른쪽 기계 체인 안쪽 타원
		hr = m_pD2DFactory->CreateEllipseGeometry(D2D1::Ellipse(D2D1::Point2F(rtSize.width, 50),10,20),&m_pChain2);
		D2D1_MATRIX_3X2_F translateChain2 = D2D1::Matrix3x2F::Translation(-100, 400);
		D2D1_MATRIX_3X2_F rotateChain2 = D2D1::Matrix3x2F::Rotation( chainAngle2  , D2D1::Point2F(rtSize.width-100, 450 ) );
	
		m_pRT->SetTransform( translateChain2 * rotateChain2 );
		m_pRT->DrawGeometry(m_pChain2, m_pBlackBrush, 2);


		
			

		//텍스트를 그리기 위해 매트릭스 초기화
		m_pRT->SetTransform(D2D1::Matrix3x2F::Identity());

		if(score < 0)
		{
			score = 0;
		}

		//경과 시간 문자열 그리기
		m_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White,1));;
		m_pRT->DrawText(buf1, sz1-1, m_pTextFormat,
									 D2D1::RectF(0, 0, rtSize.width, rtSize.height),
									 m_pBrush);


		//문자열 간격
		m_pRT->SetTransform(D2D1::Matrix3x2F::Translation(0,20));
		
		
		//볼륨 표시
		m_pRT->DrawText(buf2, sz2-1, m_pTextFormat,
									 D2D1::RectF(0, 0, rtSize.width, rtSize.height),
									  m_pBrush);
		
		
		
		//문자열 간격
		
		m_pRT->SetTransform(D2D1::Matrix3x2F::Translation(0,40));
		
		//양품달걀 문자열 그리기
		m_pRT->DrawText(buf3, sz3-1, m_pTextFormat,
									 D2D1::RectF(0, 0, rtSize.width, rtSize.height),
									  m_pBrush);
		
		


		hr = m_pRT->EndDraw();



		
			//임시 문자열 삭제
			delete[] buf1;
			delete[] buf2;
			delete[] buf3;



			if (hr == D2DERR_RECREATE_TARGET)
			{
				hr = S_OK;
				DiscardDeviceResources();
			}

			// 한바퀴를 돌면 시간 초기화
			if (float_time >= m_Animation.GetDuration())
			{
				float_time = 0.0f;
			}
			else
			{	//아니면 시간 증가
				float_time += static_cast<float>(m_DwmTimingInfo.rateCompose.uiDenominator) /
							  static_cast<float>(m_DwmTimingInfo.rateCompose.uiNumerator);
			}

			// 한바퀴를 돌면 시간 초기화
			if (fly_time >= m_linearAnimation.GetDuration())
			{
				fly_time = 0.0f;
			}
			else
			{	//아니면 시간 증가
				fly_time += static_cast<float>(m_DwmTimingInfo.rateCompose.uiDenominator) /
							  static_cast<float>(m_DwmTimingInfo.rateCompose.uiNumerator);
			}




		}
		//경과 시간 계산
			accumulatedTime += static_cast<float>(m_DwmTimingInfo.rateCompose.uiDenominator) /
								  static_cast<float>(m_DwmTimingInfo.rateCompose.uiNumerator);


         //컨베이어 벨트 위에 있는 달걀 
		cloudX  -=6.f;	  //달걀 움직임 속도
		cloudX2 -=10.f;
		cloudX3 -=12.f;
		cloudX4 -= 24.f;
		cloudX5 -= 8.f;

		t1 += tToggle1;
		t2 += tToggle2;
		t3 += tToggle3;
		t4 += tToggle4;
		t5 += tToggle5;

		if(cloudX == -1200 )
		{
			cloudX = 0;
			t1 = 0;
		}

		if(cloudX2 == -1200 )
		{
			cloudX2 = 0;
			t2 = 0;
		}

		if(cloudX3 == -1200 )
		{
			cloudX3 = 0;
			t3 = 0;
		}

		if(cloudX4 == -1200 )
		{
			cloudX4 = 0;
			t4 = 0;
		}

		if(cloudX5 == -1200 )
		{
			cloudX5 = 0;
			t5 = 0;
		}


		jumpValue1 = -5 * t1 + 0.5 * 9.8 * t1 * t1;
		jumpValue2 = -5 * t2 + 0.5 * 9.8 * t2 * t2;
		jumpValue3 = -5 * t3 + 0.5 * 9.8 * t3 * t3;
		jumpValue4 = -5 * t4 + 0.5 * 9.8 * t4 * t4;
		jumpValue5 = -5 * t5 + 0.5 * 9.8 * t5 * t5;
		
		if( jumpValue1 > 250 )
			tToggle1 *= -1;

		if( jumpValue2 > 100 )
			tToggle2 *= -1;

		if( jumpValue3 > 50 )
			tToggle3 *= -1;

		if( jumpValue4 > 25 )
			tToggle4 *= -1;

		if( jumpValue5 > 160 )
			tToggle5 *= -1;
		
		///////////////////////////////

		//컨베이어 벨트 돌아가는 장치 컨트롤
		angle += angleValue;

		if( angle == 360 )
		{
			angle = 0;
		}
	
		chainAngle1 -= 5;
		chainAngle2 -= 10;



		// 아래 디즈니 캐릭터 좌우 왔다갔다 컨트롤
		characterMove += characterMoveValue;
		if( characterMove == -100 )
		{
			characterMoveValue *= -1;

		}else if( characterMove == 100 )
		{
			characterMoveValue *= -1;
		}


		// 움직이는 달걀 컨트롤
		eggAngle += 20;

		if( eggAngle == 360 )
			eggAngle = 0;


		if( fanXpos < -530 || fanXpos > 530)
			fanXpos = 0;

		else if( fanYpos < -150 || fanYpos > 140)
			fanYpos = 0;
		

		//똥파리 날개 컨트롤
			WingX = (WingX+1)%4;	//날개 배열의 4개 중 하나 선택
			if(WingX== 12)			
				WingX=0;



		return hr;
}

void DemoApp::OnResize(UINT width, UINT height)
{
    if (m_pRT)
    {
        D2D1_SIZE_U size;
        size.width = width;
        size.height = height;

        m_pRT->Resize(size);
    }
}

void DemoApp::OnKeyDown(SHORT vkey)
{
    switch (vkey)
    {
    case 'T':
        toggle = !toggle;


		if( sound->IsSoundPlaying() )
		{
			sound->Stop();
			bossSound = false;
		}
		else 
		{
			sound->Play(0,DSBPLAY_LOOPING);
			bossSound = true;
		}

        break;

	case 0x31 :        //숫자 1
		jumpValue1 -= 15;
		break;

	case 0x32 :        //숫자 2   
		jumpValue2 -= 15;
		break;

	case 0x33 :        //숫자 3
		jumpValue3 -= -10;
		break;

	case 0x34 :        //숫자 4
		jumpValue4 -= -10;
		break;

	case 0x35 :        //숫자 5
		jumpValue5 -= -15;
		break;

	case 'U' :       // U
		

		if( volumeShow < 20 )
			volumeShow++;

		if( volumeShow == 20 )
		{
			break;
		}
		else if( volumeShow == 1 )
		{
			sound->Play(0,DSBPLAY_LOOPING);
			bossSound = true;
		}
		else
		{
			
			scaledVolume += 100;
		
			pDSBPrimary->SetVolume(scaledVolume);
			pDSBuffer->SetVolume(scaledVolume);
		}

		break;

	case 'D' :       // D
	
		 
		if( volumeShow > 0 )
			volumeShow--;

		if( volumeShow == 0 )   // 음소거
		{ 
			sound->Stop();
			bossSound = false;
		}
		else
		{
		
			scaledVolume -= 100;

			pDSBPrimary->SetVolume(scaledVolume);
			pDSBuffer->SetVolume(scaledVolume);
		}
		

		break;

 	case VK_LEFT:
			fanXpos -= 10;
		
			break;

	case VK_RIGHT:
			fanXpos += 10;

			break;

	case VK_UP:
			fanYpos -= 10;
	
			break;

	case VK_DOWN:
			fanYpos += 10;
			
			break;

    default:
        break;
    }
}

LRESULT CALLBACK DemoApp::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;

    if (message == WM_CREATE)
    {
        LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
        DemoApp *pDemoApp = (DemoApp *)pcs->lpCreateParams;

        ::SetWindowLongPtrW(
            hwnd,
            GWLP_USERDATA,
            PtrToUlong(pDemoApp)
            );

        result = 1;
    }
    else
    {
        DemoApp *pDemoApp = reinterpret_cast<DemoApp *>(static_cast<LONG_PTR>(
            ::GetWindowLongPtrW(
                hwnd,
                GWLP_USERDATA
                )));

        bool wasHandled = false;

        if (pDemoApp)
        {
            switch(message)
            {
            case WM_SIZE:
                {
                    UINT width = LOWORD(lParam);
                    UINT height = HIWORD(lParam);
                    pDemoApp->OnResize(width, height);
                }
                result = 0;
                wasHandled = true;
                break;

            case WM_DISPLAYCHANGE:
                {
                    InvalidateRect(hwnd, NULL, FALSE);
                }
                result = 0;
                wasHandled = true;
                break;

			case WM_KEYDOWN:
                {
                    pDemoApp->OnKeyDown(static_cast<SHORT>(wParam));
                }
                result = 0;
                wasHandled = true;
                break;

            case WM_PAINT:
                {
                    pDemoApp->OnRender();
                }
                result = 0;
                wasHandled = true;
                break;

            case WM_DESTROY:
                {
                    PostQuitMessage(0);
                }
                result = 1;
                wasHandled = true;
                break;



            }
        }

        if (!wasHandled)
        {
            result = DefWindowProc(hwnd, message, wParam, lParam);
        }
    }

    return result;
}

HRESULT DemoApp::LoadBitmapFromFile(
    ID2D1RenderTarget *pRenderTarget,
    IWICImagingFactory *pIWICFactory,
    PCWSTR uri,
    UINT destinationWidth,
    UINT destinationHeight,
    ID2D1Bitmap **ppBitmap
    )
{
    IWICBitmapDecoder *pDecoder = NULL;
    IWICBitmapFrameDecode *pSource = NULL;
    IWICStream *pStream = NULL;
    IWICFormatConverter *pConverter = NULL;
    IWICBitmapScaler *pScaler = NULL;

    HRESULT hr = pIWICFactory->CreateDecoderFromFilename( uri, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);
        
    if (SUCCEEDED(hr))
    {
        // Create the initial frame.
        hr = pDecoder->GetFrame(0, &pSource);
    }
    if (SUCCEEDED(hr))
    {
        // Convert the image format to 32bppPBGRA (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
        hr = pIWICFactory->CreateFormatConverter(&pConverter);
    }
 
    if (SUCCEEDED(hr))
    {
        // If a new width or height was specified, create an IWICBitmapScaler and use it to resize the image.
        if (destinationWidth != 0 || destinationHeight != 0)
        {
            UINT originalWidth, originalHeight;
            hr = pSource->GetSize(&originalWidth, &originalHeight);
            if (SUCCEEDED(hr))
            {
                if (destinationWidth == 0)
                {
                    FLOAT scalar = static_cast<FLOAT>(destinationHeight) / static_cast<FLOAT>(originalHeight);
                    destinationWidth = static_cast<UINT>(scalar * static_cast<FLOAT>(originalWidth));
                }
                else if (destinationHeight == 0)
                {
                    FLOAT scalar = static_cast<FLOAT>(destinationWidth) / static_cast<FLOAT>(originalWidth);
                    destinationHeight = static_cast<UINT>(scalar * static_cast<FLOAT>(originalHeight));
                }

                hr = pIWICFactory->CreateBitmapScaler(&pScaler);
                if (SUCCEEDED(hr))
                {
                    hr = pScaler->Initialize(pSource, destinationWidth, destinationHeight, WICBitmapInterpolationModeCubic);
                }
                if (SUCCEEDED(hr))
                {
                    hr = pConverter->Initialize(pScaler, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);
                }
            }
        }
        else // Don't scale the image.
        {
            hr = pConverter->Initialize(pSource, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);
        }
    }
    if (SUCCEEDED(hr))
    {
        // Create a Direct2D bitmap from the WIC bitmap.
        hr = pRenderTarget->CreateBitmapFromWicBitmap(pConverter, NULL, ppBitmap);
    }

	SafeRelease(&pDecoder);
    SafeRelease(&pSource);
    SafeRelease(&pStream);
    SafeRelease(&pConverter);
    SafeRelease(&pScaler);

    return hr;
}
