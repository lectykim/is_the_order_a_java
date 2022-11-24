// is_the_order_a_java.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "is_the_order_a_java.h"

#include <stdlib.h>     // 랜덤 함수 사용
#include <time.h>       // 랜덤 시드 값을 변경
#include <vector>

using namespace std;

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ISTHEORDERAJAVA, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ISTHEORDERAJAVA));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ISTHEORDERAJAVA));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ISTHEORDERAJAVA);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//


// Player
RECT g_player;

//1분동안 작동
int g_timer;

//100개의 RECT를 만든다!
vector<RECT> g_rect_top;
vector<RECT> g_rect_bot;

//Handle Vec
vector<HANDLE> g_handle;

//현재 게임 종료 상탠지 확인
bool g_status;

//윈도우의 가로 세로 크기
int vpWidth;
int vpHeight;

//난수 초기화
int g_rand;

//Life 
int g_life = 15;

//Java, Kotlin - Life Plus;
RECT g_java;
int g_java_place;

//C,C++,C#
RECT g_c;
int g_c_place;

DWORD WINAPI drawRects(LPVOID param) {
    HWND hWnd = (HWND)param;
    HDC hdc;
    
    hdc = GetDC(hWnd);


    int height = vpHeight / 2 - 100;
    int width = vpWidth - 100;


    //상단 장애물 생성
    RECT rect_top, rect_bot;
    rect_top.left = width;
    rect_top.right = width + 50;
    rect_top.top = 10;
    rect_top.bottom = g_rand % height;
    g_rect_top.push_back(rect_top);

    //하단 장애물 생성
    rect_bot.left = width;
    rect_bot.right = width + 50;
    rect_bot.bottom = vpHeight - 10;
    rect_bot.top = vpHeight - (g_rand/2 % height) - 10;
    g_rect_bot.push_back(rect_bot);

    RECT is;

        for (int i = 0; i < g_rect_top.size(); i++) {

            g_rect_top[i].left -= 100;
            g_rect_top[i].right -= 100;
            g_rect_bot[i].left -= 100;
            g_rect_bot[i].right -= 100;
            if (i % 4 == 0) {
                g_rect_top[i].top += 100;
                g_rect_top[i].bottom += 100;
                g_rect_bot[i].top -= 100;
                g_rect_bot[i].bottom -= 100;
            }

            //사각형에 겹치면,
            if (IntersectRect(&is, &g_player, &g_rect_top[i]) || IntersectRect(&is, &g_player, &g_rect_bot[i])) 
            {
                g_life--;
                InvalidateRect(hWnd, NULL, true);
            }

        }
        InvalidateRect(hWnd, NULL, true);
        Sleep(100);
    

    ReleaseDC(hWnd, hdc);

    ExitThread(0);
    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        srand(time(NULL));
        //그라운드, 플레이어 초기값


        g_player.left = 10;
        g_player.right = 60;
        g_player.bottom = 648;
        g_player.top = 548;

        //30second동안 작성
        g_timer = 30;
        
        //시작하면 true
        g_status = true;

        
        //Java,C의 위치 선정
        g_java_place = rand() % 300;
        g_c_place = rand() / 2 % 300;

        g_java.left = g_java_place;
        g_java.right = g_java.left + 50;
        g_java.top = g_java_place;
        g_java.bottom = g_java.top + 50;

        g_c.left = g_c_place;
        g_c.right = g_c.left + 50;
        g_c.top = g_c_place;
        g_c.bottom = g_c.top + 50;

        
        
        
        //Player Gravity
        SetTimer(hWnd, 1, 200, NULL);

        //제한 시간
        SetTimer(hWnd, 2, 1000,NULL);
        
        //블록 생성
        SetTimer(hWnd, 3, 500, NULL);

        //life가 없으면 종료
        SetTimer(hWnd, 4, 10, NULL);


        
    }
    break;
    case WM_TIMER:
    {
        HDC hdc = GetDC(hWnd);
        if (1 == wParam) 
        {
            if (g_player.bottom < vpHeight-10)
            {
                g_player.top += 20;
                g_player.bottom+= 20;
                
            }
            if (g_player.left < 10)
            {
                g_player.left = 10;
                g_player.right = 60;
            }
            if (g_player.top < 10)
            {
                g_player.top = 10;
                g_player.bottom = 110;
            }
            if (g_player.right > vpWidth-10)
            {
                g_player.right = vpWidth-10;
                g_player.left = vpWidth-60;
            }
            if (g_player.bottom > vpHeight-10)
            {
                g_player.bottom = vpHeight-10;
                g_player.top = vpHeight-110;
            }

        }
        else if (2 == wParam)
        {
            g_timer--;
            if (0 >= g_timer) 
            {
                g_status = false;
                KillTimer(hWnd, 2);
                KillTimer(hWnd, 1);
                KillTimer(hWnd, 3);
                KillTimer(hWnd, 4);
                MessageBox(hWnd, L"Game Over", L"Time Out", MB_OK);
            }
        }
        else if (3 == wParam)
        {
            DWORD tid; //Thread ID

            g_rand = rand();

            //쓰레드의 실행
            g_handle.push_back(CreateThread(NULL, 0, drawRects, hWnd, 0, &tid));
            
            //쓰레드 실패시
            if (g_handle.back() == NULL) {
                MessageBox(hWnd, L"스레드 생성 실패", L"저런", MB_OK);
                break;
            }




        }
        else if (4 == wParam) 
        {
            if (g_life <= 0)
            {
                for (int i = 0; i < g_handle.size(); i++) {
                    SuspendThread(g_handle[i]);
                }
                KillTimer(hWnd, 2);
                KillTimer(hWnd, 1);
                KillTimer(hWnd, 3);
                KillTimer(hWnd, 4);
                MessageBox(hWnd, L"Game Over", L"Time Out", MB_OK);
                g_status = false;
            }
        }
        InvalidateRect(hWnd, NULL, true);
    }
    break;
    case WM_KEYDOWN:
    {
        if (!g_status)
            break;

        //이동처리
        switch (wParam)
        {
        case VK_LEFT:
            g_player.left -= 10;
            g_player.right -= 10;
            break;
        case VK_RIGHT:
            g_player.right += 10;
            g_player.left+= 10;
            break;
        case VK_UP:
            g_player.top -= 50;
            g_player.bottom -= 50;
            break;
        case VK_DOWN:
            g_player.bottom += 5;
            g_player.top += 5;
            break;
        }
        if (g_player.left < 10)
        {
            g_player.left = 10;
            g_player.right = 60;
        }
        if (g_player.top < 10)
        {
            g_player.top = 10;
            g_player.bottom = 110;
        }
        if (g_player.right > vpWidth - 10)
        {
            g_player.right = vpWidth - 10;
            g_player.left = vpWidth - 60;
        }
        if (g_player.bottom > vpHeight - 10)
        {
            g_player.bottom = vpHeight - 10;
            g_player.top = vpHeight - 110;
        }
        InvalidateRect(hWnd, NULL, true);

    }
    break;
    

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            


            //윈도우 화면의 크기 구하기
            RECT rc;
            //화면 전체의 크기를 구하는 함수
            //GetClientRect(hWnd, &rc);


            //수동 설정
            rc.left = 10;
            rc.right = 700;
            rc.top = 10;
            rc.bottom = 500;

            vpWidth = rc.right - rc.left;
            vpHeight = rc.bottom - rc.top;

            //남은시간
            WCHAR word_life[1024];
            WCHAR word_time[1024];
            WCHAR word_java[1024];
            WCHAR word_c[1024];


            wsprintf(word_life, L"남은 목숨 :");
            wsprintf(word_time, L"남은 시간 : ");
            
            //남은시간 출력
            TextOut(hdc, 700, 150, word_life, wcslen(word_life));

            //남은 라이프를 그림
            for (int i = 0; i < g_life; i++) {
                RECT r;
                r.left = 900 + i * 40;
                r.right = r.left + 25;
                r.top = 150;
                r.bottom = 175;
                Rectangle(hdc, r.left, r.top, r.right, r.bottom);
            }

            //시간 텍스트
            TextOut(hdc, 700, 230, word_time, wcslen(word_time));

            //시간 박스
            RECT time_box;
            time_box.left = 800;
            time_box.right = 800 + 10 * g_timer;
            time_box.top = 230;
            time_box.bottom = 250;
            Rectangle(hdc, time_box.left, time_box.top, time_box.right, time_box.bottom);

            //Item 생성
            Ellipse(hdc, g_java.left, g_java.top, g_java.right, g_java.bottom);
            Ellipse(hdc, g_c.left, g_c.top, g_c.right, g_c.bottom);

            //플레이어
            Rectangle(hdc, g_player.left, g_player.top, g_player.right, g_player.bottom);
            for (int i = 0; i < g_rect_top.size(); i++) {
                Rectangle(hdc, g_rect_top[i].left, g_rect_top[i].top, g_rect_top[i].right, g_rect_top[i].bottom);
            }
            for (int i = 0; i < g_rect_bot.size(); i++) {
                Rectangle(hdc, g_rect_bot[i].left, g_rect_bot[i].top, g_rect_bot[i].right, g_rect_bot[i].bottom);
            }
            EndPaint(hWnd, &ps);


        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
