#pragma comment (lib, "Setupapi.lib")
#include <stdio.h>
#include <windows.h>
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <iostream>
#include <conio.h>
using namespace std;

const int BUFFER_SIZE = 300;

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
		//DWORD DataT;
		LPTSTR buffer = (LPTSTR)LocalAlloc(LPTR, BUFFER_SIZE);	//Выделение памяти под буфер
		DWORD buffersize = BUFFER_SIZE;

		deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);	//Установка размера самой структуры.

		for (int i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &deviceInfoData); i++) {
			DWORD DataT;
			SetupDiGetDeviceRegistryProperty(
				hDevInfo,									//Информация о наборе устройств
				&deviceInfoData,							//Информация о том, какое утройство из набора требуется
				SPDRP_DEVICEDESC,							//Требуемое свойство
				&DataT,
				(PBYTE)buffer,								//Буфер
				buffersize,									//Действительный размер буфера
				&buffersize);								//Требуемый размер буфера
			//cout << "Size of info " << buffersize << endl;
			cout << buffer << endl;
			SetupDiGetDeviceRegistryProperty(
				hDevInfo,
				&deviceInfoData,
				SPDRP_MFG,
				&DataT,
				(PBYTE)buffer,
				buffersize,
				&buffersize);
			cout << "Vendor ID" << buffer << endl;
		}
		_getch();
	

    return 0;
}

