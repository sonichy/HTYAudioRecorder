# Qt 海天鹰录音机
基于 Qt5 的录音软件。  
已编译好的 HTYAR 程序适用 64 位 Linux 系统 Qt5 环境，双击运行。  

![alt](preview.jpg)  

### 1.6版 增加解析分号分隔的网络媒体字符串到播放列表，遍历媒体信息，增加读取音频封面图片(Windows有效，Linux无效，Mac没试)。
![alt](music_cover.jpg)  
### 1.5版 增加显示错误信息，增加拖放打开文件。
### 1.4版 解决有视频无声音的问题。
### 1.3版 直播列表做到窗体里面，支持显隐，记忆显隐，根据媒体信息设置视频大小。
### 1.2版 增加截图和剧情连拍。
![alt](summary.jpg)  

### 1.1版 增加直播列表。

### 错误排除
* 没有权限 ：  
程序右键属性，勾选“允许以程序执行”。  
  
* 深度文件管理器直接运行程序无法载入直播列表：  
方法1：从终端启动。  
方法2：修改desktop文件中的路径，从desktop文件启动。  
  
* Project ERROR: Unknown module(s) in QT: multimedia  
sudo apt-get install qtmultimedia5-dev  

* Win7无法播放视频可以尝试安装 LAV Filters 解码解决。

* 快捷键失效：  
鼠标移动到窗口非视频区域激活，全屏下鼠标移动到进度条上激活。

### 有声音无视频:  
* 报va相关错误  
参考：https://bugreports.qt.io/browse/QTBUG-23761  
解决：sudo apt-get remove gstreamer1.0-vaapi  
感谢：liujianfeng@deepin.org
* 不报错  
sudo apt-get install gst123  
下列【新】软件包将被安装：  
gst123 gstreamer1.0-pulseaudio gstreamer1.0-x