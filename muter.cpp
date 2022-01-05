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


void changeVolume(double nVolume, bool bScalar)
{
    HRESULT hr=NULL;
    bool decibels = false;
    bool scalar = false;
    double newVolume=nVolume;

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
}


void muteSystem()
{
    for (;;)
    {
        string activeWindow = GetActiveWindowTitle();
        std::cout << activeWindow << '\n';

        string prevWindow = activeWindow;
        if (activeWindow.compare("Advertisement") == 0)
        {
            changeVolume(0.0, true);
        }
        else
        {
            if (prevWindow.compare("Advertisement") == 0)
            {
                Sleep(200);
            }
            changeVolume(0.2, true);
        }
    }
}




int main()
{
    muteSystem();

    return 0;
}
