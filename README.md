# [ASR1606](https://doc.soc.xin/ASR1606)

* [asrmicro](http://www.asrmicro.com): [Cortex-R5](https://github.com/SoCXin/Cortex)
* [L4R3](https://github.com/SoCXin/Level): 624 MHz

## [简介](https://github.com/SoCXin/ASR1606/wiki)

[ASR1606](http://www.asrmicro.com/proinfo/9.html#ajprodaline) 是021年推出的新一代LTE Cat.1 bis芯片 (第三代Cat.1芯片)。该芯片采用了更高集成度的单芯片SoC方案、先进成熟的22nm制程工艺并且集成了主频达到614MHz的ARM Cortex-R5处理器以及Modem通信单元、Codec音频单元、PSRAM+Flash存储单元和PMIC，使得芯片封装尺寸更小、性能更强大。

ASR1606芯片可广泛应用于各类标准数据模块，并且在Tracker、共享设备、电网、车联网及各种形式智能硬件等领域拥有出色表现。

### 关键参数

* 624MHz Cortex-R5 (22nm)
* 4/8 MB PSRAM + 4/8 MB QSPI Flash


## [资源收录](https://github.com/SoCXin)

* [参考资源](src/)
* [参考文档](docs/)
* [参考工程](project/)

## [选型建议](https://github.com/SoCXin/ASR1606)

Cat.4速率更高，上行最高50Mbps、下行最高150Mbps的，但在功耗、集成度、价格方面很难满足部分行业的物联网应用需求，而Cat.1兼顾制式、性能、功耗、成本等优势，尤其成本比Cat.4低近30%。

ASR1606的终端产品只要直接搭载翱捷自研的ASR5801 BT芯片，便可为该产品扩充蓝牙功能。对于有定位功能需求的终端产品，ASR1606能够与翱捷自研的ASR5311 GPS芯片进行整合设计，从而实现在单个模组上的集成。

[ASR1606](https://github.com/SoCXin/ASR1606) 同类Cat.1bis竞品包括：

* [EC618](https://github.com/SoCXin/EC618)
* [V8850](https://github.com/SoCXin/V8850)
* [UIS8910DM](https://github.com/SoCXin/UIS8910DM)

相关模组：

* GM196H模组
