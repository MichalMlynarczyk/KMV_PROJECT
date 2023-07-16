#include <iostream>
#include <windows.h>

class SCROLL_SENDER {
    public: 
        SCROLL_SENDER() {
        previousScrollValue_ = 0;
        InitializeCriticalSection(&cs_);
    }

    ~SCROLL_SENDER() {
        DeleteCriticalSection(&cs_);
    }

    void StartListening() {
        RAWINPUTDEVICE rid;
        rid.usUsagePage = 0x01;  // Generowanie danych dotyczących myszy
        rid.usUsage = 0x02;      // Generowanie danych dotyczących ruchu myszy
        rid.dwFlags = RIDEV_INPUTSINK;
        rid.hwndTarget = NULL;

        if (!RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE))) {
            std::cerr << "Failed to register raw input devices." << std::endl;
            return;
        }

        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

private:
    CRITICAL_SECTION cs_;
    int previousScrollValue_;

    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
        if (message == WM_INPUT) {
            UINT bufferSize;
            GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &bufferSize, sizeof(RAWINPUTHEADER));

            if (bufferSize > 0) {
                BYTE* buffer = new BYTE[bufferSize];
                if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, buffer, &bufferSize, sizeof(RAWINPUTHEADER)) == bufferSize) {
                    RAWINPUT* raw = (RAWINPUT*)buffer;
                    if (raw->header.dwType == RIM_TYPEMOUSE) {
                        int scrollValue = raw->data.mouse.usButtonData;
                        EnterCriticalSection(&SCROLL_SENDER::GetInstance().cs_);
                        int scrollDelta = scrollValue - SCROLL_SENDER::GetInstance().previousScrollValue_;
                        std::cout << "Scroll Delta: " << scrollDelta << std::endl;
                        SCROLL_SENDER::GetInstance().previousScrollValue_ = scrollValue;
                        LeaveCriticalSection(&SCROLL_SENDER::GetInstance().cs_);
                    }
                }
                delete[] buffer;
            }
        }

        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    static SCROLL_SENDER& GetInstance() {
        static SCROLL_SENDER instance;
        return instance;
    }
};

int main() {
    SCROLL_SENDER listener;
    listener.StartListening();

    return 0;
}