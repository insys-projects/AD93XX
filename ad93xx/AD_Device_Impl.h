#ifndef AD_DEVICE_IMPL_H
#define AD_DEVICE_IMPL_H

class AD_Device_Impl
{
protected:
	iio_context *m_pCtx;

public:
	AD_Device_Impl(){;}
	virtual ~AD_Device_Impl(){;}

	// �������� ����������
	virtual int Open(iio_context *pCtx){return 0;}
	// �������� ����������
	virtual int Close(){return 0;}

	// ��������� ���������� �������
	virtual int ChanCount(){return 0;}
	// ���������/���������� ������
	virtual int ChanEnable(int isEnable, int isOut, int nChanNo){return 0;}
	
	// ���������� ��������
	virtual int SetParam(int isOut, int nChanNo, int nParamID, void *pParam){return 0;}
	// �������� ��������
	virtual int GetParam(int isOut, int nChanNo, int nParamID, void *pParam) { return 0; }

	// ������� �����
	virtual int CreateBuffer(int isOut, int nSamplesCount, int isCyclic, void **ppData){return 0;}
	// ���������� �����
	virtual void DestroyBuffer(int isOut){;}

	// �������� ������
	virtual int RecvData(){return 0;}
	// �������� ������
	virtual int SendData(){return 0;}
};

#endif // AD_DEVICE_IMPL_H