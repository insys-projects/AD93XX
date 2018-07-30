#include "gipcy.h"
#include "ipcdefs.h"
#include "AD93xx_Device.h"

#define MAX_PATH 256

enum iodev { RX, TX };

long long g_nRxCenterFreq;
long long g_nRxSampleRate;
long long g_nRxBandwidth;

long long g_nNumberOfSamples;

int g_nChanMask;

char g_sPortSelect[1024];

char g_sGainMode0[1024];
char g_sGainMode1[1024];

long long g_nManualGain0;
long long g_nManualGain1;

IPC_str g_iniFileName[MAX_PATH] = _IPCS("/ad9361-recv.ini");
char g_sIPAddr[15];

long long g_isFirFilter;

char g_sFirFilterCoeffPath[MAX_PATH];

AD93xx_Device g_cAdDevice;

void WriteFlagSinc(int flg, int isNewParam);
int ReadFlagSinc();

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
	int nRet;
	int nLoop;

	void *pRecvData;

	if(argc == 1)
	{
		IPCS_printf(_IPCS("No argument: ad9361-recv <ipaddr>\n"));
		return 0;
	}

	IPCS_ipcstombs(g_sIPAddr, argv[1], 15);

	if(argc > 2)
		IPCS_strcpy(g_iniFileName, argv[2]);

	IPC_initKeyboard();

	if(g_cAdDevice.Open(g_sIPAddr) < 0)
	{
		IPCS_printf(_IPCS("Device is not open!"));
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

	// Создание буфера для приема данных
	g_cAdDevice.CreateBuffer(0, g_nNumberOfSamples, false, &pRecvData);

	int isExit = 0;

	IPCS_printf(_IPCS("\n"));

	for(int loop = 0;; loop++)
	{
		IPCS_printf(_IPCS("Send Loop: %d\r"), loop);

		// Прием данных
		int rcv = g_cAdDevice.RecvData();

		// Запись данных в файл
		IPC_handle hfile = 0;
		hfile = IPC_openFile(_IPCS("data.bin"), IPC_CREATE_FILE | IPC_FILE_WRONLY);
		IPC_writeFile(hfile, pRecvData, rcv);
		IPC_closeFile(hfile);

		if(loop == 0)
			WriteFlagSinc(0xffffffff, 0xffffffff);
		else
			WriteFlagSinc(0xffffffff, 0x0);

		for(int ii = 0; ;)
		{
			int rr = ReadFlagSinc();

			if(IPC_kbhit()) 
				if(0x1B == IPC_getch())
					isExit = 1;

			if(rr == 1)
			{
				IPC_delay(100);
				continue;
			}

			if(rr == 0)
				break;
			
			if(rr == 0xffffffff && ii>0x1000)
				break;

			ii++;
		}

		if(isExit)
			break;
	}
	
	IPC_cleanupKeyboard();

	return 0;
} 

void WriteFlagSinc(int flg, int isNewParam)
{
		int val[2];
		IPC_handle handle = NULL;

		while(!handle)
			handle = IPC_openFile(_IPCS("data.flg"), IPC_CREATE_FILE | IPC_FILE_WRONLY);

		val[0] = flg;
		val[1] = isNewParam;

		IPC_writeFile(handle, val, 8);
		IPC_closeFile(handle);
}

int ReadFlagSinc()
{
	int flg;
	IPC_handle handle = NULL;
	
	while(!handle)
		handle = IPC_openFile(_IPCS("data.flg"), IPC_OPEN_FILE | IPC_FILE_RDONLY);

	IPC_readFile(handle, &flg, 4);
	IPC_closeFile(handle);

	return flg;
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
	for(int i = str_size - 1; i > 1; i--)
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

	IPC_getPrivateProfileString(_IPCS("RX_Params"), _IPCS("NumberOfSamples"), _IPCS("0"), Buffer, sizeof(Buffer), iniFilePath);
	g_nNumberOfSamples = IPCS_strtoll(Buffer, &endptr, 10);

	IPC_getPrivateProfileString(_IPCS("RX_Params"), _IPCS("ChanMask"), _IPCS("0"), Buffer, sizeof(Buffer), iniFilePath);
	g_nChanMask = IPCS_strtol(Buffer, &endptr, 10);

	IPC_getPrivateProfileString(_IPCS("RX_Params"), _IPCS("SampleRate"), _IPCS("0"), Buffer, sizeof(Buffer), iniFilePath);
	g_nRxSampleRate = IPCS_strtoll(Buffer, &endptr, 10);

	IPC_getPrivateProfileString(_IPCS("RX_Params"), _IPCS("Bandwidth"), _IPCS("0"), Buffer, sizeof(Buffer), iniFilePath);
	g_nRxBandwidth = IPCS_strtoll(Buffer, &endptr, 10);

	IPC_getPrivateProfileString(_IPCS("RX_Params"), _IPCS("CenterFreq"), _IPCS("0"), Buffer, sizeof(Buffer), iniFilePath);
	g_nRxCenterFreq = IPCS_strtoll(Buffer, &endptr, 10);

	GetInifileString(iniFilePath, _IPCS("RX_Params"), _IPCS("PortSelect"), _IPCS("A_BALANCED"), Buffer, sizeof(Buffer));
	IPCS_ipcstombs(g_sPortSelect, Buffer, 1024);

	GetInifileString(iniFilePath, _IPCS("RX_Params"), _IPCS("GainMode0"), _IPCS("manual"), Buffer, sizeof(Buffer));
	IPCS_ipcstombs(g_sGainMode0, Buffer, 1024);

	GetInifileString(iniFilePath, _IPCS("RX_Params"), _IPCS("GainMode1"), _IPCS("manual"), Buffer, sizeof(Buffer));
	IPCS_ipcstombs(g_sGainMode1, Buffer, 1024);

	IPC_getPrivateProfileString(_IPCS("RX_Params"), _IPCS("ManualGain0"), _IPCS("0"), Buffer, sizeof(Buffer), iniFilePath);
	g_nManualGain0 = IPCS_strtoll(Buffer, &endptr, 10);

	IPC_getPrivateProfileString(_IPCS("RX_Params"), _IPCS("ManualGain1"), _IPCS("0"), Buffer, sizeof(Buffer), iniFilePath);
	g_nManualGain1 = IPCS_strtoll(Buffer, &endptr, 10);

	GetInifileString(iniFilePath, _IPCS("RX_Params"), _IPCS("FirFilterCoef"), _IPCS(""), Buffer, sizeof(Buffer));
	IPCS_ipcstombs(g_sFirFilterCoeffPath, Buffer, 1024);

	IPC_getPrivateProfileString(_IPCS("RX_Params"), _IPCS("FirFilterEnable"), _IPCS("0"), Buffer, sizeof(Buffer), iniFilePath);
	g_isFirFilter = IPCS_strtoll(Buffer, &endptr, 10);
}

// Установка параметров
void SetParams()
{
	int nRet=0;
	
	nRet = g_cAdDevice.SetParam(RX, 0, AD93xx_SAMPLERATE, &g_nRxSampleRate);
	nRet = g_cAdDevice.SetParam(RX, 0, AD93xx_CENTERFREQ, &g_nRxCenterFreq);
	nRet = g_cAdDevice.SetParam(RX, 0, AD93xx_BANDWIDTH,  &g_nRxBandwidth);
	nRet = g_cAdDevice.SetParam(RX, 0, AD93xx_PORTSELECT, &g_sPortSelect);
	nRet = g_cAdDevice.SetParam(RX, 0, AD93xx_GAINMODE,   &g_sGainMode0);
	nRet = g_cAdDevice.SetParam(RX, 1, AD93xx_GAINMODE,   &g_sGainMode1);
	nRet = g_cAdDevice.SetParam(RX, 0, AD93xx_MANUALGAIN, &g_nManualGain0);
	nRet = g_cAdDevice.SetParam(RX, 1, AD93xx_MANUALGAIN, &g_nManualGain1);

	if(g_isFirFilter)
		nRet = g_cAdDevice.SetParam(RX, 0, AD93xx_FIRFILTER, g_sFirFilterCoeffPath);

	nRet = g_cAdDevice.SetParam(RX, 0, AD93xx_FIRFILTERENABLE, &g_isFirFilter);
}

// Отображение парамтеров
void PrintParams()
{
	int nRet = 0;
	IPC_str sStr[1024];

	nRet = g_cAdDevice.GetParam(RX, 0, AD93xx_SAMPLERATE, &g_nRxSampleRate);
	IPCS_printf(_IPCS("SampleRate = %llu\n"), g_nRxSampleRate);
	IPCS_printf(_IPCS("CenterFreq = %llu\n"), g_nRxCenterFreq);
	nRet = g_cAdDevice.GetParam(RX, 0, AD93xx_BANDWIDTH, &g_nRxBandwidth);
	IPCS_printf(_IPCS("Bandwidth = %llu\n"), g_nRxBandwidth);

	IPCS_mbstoipcs(sStr, g_sPortSelect, 1024);
	IPCS_printf(_IPCS("PortSelect = %s\n"), sStr);

	IPCS_mbstoipcs(sStr, g_sGainMode0, 1024);
	IPCS_printf(_IPCS("GainMode0 = %s\n"), sStr);

	IPCS_printf(_IPCS("ManualGain0 = %llu\n"), g_nManualGain0);

	IPCS_mbstoipcs(sStr, g_sGainMode1, 1024);
	IPCS_printf(_IPCS("GainMode1 = %s\n"), sStr);

	IPCS_printf(_IPCS("ManualGain1 = %llu\n"), g_nManualGain1);
	IPCS_printf(_IPCS("isFilter = %d\n"), g_isFirFilter);

	if(g_isFirFilter)
		IPCS_printf(_IPCS("FirFilterCoeffPath = %s\n"), g_sFirFilterCoeffPath);
	
	IPCS_printf(_IPCS("ChanMask = %d\n"), g_nChanMask);
}

// Включение каналов
void ChansEnable()
{
	if(g_nChanMask & 1)
		g_cAdDevice.ChanEnable(1, RX, 0);

	if((g_nChanMask >> 1) & 1)
		g_cAdDevice.ChanEnable(1, RX, 1);
}