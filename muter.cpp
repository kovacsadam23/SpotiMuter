#include <iostream>
#include <stdlib.h>
#include <string>
#include <endpointvolume.h>
#include <mmdeviceapi.h>
#include <windows.h>
#include <tchar.h>

using namespace std;


string GetActiveWindowTitle()
{
    char res[2048];
    // HWND forehwnd = GetForegroundWindow();
    // HWND hwnd = GetNextWindow(forehwnd, GW_HWNDNEXT);
    HWND hwnd = FindWindowEx(0, 0, "Chrome_WidgetWin_0", 0);
    GetWindowText(hwnd, res, GetWindowTextLength(hwnd) + 1);

    if (res[0] == NULL)
    {
        return "Spotify not found";
    }
    return res;
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


void muteSystem()
{
    string prevWindow = "";
    string activeWindow = "";
    double currentVolume = 0.0;
    for (;;)
    {
        activeWindow = GetActiveWindowTitle();
        std::cout << activeWindow << '\n';

        if (activeWindow.compare("Advertisement") == 0)
        {
            manageVolume(0.0, true, false);
        }
        else
        {
            if (prevWindow.compare("Advertisement") == 0)
            {
                Sleep(300);
                manageVolume(currentVolume, true, false);
            }
            currentVolume = manageVolume(0.0, false, true);
        }

        prevWindow = activeWindow;
    }
}




int main()
{
    muteSystem();

    return 0;
}
