#ifndef _BSP_I2C_GPIO_H
#define _BSP_I2C_GPIO_H

#include "include.h"
#include <inttypes.h>

#define EEPROM_I2C_WR	0		/* д����bit */
#define EEPROM_I2C_RD	1		/* ������bit */


/* ����I2C�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����4�д��뼴������ı�SCL��SDA������ */
#define EEPROM_GPIO_PORT_I2C	GPIOA			/* GPIO�˿� */
#define EEPROM_RCC_I2C_PORT 	RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define EEPROM_I2C_SCL_PIN		GPIO_PIN_7			/* ���ӵ�SCLʱ���ߵ�GPIO */

#define EEPROM_I2C_SDA_PIN		GPIO_PIN_6			/* ���ӵ�SDA�����ߵ�GPIO */


/* �����дSCL��SDA�ĺ꣬�����Ӵ���Ŀ���ֲ�ԺͿ��Ķ��� */
#if 0	/* �������룺 1 ѡ��GPIO�Ŀ⺯��ʵ��IO��д */
	#define EEPROM_I2C_SCL_1()  GPIO_SetBits(EEPROM_GPIO_PORT_I2C, EEPROM_I2C_SCL_PIN)		/* SCL = 1 */
	#define EEPROM_I2C_SCL_0()  GPIO_ResetBits(EEPROM_GPIO_PORT_I2C, EEPROM_I2C_SCL_PIN)		/* SCL = 0 */
	
	#define EEPROM_I2C_SDA_1()  GPIO_SetBits(EEPROM_GPIO_PORT_I2C, EEPROM_I2C_SDA_PIN)		/* SDA = 1 */
	#define EEPROM_I2C_SDA_0()  GPIO_ResetBits(EEPROM_GPIO_PORT_I2C, EEPROM_I2C_SDA_PIN)		/* SDA = 0 */
	
	#define EEPROM_I2C_SDA_READ()  GPIO_ReadInputDataBit(EEPROM_GPIO_PORT_I2C, EEPROM_I2C_SDA_PIN)	/* ��SDA����״̬ */
#else	/* �����֧ѡ��ֱ�ӼĴ�������ʵ��IO��д */
    /*��ע�⣺����д������IAR��߼����Ż�ʱ���ᱻ�����������Ż� */
	#define EEPROM_I2C_SCL_1()  EEPROM_GPIO_PORT_I2C->BSRR = EEPROM_I2C_SCL_PIN				/* SCL = 1 */
	#define EEPROM_I2C_SCL_0()  EEPROM_GPIO_PORT_I2C->BRR = EEPROM_I2C_SCL_PIN				/* SCL = 0 */
	
	#define EEPROM_I2C_SDA_1()  EEPROM_GPIO_PORT_I2C->BSRR = EEPROM_I2C_SDA_PIN				/* SDA = 1 */
	#define EEPROM_I2C_SDA_0()  EEPROM_GPIO_PORT_I2C->BRR = EEPROM_I2C_SDA_PIN				/* SDA = 0 */
	
	#define EEPROM_I2C_SDA_READ()  ((EEPROM_GPIO_PORT_I2C->IDR & EEPROM_I2C_SDA_PIN) != 0)	/* ��SDA����״̬ */
#endif


extern uint16_t IIC_Read_Byte(u8 ack);
void i2c_Start(void); //CPU����I2C���������ź�
void i2c_Stop(void);  // CPU����I2C����ֹͣ�ź�
void i2c_SendByte(uint8_t _ucByte);//CPU��I2C�����豸����8bit����
uint8_t i2c_ReadByte(void);// CPU��I2C�����豸��ȡ8bit����
uint8_t i2c_WaitAck(void);//CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
void i2c_Ack(void);//CPU����һ��ACK�ź�
void i2c_NAck(void);//CPU����һ��NACK�ź�
uint8_t i2c_CheckDevice(uint8_t _Address);//���I2C�����豸��CPU�����豸��ַ��Ȼ���ȡ�豸Ӧ�����жϸ��豸�Ƿ����


#endif

