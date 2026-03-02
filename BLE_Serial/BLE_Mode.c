    if (Serial_RxFlag == 1)
    {
      OLED_ShowString(0, 0, "                       ", OLED_6X8);
      OLED_ShowString(0, 0, Serial_RxPacket, OLED_6X8);
      OLED_Update();

      // strtok strcmp atoi/atof

      char *Tag = strtok(Serial_RxPacket, ",");
      if (strcmp(Tag, "key") == 0)
      {
        char *Name = strtok(NULL, ",");
        char *Action = strtok(NULL, ",");

        if (strcmp(Name, "1") == 0 && strcmp(Action, "up") == 0)
        {
          BLE_Serial_Printf("key,1,up\r\n");
        }
        else if (strcmp(Name, "2") == 0 && strcmp(Action, "down") == 0)
        {
          BLE_Serial_Printf("key,2,down\r\n");
        }
      }
      else if (strcmp(Tag, "slider") == 0)
      {
        char *Name = strtok(NULL, ",");
        char *Value = strtok(NULL, ",");

        if (strcmp(Name, "1") == 0)
        {
          uint8_t IntValue = atoi(Value);

          BLE_Serial_Printf("slider,1,%d\r\n", IntValue);
        }
        else if (strcmp(Name, "2") == 0)
        {
          float FloatValue = atof(Value);

          BLE_Serial_Printf("slider,2,%f\r\n", FloatValue);
        }
      }
      else if (strcmp(Tag, "joystick") == 0)
      {
        int8_t LH = atoi(strtok(NULL, ","));
        int8_t LV = atoi(strtok(NULL, ","));
        int8_t RH = atoi(strtok(NULL, ","));
        int8_t RV = atoi(strtok(NULL, ","));

        BLE_Serial_Printf("joystick,%d,%d,%d,%d\r\n", LH, LV, RH, RV);
      }

      Serial_RxFlag = 0;
    }
    
