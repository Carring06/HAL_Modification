#include "stm32f1xx_hal.h"
#include "tim.h"  // 包含定时器头文件，用于微秒延时

/**
 * 新增：HAL库微秒延时函数（基于TIM2实现，精度1us）
 * @param us 要延时的微秒数，范围0~65535
 */
void HAL_Delay_us(uint16_t us)
{
  __HAL_TIM_SET_COUNTER(&htim2, 0);  // 清空定时器计数器
  while (__HAL_TIM_GET_COUNTER(&htim2) < us);  // 等待计数器达到指定微秒数
}

/**
 * 函数功能：I2C总线SCL引脚电平设置
 * 输入参数：BitValue 输入需要写入SCL的电平，范围0~1
 * 返回值：无
 * 注意事项：当BitValue为0时，将SCL置为低电平，当BitValue为1时，将SCL置为高电平
 */
void MyI2C_W_SCL(uint8_t BitValue)
{
	// HAL库：根据BitValue设置SCL引脚电平
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, BitValue ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_Delay_us(10);	// 恢复原代码的10us微秒延时
}

/**
 * 函数功能：I2C总线SDA引脚电平设置
 * 输入参数：BitValue 输入需要写入SDA的电平，范围0~1
 * 返回值：无
 * 注意事项：当BitValue为0时，将SDA置为低电平，当BitValue为1时，将SDA置为高电平
 */
void MyI2C_W_SDA(uint8_t BitValue)
{
	// HAL库：根据BitValue设置SDA引脚电平
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, BitValue ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_Delay_us(10);	// 恢复原代码的10us微秒延时
}

/**
 * 函数功能：I2C读取SDA引脚电平
 * 输入参数：无
 * 返回值：当前SDA的电平，范围0~1，0为低电平，1为高电平
 */
uint8_t MyI2C_R_SDA(void)
{
	uint8_t BitValue;
	// HAL库：读取SDA引脚电平
	BitValue = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == GPIO_PIN_SET ? 1 : 0;
	HAL_Delay_us(10);	// 恢复原代码的10us微秒延时
	return BitValue;	// 返回SDA电平
}

/**
 * 函数功能：I2C初始化
 * 输入参数：无
 * 返回值：无
 * 注意事项：GPIO初始化由CubeMX自动生成，此处仅设置默认电平
 */
void MyI2C_Init(void)
{
	// 【CubeMX已自动生成以下代码，无需手动编写】
	// 1. GPIOB时钟自动使能
	// 2. PB10/PB11配置为开漏输出、上拉、50MHz速度
	
	// 仅保留默认电平设置，与原逻辑一致
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);	// SCL默认高电平
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);	// SDA默认高电平
}

/*上层协议*/

/**
 * 函数功能：I2C开始
 * 输入参数：无
 * 返回值：无
 */
void MyI2C_Start(void)
{
	MyI2C_W_SDA(1);							// 释放SDA，确保SDA为高电平
	MyI2C_W_SCL(1);							// 释放SCL，确保SCL为高电平
	MyI2C_W_SDA(0);							// 在SCL高电平期间，拉低SDA，产生起始信号
	MyI2C_W_SCL(0);							// 开始后把SCL也拉低，为了占用总线，也为了后续总线时序的节拍
}

/**
 * 函数功能：I2C结束
 * 输入参数：无
 * 返回值：无
 */
void MyI2C_Stop(void)
{
	MyI2C_W_SDA(0);							// 拉低SDA，确保SDA为低电平
	MyI2C_W_SCL(1);							// 释放SCL，使SCL恢复高电平
	MyI2C_W_SDA(1);							// 在SCL高电平期间，释放SDA，产生结束信号
}

/**
 * 函数功能：I2C发送一个字节
 * 输入参数：Byte 要发送的一个字节数据，范围：0x00~0xFF
 * 返回值：无
 */
void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i ++)				// 循环8次，主机依次发送数据的每一位
	{
		// 两个!可以对数据进行两次逻辑非，作用是把非0值统一转换为1，即：!!(0) = 0，!!(非0) = 1
		MyI2C_W_SDA(!!(Byte & (0x80 >> i)));// 使用掩码的方式取出Byte的指定一位数据并写入到SDA线
		MyI2C_W_SCL(1);						// 释放SCL，从机在SCL高电平期间读取SDA
		MyI2C_W_SCL(0);						// 拉低SCL，主机开始发送下一位数据
	}
}

/**
 * 函数功能：I2C接收一个字节
 * 输入参数：无
 * 返回值：接收到的一个字节数据，范围：0x00~0xFF
 */
uint8_t MyI2C_ReceiveByte(void)
{
	uint8_t i, Byte = 0x00;					// 定义接收的数据，并初始化为0x00，此处必须初始化为0x00，后续会用到
	MyI2C_W_SDA(1);							// 接收前，主机先确保释放SDA，避免干扰从机的数据发送
	for (i = 0; i < 8; i ++)				// 循环8次，主机依次接收从机的每一位
	{
		MyI2C_W_SCL(1);						// 释放SCL，主机在SCL高电平期间读取SDA
		if (MyI2C_R_SDA()){Byte |= (0x80 >> i);}	// 读取SDA数据，并存储到Byte变量
												// 当SDA为1时，修改变量指定位为1，当SDA为0时，不做处理，指定位为默认的初始值0
		MyI2C_W_SCL(0);						// 拉低SCL，从机在SCL低电平期间写入SDA
	}
	return Byte;							// 返回接收到的一个字节数据
}

/**
 * 函数功能：I2C发送应答位
 * 输入参数：AckBit 要发送的应答位，范围：0~1，0表示应答，1表示非应答
 * 返回值：无
 */
void MyI2C_SendAck(uint8_t AckBit)
{
	MyI2C_W_SDA(AckBit);					// 主机把应答位数据放到SDA线
	MyI2C_W_SCL(1);							// 释放SCL，从机在SCL高电平期间，读取应答位
	MyI2C_W_SCL(0);							// 拉低SCL，开始下一个时序节拍
}

/**
 * 函数功能：I2C接收应答位
 * 输入参数：无
 * 返回值：接收到的应答位，范围：0~1，0表示应答，1表示非应答
 */
uint8_t MyI2C_ReceiveAck(void)
{
	uint8_t AckBit;							// 定义应答位变量
	MyI2C_W_SDA(1);							// 接收前，主机先确保释放SDA，避免干扰从机的数据发送
	MyI2C_W_SCL(1);							// 释放SCL，主机在SCL高电平期间读取SDA
	AckBit = MyI2C_R_SDA();					// 将应答位存储到变量里
	MyI2C_W_SCL(0);							// 拉低SCL，开始下一个时序节拍
	return AckBit;							// 返回定义应答位变量
}

