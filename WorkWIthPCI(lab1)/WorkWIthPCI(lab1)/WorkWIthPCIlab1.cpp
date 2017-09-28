#pragma comment (lib, "Setupapi.lib")
#include <stdio.h>
#include <windows.h>
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <iostream>
#include <conio.h>
#include <regex>
#include <fstream>
#include <map>
#include <iterator>
using namespace std;

const int BUFFER_SIZE = 350;

int main()
{
	HDEVINFO hDevInfo;								//Дескриптор нбора информации об устройстве

	//Получение дескриптора информации об устройсве
	hDevInfo = SetupDiGetClassDevs(
		NULL,										//Указатель на класс установки устройства
		REGSTR_KEY_PCIENUM,							//Счетчик PnP
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
	
		//Заполняет структуру devieInfoData
		DWORD buffersize = BUFFER_SIZE;

		deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);	//Установка размера самой структуры.
		char vendorBuf[BUFFER_SIZE];
		char deviceBuf[BUFFER_SIZE];

		for (int i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &deviceInfoData); i++) {
			memset(vendorBuf, 0, BUFFER_SIZE);
			memset(deviceBuf, 0, BUFFER_SIZE);
			DWORD DataT;
			SetupDiGetDeviceRegistryProperty(
				hDevInfo,									//Информация о наборе устройств
				&deviceInfoData,							//Информация о том, какое утройство из набора требуется
				SPDRP_DEVICEDESC,							//Требуемое свойство
				&DataT,
				(PBYTE)deviceBuf,							//Буфер
				BUFFER_SIZE,								//Действительный размер буфера
				&buffersize);								//Требуемый размер буфера
			cout << "Devicd ID: ";
			for (int i = 0; i < BUFFER_SIZE; i++) {
				if (deviceBuf[i])cout << deviceBuf[i];
			}
			cout << endl;
			//Аналогичный вывод vendorID
			SetupDiGetDeviceRegistryProperty(
				hDevInfo,
				&deviceInfoData,
				SPDRP_MFG,
				&DataT,
				(PBYTE)vendorBuf,
				BUFFER_SIZE,
				&buffersize);
			cout << "	Vendor ID: ";
			for (int i = 0; i < BUFFER_SIZE; i++) {
				if (vendorBuf[i])cout << vendorBuf[i];
			}
			cout << endl;
			cout << endl;
		}
		//Test
	system("pause");
    return 0;
}