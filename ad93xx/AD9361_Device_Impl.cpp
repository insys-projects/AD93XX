#include <stdio.h>
#include <string>
#include <fstream>

#include "ad9361.h"

#include "AD93xx_Device_Params.h"
#include "AD9361_Device_Impl.h"

AD9361_Device_Impl::AD9361_Device_Impl():AD_Device_Impl()
{
	m_isOpen = 0;

	m_apBuffer[RX] = 0;
	m_apBuffer[TX] = 0;
}

AD9361_Device_Impl::~AD9361_Device_Impl()
{
	// Закрытие устройства
	Close();
}

// Открытие устройства
int AD9361_Device_Impl::Open(iio_context *pCtx)
{
	int i;
	int j;
	char str[64];

	if(!pCtx)
		return -1;

	Close();

	m_pCtx = pCtx;
	
	m_pPhyDev = iio_context_find_device(m_pCtx, "ad9361-phy");

	if(!m_pPhyDev)
		return -1;

	// Получение физических каналов
	for(i = 0; i < CHAN_NUM; i++)
	{
		sprintf(str, "voltage%d", i);
		m_apPhyChan[RX][i] = iio_device_find_channel(m_pPhyDev, str, RX);
		m_apPhyChan[TX][i] = iio_device_find_channel(m_pPhyDev, str, TX);
	}

	for(i = 0; i < 2; i++)
	{
		sprintf(str, "altvoltage%d", i);
		m_apLOChan[i] = iio_device_find_channel(m_pPhyDev, str, 1);
	}

	// Получение стримов
	m_apStreamDev[TX] = iio_context_find_device(m_pCtx, "cf-ad9361-dds-core-lpc");

	if(!m_apStreamDev[TX])
		return -1;

	m_apStreamDev[RX] = iio_context_find_device(m_pCtx, "cf-ad9361-lpc");

	if(!m_apStreamDev[RX])
		return -1;

	// Получение каналов стримов
	for(i = 0; i < CHAN_NUM; i++)
		for(j = 0; j < 2; j++)
		{
			sprintf(str, "voltage%d", i * 2 + j);
			m_apStreamChan[RX][i * 2 + j] = iio_device_find_channel(m_apStreamDev[RX], str, RX);
			m_apStreamChan[TX][i * 2 + j] = iio_device_find_channel(m_apStreamDev[TX], str, TX);
		}

	m_isOpen = 1;

	return 0;
}

// Закрытие устройства
int AD9361_Device_Impl::Close()
{
	int i;

	if(!m_isOpen)
		return 0;
	
	// Выключение каналов
	for(i = 0; i < CHAN_NUM; i++)
	{
		ChanEnable(0, 0, i);
		ChanEnable(0, 1, i);
	}

	m_isOpen = 0;

	return 0;
}

// Получение количества каналов
int AD9361_Device_Impl::ChanCount()
{
	return CHAN_NUM;
}

// Включение/выключение канала
int AD9361_Device_Impl::ChanEnable(int isEnable, int isOut, int nChanNo)
{
	iio_channel *pChanI;
	iio_channel *pChanQ;

	if(nChanNo >= ChanCount())
		return -1;

	pChanI = m_apStreamChan[isOut][nChanNo * 2];
	pChanQ = m_apStreamChan[isOut][nChanNo * 2 + 1];
	
	if((pChanI == 0) || (pChanQ == 0))
		return -1;

	if(isEnable)
	{
		iio_channel_enable(pChanI);
		iio_channel_enable(pChanQ);
	}
	else
	{
		iio_channel_disable(pChanI);
		iio_channel_disable(pChanQ);
	}

	return 0;
}

// Установить параметр
int AD9361_Device_Impl::SetParam(int isOut, int nChanNo, int nParamID, void *pParam)
{
	switch(nParamID)
	{
		// Установка центральной полосы
		case AD93xx_CENTERFREQ:
			return SetCenterFreq(isOut, *(long long *)pParam);

		// Установка частоты дискретизации
		case AD93xx_SAMPLERATE:
			return SetSampleRate(isOut, *(long long *)pParam);

		// Установка полосы
		case AD93xx_BANDWIDTH:
			return SetBandwidth(isOut, *(long long *)pParam);

		case AD93xx_PORTSELECT:
			return SetPortSelect(isOut, (char*)pParam);

		case AD93xx_GAINMODE:
			return SetGainMode(isOut, nChanNo, (char*)pParam);

		case AD93xx_MANUALGAIN:
		case AD93xx_ATTENUATION:
			return SetManualGain(isOut, nChanNo, *(long long *)pParam);

		case AD93xx_FIRFILTER:
			return LoadFirFilter((char *)pParam);

		case AD93xx_FIRFILTERENABLE:
			return FirFilterEnable(*(long long *)pParam);

		default:
			return -1;
	}

	return 0;
}

// Получить параметр
int AD9361_Device_Impl::GetParam(int isOut, int nChanNo, int nParamID, void *pParam)
{
	switch(nParamID)
	{
		// Получение центральной полосы
		case AD93xx_CENTERFREQ:
			return GetCenterFreq(isOut, (long long *)pParam);

		// Получение частоты дискретизации
		case AD93xx_SAMPLERATE:
			return GetSampleRate(isOut, (long long *)pParam);

		// Получение полосы
		case AD93xx_BANDWIDTH:
			return GetBandwidth(isOut, (long long *)pParam);

		case AD93xx_PORTSELECT:
			return GetPortSelect(isOut, (char*)pParam);

		case AD93xx_GAINMODE:
			return GetGainMode(isOut, nChanNo, (char*)pParam);

		case AD93xx_MANUALGAIN:
			return GetManualGain(isOut, nChanNo, (long long *)pParam);

		case AD93xx_FIRFILTERENABLE:
			return IsFirFilterEnable(isOut, (long long *)pParam);

		default:
			return -1;
	}

	return 0;
}

// Создать буфер
int AD9361_Device_Impl::CreateBuffer(int isOut, int nSamplesCount, int isCyclic, void **ppData)
{
	if(m_apBuffer[isOut])
		iio_buffer_destroy(m_apBuffer[isOut]);
	
	m_apBuffer[isOut] = iio_device_create_buffer(m_apStreamDev[isOut], nSamplesCount, isCyclic);

	if(m_apBuffer[isOut] == 0)
		return -1;

	*ppData = iio_buffer_start(m_apBuffer[isOut]);

	return 0;
}

// Освободить буфер
void AD9361_Device_Impl::DestroyBuffer(int isOut)
{
	if(m_apBuffer[isOut])
	{
		iio_buffer_destroy(m_apBuffer[isOut]);
		m_apBuffer[isOut] = 0;
	}
}

// Получить данные
int AD9361_Device_Impl::RecvData()
{
	int nRecvCnt = 0;

	if(m_apBuffer[RX] == 0)
		return nRecvCnt;

	nRecvCnt = iio_buffer_refill(m_apBuffer[RX]);

	return nRecvCnt;
}

// Передать данные
int AD9361_Device_Impl::SendData()
{
	int nSendCnt = 0;

	if(m_apBuffer[TX] == 0)
		return nSendCnt;

	nSendCnt = iio_buffer_push(m_apBuffer[TX]);

	return nSendCnt;
}

// Установка центральной частоты
int AD9361_Device_Impl::SetCenterFreq(int isOut, long long nCenterFreq)
{
	return iio_channel_attr_write_longlong(m_apLOChan[isOut], "frequency", nCenterFreq);
}

// Получение центральной частоты
int AD9361_Device_Impl::GetCenterFreq(int isOut, long long *pnCenterFreq)
{
	return iio_channel_attr_read_longlong(m_apLOChan[isOut], "frequency", pnCenterFreq);
}

// Установка частоты дискретизации
int AD9361_Device_Impl::SetSampleRate(int isOut, long long nSampleRate)
{
	return ad9361_set_bb_rate(m_pPhyDev, nSampleRate);
	//return iio_channel_attr_write_longlong(m_apPhyChan[isOut][0], "sampling_frequency", nSampleRate);
}

// Получение частоты дискретизации
int AD9361_Device_Impl::GetSampleRate(int isOut, long long *pnSampleRate)
{
	return iio_channel_attr_read_longlong(m_apPhyChan[isOut][0], "sampling_frequency", pnSampleRate);
}

// Установка полосы
int AD9361_Device_Impl::SetBandwidth(int isOut, long long nBandwidth)
{
	return iio_channel_attr_write_longlong(m_apPhyChan[isOut][0], "rf_bandwidth", nBandwidth);
}

// Получение полосы
int AD9361_Device_Impl::GetBandwidth(int isOut, long long *pnBandwidth)
{
	return iio_channel_attr_read_longlong(m_apPhyChan[isOut][0], "rf_bandwidth", pnBandwidth);
}

int AD9361_Device_Impl::SetPortSelect(int isOut, char *pPortSelect)
{
	return iio_channel_attr_write(m_apPhyChan[isOut][0], "rf_port_select", pPortSelect);
}

int AD9361_Device_Impl::GetPortSelect(int isOut, char *pPortSelect)
{
	return iio_channel_attr_read(m_apPhyChan[isOut][0], "rf_port_select", pPortSelect, 32);
}

int AD9361_Device_Impl::SetGainMode(int isOut, int nChanNo, char *pGainMode)
{
	return iio_channel_attr_write(m_apPhyChan[isOut][nChanNo], "gain_control_mode", pGainMode);
}

int AD9361_Device_Impl::GetGainMode(int isOut, int nChanNo, char *pGainMode)
{
	return iio_channel_attr_read(m_apPhyChan[isOut][nChanNo], "gain_control_mode", pGainMode, 32);
}

int AD9361_Device_Impl::SetManualGain(int isOut, int nChanNo, long long nManualGain)
{
	return iio_channel_attr_write_longlong(m_apPhyChan[isOut][nChanNo], "hardwaregain", nManualGain);
}

int AD9361_Device_Impl::GetManualGain(int isOut, int nChanNo, long long *pnManualGain)
{
	return iio_channel_attr_read_longlong(m_apPhyChan[isOut][nChanNo], "hardwaregain", pnManualGain);
}

int AD9361_Device_Impl::LoadFirFilter(char *pBuf)
{
	std::string filter(pBuf);

	if(filter.empty() || !iio_device_find_attr(m_pPhyDev, "filter_fir_config"))
		return -1;

	std::ifstream ifs(filter.c_str(), std::ifstream::binary);
	
	if(!ifs)
		return -1;

	/* Here, we verify that the filter file contains data for both RX+TX. */
	{
		char buf[256];

		do 
		{
			ifs.getline(buf, sizeof(buf));
		} while(!(buf[0] == '-' || (buf[0] >= '0' && buf[0] <= '9')));

		std::string line(buf);

		if(line.find(',') == std::string::npos)
			throw std::runtime_error("Incompatible filter file");
	}

	ifs.seekg(0, ifs.end);
	int length = ifs.tellg();
	ifs.seekg(0, ifs.beg);

	char *buffer = new char[length];

	ifs.read(buffer, length);
	ifs.close();
	
	FirFilterEnable(0);

	int ret = iio_device_attr_write_raw(m_pPhyDev, "filter_fir_config", buffer, length);

	delete[] buffer;

	if(ret <= 0)
		return -1;

	return 0;
}

int AD9361_Device_Impl::FirFilterEnable(long long isEnable)
{
	return ad9361_set_trx_fir_enable(m_pPhyDev, isEnable);
}

int AD9361_Device_Impl::IsFirFilterEnable(int isOut, long long *pEnable)
{
	return iio_channel_attr_read_longlong(m_apPhyChan[isOut][0], "filter_fir_en", pEnable);
}