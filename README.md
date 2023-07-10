# air--detect
由于隐去了部分项目中的信息，因此，本仓库中的文件是不完整的。
# 项目介绍
系统采用模块化设计，多节点lora采集器通过传感器获取到数据后上传到协调器，协调器将节点的数据汇集后使用串口屏进行显示和交互，客户端（PC）以网络通信方式获取设备信息。
网络通信方面：
协调器采用lwip协议与客户端（PC）连接，客户端通过UDP广播查询同一网络下可用设备，与协调器建立UDP连接后，获取设备的可用服务信息；
再以TCP协议的方式使用服务，如获取传感器信息、操作设备等；客户端与服务端以字符串的方式交互，命令与参数使用空格分割。
系统框架如下：

![image](https://github.com/lainghaonan/air--detect/blob/master/img/%E5%BE%AE%E4%BF%A1%E6%88%AA%E5%9B%BE_20230710130810.png)

协调器（服务端）与PC（客户端）通信框架如下：

![image](https://github.com/lainghaonan/air--detect/blob/master/img/%E5%BE%AE%E4%BF%A1%E6%88%AA%E5%9B%BE_20230710160848.png)

