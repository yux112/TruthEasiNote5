# 希沃“真”白板
## 项目简介
注意：这不是希沃的产品，仅仅是将非希沃一体机上的希沃白板5的启动过程更改得如同希沃品牌一体机一样
## 功能
- [x] 修补希沃白板5的配置文件，将其启动过程更改为希沃一体机的启动过程
- [x] 使用命名管道将希沃有关进程结束
- [x] 启动希沃白板5的服务
- [x] 启动希沃白板5
## 依赖
- Windows 7及以上
- Visual Studio 2019
## 使用
### 自行编译
1. 使用Visual Studio 2019打开项目
3. 选择Release x64
2. 编译
4. 将项目输出的可执行文件放到你喜欢的位置
5. 使用nssm将Piep_Server设置为系统服务
6. 为Piep_Client创建快捷方式
### 使用预编译的版本
1. 下载Release中的安装文件
2. 安装
## 日志
服务端会将日志输出到`运行目录\Piep_Server.log`
## 错误处理
服务器在遇到错误时会输出错误信息到日志文件并退出程序。
## 许可证
该项目使用 MIT 许可证。详细信息请参阅 LICENSE 文件
## 完成

![image](Tianyi.jpg)
