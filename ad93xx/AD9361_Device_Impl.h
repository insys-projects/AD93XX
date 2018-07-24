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

	// �������� ����������
	virtual int Open(iio_context *pCtx);
	// �������� ����������
	virtual int Close();

	// ��������� ���������� �������
	virtual int ChanCount();
	// ���������/���������� ������
	virtual int ChanEnable(int isEnable, int isOut, int nChanNo);

	// ���������� ��������
	virtual int SetParam(int isOut, int nChanNo, int nParamID, void *pParam);
	// �������� ��������
	virtual int GetParam(int isOut, int nChanNo, int nParamID, void *pParam);

	// ������� �����
	virtual int CreateBuffer(int isOut, int nSamplesCount, int isCyclic, void **ppData);
	// ���������� �����
	virtual void DestroyBuffer(int isOut);

	// �������� ������
	virtual int RecvData();
	// �������� ������
	virtual int SendData();

private:
	// ��������� ����������� �������
	int SetCenterFreq(int isOut, long long nCenterFreq);
	// ��������� ����������� �������
	int GetCenterFreq(int isOut, long long *pnCenterFreq);

	// ��������� ������� �������������
	int SetSampleRate(int isOut, long long nSampleRate);
	// ��������� ������� �������������
	int GetSampleRate(int isOut, long long *pnSampleRate);

	// ��������� ������
	int SetBandwidth(int isOut, long long nBandwidth);
	// ��������� ������
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