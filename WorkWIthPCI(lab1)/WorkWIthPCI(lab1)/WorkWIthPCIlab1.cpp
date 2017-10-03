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

multimap<string, string> getDeviceList() {
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
		return multimap<string, string>();
	}

	SP_DEVINFO_DATA deviceInfoData;					//Структура с информацией об устройстве

													//Заполняет структуру devieInfoData
	DWORD buffersize = BUFFER_SIZE;

	deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);	//Установка размера самой структуры.
	char buffer[BUFFER_SIZE];
	regex code("(VEN_)([0-9A-Fa-f]{4})(&DEV_)([0-9A-Fa-f]{4})");
	multimap<string, string> values;
	for (int i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &deviceInfoData); i++) {
		memset(buffer, 0, BUFFER_SIZE);

		SetupDiGetDeviceRegistryProperty(
			hDevInfo,									//Информация о наборе устройств
			&deviceInfoData,							//Информация о том, какое утройство из набора требуется
			SPDRP_HARDWAREID,							//Требуемое свойство
			NULL,
			(PBYTE)buffer,								//Буфер
			BUFFER_SIZE,								//Действительный размер буфера
			&buffersize);								//Требуемый размер буфера
		char info[BUFFER_SIZE];
		for (int i = 0, j = 0; i < BUFFER_SIZE; i++) {
			if (buffer[i]) {
				info[j] = buffer[i];
				j++;
			}
		}
		
		cmatch result;
		if (regex_search(info, result, code)) {
			values.insert(pair<string, string>(string(result[2]), string(result[4])));
		}
		cout << endl;
	}
	return values;
}

int main()
{
	regex regVendorID("([0-9a-f]{4})( {2})(.*)");
	regex regDeviceID("(\t)([0-9a-f]{4})( {2})(.*)");
	multimap<string, string> values = getDeviceList();
	auto it = values.begin();
	while (it != values.end()) {
		cout << it->first << " " << it->second << endl;
		it++;
	}
	system("pause");
    return 0;
}