# APM32E103 ILI9341 TFT LCD 示例 / APM32E103 ILI9341 TFT LCD Example

[中文](#中文说明) | [English](#english)

## 中文说明

本项目演示 APM32E103 驱动 ILI9341 TFT LCD，读取 LCD ID、显示文本并循环切换背景颜色。

### 开发环境与演示内容

- 目标板：APM32E103 MINI
- 显示控制器：ILI9341
- 工程：Keil MDK-ARM
- 示例版本：V1.0.1
- 演示：硬件复位、背光控制、LCD 初始化、ID 显示、文字显示及 12 种颜色切换

### 使用方法

1. 根据 `lcd.h` 及板级文件连接数据、控制、复位和背光引脚。
2. 打开 `Examples/LCD/Project/MDK/LCD.uvprojx`。
3. 检查屏幕接口、电平及分辨率配置。
4. 编译并下载程序，观察 LCD ID、文本和颜色循环。

### 注意事项

不同 ILI9341 模组的接口方式、引脚定义、扫描方向和背光电路可能不同。如显示异常，请优先检查总线时序、复位流程、控制器 ID 和屏幕供电。

### 技术支持

更多资料请访问：[www.geehy.com](https://www.geehy.com/)。

---

## English

This project demonstrates an ILI9341 TFT LCD on the APM32E103. It reads the LCD ID, displays text, and cycles through background colors.

### Environment and demonstration

- Target board: APM32E103 MINI
- Display controller: ILI9341
- Project: Keil MDK-ARM
- Example version: V1.0.1
- Demonstrates hardware reset, backlight control, LCD initialization, ID/text display, and 12 background colors

### Usage

1. Connect the data, control, reset, and backlight signals according to `lcd.h` and the board files.
2. Open `Examples/LCD/Project/MDK/LCD.uvprojx`.
3. Verify the display interface, voltage levels, and resolution configuration.
4. Build and program the project, then observe the LCD ID, text, and color cycle.

### Notes

ILI9341 modules may differ in interface, pinout, scan direction, and backlight circuitry. If the display is incorrect, check bus timing, reset sequencing, controller ID, and display power first.

### Support

For more resources, visit [www.geehy.com](https://www.geehy.com/).
