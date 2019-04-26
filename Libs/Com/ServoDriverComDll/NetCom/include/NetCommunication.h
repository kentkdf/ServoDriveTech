//////////////////////////////////////////////////////////////////////////////////////////
//	summary				:	Communicaiton top level		 								//
//	file				:	NetCommunication.h											//
//	Description			:	use for pc and fpga dsp communicaiton						//
//	lib					:	none														//
//																						//
//======================================================================================//
//		programmer:		|	date:		|	Corporation:	|		copyright(C):		//
//--------------------------------------------------------------------------------------//
//		wang.bin(1420)  |	2016/1/20	|	googoltech		|		2016 - 2019			//
//--------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef		__GTSD_NET_COMMUNICATION__
#define		__GTSD_NET_COMMUNICATION__

#include "Net.h"

//////////////////////////////////////////////////////////////////////////

class CNetCom
{
public:
	CNetCom(void);
	~CNetCom(void);

public:
	CNet* p_Net;											//定义底层NET指针，用于调用底层函数

	/*******************************************************************************************
	功能：		从扫描到的设备中找到有响应的设备并打开该设备，找到一个有响应的设备后就退出
	输入：		无
	返回：		0成功，其他参看错误列表。
	*******************************************************************************************/
	int16 NetCom_Open(void(*tpfUpdataProgressPt)(void*, int16*), void* ptrv, int16& progress);
	/*******************************************************************************************
	功能：关闭扫描到的有响应的设备
	输入：   无
	返回：   无
	*******************************************************************************************/
	int16 NetCom_Close();

	/*******************************************************************************************
	功能：FPGA数据通信交互函数
	输入：
	mode:			1:写  0：读
	dma_addr:		dma地址
	pData:			输入数据
	dma_num:		输入数据长度
	输出：pData:    输出数据
	dma_num:		输出数据长度
	返回：0成功，其他参看错误列表。
	*******************************************************************************************/
	int16 NetCom_FPGA_ComHandler(int16 mode, int16 dma_addr, int16* pData, int16 dma_num);

	/*******************************************************************************************
	功能：dsp数据通信交互函数
	输入：
	mode:			1:写  0：读
	dma_addr:		dma地址
	pData:          输入数据
	dma_num:		输入数据长度
	输出：pData:    输出数据
	dma_num:		输出数据长度
	返回：0成功，其他参看错误列表。
	*******************************************************************************************/
	int16 NetCom_DSP_ComHandler(int16 mode, int16 dma_addr, int16* pData, int16 dma_num);
private:
	int16 NC_GetDevs(int8* devsList, int32& btyeSize);
	int16 NC_GetAdapterDesp(vector<wstring> &list, vector<wstring> &nameList);
	int16 NC_Open(const int8* adapter, int32 port);
	int16 NC_Close(void);
	bool  ReturnFlaghandler(int16 FpgaMode, int16 cmd, int16 addr);
};

#endif