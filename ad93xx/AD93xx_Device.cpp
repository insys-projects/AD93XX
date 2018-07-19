#include "AD93xx_Device.h"
#include "AD9361_Device_Impl.h"

AD93xx_Device::AD93xx_Device()
{
	m_isOpen = 0;
	m_pCtx = 0;
	m_pDevImpl = 0;
}

AD93xx_Device::~AD93xx_Device()
{
	Close();
}

// Открытие устройства
// ip - адрес устройства
int AD93xx_Device::Open(const char* ip)
{
	// Закрытие устройства, если оно открыто
	Close();

	m_pCtx = iio_create_network_context(ip);

	if(!m_pCtx)
		// Устройство не найдено
		return -1;
	
	// Определяем тип устройства
	if(iio_context_find_device(m_pCtx, AD9361_PHY))
	{	// AD9361
		m_pDevImpl = dynamic_cast<AD_Device_Impl *>(new AD9361_Device_Impl);

		// Открытие устройства
		if(m_pDevImpl->Open(m_pCtx) < 0)
			return -1;
	}

	m_isOpen = 1;

	return 0;
}

// Закрытие устройства
int AD93xx_Device::Close()
{
	if(!m_isOpen)
		return 0;

	DestroyBuffer(0);
	DestroyBuffer(1);

	delete m_pDevImpl;
	m_pDevImpl = 0;

	iio_context_destroy(m_pCtx);
	m_pCtx = 0;

	m_isOpen = 0;

	return 0;
}

// Получение количества каналов
int AD93xx_Device::ChanCount()
{
	if(m_pDevImpl == 0)
		return -1;

	return m_pDevImpl->ChanCount();
}

// Включение/выключение канала
int AD93xx_Device::ChanEnable(int isEnable, int isOut, int nChanNo)
{
	if(m_pDevImpl == 0)
		return -1;

	return m_pDevImpl->ChanEnable(isEnable, isOut, nChanNo);
}

// Установить параметр
int AD93xx_Device::SetParam(int isOut, int nChanNo, int nParamID, void *pParam)
{
	if(m_pDevImpl == 0)
		return -1;

	return m_pDevImpl->SetParam(isOut, nChanNo, nParamID, pParam);
}

// Получить параметр
int AD93xx_Device::GetParam(int isOut, int nChanNo, int nParamID, void *pParam)
{
	if(m_pDevImpl == 0)
		return -1;

	return m_pDevImpl->GetParam(isOut, nChanNo, nParamID, pParam);
}

// Создать буфер
int AD93xx_Device::CreateBuffer(int isOut, int nSamplesCount, int isCyclic, void **ppData)
{
	if(m_pDevImpl == 0)
		return -1;

	return m_pDevImpl->CreateBuffer(isOut, nSamplesCount, isCyclic, ppData);
}

// Освободить буфер
void AD93xx_Device::DestroyBuffer(int isOut)
{
	if(m_pDevImpl == 0)
		return;

	return m_pDevImpl->DestroyBuffer(isOut);
}

// Получить данные
int AD93xx_Device::RecvData()
{
	if(m_pDevImpl == 0)
		return 0;

	return m_pDevImpl->RecvData();
}

// Передать данные
int AD93xx_Device::SendData()
{
	if(m_pDevImpl == 0)
		return 0;

	return m_pDevImpl->SendData();
}