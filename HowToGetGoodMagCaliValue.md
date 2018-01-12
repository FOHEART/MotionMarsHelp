# 如何获得正确的磁场校准值
## 1、远离干扰源
将MotionMars节点远离磁干扰，例如手机、智能手表、铁、钴、镍以及它们组成的合金等磁体。<br>
磁干扰不限于硬磁软磁等可见实体，大功率变电站、无线基站、空调机柜、或大功率电机等工作时周围数米之内都有极强的磁场干扰。
## 2、与载体同时校准
MotionMars可对其真实工作环境周围磁场进行测量，获得硬软磁的强度值，从而在后期姿态解算中将此硬软磁干扰去除，获得无干扰的姿态数据。
### 2.1载体含有磁性材料
由于在最终使用时，MotionMars节点测量的是它所依附载体的姿态，所以需要将MotionMars节点安装到载体后，将载体与MotionMars**同时转动**校准，以获取载体的硬磁干扰，MotionMars的校准算法会在后续运算过程中补偿此硬磁干扰。<br>
### 2.2载体不含磁性材料
若确定载体不含任何硬磁及软磁材料，则不需要将MotionMars节点安装到载体后，与载体一起转动校准。选择**无磁空旷**环境，单独校准MotionMars节点即可。
## 3、图例
下面以图例说明何为正确与错误的校准。<br>
下图为在无干扰磁场环境下，校准MotionMars节点获得的磁场点绘制的3D图，可以看出磁场形成了一个球体（或椭球体），在球体外部没有干扰点。

<div align=center>
<img src="https://raw.githubusercontent.com/FOHEART/MotionMarsHelp/master/img/goodcali1.png"/>
</div>

下图是在存在磁场干扰（铁）情况下，获取到的磁场点绘制的3D图，可以看出在球体之外存在若干**干扰点**（Interference），在此种环境下获取的硬软磁值会有较大误差，应避免此种情况发生。

<div align=center>
<img src="https://raw.githubusercontent.com/FOHEART/MotionMarsHelp/master/img/badcali1.png"/>
</div>