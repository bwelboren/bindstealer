#include <Windows.h>
#include <iostream>
#include <string>
#include <tlhelp32.h>

using namespace std;

void toClipboard(const std::string& s) {
	OpenClipboard(0);
	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size());
	if (!hg) {
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(hg), s.c_str(), s.size());
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
}

int main() {

	HWND sof2_window = FindWindow(NULL, "SoF2 MP Test");
	DWORD process_id = 0; //placeholder
	GetWindowThreadProcessId(sof2_window, &process_id); //give value to process_id
	HANDLE sof2_process = OpenProcess(PROCESS_ALL_ACCESS, true, process_id);

	DWORD console = 0xB434DC; // EAX value
	char consoleMsg[124];
	char search[] = "chat";
	char test[] = "a";

	while (true) {
		if (GetAsyncKeyState(VK_F1) & 0x0001) {
			ReadProcessMemory(sof2_process, (void*)console, &consoleMsg, sizeof(consoleMsg), NULL); // Reading console
			string copiedBind = "";
			char *ptr = strstr(consoleMsg, search);

			if (ptr != NULL) {
				char startOfMsg[] = ":";
				char* position_ptr = strchr(consoleMsg, startOfMsg[0]);
				int position = (position_ptr == NULL ? -1 : position_ptr - consoleMsg); // start of chat message
				for (int i = position + 1; i < strlen(consoleMsg); i++) {
					copiedBind += consoleMsg[i];
				}

				cout << copiedBind << endl;

				toClipboard(copiedBind);

			} 
			else cout << "No chat found" << endl; 

		}


	}

	return 0;
}
