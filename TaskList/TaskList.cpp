#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN  
#define WM_USER_UPDATE_NOTE (WM_USER + 1) // Определение пользовательского сообщения для обновления заметки
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib,"Comctl32.lib")

#include "resource.h"
#include <windowsx.h>
#include <windows.h>
#include <commctrl.h>
#include <wchar.h>
#include <iostream>
#include <SDKDDKVer.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime> // Для работы с датой и временем



HINSTANCE hInst;
HBRUSH hBrushEditTopic;
HWND hClild;
HWND hUpd;
HWND hParent;
HWND hEdit;
HWND hEditTopic;
HWND hEditUpd;
HWND hEditTopicUpd;
HWND hNoteText;


struct Note {
	std::wstring time;
	std::wstring topic;
	std::wstring text;
};
std::vector<Note> notes;
std::vector<Note> noteslist;
int verticalPosition = 10; // Начальная вертикальная позиция для первой заметки
int editNoteIndex = -1; // Индекс редактируемой заметки (-1, если редактируется новая заметка)
int id_upd;
int nowlist = 1;



void AddNoteToList(const Note& newNote, int number, int id_menu)
{
	std::wstring wTimeString = L"Дата и время создания заметки: ";
	wTimeString += newNote.time;

	verticalPosition = 10 + number * 110;

	HWND hNoteTime = CreateWindowEx(0, L"STATIC", wTimeString.c_str(),
		WS_CHILD | WS_VISIBLE | SS_LEFT, 10, verticalPosition, 450, 20, hParent, NULL, hInst, NULL);

	std::wstring noteTitle(L"Тема: ");
	noteTitle += newNote.topic;

	HWND hNoteTopic = CreateWindowEx(0, L"BUTTON", noteTitle.c_str(),
		WS_CHILD | WS_VISIBLE | SS_LEFT, 10, verticalPosition + 20, 450, 20, hParent, (HMENU)(1000 + id_menu), hInst, NULL);
	HWND hNoteText = CreateWindowEx(0, L"STATIC", newNote.text.c_str(),
		WS_CHILD | WS_VISIBLE | SS_LEFT | WS_BORDER, 10, verticalPosition + 40, 450, 60, hParent, (HMENU)(2000 + id_menu), hInst, NULL);
}
void ClearList()
{
	HWND hwndChild = GetWindow(hParent, GW_CHILD);
	while (hwndChild)
	{
		HWND hwndNext = GetWindow(hwndChild, GW_HWNDNEXT);
		DestroyWindow(hwndChild);
		hwndChild = hwndNext;
	}

	// Затем отправляем сообщение о том, что окно нужно перерисовать
	RedrawWindow(hParent, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);
}

void UpdateList()
{
	ClearList();
	int number = 0;
	for (int i = nowlist * 4 - 4; i < notes.size() && i < nowlist * 4; i++) {
		AddNoteToList(notes[i], number, i);
		number++;
	}
	//for (int i = 0; i < notes.size(); i++) {
	//	
	//}
}
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProcD(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProcUpd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);




int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR szCmdLine, int nCmdShow)
{
	MSG msg{};
	HWND hwnd{};
	WNDCLASSEX wc{};
	wc.hInstance = hInstance;
	wc.lpszClassName = L"Родитель";
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(226, 198, 242));
	wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TASKLIST));
	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));
	wc.lpszMenuName = MAKEINTRESOURCE(IDC_TASKLIST);
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.cbSize = sizeof(WNDCLASSEX);
	hInst = hInstance;
	if (!RegisterClassEx(&wc)) return EXIT_FAILURE;
	hwnd = CreateWindow(wc.lpszClassName, L"TaskList", WS_OVERLAPPEDWINDOW | WS_SYSMENU , 200, 210, 800, 500, nullptr, nullptr, wc.hInstance, nullptr);

	hParent = hwnd;
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	

	return (int)(msg.wParam);
}
INT CALLBACK CreateWindowChild(HWND hwnd) {
	WNDCLASSEX wcChild{};
	wcChild.cbSize = sizeof(WNDCLASSEX);
	wcChild.style = CS_HREDRAW | CS_VREDRAW;
	wcChild.lpfnWndProc = WndProcD;
	wcChild.cbClsExtra = 0;
	wcChild.cbWndExtra = 0;
	wcChild.hInstance = hInst;
	wcChild.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcChild.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(230, 230, 250));
	wcChild.lpszClassName = L"NoteChildClass";
	if (!RegisterClassEx(&wcChild)) 
		return EXIT_FAILURE;
	HWND hChildWnd = CreateWindow(wcChild.lpszClassName, L"Создать заметку", WS_OVERLAPPEDWINDOW | WS_SYSMENU, 100, 100, 500, 400, hwnd, nullptr, nullptr, nullptr);

	hClild = hChildWnd;
	UpdateWindow(hChildWnd);
}
INT CALLBACK CreateWindowUpd(HWND hwnd) {
	WNDCLASSEX wcChild{};
	wcChild.cbSize = sizeof(WNDCLASSEX);
	wcChild.style = CS_HREDRAW | CS_VREDRAW;
	wcChild.lpfnWndProc = WndProcUpd;
	wcChild.cbClsExtra = 0;
	wcChild.cbWndExtra = 0;
	wcChild.hInstance = hInst;
	wcChild.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcChild.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(230, 230, 250));
	wcChild.lpszClassName = L"NoteUpdateClass";
	if (!RegisterClassEx(&wcChild))
		return EXIT_FAILURE;
	HWND hChildWnd = CreateWindow(wcChild.lpszClassName, L"Редактировать заметку", WS_OVERLAPPEDWINDOW | WS_SYSMENU, 100, 100, 500, 400, hwnd, nullptr, nullptr, nullptr);

	hUpd = hChildWnd;
	UpdateWindow(hChildWnd);
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)	//обработка сообщений главного окна
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		CreateWindowChild(hwnd);
		CreateWindowUpd(hwnd);
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;
	}
	return 0;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDM_EXIT:
		{
			DestroyWindow(hwnd);
		}
		break;

		case IDM_ABOUT:
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
		}
		break;
		case IDM_CREATE:
		{
			
			ShowWindow(hClild, SW_RESTORE);
		}
		break;
		case IDM_NEXT:
		{
			if (notes.size() > nowlist * 4) { 
				nowlist++; 
				UpdateList();
			}
			else MessageBox(hwnd, L"Страница не существует!", L"Предупреждение", MB_ICONWARNING | MB_OK);
		}
		break;
		case IDM_LAST:
		{
			if (nowlist == 1) MessageBox(hwnd, L"Страница не существует!", L"Предупреждение", MB_ICONWARNING | MB_OK);
			else {
				nowlist--;
				UpdateList();
			}
		}
		break;
		default:
			// Обработка выбора заметки для редактирования
			if (wmId >= 1000 && wmId < 2000)
			{
				editNoteIndex = wmId - 1000;
				id_upd = editNoteIndex;
				const Note& noteToEdit = notes[editNoteIndex];
				SetWindowText(hEditTopicUpd, noteToEdit.topic.c_str());
				SetWindowText(hEditUpd, noteToEdit.text.c_str());
				ShowWindow(hUpd, SW_RESTORE);
				std::cout << "123";
			}
			break;
		
		}
	}
	return 0;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
	}
	return 0;
	case WM_DESTROY:
	{
		PostQuitMessage(EXIT_SUCCESS);
	}
	return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
LRESULT CALLBACK WndProcD(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)	//обработка сообщений главного окна
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		// Добавляем текст "Введите тему:"
		CreateWindowEx(0, L"STATIC", L"Введите тему:",
			WS_CHILD | WS_VISIBLE | SS_LEFT, 10, 10, 100, 20, hwnd, NULL, hInst, NULL);

		// Добавляем текстовое поле для ввода темы
		hEditTopic = CreateWindowEx(0, L"EDIT", L"",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, 120, 10, 340, 20, hwnd, (HMENU)IDC_EDIT_TOPIC, hInst, NULL);
		// Устанавливаем обработчик сообщения WM_CTLCOLOREDIT для изменения цвета фона текстового поля


		// Добавляем текстовое поле для ввода заметки
		hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL,
			10, 50, 450, 250, hwnd, (HMENU)IDC_EDIT_CHILD, hInst, NULL);
		// Устанавливаем обработчик сообщения WM_CTLCOLOREDIT для изменения цвета фона текстового поля


		// Добавляем кнопку "Сохранить"
		HWND hButton = CreateWindowEx(0, L"BUTTON", L"Сохранить",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 380, 325, 100, 30, hwnd, (HMENU)IDM_SAVE, hInst, NULL);
		ShowWindow(hwnd, SW_HIDE);
	}
	return 0;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDM_EXIT:
		{
			ShowWindow(hwnd, SW_HIDE);
			ShowWindow(hParent, SW_RESTORE);
		}
		break;
		case IDM_ABOUT:
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
		}
		break;
		
		case IDM_SAVE:
		{
				// Добавляем сохраненную заметку в вектор
				std::wstring titleText, noteText;
				int lenTopic = GetWindowTextLength(hEditTopic);
				int lenText = GetWindowTextLength(hEdit);
				int len = GetWindowTextLength(hEdit);
				if (len > 0) {
					wchar_t* bufferTopic = new wchar_t[lenTopic + 1];
					wchar_t* bufferText = new wchar_t[lenText + 1];
					GetWindowText(hEditTopic, bufferTopic, lenTopic + 1);
					GetWindowText(hEdit, bufferText, lenText + 1);
					wchar_t* buffer = new wchar_t[len + 1];
					GetWindowText(hEdit, buffer, len + 1);
					int result = MessageBox(hwnd, L"Вы действительно хотите сохранить заметку?", L"Подтверждение", MB_YESNO | MB_ICONQUESTION);
					if (result == IDYES) {
						Note newNote;
						time_t currentTime = time(nullptr);
						tm* localTime = localtime(&currentTime);
						char timeStr[64];
						strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localTime);
						std::string timeString(timeStr);
						newNote.time = std::wstring(timeString.begin(), timeString.end());
						newNote.topic = bufferTopic;
						newNote.text = bufferText;
						notes.push_back(newNote);
						UpdateList();
						ShowWindow(hClild, SW_HIDE);  // Закрываем дочернее окно после сохранения
						SetWindowPos(hParent, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
						SetWindowText(hEdit, L"");
						SetWindowText(hEditTopic, L"");
					}
					else {
						delete[] buffer;
					}
				}
				else {
					MessageBox(hwnd, L"Введите текст заметки", L"Ошибка", MB_OK | MB_ICONERROR);
				}
			
		}
		
		break;
		}
	}
	return 0;
	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		SetBkColor(hdcStatic, RGB(230, 230, 250));
		return (LRESULT)GetStockObject(NULL_BRUSH);
	}
	case WM_CTLCOLOREDIT:
	{
		HDC hdcEdit = (HDC)wParam;
		SetBkColor(hdcEdit, RGB(255, 255, 255));
		SetTextColor(hdcEdit, RGB(0, 0, 0));
		return (INT_PTR)hBrushEditTopic;
	}
	case WM_CLOSE:
	{
		ShowWindow(hwnd, SW_HIDE);
		SetWindowPos(hParent, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
	return 0;
	case WM_DESTROY:
		DeleteObject(hBrushEditTopic);
		return 0;
	
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


LRESULT CALLBACK WndProcUpd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)	//обработка сообщений окна delete
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		// Добавляем текст "Введите тему:"
		CreateWindowEx(0, L"STATIC", L"Введите тему:",
			WS_CHILD | WS_VISIBLE | SS_LEFT, 10, 10, 100, 20, hwnd, NULL, hInst, NULL);

		// Добавляем текстовое поле для ввода темы
		hEditTopicUpd = CreateWindowEx(0, L"EDIT", L"",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, 120, 10, 340, 20, hwnd, (HMENU)IDC_EDIT_TOPIC_UPD, hInst, NULL);
		// Устанавливаем обработчик сообщения WM_CTLCOLOREDIT для изменения цвета фона текстового поля


		// Добавляем текстовое поле для ввода заметки
		hEditUpd = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"",
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL,
			10, 50, 450, 250, hwnd, (HMENU)IDC_EDIT_UPD, hInst, NULL);
		// Устанавливаем обработчик сообщения WM_CTLCOLOREDIT для изменения цвета фона текстового поля


		// Добавляем кнопку "Сохранить"
		HWND hButtonUpdate = CreateWindowEx(0, L"BUTTON", L"Сохранить",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 380, 325, 100, 30, hwnd, (HMENU)IDM_UPDATE, hInst, NULL);
		HWND hButtonDelete = CreateWindowEx(0, L"BUTTON", L"Удалить",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 270, 325, 100, 30, hwnd, (HMENU)IDM_DELETE, hInst, NULL);
		ShowWindow(hwnd, SW_HIDE);
	}
	return 0;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDM_EXIT:
		{
			ShowWindow(hwnd, SW_HIDE);
			ShowWindow(hParent, SW_RESTORE);
		}
		break;
		case IDM_ABOUT:
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
		}
		break;
		case IDM_UPDATE:
		{
			std::wstring titleText, noteText;
			int lenTopic = GetWindowTextLength(hEditTopicUpd);
			int lenText = GetWindowTextLength(hEditUpd);
			int len = GetWindowTextLength(hEditUpd);
			if (len > 0) {
				wchar_t* bufferTopic = new wchar_t[lenTopic + 1];
				wchar_t* bufferText = new wchar_t[lenText + 1];
				GetWindowText(hEditTopicUpd, bufferTopic, lenTopic + 1);
				GetWindowText(hEditUpd, bufferText, lenText + 1);
				wchar_t* buffer = new wchar_t[len + 1];
				GetWindowText(hEditUpd, buffer, len + 1);
				int result = MessageBox(hwnd, L"Вы действительно хотите изменить заметку?", L"Подтверждение", MB_YESNO | MB_ICONQUESTION);
				if (result == IDYES) {
					Note newNote;
					time_t currentTime = time(nullptr);
					tm* localTime = localtime(&currentTime);
					char timeStr[64];
					strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localTime);
					std::string timeString(timeStr);
					newNote.time = std::wstring(timeString.begin(), timeString.end());
					newNote.topic = bufferTopic;
					newNote.text = bufferText;
					notes[id_upd] = newNote;
					UpdateList();
					ShowWindow(hUpd, SW_HIDE);  // Закрываем дочернее окно после сохранения
					SetWindowPos(hParent, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
					SetWindowText(hEditUpd, L"");
					SetWindowText(hEditTopicUpd, L"");
				}
				else {
					delete[] buffer;
				}
			}
			else {
				MessageBox(hwnd, L"Введите текст заметки", L"Ошибка", MB_OK | MB_ICONERROR);
			}
		}
		break;
		case IDM_DELETE:
		{
			int result = MessageBox(hwnd, L"Вы действительно хотите удалить заметку?", L"Подтверждение", MB_YESNO | MB_ICONQUESTION);
			if (result == IDYES) {
				notes.erase(notes.begin() + id_upd);
				if (notes.size() < nowlist * 4 - 3) nowlist--;
				UpdateList();
				ShowWindow(hUpd, SW_HIDE);  // Закрываем дочернее окно
				SetWindowPos(hParent, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
				SetWindowText(hEditUpd, L"");
				SetWindowText(hEditTopicUpd, L"");
			}
		}
		}
		break;
	}
	return 0;
	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		SetBkColor(hdcStatic, RGB(230, 230, 250));
		return (LRESULT)GetStockObject(NULL_BRUSH);
	}
	case WM_CTLCOLOREDIT:
	{
		HDC hdcEdit = (HDC)wParam;
		SetBkColor(hdcEdit, RGB(255, 255, 255));
		SetTextColor(hdcEdit, RGB(0, 0, 0));
		return (INT_PTR)hBrushEditTopic;
	}
	case WM_CLOSE:
	{
		ShowWindow(hwnd, SW_HIDE);
		SetWindowPos(hParent, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
	return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
