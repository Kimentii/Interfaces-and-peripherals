#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <WinIoCtl.h>
#include <ntddscsi.h>
#include <conio.h>

using namespace std;

#define bThousand 1024
#define Hundred 100

char* busType[] = { "UNKNOWN", "SCSI", "ATAPI", "ATA", "ONE_TREE_NINE_FOUR", "SSA", "FIBRE", "USB", "RAID", "ISCSI", "SAS", "SATA", "SD", "MMC" };

void getDeviceInfo(HANDLE diskHandle, STORAGE_PROPERTY_QUERY storageProtertyQuery) {

	//Выделение памяти под дескриптор устройства
	STORAGE_DEVICE_DESCRIPTOR* deviceDescriptor = (STORAGE_DEVICE_DESCRIPTOR*)calloc(bThousand, 1);

	deviceDescriptor->Size = bThousand;

	//Отправляем управляющий код непосредственно указанному драйверу устройства, заставляя устройство выполнить соответствующую операцию.
	if (!DeviceIoControl(diskHandle,
		IOCTL_STORAGE_QUERY_PROPERTY,				//Отправляем запрос на возврат свойств устройства.
		&storageProtertyQuery, sizeof(storageProtertyQuery), deviceDescriptor, bThousand, NULL, 0)) {
		printf("%d", GetLastError());
		CloseHandle(diskHandle);
		exit(-1);
	}

	//Выводим свойства диска.
	cout << "Model:    " <<setw(30) << (char*)(deviceDescriptor)+deviceDescriptor->ProductIdOffset << endl;
	cout << "Version        " << (char*)(deviceDescriptor)+deviceDescriptor->ProductRevisionOffset << endl;
	cout << "Bus type:      " << busType[deviceDescriptor->BusType] << endl;
	cout << "Serial number: " << (char*)(deviceDescriptor)+deviceDescriptor->SerialNumberOffset << endl;
}

void getMemoryInfo() {
	string path;
	_ULARGE_INTEGER diskSpace;
	_ULARGE_INTEGER freeSpace;

	diskSpace.QuadPart = 0;
	freeSpace.QuadPart = 0;

	_ULARGE_INTEGER totalDiskSpace;
	_ULARGE_INTEGER totalFreeSpace;

	totalDiskSpace.QuadPart = 0;				//Будет содержать полный размер диска.
	totalFreeSpace.QuadPart = 0;				//Будет содеражть свободное место диска.
	
	//Получаем битовую маску, представляющую имеющиеся в настоящие время дисковые накопители.
	unsigned long int logicalDrivesCount = GetLogicalDrives();
		
	cout.setf(ios::left);
	cout<< setw(16) << "Total space[Mb]"
		<< setw(16) << "Free space[Mb]"
		<< setw(16) << "Busy space[%]"
		<< endl;

	//Анализ полученной битовой маски(бит 0 - диск А, бит 1 - диск B).
	for (char var = 'A'; var < 'Z'; var++) {
		if ((logicalDrivesCount >> var - 65) & 1 && var != 'F') {
			path = var;
			path.append(":\\");
			//Получаем информация о размере диска и свободном пространстве диска.
			GetDiskFreeSpaceEx(path.c_str(), 0, &diskSpace, &freeSpace);
			diskSpace.QuadPart = diskSpace.QuadPart / (bThousand * bThousand);
			freeSpace.QuadPart = freeSpace.QuadPart / (bThousand * bThousand);

			//Определяем тип диска(3 - жесткий диск)
			if (GetDriveType(path.c_str()) == 3) {
				totalDiskSpace.QuadPart += diskSpace.QuadPart;
				totalFreeSpace.QuadPart += freeSpace.QuadPart;
			}
		}
	}

	cout	<< setw(16) << totalDiskSpace.QuadPart
		<< setw(16) << totalFreeSpace.QuadPart
		<< setw(16) << setprecision(3) << 100.0 - (double)totalFreeSpace.QuadPart / (double)totalDiskSpace.QuadPart * Hundred
		<< endl;
}

int main()
{
	STORAGE_PROPERTY_QUERY storageProtertyQuery;				//Структура с информацией об запросе
	storageProtertyQuery.QueryType = PropertyStandardQuery;		//Запрос драйвера, чтобы он вернул дескриптор устройства.
	storageProtertyQuery.PropertyId = StorageDeviceProperty;	//Флаг, гооврящий мы хотим получить дескриптор устройства.

																//Открытие файла с информацией о диске
	HANDLE diskHandle = CreateFile("//./PhysicalDrive0", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if (diskHandle == INVALID_HANDLE_VALUE) {
		cout << GetLastError();
		_getch();
		return -1;
	}
	getDeviceInfo(diskHandle, storageProtertyQuery);
	getMemoryInfo();
	_getch();
	return 0;
}