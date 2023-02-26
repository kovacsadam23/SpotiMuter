#include <iostream>
#include <endpointvolume.h>
#include <mmdeviceapi.h>
#include <windows.h>
#include <tchar.h>
#include <unistd.h>

using namespace std;

struct ProgramData {
    char res[1024];
} ProgramData;


void GetActiveWindowTitle()
{
    // HWND forehwnd = GetForegroundWindow();
    // HWND hwnd = GetNextWindow(forehwnd, GW_HWNDNEXT);
    HWND hwnd = FindWindowEx(0, 0, "Chrome_WidgetWin_0", 0);
    GetWindowText(hwnd, ProgramData.res, GetWindowTextLength(hwnd) + 1);

    if (ProgramData.res[0] == NULL)
    {
        strcpy(ProgramData.res, "Spotify not found");
    }
}


double manageVolume(double nVolume, bool bScalar, bool getVolume)
{
    HRESULT hr = NULL;
    bool decibels = false;
    bool scalar = false;
    double newVolume = nVolume;

    CoInitialize(NULL);
    IMMDeviceEnumerator *deviceEnumerator = NULL;
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER,
                          __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
    IMMDevice *defaultDevice = NULL;

    hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
    deviceEnumerator->Release();
    deviceEnumerator = NULL;

    IAudioEndpointVolume *endpointVolume = NULL;
    hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume),
         CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
    defaultDevice->Release();
    defaultDevice = NULL;


    float currentVolume = 0;
    endpointVolume->GetMasterVolumeLevel(&currentVolume);


    hr = endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);

    if (getVolume) {
        return (double)currentVolume;
    }

    else
    {
        if (bScalar == false)
        {
            hr = endpointVolume->SetMasterVolumeLevel((float)newVolume, NULL);
        }
        else if (bScalar == true)
        {
            hr = endpointVolume->SetMasterVolumeLevelScalar((float)newVolume, NULL);
        }
        endpointVolume->Release();

        CoUninitialize();

        return 0.0;
    }
}


[[noreturn]] void muteSystem()
{
    char prevWindow[1024] = "";
    char activeWindow[1024] = "";
    double currentVolume = 0.0;

    for (;;)
    {
        GetActiveWindowTitle();

        strncpy(activeWindow, ProgramData.res, sizeof activeWindow);

        if (strcmp(activeWindow, prevWindow) != 0) {
            system("cls");
            std::cout << activeWindow << std::endl;
        }


        if (strcmp(activeWindow,"Advertisement") == 0)
        {
            manageVolume(0.0, true, false);
        }
        else
        {
            if (strcmp(prevWindow, "Advertisement") == 0)
            {
                Sleep(300);
                manageVolume(currentVolume, true, false);
            }
            currentVolume = manageVolume(0.0, false, true);
        }

        memset(prevWindow, 0, sizeof prevWindow);
        strncpy(prevWindow, activeWindow, sizeof activeWindow);
        memset(activeWindow, 0, sizeof activeWindow);

        Sleep(150);
    }
}


int main()
{
    muteSystem();

    return 0;
}
