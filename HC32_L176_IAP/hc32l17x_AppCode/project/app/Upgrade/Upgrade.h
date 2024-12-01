#ifndef __UPGRADE_H__
#define __UPGRADE_H__

#define Upgrade_Request_CMD			1120					//设备通过TCP接口请求获取升级包信息
#define Upgrade_RequestInfo_CMD     1220					//当平台验证通过可升级时，包括升级包信息
#define Upgrade_Getpack_CMD 		1121				  //设备通过TCP接口请求下载升级包
#define Upgrade_GetpackInfo_CMD     1221  		            //该消息升级包内容，为二进制消息 
#define Upgrade_GetOver_CMD			1122					//设备通过TCP接口上报升级结果
#define Upgrade_GetOverInfo_CMD	    1222					//云平台认证升级结果

#define Upgrade_GetIp				    0
#define Upgrade_DisMQTT				    1
#define Upgrade_TcpConnect				2
#define Upgrade_GetRequest				3
#define Upgrade_Repack				    4
#define Upgrade_Over				    5
#define Upgrade_Failure				    6
extern unsigned int Crc16(unsigned char *puchMsg, unsigned int usDataLen);
extern volatile uint8_t Upgrade_STATE;
extern  void Upgrade_satus(void);
extern uint8_t Send_satus(uint16_t Mod);
extern uint8_t upgradeIp[50];
extern uint8_t upgradePort[5];
extern uint8_t uretry_Counter;
extern uint8_t Crc_check(void);
extern uint8_t Flashwrite_check(void);
extern uint8_t XOR_check(uint8_t* pBuffer,uint16_t NumByte);

//GSM模块初始化状态常量
typedef enum
{
	Add_Upgrade_Device_Num = 2,						//设备序列号起始地址
	Add_Upgrade_CMD_H = 9,								//消息命令码起始地址
	Add_Upgrade_CMD_L = 10,								//消息命令码起始地址
	Add_Upgrade_Data = 11,								//数据内容起始地址
}Upgrade_Dtata_Add;






#endif //__UPGRADE_H__
