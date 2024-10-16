#include "stm32f10x.h"                  // Device header

/*本AD转化用于温度传感器，温度传感器挂在A0口*/

void AD_Init(void)
{
	/**开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	/*ADC时钟分频*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN ;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);           //使用A0口作为温度传感器的输入端口
	
	/*配置ADC输入通道，设置采样时间*/
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_1Cycles5); 
	
	/*ADC初始化*/
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE ; //配置成单次转化模式
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  //数据对齐，align vi. 排列；排成一行
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  //不存在外部触发源，使用软件触发
	ADC_InitStructure.ADC_Mode =ADC_Mode_Independent ; //ADC单独转化
	ADC_InitStructure.ADC_NbrOfChannel = 1; //设置通道数目，该设置只有在扫描模式下才有效
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;  //配置成非扫描模式
	ADC_Init(ADC1,&ADC_InitStructure);
	
	/*ADC开始转化*/
	ADC_Cmd(ADC1,ENABLE); 
	
	/*ADC校准*/
	ADC_ResetCalibration(ADC1); //开始复位校准
	while(ADC_GetCalibrationStatus(ADC1)==SET); //等待复位校准完成
	ADC_StartCalibration(ADC1);//开始校准
	while(ADC_GetCalibrationStatus(ADC1)==SET); //等待复位校准完成
		
	//ADC_SoftwareStartConvCmd(ADC1,ENABLE); //连续转化模式下，需要这行代码
}

uint16_t  AD_GetValue(void)
{
	
	//单次转化模式 需要进行以下步骤
	ADC_SoftwareStartConvCmd(ADC1,ENABLE); //启动
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);//等待
	
	return ADC_GetConversionValue(ADC1);//读取
	
}



