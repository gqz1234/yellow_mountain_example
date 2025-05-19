#include "rtthread.h"
#include "bf0_hal.h"
#include "drv_io.h"
#include "stdio.h"
#include "string.h"
#include "board.h"
#define DBG_TAG "adc"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#include "bf0_sys_cfg.h"
#define ADC_DEV_NAME        "bat1"      /* ADC name */
#define BAT_CHANNEL 7       // BAT检测引脚ADC通道号
#define ADC_CHANNEL 6       // PA_34引脚ADC通道号

static rt_device_t s_adc_dev;
static rt_adc_cmd_read_arg_t read_arg;
void adc_example(void)
{
    rt_err_t r;

    /* find device */
    s_adc_dev = rt_device_find(ADC_DEV_NAME);
    /* set channel 0*/
    read_arg.channel = BAT_CHANNEL;
    r = rt_adc_enable((rt_adc_device_t)s_adc_dev, read_arg.channel);
    if (r != RT_EOK)
    {
        rt_kprintf("ADC enable failed\n");
        return;
    }
    // 这个接口会调用到sifli_get_adc_value函数,会进行默认22次平均
    rt_uint32_t value = rt_adc_read((rt_adc_device_t)s_adc_dev, BAT_CHANNEL);
    if (value == RT_ERROR)
    {
        rt_kprintf("ADC read failed\n");
        return;
    }
    rt_kprintf("VBAT read value: %d\n", value);                       // 打印PA_34读取的值
    rt_adc_disable((rt_adc_device_t)s_adc_dev, read_arg.channel);    // 禁用ADC通道
    HAL_PIN_Set_Analog(PAD_PA28, 1);                                 // 设置PA28为模拟输入模式
    read_arg.channel = ADC_CHANNEL;                                  // 设置ADC通道
    r = rt_adc_enable((rt_adc_device_t)s_adc_dev, read_arg.channel); // 使能ADC通道
    if (r != RT_EOK)
    {
        rt_kprintf("ADC enable failed\n");
        return;
    }
    // 这个接口会调用 sifli_adc_control 函数,只读取一次,用户可以自行对数据进行处理
    value = rt_device_control((rt_device_t)s_adc_dev, RT_ADC_CMD_READ, &read_arg.channel);
    if (value == RT_ERROR)
    {
        rt_kprintf("ADC read failed\n");
        return;
    }
    rt_kprintf("ADC read value: %d\n", read_arg.value); // 打印ADC读取的值

    rt_kprintf("ADC example end\n");
}
/**
  * @brief  Main program
  * @param  None
  * @retval 0 if success, otherwise failure number
  */
int main(void)
{
    rt_kprintf("Start adc demo!\n");
    while (1)
    {
        adc_example();
        rt_thread_mdelay(1000);
    }
    return RT_EOK;
}

/************************ (C) COPYRIGHT Sifli Technology *******END OF FILE****/

