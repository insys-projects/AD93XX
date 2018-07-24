#ifndef AD9361_DEVICE_IMPL_H
#define AD9361_DEVICE_IMPL_H

#include "iio.h"

#include "AD_Device_Impl.h"

#define AD9361_PHY "ad9361-phy"

#define CHAN_NUM 2

class AD9361_Device_Impl : public AD_Device_Impl
{
	enum
	{
		RX = 0,
		TX
	};

	volatile int m_isOpen;

	iio_device	*m_pPhyDev;
	iio_channel	*m_apPhyChan[2][CHAN_NUM];
	iio_channel	*m_apLOChan[2];

	iio_device	*m_apStreamDev[2];
	iio_channel	*m_apStreamChan[2][CHAN_NUM * 2];
	
	iio_buffer	*m_apBuffer[2];

public:
	AD9361_Device_Impl();
	virtual ~AD9361_Device_Impl();

	// Открытие устройства
	virtual int Open(iio_context *pCtx);
	// Закрытие устройства
	virtual int Close();

	// Получение количества каналов
	virtual int ChanCount();
	// Включение/выключение канала
	virtual int ChanEnable(int isEnable, int isOut, int nChanNo);

	// Установить параметр
	virtual int SetParam(int isOut, int nChanNo, int nParamID, void *pParam);
	// Получить параметр
	virtual int GetParam(int isOut, int nChanNo, int nParamID, void *pParam);

	// Создать буфер
	virtual int CreateBuffer(int isOut, int nSamplesCount, int isCyclic, void **ppData);
	// Освободить буфер
	virtual void DestroyBuffer(int isOut);

	// Получить данные
	virtual int RecvData();
	// Передать данные
	virtual int SendData();

private:
	// Установка центральной частоты
	int SetCenterFreq(int isOut, long long nCenterFreq);
	// Получение центральной частоты
	int GetCenterFreq(int isOut, long long *pnCenterFreq);

	// Установка частоты дискретизации
	int SetSampleRate(int isOut, long long nSampleRate);
	// Получение частоты дискретизации
	int GetSampleRate(int isOut, long long *pnSampleRate);

	// Установка полосы
	int SetBandwidth(int isOut, long long nBandwidth);
	// Получение полосы
	int GetBandwidth(int isOut, long long *pnBandwidth);

	int SetPortSelect(int isOut, char *pPortSelect);
	int GetPortSelect(int isOut, char *pPortSelect);

	int SetGainMode(int isOut, int nChanNo, char *pGainMode);
	int GetGainMode(int isOut, int nChanNo, char *pGainMode);

	int SetManualGain(int isOut, int nChanNo, long long nManualGain);
	int GetManualGain(int isOut, int nChanNo, long long *pManualGain);

	int LoadFirFilter(char *pBuf);
	int FirFilterEnable(long long isEnable);
	int IsFirFilterEnable(int isOut, long long *pEnable);
};

#endif // AD_DEVICE_IMPL_H