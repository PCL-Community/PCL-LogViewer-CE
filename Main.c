#include <windows.h>
#include <commctrl.h>
#include <stdio.h>

#define ID_TABCONTROL 1
#define ID_TEXTBOX 2
#define MAX_LOADSTRING 100

HINSTANCE hInst;
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];
HWND hTab;
HWND hEdit;
OPENFILENAME ofn;
char szFile[260];

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void CreateTabControl(HWND);
void DisplayFileInEditControl(HWND, int);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    MSG msg;
    HACCEL hAccelTable;

    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow)) {
        return FALSE;
    }

    hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_MYAPPLICATION);

    while (GetMessage(&msg, NULL, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX); 
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, (LPCTSTR)IDI_APPLICATION);
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = (LPCTSTR)IDC_MYAPPLICATION;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_APPLICATION);

    return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
   HWND hWnd;

   hInst = hInstance;

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd) {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message) {
        case WM_COMMAND:
            wmId    = LOWORD(wParam);
            wmEvent = HIWORD(wParam);
            // Parse the menu selections:
            switch (wmId) {
                case IDM_ABOUT:
                   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
                   break;
                case IDM_EXIT:
                   DestroyWindow(hWnd);
                   break;
                default:
                   return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;
        case WM_CREATE:
            CreateTabControl(hWnd);
            hEdit = CreateWindow(TEXT("EDIT"), NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL, 0, 30, 400, 400, hWnd, (HMENU)ID_TEXTBOX, GetModuleHandle(NULL), NULL);
            break;
        case WM_SIZE:
            MoveWindow(hTab, 0, 0, LOWORD(lParam), 30, TRUE);
            MoveWindow(hEdit, 0, 30, LOWORD(lParam), HIWORD(lParam) - 30, TRUE);
            break;
        case WM_NOTIFY:
            switch (((LPNMHDR)lParam)->code) {
                case TCN_SELCHANGE:
int iPage = TabCtrl_GetCurSel(hTab);
                    DisplayFileInEditControl(hEdit, iPage);
                    break;
            }
            break;
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void CreateTabControl(HWND hWndParent) {
    RECT rcClient;
    GetClientRect(hWndParent, &rcClient);

    hTab = CreateWindow(WC_TABCONTROL, NULL, WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE, 0, 0, rcClient.right, 30, hWndParent, (HMENU)ID_TABCONTROL, hInst, NULL);

    TCITEM tie;
    tie.mask = TCIF_TEXT;
    tie.pszText = "Tab 1";
    TabCtrl_InsertItem(hTab, 0, &tie);
    tie.pszText = "Tab 2";
    TabCtrl_InsertItem(hTab, 1, &tie);
    tie.pszText = "Tab 3";
    TabCtrl_InsertItem(hTab, 2, &tie);
    tie.pszText = "Tab 4";
    TabCtrl_InsertItem(hTab, 3, &tie);
    tie.pszText = "Tab 5";
    TabCtrl_InsertItem(hTab, 4, &tie);
}

void DisplayFileInEditControl(HWND hEdit, int fileIndex) {
    char *filenames[] = {"log1.txt", "log2.txt", "log3.txt", "log4.txt", "log5.txt"};
    char filePath[260];
    FILE *file;
    int length;
    char *data;

    sprintf(filePath, "PCL\\%s", filenames[fileIndex]);
    file = fopen(filePath, "rb");
    if (file != NULL) {
        fseek(file, 0, SEEK_END);
        length = ftell(file);
        fseek(file, 0, SEEK_SET);
        data = (char *)malloc(length + 1);
        fread(data, 1, length, file);
        data[length] = '\0';
        fclose(file);

        SetWindowText(hEdit, data);
        free(data);
    } else {
        MessageBox(NULL, "无法打开文件！", "错误", MB_ICONERROR);
    }
}
