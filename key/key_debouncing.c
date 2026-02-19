uint8_t debounceCounterPB1 = 0;         // 定义PB1消抖计数器
uint8_t lastPinStatePB1 = GPIO_PIN_SET; // 定义PB1上次按键状态

uint8_t debounceCounterPB11 = 0;         // 定义PB11消抖计数器
uint8_t lastPinStatePB11 = GPIO_PIN_SET; // 定义PB11上次按键状态

void checkButtons(void)
{
  uint8_t currentPinStatePB1 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);
  uint8_t currentPinStatePB11 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11);

  // 处理PB1按键
  if (currentPinStatePB1 == GPIO_PIN_RESET)
  {
    if (lastPinStatePB1 == GPIO_PIN_RESET)
    {
      debounceCounterPB1++;
      if (debounceCounterPB1 >= 10) // 使用固定的阈值10
      {
        // 当按键状态为低电平的时间超过阈值，认为按键被稳定按下
        Target_Speed += 10;
        debounceCounterPB1 = 0; // 按键处理后重置计数器
      }
    }
    else
    {
      debounceCounterPB1 = 1; // 如果上次是高电平，这次是低电平，计数器设为1
    }
  }
  else
  {
    debounceCounterPB1 = 0; // 如果按键状态不是低电平，重置计数器
  }

  // 处理PB11按键
  if (currentPinStatePB11 == GPIO_PIN_RESET)
  {
    if (lastPinStatePB11 == GPIO_PIN_RESET)
    {
      debounceCounterPB11++;
      if (debounceCounterPB11 >= 10) // 使用固定的阈值10
      {
        // 当按键状态为低电平的时间超过阈值，认为按键被稳定按下
        Target_Speed -= 10;
        debounceCounterPB11 = 0; // 按键处理后重置计数器
      }
    }
    else
    {
      debounceCounterPB11 = 1; // 如果上次是高电平，这次是低电平，计数器设为1
    }
  }
  else
  {
    debounceCounterPB11 = 0; // 如果按键状态不是低电平，重置计数器
  }

  lastPinStatePB1 = currentPinStatePB1;   // 更新PB1上次按键状态
  lastPinStatePB11 = currentPinStatePB11; // 更新PB11上次按键状态
}
