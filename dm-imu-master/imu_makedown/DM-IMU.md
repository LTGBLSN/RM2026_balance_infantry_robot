---
writer: tuyu
---



# A 通信设置

使用VOFA设置，设置为hex输出模式![Pasted image 20251224214105](assets/Pasted image 20251224214105.png)

## A.1 进入设置模式
```
AA 06 01 0D
```
发送成功后，串口读取部分会变为慢速输出数据

![Pasted image 20251224214155](assets/Pasted image 20251224214155.png)

## A.2 设置为CAN输出模式
```
AA 0A 02 0D
```


## A.3 设置CANID
```
AA 08 X 0D
```
- X:是你想控制这个imu时的目标can id


## A.4 设置MSTID
```
AA 09 X 0D
```
- X:是你想让这个imu反馈时的can id


## A.5 开启CAN自动上报
```
AA 01 18 0D
```

## A.6 保存参数
```
AA 03 01 0D
```


## A.7 退出设置模式
```
AA 06 00 0D
```



## A.8 最后把串口读取和输出改回ascii
![Pasted image 20251224214735](assets/Pasted image 20251224214735.png)




# B 代码设置
## B.1 包含这两个文件
![Pasted image 20251224223816](assets/Pasted image 20251224223816.png)
## B.2 在大疆can通信中包含此文件

![Pasted image 20251224223718](assets/Pasted image 20251224223718.png)


## B.3 枚举类型中添加imu的mst id 
![Pasted image 20251224224026](assets/Pasted image 20251224224026.png)

## B.4 在can中断回调函数加上此函数
![Pasted image 20251224223943](assets/Pasted image 20251224223943.png)



## B.5 最后就可以使用了(记得包含头文件)

![Pasted image 20251224224118](assets/Pasted image 20251224224118.png)