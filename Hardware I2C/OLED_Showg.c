 	OLED_ShowString(0, 0, "ID:", OLED_6X8);  // 显示静态字符串
  	ID = MPU6050_GetID();                    // 获取MPU6050的ID号
  	OLED_ShowHexNum(32, 0, ID, 2, OLED_6X8); // OLED显示ID号
  	OLED_Update();
    MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ); // 获取MPU6050的数据
    OLED_ShowString(0, 16, "AX:", OLED_6X8);
    OLED_ShowSignedNum(18, 16, AX, 5, OLED_6X8); // OLED显示数据
    OLED_ShowString(0, 32, "AY:", OLED_6X8);
    OLED_ShowSignedNum(18, 32, AY, 5, OLED_6X8);
    OLED_ShowString(0, 48, "AZ:", OLED_6X8);
    OLED_ShowSignedNum(18, 48, AZ, 5, OLED_6X8);
    OLED_ShowString(62, 16, "GX:", OLED_6X8);
    OLED_ShowSignedNum(78, 16, GX, 5, OLED_6X8);
    OLED_ShowString(62, 32, "GY:", OLED_6X8);
    OLED_ShowSignedNum(78, 32, GY, 5, OLED_6X8);
    OLED_ShowString(62, 48, "GZ:", OLED_6X8);
    OLED_ShowSignedNum(78, 48, GZ, 5, OLED_6X8);
    OLED_Update();
