#ifndef __UPGRADE_H__
#define __UPGRADE_H__

#define Upgrade_Request_CMD			1120					//�豸ͨ��TCP�ӿ������ȡ��������Ϣ
#define Upgrade_RequestInfo_CMD     1220					//��ƽ̨��֤ͨ��������ʱ��������������Ϣ
#define Upgrade_Getpack_CMD 		1121				  //�豸ͨ��TCP�ӿ���������������
#define Upgrade_GetpackInfo_CMD     1221  		            //����Ϣ���������ݣ�Ϊ��������Ϣ 
#define Upgrade_GetOver_CMD			1122					//�豸ͨ��TCP�ӿ��ϱ��������
#define Upgrade_GetOverInfo_CMD	    1222					//��ƽ̨��֤�������

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

//GSMģ���ʼ��״̬����
typedef enum
{
	Add_Upgrade_Device_Num = 2,						//�豸���к���ʼ��ַ
	Add_Upgrade_CMD_H = 9,								//��Ϣ��������ʼ��ַ
	Add_Upgrade_CMD_L = 10,								//��Ϣ��������ʼ��ַ
	Add_Upgrade_Data = 11,								//����������ʼ��ַ
}Upgrade_Dtata_Add;






#endif //__UPGRADE_H__