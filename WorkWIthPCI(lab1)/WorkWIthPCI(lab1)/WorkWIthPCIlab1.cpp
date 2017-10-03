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
#include <algorithm>
#include <string> 
using namespace std;

const int BUFFER_SIZE = 350;

string toLowercase(string str) {
	std::transform(str.begin(), str.end(), str.begin(), tolower);
	return str;
}

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
			values.insert(pair<string, string>(toLowercase(string(result[2])), toLowercase(string(result[4]))));
		}
	}
	return values;
}

multimap<string, string> getDescription(multimap<string, string> values) {
	multimap<string, string> description;
	regex regVendorID("([0-9a-f]{4})( {2})(.*)");
	regex regDeviceID("(\t)([0-9a-f]{4})( {2})(.*)");
	cmatch result;
	ifstream file("pci.ids.txt");
	if (!file.is_open()) {
		cout << "Error with opening file." << endl;
	}
	char str[BUFFER_SIZE];
	auto it = values.begin();
	bool isFound;
	for (int i =0; it != values.end(); it++) {
		isFound = false;
		for (int i = 0; !file.eof(); i++) {
			file.getline(str, BUFFER_SIZE);
			if (str[0] == '#' || strlen(str) == 0) continue;
			if (regex_match(str, result, regVendorID)) {
				if (string(result[1]) == it->first) {
					string vendor = result[3];
					file.getline(str, BUFFER_SIZE);
					while (!regex_match(str, result, regVendorID)) {
						if (regex_match(str, result, regDeviceID)) {
							if (string(result[2]) == it->second) {
								string device = result[4];
								cout << "Vendor: " << vendor << endl;
								cout << "	Device: " << device << endl;
								description.insert(pair<string, string>(vendor, device));
								isFound = true;
								break;
							}
						}
						file.getline(str, BUFFER_SIZE);
					}
				}
			}
			if (isFound) break;
		}
		file.close();
		file.open("pci.ids.txt");
	}
	file.close();
	return description;
}

int main()
{
	multimap<string, string> values = getDeviceList();
	multimap<string, string> des = getDescription(values);
	system("pause");
    return 0;
}