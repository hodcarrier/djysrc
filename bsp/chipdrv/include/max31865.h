// =============================================================================

// 文件名     ：max31865.h
// 模块描述: 温度检测芯片max31865的驱动
// 模块版本: V1.00
// 创建人员:
// 创建时间: 2/9.2017
// =============================================================================

#ifndef _CHIP_MAX31865_H_
#define _CHIP_MAX31865_H_

#ifdef __cplusplus
extern "C" {
#endif

#define MAX31865_CONTINOUS_MODE      0      //连续采样模式
#define MAX31865_SINGLE_MODE         1      //单次触发采样模式
// =============================================================================
//功能：初始化Max31865芯片，若采用连续读模式，则配置其模式为连续采样
//参数：参数：
//     BusName,总结名称，如“SPI1”
//     Mode，连续模式或单次采样模式
//返回：true = 成功初始化，false = 初始化失败
// =============================================================================
bool_t ModuleInstall_Max31865(char *BusName,u8 SampleMode);
// =============================================================================
// 函数功能：读温度(注意：函数类型为float，必须声名，否则返回值出错)
// 输入参数：无
// 返回值  ：温度值，为float类型；-1，表示出错
// =============================================================================
float Max31865_TemperatureGet(void);

#ifdef __cplusplus
}
#endif

#endif





