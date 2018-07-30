#include "gipcy.h"
#include "ipcdefs.h"
#include "AD93xx_Device.h"

#define MAX_PATH 256

enum iodev { RX, TX };

long long g_nTxCenterFreq;
long long g_nTxSampleRate;
long long g_nTxBandwidth;

long long g_nNumberOfSamples;

int g_nChanMask;

char g_sPortSelect[1024];

long long g_nAttenuation0;
long long g_nAttenuation1;

IPC_str g_iniFileName[MAX_PATH] = _IPCS("/ad9361-send.ini");

char g_sIPAddr[15];

long long g_isFirFilter=0;

char g_sFirFilterCoeffPath[MAX_PATH];
IPC_str g_sDataFilePath[MAX_PATH];

AD93xx_Device g_cAdDevice;

static void GetInifileString(const IPC_str *FileName, const IPC_str *SectionName, const IPC_str *ParamName, IPC_str *defValue, IPC_str *strValue, int strSize);

// Чтение ini-файла
void GetOptions();
// Установка параметров
void SetParams();
// Отображение парамтеров
void PrintParams();
// Включение каналов
void ChansEnable();

int IPCS_main(int argc, IPC_str **argv)
{
	void *pSendData;

	if(argc == 1)
	{
		IPCS_printf(_IPCS("No argument: ad9361-recv <ipaddr> <inifile>\n"));
		return 0;
	}

	IPCS_ipcstombs(g_sIPAddr, argv[1], 15);

	if(argc > 2)
		IPCS_strcpy(g_iniFileName, argv[2]);

	IPC_initKeyboard();

	if(g_cAdDevice.Open(g_sIPAddr) < 0)
	{
		IPCS_printf(_IPCS("Can not open device!"));
		return 0;
	}

	// Чтение параметров из ini-файла
	GetOptions();
	
	// Установка параметров
	SetParams();

	// Вывод парамтеров
	PrintParams();

	// Включение каналов
	ChansEnable();

	// Открытие файла с данными
	IPC_handle hFile = 0;
	hFile = IPC_openFile(g_sDataFilePath, IPC_OPEN_FILE | IPC_FILE_RDONLY);

	if(hFile == 0)
	{	// Ошибка открытия файла
		IPCS_printf(_IPCS("Can not open file %s!"), g_sDataFilePath);
		return 0;
	}
		
	// Вычисление количества отсчетов
	IPC_getFileSize(hFile, &g_nNumberOfSamples);
	g_nNumberOfSamples /= 4;

	// Создание циклического буфера для передачи данных
	g_cAdDevice.CreateBuffer(TX, g_nNumberOfSamples, true, &pSendData);

	// Чтение данных из файла
	IPC_readFile(hFile, pSendData, g_nNumberOfSamples * 4);

	// Закрытие файла
	IPC_closeFile(hFile);

	// Передача данных
	g_cAdDevice.SendData();

	// Выход из программы по Esc
	for(;;)
	{
		if(IPC_kbhit())
			if(0x1B == IPC_getch())
				break;

		IPC_delay(100);
	}
	
	IPC_cleanupKeyboard();

	return 0;
} 

static void GetInifileString(const IPC_str *FileName, const IPC_str *SectionName, const IPC_str *ParamName, IPC_str *defValue, IPC_str *strValue, int strSize)
{
#if defined(__IPC_WIN__) || defined(__IPC_LINUX__)
	IPC_getPrivateProfileString(SectionName, ParamName, defValue, strValue, strSize, FileName);
#else
	GetPrivateProfileString(SectionName, ParamName, defValue, strValue, strSize, FileName);
#endif
	// удалить комментарий из строки 
	IPC_str* pChar = IPCS_strchr(strValue, ';'); // признак комментария или ;
	if(pChar) *pChar = 0;
	pChar = IPCS_strchr(strValue, '/');			// или //
	if(pChar) if(*(pChar + 1) == '/')	*pChar = 0;

	// Удалить пробелы в конце строки
	int str_size = (int)IPCS_strlen(strValue);
	for(int i = str_size - 1; i > -1; i--)
		if(strValue[i] != ' ' && strValue[i] != '\t')
		{
			strValue[i + 1] = 0;
			break;
		}
}

// Чтение ini-файла
void GetOptions()
{
	IPC_str Buffer[128];
	IPC_str iniFilePath[MAX_PATH];
	IPC_str* endptr;

	IPC_getCurrentDir(iniFilePath, sizeof(iniFilePath) / sizeof(IPC_str));
	IPCS_strcat(iniFilePath, g_iniFileName);

	IPC_getPrivateProfileString(_IPCS("TX_Params"), _IPCS("ChanMask"), _IPCS("0"), Buffer, sizeof(Buffer), iniFilePath);
	g_nChanMask = IPCS_strtol(Buffer, &endptr, 10);

	IPC_getPrivateProfileString(_IPCS("TX_Params"), _IPCS("SampleRate"), _IPCS("0"), Buffer, sizeof(Buffer), iniFilePath);
	g_nTxSampleRate = IPCS_strtoll(Buffer, &endptr, 10);

	IPC_getPrivateProfileString(_IPCS("TX_Params"), _IPCS("Bandwidth"), _IPCS("0"), Buffer, sizeof(Buffer), iniFilePath);
	g_nTxBandwidth = IPCS_strtoll(Buffer, &endptr, 10);

	IPC_getPrivateProfileString(_IPCS("TX_Params"), _IPCS("CenterFreq"), _IPCS("0"), Buffer, sizeof(Buffer), iniFilePath);
	g_nTxCenterFreq = IPCS_strtoll(Buffer, &endptr, 10);

	GetInifileString(iniFilePath, _IPCS("TX_Params"), _IPCS("PortSelect"), _IPCS("A"), Buffer, sizeof(Buffer));
	IPCS_ipcstombs(g_sPortSelect, Buffer, 1024);

	IPC_getPrivateProfileString(_IPCS("TX_Params"), _IPCS("Attenuation0"), _IPCS("0"), Buffer, sizeof(Buffer), iniFilePath);
	g_nAttenuation0 = IPCS_strtoll(Buffer, &endptr, 10);

	IPC_getPrivateProfileString(_IPCS("TX_Params"), _IPCS("Attenuation1"), _IPCS("0"), Buffer, sizeof(Buffer), iniFilePath);
	g_nAttenuation1 = IPCS_strtoll(Buffer, &endptr, 10);

	GetInifileString(iniFilePath, _IPCS("TX_Params"), _IPCS("FirFilterCoef"), _IPCS(""), Buffer, sizeof(Buffer));
	IPCS_ipcstombs(g_sFirFilterCoeffPath, Buffer, 1024);

	IPC_getPrivateProfileString(_IPCS("TX_Params"), _IPCS("FirFilterEnable"), _IPCS("0"), Buffer, sizeof(Buffer), iniFilePath);
	g_isFirFilter = IPCS_strtoll(Buffer, &endptr, 10);

	IPC_getPrivateProfileString(_IPCS("TX_Params"), _IPCS("DataFile"), _IPCS(""), g_sDataFilePath, sizeof(g_sDataFilePath), iniFilePath);
}

// Установка параметров
void SetParams()
{
	int nRet=0;
	
	nRet = g_cAdDevice.SetParam(TX, 0, AD93xx_SAMPLERATE,  &g_nTxSampleRate);
	nRet = g_cAdDevice.SetParam(TX, 0, AD93xx_CENTERFREQ,  &g_nTxCenterFreq);
	nRet = g_cAdDevice.SetParam(TX, 0, AD93xx_BANDWIDTH,   &g_nTxBandwidth);
	nRet = g_cAdDevice.SetParam(TX, 0, AD93xx_PORTSELECT,  &g_sPortSelect);
	nRet = g_cAdDevice.SetParam(TX, 0, AD93xx_ATTENUATION, &g_nAttenuation0);
	nRet = g_cAdDevice.SetParam(TX, 1, AD93xx_ATTENUATION, &g_nAttenuation1);

	if(g_isFirFilter)
		nRet = g_cAdDevice.SetParam(TX, 0, AD93xx_FIRFILTER, g_sFirFilterCoeffPath);

	nRet = g_cAdDevice.SetParam(TX, 0, AD93xx_FIRFILTERENABLE, &g_isFirFilter);
}

// Отображение парамтеров
void PrintParams()
{
	int nRet = 0;
	IPC_str sStr[1024];

	IPCS_printf(_IPCS("DataFile = %s\n"), g_sDataFilePath);

	nRet = g_cAdDevice.GetParam(RX, 0, AD93xx_SAMPLERATE, &g_nTxSampleRate);
	IPCS_printf(_IPCS("SampleRate = %llu\n"), g_nTxSampleRate);
	IPCS_printf(_IPCS("CenterFreq = %llu\n"), g_nTxCenterFreq);
	nRet = g_cAdDevice.GetParam(RX, 0, AD93xx_BANDWIDTH, &g_nTxBandwidth);
	IPCS_printf(_IPCS("Bandwidth = %llu\n"), g_nTxBandwidth);

	IPCS_mbstoipcs(sStr, g_sPortSelect, 1024);
	IPCS_printf(_IPCS("PortSelect = %s\n"), sStr);

	IPCS_printf(_IPCS("Attenuation0 = %llu\n"), g_nAttenuation0);
	IPCS_printf(_IPCS("Attenuation1 = %llu\n"), g_nAttenuation1);
	
	IPCS_printf(_IPCS("isFilter = %d\n"), g_isFirFilter);

	if(g_isFirFilter)
		IPCS_printf(_IPCS("FirFilterCoeffPath = %s\n"), g_sFirFilterCoeffPath);
	
	IPCS_printf(_IPCS("ChanMask = %d\n"), g_nChanMask);
}

// Включение каналов
void ChansEnable()
{
	g_cAdDevice.ChanEnable(0, TX, 0);
	g_cAdDevice.ChanEnable(0, TX, 1);

	if(g_nChanMask & 1)
		g_cAdDevice.ChanEnable(1, TX, 0);

	if((g_nChanMask >> 1) & 1)
		g_cAdDevice.ChanEnable(1, TX, 1);
}