#include "MediaInfoDLL.h"
#include <iostream>
#include <iomanip>
#define MediaInfoNameSpace MediaInfoDLL;
using namespace MediaInfoNameSpace;
using namespace std;

String intToWstring(int num) {
    string sTemp = to_string(num);
    String wTemp(sTemp.begin(), sTemp.end());
    return wTemp;
}

int main(int argc, char* argv[])
{
    MediaInfo MI;
    if (argc < 2) {
        wcout << L"You need to pass an argument" << endl;
        return 0;
    }
    string path = argv[1];
    String wPath(path.begin(), path.end());
    try {
        MI.Open(wPath);
        if (!MI.IsReady()) {
            MI.Close();
            throw(wPath);
        }
    }
    catch(String wPath) {
        wcout << L"Can't open file in path " << wPath << endl;
        return 0;
    }
    
    String To_Display = L"";

    To_Display += L"Duration\t";
    int duration = stoi(MI.Get(Stream_General, 0, __T("Duration"), Info_Text, Info_Name).c_str());
    duration /= 1000; // convert to seconds
    To_Display += intToWstring(duration);

    String audio = MI.Get(Stream_General, 0, __T("AudioCount"), Info_Text, Info_Name).c_str();
    To_Display += L"\nAudio\t\t";
    if (audio == L"") To_Display += L"None";
    else {
        To_Display += audio;
        for (int i = 0; i < stoi(audio); i++) {  // for each audio stream print metadata about it
            To_Display += L"\n  Audio #" + intToWstring(i + 1) + L"\n";
            To_Display += L"    Codec\t";
            To_Display += MI.Get(Stream_Audio, i, __T("Format"), Info_Text, Info_Name).c_str();
            To_Display += L"\n    Duration\t";
            int aDuration = stoi(MI.Get(Stream_Audio, i, __T("Duration"), Info_Text, Info_Name).c_str());
            aDuration /= 1000;
            To_Display += intToWstring(aDuration);
        }
    }
    To_Display += L"\n";

    String video = MI.Get(Stream_General, 0, __T("VideoCount"), Info_Text, Info_Name).c_str();
    To_Display += L"\nVideo\t\t";
    if (video == L"") To_Display += L"None";
    else {
        To_Display += video;
        for (int i = 0; i < stoi(video); i++) { // for each video stream print metadata about it
            To_Display += L"\n  Video #" + intToWstring(i + 1) + L"\n";
            To_Display += L"    Codec\t";
            String codec = MI.Get(Stream_Video, i, __T("InternetMediaType"), Info_Text, Info_Name).c_str();
            String delimiter = L"/";
            To_Display += codec.substr(codec.find(delimiter) + 1, codec.length() - 1);
            To_Display += L"\n    Width\t";
            To_Display += MI.Get(Stream_Video, i, __T("Width"), Info_Text, Info_Name).c_str();
            To_Display += L"\n    Height\t";
            To_Display += MI.Get(Stream_Video, i, __T("Height"), Info_Text, Info_Name).c_str();
            To_Display += L"\n    Duration\t";
            int vDuration = stoi(MI.Get(Stream_Video, i, __T("Duration"), Info_Text, Info_Name).c_str());
            vDuration /= 1000;
            To_Display += intToWstring(vDuration);
        }
    }
    To_Display += L"\n";

    MI.Close();

    wcout << To_Display << endl;

    return 0;
}