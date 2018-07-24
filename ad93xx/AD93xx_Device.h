#ifndef AD93XX_DEVICE_H
#define AD93XX_DEVICE_H

#ifndef __linux__
#ifdef ad93xx_EXPORTS
#define AD93XX_API __declspec(dllexport) 
#else
#define AD93XX_API __declspec(dllimport) 
#endif
#else
#define AD93XX_API	
#endif
	
#include "AD93xx_Device_Params.h"

class AD_Device_Impl;

struct iio_context;

class AD93XX_API AD93xx_Device
{
	volatile int m_isOpen;

	iio_context *m_pCtx;
	AD_Device_Impl *m_pDevImpl;

public:
	AD93xx_Device();
	~AD93xx_Device();

	// �������� ����������
	int Open(const char* ip);
	// �������� ����������
	int Close();

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
};

#endif // AD93XX_DEVICE_H
