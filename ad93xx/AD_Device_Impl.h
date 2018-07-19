#ifndef AD_DEVICE_IMPL_H
#define AD_DEVICE_IMPL_H

class AD_Device_Impl
{
protected:
	iio_context *m_pCtx;

public:
	AD_Device_Impl(){;}
	virtual ~AD_Device_Impl(){;}

	// Открытие устройства
	virtual int Open(iio_context *pCtx){return 0;}
	// Закрытие устройства
	virtual int Close(){return 0;}

	// Получение количества каналов
	virtual int ChanCount(){return 0;}
	// Включение/выключение канала
	virtual int ChanEnable(int isEnable, int isOut, int nChanNo){return 0;}
	
	// Установить параметр
	virtual int SetParam(int isOut, int nChanNo, int nParamID, void *pParam){return 0;}
	// Получить параметр
	virtual int GetParam(int isOut, int nChanNo, int nParamID, void *pParam) { return 0; }

	// Создать буфер
	virtual int CreateBuffer(int isOut, int nSamplesCount, int isCyclic, void **ppData){return 0;}
	// Освободить буфер
	virtual void DestroyBuffer(int isOut){;}

	// Получить данные
	virtual int RecvData(){return 0;}
	// Передать данные
	virtual int SendData(){return 0;}
};

#endif // AD_DEVICE_IMPL_H