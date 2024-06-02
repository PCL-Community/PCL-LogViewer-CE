#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ID_TABCONTROL 1
#define ID_TEXTBOX 2
#define MAX_LOADSTRING 100
#define MAX_FILES 5

HINSTANCE hInst;
TCHAR szTitle[MAX_LOADSTRING] = TEXT("PCL2 日志查看器");
TCHAR szWindowClass[MAX_LOADSTRING] = TEXT("PCL2 日志查看器");
HWND hTab;
HWND hEdit;
OPENFILENAMEW ofn;
WCHAR szFolder[MAX_PATH];

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void CreateTabControl(HWND);
void DisplayFileInEditControl(HWND, int);
LRESULT CALLBACK WndProcTabSubclass(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK TabSubclassProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    MSG msg;
    HACCEL hAccelTable;

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MYAPP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }

    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYAPP));

    while (GetMessage(&msg, NULL, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEXW);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYAPP));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MYAPP);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
    hInst = hInstance;

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

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
    case WM_CREATE:
        CreateTabControl(hWnd);
        break;
    case WM_COMMAND:
        wmId = LOWORD(wParam);
        wmEvent = HIWORD(wParam);
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

void CreateTabControl(HWND hWnd) {
    RECT rcClient;

    // Get the dimensions of the client area.
    GetClientRect(hWnd, &rcClient);

    // Create the tab control.
    hTab = CreateWindowW(WC_TABCONTROL, TEXT("选项卡"), WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE, 
                        0, 0, rcClient.right, rcClient.bottom, hWnd, (HMENU)ID_TABCONTROL, hInst, NULL);

    // Add tabs to the tab control.
    TCITEMW tie;
    tie.mask = TCIF_TEXT;

    // Select a folder
    BROWSEINFOW bi = { 0 };
    bi.lpszTitle = L"选择一个文件夹";
    LPITEMIDLIST pidl = SHBrowseForFolderW(&bi);
    if (pidl != NULL) {
        SHGetPathFromIDListW(pidl, szFolder);
        // Release the PIDL
        CoTaskMemFree(pidl);
    } else {
        MessageBoxW(hWnd, L"请选择一个文件夹！", L"提示", MB_ICONINFORMATION);
        return;
    }

    // List files in the folder
    WIN32_FIND_DATAW findFileData;
    HANDLE hFind;
    WCHAR szSearch[MAX_PATH];
    WCHAR szFilePath[MAX_PATH];
    wcscpy_s(szSearch, MAX_PATH, szFolder);
    wcscat_s(szSearch, MAX_PATH, L"\\*.txt");

    // Initialize tab index
    int tabIndex = 0;

    hFind = FindFirstFileW(szSearch, &findFileData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                continue; // Skip directories
            }
            if (tabIndex >= MAX_FILES) {
                break; // Stop if reached maximum number of files
            }

            // Get full file path
            wcscpy_s(szFilePath, MAX_PATH, szFolder);
            wcscat_s(szFilePath, MAX_PATH, L"\\");
            wcscat_s(szFilePath, MAX_PATH, findFileData.cFileName);

            // Add tab
            tie.pszText = findFileData.cFileName;
            TabCtrl_InsertItem(hTab, tabIndex, &tie);

            // Increment tab index
            tabIndex++;

        } while (FindNextFileW(hFind, &findFileData) != 0);
        FindClose(hFind);
    }

    // Subclass the tab control to handle tab selection change
    SetWindowLongPtrW(hTab, GWLP_USERDATA, (LONG_PTR)WndProcTabSubclass);
    SetWindowLongPtrW(hTab, GWLP_WNDPROC, (LONG_PTR)TabSubclassProc);
}

LRESULT CALLBACK WndProcTabSubclass(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_NOTIFY) {
        LPNMHDR nmhdr = (LPNMHDR)lParam;
        if (nmhdr->code == TCN_SELCHANGE) {
            int tabIndex = TabCtrl_GetCurSel(hTab);
            DisplayFileInEditControl(hWnd, tabIndex);
        }
    }
    return CallWindowProcW((WNDPROC)GetWindowLongPtrW(hTab, GWLP_USERDATA), hWnd, message, wParam,

 lParam);
}

LRESULT CALLBACK TabSubclassProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    return CallWindowProcW((WNDPROC)GetWindowLongPtrW(hTab, GWLP_USERDATA), hWnd, message, wParam, lParam);
}

void DisplayFileInEditControl(HWND hWnd, int tabIndex) {
    WCHAR szFilePath[MAX_PATH];
    SendMessageW(hTab, TCM_GETITEMTEXTW, tabIndex, (LPARAM)szFilePath);

    // Get full file path
    wcscpy_s(szFilePath, MAX_PATH, szFolder);
    wcscat_s(szFilePath, MAX_PATH, L"\\");
    wcscat_s(szFilePath, MAX_PATH, szFile);

    FILE *file;
    long fileSize;
    char *buffer;

    // Open the file
    file = _wfopen(szFilePath, L"rb");
    if (file == NULL) {
        MessageBoxW(hWnd, L"打开文件失败！", L"错误", MB_ICONERROR);
        return;
    }

    // Determine file size
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    rewind(file);

    // Allocate memory for file content
    buffer = (char *)malloc(fileSize + 1);
    if (buffer == NULL) {
        fclose(file);
        MessageBoxW(hWnd, L"内存分配失败！", L"错误", MB_ICONERROR);
        return;
    }

    // Read file content into buffer
    if (fread(buffer, 1, fileSize, file) != fileSize) {
        fclose(file);
        free(buffer);
        MessageBoxW(hWnd, L"读取文件失败！", L"错误", MB_ICONERROR);
        return;
    }

    // Close the file
    fclose(file);

    // Null-terminate the buffer
    buffer[fileSize] = '\0';

    // Set the text of the edit control
    SetWindowTextA(hEdit, buffer);

    // Free allocated memory
    free(buffer);
}
