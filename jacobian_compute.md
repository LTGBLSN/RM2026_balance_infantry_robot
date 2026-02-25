# 雅可比矩阵计算



## 正向运动学得到



1. 腿的摆角

$$
\theta_{47}=\frac{\theta_{7}+\theta_{2}+\pi}{2}
$$

2. 腿长

$$
L = L_{1} \cos\theta_{4} + \sqrt{L_{2}^{2} - L_{1}^{2} \sin^{2}\theta_{4}}
$$



## 角度对时间求导即为速度，可得：



1. 腿的摆动角速度


$$
\dot{\theta}_{47} = \frac{\dot{\theta}_{7} + \dot{\theta}_{2}}{2}
$$


2. 腿长变化率

​		这里腿长变化率需要使用链式法则

​		令K ：
$$
K = \frac{dL}{d\theta_{4}} = -L_{1} \sin\theta_{4} - \frac{L_{1}^{2} \sin\theta_{4} \cos\theta_{4}}{\sqrt{L_{2}^{2} - L_{1}^{2} \sin^{2}\theta_{4}}}
$$
​		再根据θ~4~ 与输入的关系：
$$
\frac{\partial \theta_{4}}{\partial \theta_{7}} = -\frac{1}{2}, \quad \frac{\partial \theta_{4}}{\partial \theta_{2}} = \frac{1}{2}
$$
​		结合链式法则
$$
\frac{\partial L}{\partial \theta_{i}} = \frac{dL}{d\theta_{4}} \cdot \frac{\partial \theta_{4}}{\partial \theta_{i}}
$$
​		得到
$$
\frac{\partial L}{\partial \theta_{7}} = K \cdot (-\frac{1}{2}) = -\frac{1}{2}K
$$

$$
\frac{\partial L}{\partial \theta_{2}} = K \cdot (\frac{1}{2}) = \frac{1}{2}K
$$

​		此时可晓得腿长变化率为
$$
\dot{L} = \frac{dL}{dt} = \frac{\partial L}{\partial \theta_{7}} \cdot \frac{d\theta_{7}}{dt} + \frac{\partial L}{\partial \theta_{2}} \cdot \frac{d\theta_{2}}{dt}
$$
​		带入K得到
$$
\dot{L} = \frac{1}{2} K (\dot{\theta}_{2} - \dot{\theta}_{7})
$$


## 此时可得到雅可比矩阵

根据正向运动学求导后的速度运动学
$$
\begin{bmatrix} \dot{L} \\ \dot{\theta}_{47} \end{bmatrix} = \mathbf{J} \begin{bmatrix} \dot{\theta}_{7} \\ \dot{\theta}_{2} \end{bmatrix}
$$
其中J
$$
\mathbf{J} = \begin{bmatrix} -\frac{1}{2}K & \frac{1}{2}K \\ \frac{1}{2} & \frac{1}{2} \end{bmatrix}
$$


## 静力学力矩转换

根据虚功原理，转矩*τ* 和末端力F的关系为：
$$
\begin{bmatrix} \tau_{7} \\ \tau_{2} \end{bmatrix} = \mathbf{J}^{T} \cdot \begin{bmatrix} F_{L} \\ T_{p\theta 47} \end{bmatrix}
$$
（太美妙了🥰）