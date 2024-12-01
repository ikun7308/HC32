#include "include.h"

typedef  void (*IRQ_Handler)(void);
/* CRC 高位字节值表 */
const  unsigned int auchCRCHi[256] = {
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

/* CRC低位字节值表*/
const  unsigned int auchCRCLo[256] = {
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
	0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
	0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
	0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
	0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
	0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
	0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
	0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
	0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
	0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
	0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
	0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
	0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
	0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
	0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
	0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
	0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
	0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
	0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
	0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
	0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
	0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};
uint8_t Upgrade_Sendbuf[21];//
volatile uint8_t Upgrade_STATE = 0;
uint8_t upgradeAdd[52]={0};
uint8_t upgradeIp[50];
uint8_t upgradePort[5];
uint8_t Header = 0;
uint8_t CRC_error = 0;
uint8_t oVer[12];
uint8_t nVer[12];
uint16_t Up_Timeout = 0;
uint16_t Packnum = 0;
uint16_t Packnum_Max = 0;
uint8_t uretry_Counter=0;
//==============================================================================
//函数名：
//函数功能说明：
//外部输入变量说明：
//返回值说明：
//内部变量说明：
//变量关系说明：
//调用位置：
//调试记录：
//==============================================================================
unsigned int Crc16(unsigned char *puchMsg, unsigned int usDataLen)
{
	unsigned int uchCRCHi = 0xFF ;												//*  高CRC字节初始化  */
	unsigned int uchCRCLo = 0xFF ;												//*   低CRC 字节初始化 */
	unsigned int temp16;
	unsigned int uIndex ;													    //* CRC循环中的索引  */
	while (usDataLen--)														    //* 传输消息缓冲区   */
	{
		temp16 = *puchMsg++;
		uIndex = uchCRCHi ^ temp16 ;								            //* 计算CRC		  */
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
		uchCRCLo = auchCRCLo[uIndex] ;
	}
	return (uchCRCHi << 8 | uchCRCLo) ;
}
//==============================================================================
//函数名：
//函数功能说明：
//外部输入变量说明：
//返回值说明：
//内部变量说明：
//变量关系说明：
//调用位置：
//调试记录：
//==============================================================================
void Upgrade_satus(void)
{
//	uint16_t i=0;
//	uint8_t ReturnValue=0;
//	static uint8_t STATE=0;
//	static uint8_t buf_len=0;
//	static uint16_t returntime=0;

//	switch(Upgrade_STATE)
//	{
//		
//		case Upgrade_GetIp:
//		{
//				Packnum = 0;		
//				get_arrayformdata((uint8_t*)"oVer",4,oVer);
//				//get_arrayformdata((uint8_t*)"nVer",4,nVer);
//				get_arrayformdata((uint8_t*)"upgradeIp",9,upgradeIp);
//				get_arrayformdata((uint8_t*)"upgradeProt",11,upgradePort);
//				Combinationof_TCPSERARRAY(upgradeAdd,upgradeIp,upgradePort);
//				
//				//判断旧软件版本号是否一致
//				for(i=0;i<=11;i++)																						
//				{
//					if(oVer[i]!=Soft_Version[i+1])
//						i=20;
//				}
//				
//				if(i<20)
//				{
//					Upgrade_STATE = Upgrade_DisMQTT;
//				}
//				else  						 //无法升级，MQTT回到空闲状态
//				{
//					Upgrade_STATE = Upgrade_Failure;
//				}
//				break;
//		}
//		
//		case Upgrade_DisMQTT:   //断开MQTT连接
//		{
//			ReturnValue = GSM_AT_COMM(50000, GSM_AT_CIPCLOSE,RxData_CLOSE);//断开MQTT
//			if (ReturnValue == 1)
//			{
//				ReturnValue = 0;
//				Upgrade_STATE = Upgrade_TcpConnect;
//				retry_Counter = 0;
//			}
//			else if ((ReturnValue == 0) || (ReturnValue == 3))
//			{
//				if ( retry_Counter++ >= 3)
//				{
//					//上报升级失败，设备重启	
//					retry_Counter = 0;
//					Upgrade_STATE = Upgrade_Failure;//进入升级失败处理				
//				}
//				
//			}			
//			break;
//		}
//		
//		case Upgrade_TcpConnect:
//		{
//			ReturnValue = GSM_AT_COMM(5000,upgradeAdd,RxData_CONNECT);
//			if(ReturnValue==1)
//			{
//                if(Wrong_Num_f.bits.Networking3wrong_f==1)
//                {
//                    Wrong_Num_f.bits.Networking3wrong_f=0;
//                }
//				ReturnValue = 0;
//				Upgrade_STATE = Upgrade_GetRequest;
//				retry_Counter = 0;
//				STATE=0;
//			}
//			else if ((ReturnValue == 0) || (ReturnValue == 3))
//			{
//				if ( retry_Counter++ >= 3)
//				{
//					//上报升级失败，设备重启，重新连接
//                    if(Wrong_Num_f.bits.Networking3wrong_f==0)
//                    {
//                        Wrong_Num_f.bits.Networking3wrong_f=1;
//                    }
//					retry_Counter = 0;
//					Upgrade_STATE = Upgrade_Failure;//进入升级失败处理
//				}
//			}
//			break;
//		}
//		
//		case Upgrade_GetRequest://发送获取升级包请求
//		{
//           switch(STATE)
//            {
//			
//			   case 0:
//					buf_len = Send_satus(Upgrade_Request_CMD);
//					//memcpy(MQTT_UPLOAD_send_buff,Upgrade_Sendbuf,buf_len);
//					//MQTT_UPLOAD_send_buff_len=buf_len;
//					STATE=1;                    
//				break;
//				case 1:
//					if((GSM_Send_Data(buf_len,Upgrade_Sendbuf)==1))
//					{
//						buf_len=0;
//						STATE=2;
//					}                          
//					break;              
//				
//					case 2:
//					 if(GSM_receive_finish==1)
//					{					
//						if(Crc_check()==1)
//						{
//							GSM_receive_finish=0;
//							if(((GSM_SERVER_rec_buf[Header+Add_Upgrade_CMD_H]<<8) | GSM_SERVER_rec_buf[Header+Add_Upgrade_CMD_L]) == Upgrade_RequestInfo_CMD)
//							{
//								if((GSM_SERVER_rec_buf[Header+Add_Upgrade_Data])== 0x00)   //是否允许升级
//								{
//									for(i=0;i<=11;i++)//判断硬件版本号是否一致
//									{
//										if(GSM_SERVER_rec_buf[Header+i+Add_Upgrade_Data+12]!=Hard_Version[i+1])
//											i=20;
//									}
//									
//									Packnum_Max = (GSM_SERVER_rec_buf[Header+Add_Upgrade_Data+40]<<8)|GSM_SERVER_rec_buf[Header+Add_Upgrade_Data+41];
//									if(Packnum_Max<=210)
//									{
//										if((GSM_SERVER_rec_buf[Header+Add_Upgrade_Device_Num+1]==Flash_Buff[Serial_Num1])&&(i<20)&&
//											(((GSM_SERVER_rec_buf[Header+Add_Upgrade_Data+36]<<24)|(GSM_SERVER_rec_buf[Header+Add_Upgrade_Data+37]<<16)|(GSM_SERVER_rec_buf[Header+Add_Upgrade_Data+38]<<8)|GSM_SERVER_rec_buf[Header+Add_Upgrade_Data+39])<=53760))                                   
//										{
//											Upgrade_STATE = Upgrade_Repack;								//校验成功，进入获取升级包模式
//											W25Q20_Sector_Eraser(Sector_3);	
//											W25Q20_Sector_Eraser(Sector_4);	
//											W25Q20_Sector_Eraser(Sector_5);	
//											W25Q20_Sector_Eraser(Sector_6);	
//											W25Q20_Sector_Eraser(Sector_7);	
//											W25Q20_Sector_Eraser(Sector_8);	
//											W25Q20_Sector_Eraser(Sector_9);	
//											W25Q20_Sector_Eraser(Sector_A);	
//											W25Q20_Sector_Eraser(Sector_B);	
//											W25Q20_Sector_Eraser(Sector_C);	
//											W25Q20_Sector_Eraser(Sector_D);
//											W25Q20_Sector_Eraser(Sector_E);
//											W25Q20_Sector_Eraser(Sector_F);
//											Upgrade_STATE=Upgrade_Repack;
//											uretry_Counter = 0;
//											Up_Timeout = 0;
//											STATE = 0;
//											break;                                    
//										}
//										else
//										{
//											STATE = 0;
//											if(uretry_Counter++>= 3)
//											{
//												//请求升级失败，设备重启，重新连接
//												uretry_Counter = 0;
//												Upgrade_STATE = Upgrade_Failure;                                      
//											}                                     
//											break;
//										}                                    
//									}
//									else
//									{
//										STATE = 0;
//										if(uretry_Counter++>= 3)
//										{
//											//请求升级失败，设备重启，重新连接
//											uretry_Counter = 0;
//											Upgrade_STATE = Upgrade_Failure;                                      
//										}                                     
//										break;
//									}
//								}
//								else 
//								{
//									
//									STATE = 0;                               
//									if(uretry_Counter++>= 3)
//									{
//										//请求升级失败，设备重启，重新连接
//										Upgrade_STATE = Upgrade_Failure;
//										uretry_Counter = 0;
//									} 
//									break;
//								}
//							}
//						}
//						if(returntime++>=5000) 		//CRC校验失败失败
//						{
//							//CRC校验失败，重新向平台请求数据                      
//							STATE = 0;
//							GSM_receive_finish=0;
//							returntime=0;
//							if ( uretry_Counter++ >= 3)	
//							{
//								Upgrade_STATE = Upgrade_Failure;
//								uretry_Counter = 0;
//							}
//							break;
//						}
//					}
//					if ( Up_Timeout++ >= 60000)//若超时未收到回复，重新请求,超过三次，回复升级失败，并重新连接MQTT服务器
//					{                  
//						Up_Timeout = 0;
//						STATE = 0;                                              
//						 if(uretry_Counter++>= 3)
//						{
//							//上报升级失败，设备重启，重新连接
//							Upgrade_STATE = Upgrade_Failure;
//							uretry_Counter = 0;

//						}                        
//						break;
//					}             
//					break;
//					
//					default:
//					break;
//				}
//			break;					
//			}
//			

//		case Upgrade_Repack:
//		{
//			switch(STATE)
//            {
//                case 0:
//                    buf_len = Send_satus(Upgrade_Getpack_CMD);
////                    memcpy(MQTT_UPLOAD_send_buff,Upgrade_Sendbuf,buf_len);
////                    MQTT_UPLOAD_send_buff_len=buf_len;                
//                    STATE=1;                    
//                break;
//                
//                case 1:
//                    if((GSM_Send_Data(buf_len,Upgrade_Sendbuf)==1))
//                    {
//                        buf_len=0;
//                        STATE=2;
//                    }                    
//                break;
//                case 2:
//                    Up_Timeout++;
//                    if(GSM_receive_finish==1)
//                    {
//                        Up_Timeout = 0;
//                        if(Crc_check()==1)
//                        {
//                            GSM_receive_finish=0;
//                            if((((GSM_SERVER_rec_buf[Header+Add_Upgrade_CMD_H]<<8) | GSM_SERVER_rec_buf[Header+Add_Upgrade_CMD_L])  == Upgrade_GetpackInfo_CMD)&&
//                                ((((GSM_SERVER_rec_buf[Header+Add_Upgrade_Data]<<8)|  GSM_SERVER_rec_buf[Header+Add_Upgrade_Data+1]))==Packnum)&&
//                                ((((GSM_SERVER_rec_buf[Header+Add_Upgrade_Data+2]<<8)+GSM_SERVER_rec_buf[Header+Add_Upgrade_Data+3]))==252))
//                                {
//                                if(Flashwrite_check() == 1)
//                                {
//                                    uretry_Counter = 0;//错误计数清零
//                                    Packnum ++;
//                                    if(Packnum >= Packnum_Max)
//                                    {
//                                        Upgrade_STATE = Upgrade_Over;//文件包接收结束                                     
//                                    }
//                                }
//                                else		//验证失败，重新获取程序包
//                                {
//                                    uretry_Counter ++;//错误计数加一
//                                    
//                                }
//                            }
//                            else		//验证失败，重新获取程序包	
//                            {
//                                uretry_Counter ++;//错误计数加一
//                            }
//                            STATE = 0;

//                        }
//                        else if(returntime++>=5000)//crc校验失败重新获取程序包
//                        {
//                            STATE = 0;
//                            GSM_receive_finish=0;
//                            returntime=0;
//                            uretry_Counter ++;//错误计数加一
//                        }
//                    }

//                    if(Up_Timeout >=50000) //响应超时，重新获取
//                    {
//                        STATE = 0;                      
//                        Up_Timeout = 0;
//                        uretry_Counter ++;
//                    }

//                    if ( uretry_Counter >= 3)//连续错误超过三次重启并上报
//                    {
//                        //上报升级失败，设备重启，重新连接
//                        uretry_Counter = 0;
//                        Upgrade_STATE = Upgrade_Failure;//进入升级失败处理
//                    }
//                 break;
//                    
//                
//                default:
//                    
//                break;
//            }			
//			break;
//		}			
//		case Upgrade_Over:
//		{
//			Flash_Buff[Upgrade] = 1;
//			W25Q20_Sector_Eraser(Sector_1);
//			W25Q20_Write(Flash_Buff,Sector_1,128);
//			DisableInterrupts();
//			//重启
//			MCU_SoftRest(); 
//			//Jump_to_boot_Reset_Handler();			
//			break;
//		}
//		case Upgrade_Failure:
//		{
//			Flash_Buff[Upgrade]=3;								//存入失败标志3
//			W25Q20_Sector_Eraser(Sector_1);
//			W25Q20_Write(Flash_Buff,Sector_1,128);
//			DisableInterrupts();
//			//重启
//			MCU_SoftRest(); 
//			Upgrade_STATE = Upgrade_GetIp;
//			break;
//		}
//		default:
//			Flash_Buff[Upgrade]=3;								//存入失败标志3
//			W25Q20_Sector_Eraser(Sector_1);
//			W25Q20_Write(Flash_Buff,Sector_1,128);
//			DisableInterrupts();
//			MCU_SoftRest(); 		
//			Upgrade_STATE = Upgrade_GetIp;            

//		break;
//	}
}
//==============================================================================
//函数名：
//函数功能说明：
//外部输入变量说明：
//返回值说明：
//内部变量说明：
//变量关系说明：
//调用位置：
//调试记录：
//==============================================================================
uint8_t Send_satus(uint16_t Mod)
{
	uint8_t buf_len=0;
	switch(Mod)
	{
		case Upgrade_Request_CMD:
		{
			Upgrade_Sendbuf[0] = 0;
			Upgrade_Sendbuf[1] = 19;
			Upgrade_Sendbuf[2] = 0x01;
			Upgrade_Sendbuf[3] = 0x55;
			Upgrade_Sendbuf[4] = 0x55;
			Upgrade_Sendbuf[5] = 'U';
			Upgrade_Sendbuf[6] = Flash_Buff[Serial_Num1];
			Upgrade_Sendbuf[7] = Flash_Buff[Serial_Num2];
			Upgrade_Sendbuf[8] = Flash_Buff[Serial_Num3];
			Upgrade_Sendbuf[9] = Flash_Buff[Serial_Num4];
			Upgrade_Sendbuf[10] = Flash_Buff[Serial_Num5];
			Upgrade_Sendbuf[11] = Flash_Buff[Serial_Num6];
			Upgrade_Sendbuf[12] = (Upgrade_Request_CMD>>8)&0x00ff;
			Upgrade_Sendbuf[13] = Upgrade_Request_CMD&0x00ff;	
			Upgrade_Sendbuf[14] = 0x00;
			Upgrade_Sendbuf[15] = 0x00;
			Upgrade_Sendbuf[16] = 0x00;
			Upgrade_Sendbuf[17] = 0xfc;
			Upgrade_Sendbuf[18] = Crc16(&Upgrade_Sendbuf[3],(Upgrade_Sendbuf[1]-4))/256;
			Upgrade_Sendbuf[19] = Crc16(&Upgrade_Sendbuf[3],(Upgrade_Sendbuf[1]-4))%256;
			Upgrade_Sendbuf[20] = 0xaa;	
			buf_len = Upgrade_Sendbuf[1]+2;
			break;
		}
		case Upgrade_Getpack_CMD:
		{
			Upgrade_Sendbuf[0] = 0;
			Upgrade_Sendbuf[1] = 17;
			Upgrade_Sendbuf[2] = 0x01;
			Upgrade_Sendbuf[3] = 0x55;
			Upgrade_Sendbuf[4] = 0x55;
			Upgrade_Sendbuf[5] = 'U';
			Upgrade_Sendbuf[6] = Flash_Buff[Serial_Num1];
			Upgrade_Sendbuf[7] = Flash_Buff[Serial_Num2];
			Upgrade_Sendbuf[8] = Flash_Buff[Serial_Num3];
			Upgrade_Sendbuf[9] = Flash_Buff[Serial_Num4];
			Upgrade_Sendbuf[10] = Flash_Buff[Serial_Num5];
			Upgrade_Sendbuf[11] = Flash_Buff[Serial_Num6];
			Upgrade_Sendbuf[12] = (Upgrade_Getpack_CMD>>8)&0x00ff;
			Upgrade_Sendbuf[13] = Upgrade_Getpack_CMD&0x00ff;	
			Upgrade_Sendbuf[14] = (Packnum>>8)&0x00ff;
			Upgrade_Sendbuf[15] = Packnum&0x00ff;
			Upgrade_Sendbuf[16] = Crc16(&Upgrade_Sendbuf[3],(Upgrade_Sendbuf[1]-4))/256;
			Upgrade_Sendbuf[17] = Crc16(&Upgrade_Sendbuf[3],(Upgrade_Sendbuf[1]-4))%256;
			Upgrade_Sendbuf[18] = 0xaa;
			buf_len = Upgrade_Sendbuf[1]+2;
			break;
		}
		case Upgrade_GetOver_CMD:
			
			break;
		default:
			break;
	}
	return buf_len;
}
//==============================================================================
//函数名：
//函数功能说明：
//外部输入变量说明：
//返回值说明：
//内部变量说明：
//变量关系说明：
//调用位置：
//调试记录：
//==============================================================================
uint8_t Crc_check(void)
{
	uint16_t i=0;
	uint16_t num,crc;
	Header = 0;
	for(i=0;i<=511;i++)
	{
		if((GSM_SERVER_rec_buf[i]==0x55)&&(GSM_SERVER_rec_buf[i+1] == 0x55)&&(GSM_SERVER_rec_buf[i+2] == 0x55))
		{
			num = ((GSM_SERVER_rec_buf[i-3] << 8) & 0xff00 ) |(GSM_SERVER_rec_buf[i-2] & 0x00ff); 
			if(GSM_SERVER_rec_buf[i+ num-2]==0xaa)
			{
				Header = i;
				crc = Crc16(&GSM_SERVER_rec_buf[Header], num-4);
				if(crc == ((GSM_SERVER_rec_buf[Header+num-4] << 8) | GSM_SERVER_rec_buf[Header+num-3]))
							return 1;
				else
							return 0;
				}
		}	
	}
	return 0;
}
//==============================================================================
//函数名：
//函数功能说明：
//外部输入变量说明：
//返回值说明：
//内部变量说明：
//变量关系说明：
//调用位置：
//调试记录：
//==============================================================================
uint8_t Flashwrite_check(void)
{
	uint8_t XOR_Value1=0,XOR_Value2=0,Value[252];
	uint32_t flash_add = 0;
	static uint8_t Flash_Write_count=0;
	flash_add  = Sector_4 + (Packnum <<8 );
	
	W25Q20_Write(&GSM_SERVER_rec_buf[Header+Add_Upgrade_Data+5],flash_add,252);
	W25Q20_Read(&GSM_SERVER_rec_buf[Header+Add_Upgrade_Data+5],flash_add,252);
	XOR_Value1 = GSM_SERVER_rec_buf[Header+Add_Upgrade_Data+4];
	XOR_Value2 = XOR_check(&GSM_SERVER_rec_buf[Header+Add_Upgrade_Data+5],252);

	flash_add = Sector_3 + (((Packnum+2)/255)<<8);
	if(Packnum/255== 0)
	{
		Flash_Write_count= 2 + Packnum;
	}
	else
	{
		Flash_Write_count = ((Packnum/255)*256 + (Packnum%255));
	}
	W25Q20_Read(&Value[0],flash_add,252);
	if(Packnum == 0)
	{
		Value[0] = Packnum_Max/256;
		Value[1] = Packnum_Max%256;
	}
	Value[Flash_Write_count] = GSM_SERVER_rec_buf[Header+Add_Upgrade_Data+4];
	W25Q20_Write(&Value[0],flash_add,252);
	W25Q20_Read(&Value[0],flash_add,252);
	
	if((XOR_Value1 == XOR_Value2)&&(Value[Flash_Write_count] == GSM_SERVER_rec_buf[Header+Add_Upgrade_Data+4]))
	{
		Flash_Write_count++;
		return 1;
	}
	else
		return 0;
}
//==============================================================================
//函数名：uint8_t XOR_check(uint8_t* pBuffer,uint16_t NumByte)
//函数功能说明：异或校验
//外部输入变量说明：
//返回值说明：
//内部变量说明：
//变量关系说明：
//调用位置：
//调试记录：
//==============================================================================
uint8_t XOR_check(uint8_t* pBuffer,uint16_t NumByte)
{
	uint16_t i;
	uint8_t XOR_Value;
	XOR_Value = pBuffer[0];
	for(i=1;i<NumByte;i++)
	{
		XOR_Value = XOR_Value ^ pBuffer[i];
	}
	return XOR_Value;
}
