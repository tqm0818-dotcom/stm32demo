OLED 项目说明

项目简介:
本项目基于 STM32F103 系列 MCU，驱动 SSD1306/类似 I2C 接口 OLED 显示屏。包含 OLED 控制、字体渲染和示例应用。

主要目录:
- `APP/`：应用入口与示例（`APP/app.c`）。
- `BSP/`：板级驱动，OLED 驱动与字体（`BSP/bsp_oled.c`、`BSP/bsp_oledfont.c`、`font.c`）。
- `Core/`：STM32 HAL 初始化与外设实现（`Core/Src/i2c.c`、`Core/Src/main.c`）。
- `SYSTEM/`：延时等系统辅助（`SYSTEM/delay.c`）。
- `MDK-ARM/`：Keil MDK 项目文件（`MDK-ARM/OLED.uvprojx`）与编译输出。

依赖与工具:
- STM32 HAL 库（已包含在 `Drivers/` 中）。
- Keil MDK（打开 `MDK-ARM/OLED.uvprojx` 编译）。
- 烧录工具：ST-Link 或兼容的调试器。

硬件连接（默认）:
本工程默认使用 I2C1（常见接线）：SCL -> PB6，SDA -> PB7（请根据实际硬件与 `Core/Src/i2c.c` 配置核对）。

快速使用指南:
1. 用 Keil 打开 `MDK-ARM/OLED.uvprojx` 并编译。
2. 通过 ST-Link 烧录或调试，重启 MCU 即可看到 OLED 示例显示。
3. 修改显示内容请编辑 `APP/app.c` 或 `BSP/bsp_oled.c` 中的显示函数与字体。

常见文件说明:
- `Core/Src/main.c`：系统启动与初始化流程。
- `Core/Src/i2c.c`：I2C 设备初始化与收发实现。
- `BSP/bsp_oled.c`：OLED 画面绘制与页面更新接口。
- `BSP/bsp_oledfont.c` / `font.c`：字体数据与字符绘制支持。

许可与作者:
请在使用或修改时保留原作者信息。如需帮助或协作，请联系项目维护者。
