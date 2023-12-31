# air--detect
由于隐去了部分项目中的信息，因此，本仓库中的文件是不完整的。
# 项目介绍
系统采用模块化设计，采集器利用无线串口模块实现组网，通过传感器获取到数据后上传到协调器，协调器（服务端）将节点的数据汇集后使用串口屏进行显示和交互，客户端（PC终端）以网络通信方式获取设备信息。  
**系统总体框架如下**：

![image](https://github.com/lainghaonan/air--detect/blob/master/img/%E7%B3%BB%E7%BB%9F%E6%95%B4%E4%BD%93%E6%A1%86%E6%9E%B6.png)

## 1、采集器与协调器
这两部分是stm32实现的，硬件比较简单没有上传，只上传了  /github-air-detect/项目-项目调试说明书/项目-BOM表，  

**采集器** 的传感器用的是龙戈电子的六合一传感器模块，/github-air-detect/项目-项目调试说明书/传感器模块说明书，通过modbus-rtu协议采集传感器模块数据后上传协调器，自组网部分是利用无线串口模块HC-12实现的  

**协调器** 使用迪文串口屏进行节点数据显示与交互，在 /github-air-detect/项目-DGUS/DWIN_SET 目录下可以看到大致的界面

## 2、网络通信方面：  
协调器采用lwip协议与客户端（PC）连接，客户端通过UDP广播查询同一网络下可用设备，与协调器建立UDP连接后，获取设备的可用服务信息；
再以TCP协议的方式使用服务，如获取传感器信息、操作设备等；客户端与服务端以字符串的方式交互，命令与参数使用空格分割。  

  
**服务端（协调器）与客户端（PC终端）通信流程如下**：

![image](https://github.com/lainghaonan/air--detect/blob/master/img/%E7%B3%BB%E7%BB%9F%E7%BD%91%E7%BB%9C%E9%80%9A%E4%BF%A1%E6%B5%81%E7%A8%8B.png)

## 3、网络通信框架
参考网上资料，初步实现了客户端与服务端网络通信框架，包括以下模块： **消息封装Message、协议解析器MParser、TCP通信实体TcpClient、TcpServer、UDP通信实体UdpPoint** ，  
完成框架到协调器与PC的移植，并实现 **服务端（协调器）** 与 **客户端（PC终端）** 通信。 

  
**网络通信框架如下**：  

![image](https://github.com/lainghaonan/air--detect/blob/master/img/%E7%BD%91%E7%BB%9C%E9%80%9A%E4%BF%A1%E6%A1%86%E6%9E%B6.png)
