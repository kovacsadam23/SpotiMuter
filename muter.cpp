#include <iostream>
#include <stdlib.h>
#include <string>
#include <endpointvolume.h>
#include <mmdeviceapi.h>
#include <windows.h>

using namespace std;


string GetActiveWindowTitle()
{
    char res[2048];
    HWND hwnd = GetForegroundWindow();
    GetWindowText(hwnd, res, GetWindowTextLength(hwnd) + 1);
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


void muteSystem(double volume)
{
    string prevWindow = "";
    string activeWindow = "";
    for (;;)
    {
        activeWindow = GetActiveWindowTitle();
        std::cout << activeWindow << '\n';

        if (activeWindow.compare("Advertisement") == 0)
        {
            manageVolume(0.0, true, false);
        }
        else if (prevWindow.compare("Advertisement") == 0)
        {
            Sleep(200);
            manageVolume(volume, true, false);
        }

        prevWindow = activeWindow;
    }
}




int main()
{
    double initVolume = manageVolume(0.0, false, true);
    muteSystem(initVolume);

    return 0;
}
