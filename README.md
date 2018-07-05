### COPYRIGHT
THE INFORMATION CONTAINED HERE IS PROPRIETARY TECHNICAL 
INFORMATION OF FOHEART CO., LTD. TRANSMITTING, REPRODUCTION, 
DISSEMINATION AND EDITING OF THIS DOCUMENT AS WELL AS 
UTILIZATION OF THE CONTENT ARE FORBIDDEN WITHOUT PERMISSION. 
OFFENDERS WILL BE HELD LIABLE FOR PAYMENT OF DAMAGES. ALL 
RIGHTS ARE RESERVED IN THE EVENT OF A PATENT GRANT OR 
REGISTRATION OF A UTILITY MODEL OR DESIGN.
### GENERAL NOTES
FOHEART OFFERS THE INFORMATION AS A SERVICE TO ITS CUSTOMERS. 
THE INFORMATION PROVIDED IS BASED UPON CUSTOMERS’ 
REQUIREMENTS. FOHEART MAKES EVERY EFFORT TO ENSURE THE 
QUALITY OF THE INFORMATION IT MAKES AVAILABLE. FOHEART DOES 
NOT MAKE ANY WARRANTY AS TO THE INFORMATION CONTAINED 
HEREIN, AND DOES NOT ACCEPT ANY LIABILITY FOR ANY INJURY, LOSS 
OR DAMAGE OF ANY KIND INCURRED BY USE OF OR RELIANCE UPON 
THE INFORMATION. ALL INFORMATION SUPPLIED HEREIN IS SUBJECT TO 
CHANGE WITHOUT PRIOR NOTICE.<br>
**For technical support, please visit:**<br>
[http://www.foheart.com/support.html](http://www.foheart.com/support.html) or<br>
[http://www.foheart.com/question.html](http://www.foheart.com/question.html)<br>
**北京总部**<br>
Tel：(+86)010-56106165<br>
Email：contact@foheart.com<br>
地址：北京市海淀区黑山扈路红山口8号D2-南-3号<br>
邮编：100091<br>

<div align=center>
<img src="https://raw.githubusercontent.com/FOHEART/FOHEART_Unity3D_Plugin/master/help/img/address1.png"/>
</div>

**Copyright(C) FOHEART Co., Ltd. 2015-2018. All rights reserved.**<br>

# MotionMars Docs
惯性测量单元客户端软件帮助文档[【更新日志】](https://github.com/FOHEART/MotionMarsHelp/blob/master/releasenote.md)

<div align=center>
<img src="https://raw.githubusercontent.com/FOHEART/MotionMarsHelp/master/img/3dcube.png" width="100%"/>
</div>

## 1、客户端下载&驱动安装
客户端下载地址：<br>
[http://foheart.com/software/motionmars.html](http://foheart.com/software/motionmars.html)<br>
驱动文件路径为MotionMars安装文件夹下的**FOHEART MP Driver**文件夹。<br>
打开MotionMars\FOHEART MP Driver文件夹，根据自身电脑系统版本，选择安装x86或x64版本驱动，如下图所示：<br>

<div align=center>
<img src="https://raw.githubusercontent.com/FOHEART/MotionMarsHelp/master/img/driverfolder.png"/>
</div>

成功安装驱动：<br>

<div align=center>
<img src="https://raw.githubusercontent.com/FOHEART/MotionMarsHelp/master/img/driverinstallsuccess.png"/>
</div>

成功安装驱动后，断开MP1硬件开发板与PC的连接，重新插拔后即可出现正确识别后的设备。<br>

**注意**：MotionMars的**安装路径不可包含中文**，否则会造成驱动无法识别等错误。<br>
驱动安装成功后在计算机的设备管理器中会出现下图箭头所示设备：<br>

<div align=center>
<img src="https://raw.githubusercontent.com/FOHEART/MotionMarsHelp/master/img/deviceinpc.png"/>
</div>

## 2、设备检测
单击下图箭头所示的按钮，等待1秒左右会在设备列表中出现连接上的设备，如果未出现设备，请检查设备驱动是否正常安装。<br>
成功识别的设备如下图：

<div align=center>
<img src="https://raw.githubusercontent.com/FOHEART/MotionMarsHelp/master/img/detectdevice.png" width="100%"/>
</div>

单击左侧设备，以显示右侧绘图区域：

<div align=center>
<img src="https://raw.githubusercontent.com/FOHEART/MotionMarsHelp/master/img/clickdevice.png" width="100%"/>
</div>

右键单击设备，在弹出菜单中选择开始：

<div align=center>
<img src="https://raw.githubusercontent.com/FOHEART/MotionMarsHelp/master/img/start100hz.png" width="100%"/>
</div>

## 3、窗体显示

MotionMars共有5个窗体，分别为：<br>
1. 姿态原始数据，分别为加速度、角速度、磁场强度。
2. 姿态融合角，分别为四元数、欧拉角，线性加速度（尚未使用）。
3. 其它原始数据，分别为大气压强（尚未使用）、海拔高度（尚未使用）、温度（尚未使用）。
4. 3D显示，使用3D图形显示融合姿态角。
5. 磁场拟合，使用地磁场拟合数据，以获取无漂移的航向信息。

## 4、磁场校准

准备工作：将MotionMars节点远离磁干扰，例如手机、智能手表、铁、钴、镍以及它们组成的合金等磁体。<br>
在最后一个页面，点击**计算**按钮，开始进行新的磁场校准流程。<br>
将MotionMars节点沿XYZ三轴匀速旋转，尽量使获取到的磁场点在空间绘制出一个均匀的椭球体，这个过程大概需要60s。<br>
### [如何获得正确的磁场校准值](https://github.com/FOHEART/MotionMarsHelp/blob/master/HowToGetGoodMagCaliValue.md)
整个过程如下面四个图所示：<br>

<div align=center>
<img src="https://raw.githubusercontent.com/FOHEART/MotionMarsHelp/master/img/magcalibration1.png" width="100%"/>
</div>

<div align=center>
<img src="https://raw.githubusercontent.com/FOHEART/MotionMarsHelp/master/img/magcalibration2.png" width="100%"/>
</div>

<div align=center>
<img src="https://raw.githubusercontent.com/FOHEART/MotionMarsHelp/master/img/magcalibration3.png" width="100%"/>
</div>

**校准完成。**
<div align=center>
<img src="https://raw.githubusercontent.com/FOHEART/MotionMarsHelp/master/img/magcalibration4.png" width="100%"/>
</div>

磁场校准完成后，就可以得到无漂移的XYZ三轴角度。

## 5、数据录制

MotionMars可以同时录制保存多个节点的原始数据与解算数据。<br>
1. 选中设备之前的复选框，如下图中1号箭头所示。
2. 选中需要保存到的地址，如下图中2号箭头所示。
3. 点击录制按钮，如下图中3号箭头所示。
4. 录制的数据默认保存在安装目录下的**ExportData**文件夹中。

<div align=center>
<img src="https://raw.githubusercontent.com/FOHEART/MotionMarsHelp/master/img/clicksave.png" width="100%"/>
</div>

另外，在工具->选项中，还可以自定义导出数据的格式，例如名称规则、分隔符、占位符、包含数据、时间戳等。

<div align=center>
<img src="https://raw.githubusercontent.com/FOHEART/MotionMarsHelp/master/img/config.png" width="100%"/>
</div>

## 6、数据回放

在文件菜单中打开录制的文件，即可回放数据，拖动底部进度标尺可以按帧回放数据。

<div align=center>
<img src="https://raw.githubusercontent.com/FOHEART/MotionMarsHelp/master/img/replay.png" width="100%"/>
</div>

## 7、置零旋转
置零旋转的功能是将现在无论角度如何，强制归为0旋转，即四元数为（w,x,y,z）=(1.0, 0.0, 0.0, 0.0)。<br>
再次单击置零旋转之后，旋转恢复真实值。<br>
此功能的作用是将模块安装到载体之后，载体几乎不可能同时保持xyz三个方向旋转均为0，启动此功能之后，测量的就是实时值与初始值的相对旋转，方便于后期计算相对旋转量。

## 8、注意事项
1. 请勿用力弯折评估板以及核心板，外力会对电路板造成永久性损伤；即使是0.1度的弯折角度，也会对最终的解算角度造成高达每小时数十至上百度的积分误差。
2. 模块的安装需要与载体平面完全贴合，安装角度误差会对姿态测量造成不稳定的影响。
3. MotionMars的安装目录若在系统盘C盘，则需以**管理员模式**运行，否则无法执行数据录制的操作。

<div align=center>
<img src="https://raw.githubusercontent.com/FOHEART/FOHEART_Unity3D_Plugin/master/help/img/tail.png" width="100%"/>
</div>