#pragma once
#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <string>

#define DIV 1048576
class PcInfoHelper
{
    typedef unsigned __int64 QWORD;
public:
	PcInfoHelper();
    
	~PcInfoHelper();
	static std::string getGPUVRAM()
	{
        // Define values & start process
        HKEY hkey;
        QWORD val = 0;
        DWORD dwSize = sizeof(val);
        std::string gpuGB;
        if (RegOpenKey(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e968-e325-11ce-bfc1-08002be10318}\\0000", &hkey) == ERROR_SUCCESS)
        {
            QWORD value = 0;
            DWORD dwType, dwSize = sizeof(value);

            if (RegQueryValueEx(hkey, _T("HardwareInformation.qwMemorySize"), NULL, &dwType, reinterpret_cast<LPBYTE>(&value), &dwSize) == ERROR_SUCCESS)
            {
                if (dwType == REG_QWORD || dwType == REG_BINARY)
                {
                    // to gigabytes
                    gpuGB = std::to_string(value / 1024 / 1024 / 1024);
                    return gpuGB;
                }
            }
            RegCloseKey(hkey);
        }
        return "";
        
	}// End 
    static std::string GetCpuInfo()
    {
        std::array<int, 4> integerBuffer = {};
        constexpr size_t sizeofIntegerBuffer = sizeof(int) * integerBuffer.size();
        std::array<char, 64> charBuffer = {};
        constexpr std::array<int, 3> functionIds = {
            0x8000'0002,
            0x8000'0003,
            0x8000'0004
        };
        std::string cpu;
        for (int id : functionIds)
        {
            __cpuid(integerBuffer.data(), id);
            std::memcpy(charBuffer.data(), integerBuffer.data(), sizeofIntegerBuffer);
            cpu += std::string(charBuffer.data());
        }
        return cpu;
    }
    
private:

};

PcInfoHelper::PcInfoHelper()
{
}

PcInfoHelper::~PcInfoHelper()
{
}