#pragma comment (lib, "Setupapi.lib")
#include <stdio.h>
#include <windows.h>
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <iostream>
#include <conio.h>
using namespace std;

int main()
{
	HDEVINFO hDevInfo;								//Дескриптор нбора информации об устройстве

	//Получение дескриптора информации об устройсве
	hDevInfo = SetupDiGetClassDevs(
		NULL,										//Указатель на класс установки устройства
		0,											//Счетчик PnP
		0,											//Дескриптор интерфейса
		DIGCF_PRESENT |								//Только устройства, присутсвующие в системе
		DIGCF_ALLCLASSES);							//Возвратить все устройства

	if (hDevInfo == INVALID_HANDLE_VALUE)
	{
		cout << "Error with getting info about devices." << endl;
		_getch();
		return 1;
	}

	SP_DEVINFO_DATA deviceInfoData;					//Структура с информацией об устройстве
	
	deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);	//Установка размера самой структуры.

	SetupDiEnumDeviceInfo(hDevInfo, 0, &deviceInfoData);

		LPTSTR buffer = (LPTSTR)LocalAlloc(LPTR, 300);
		DWORD buffersize = 300;

		SetupDiGetDeviceRegistryProperty(
			hDevInfo,
			&deviceInfoData,
			SPDRP_HARDWAREID,
			NULL,
			(PBYTE)buffer,
			buffersize,
			&buffersize);

		cout << "Size of info " << buffersize << endl;
		cout << buffer << endl;
		_getch();
	

    return 0;
}

