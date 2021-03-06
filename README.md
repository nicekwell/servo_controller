# servo_controller
通用舵机控制器（PWM信号发生器）。  
![servo.jpg](https://github.com/nicekwell/servo_controller/raw/master/servo.jpg)

优酷视频地址：<http://v.youku.com/v_show/id_XMTg4NzQ0MDA5Mg==.html>  
控制机械臂：<http://v.youku.com/v_show/id_XMTg4NzUyNDA0OA==.html>

# 功能
可以产生7路PWM信号。  
周期固定为20ms，脉宽可调，精度为10us。  
上方7路输出通道，最上面为信号脚，中间是VCC，下面是GND。  
下方7个按钮选择当天调节哪路信号，按下按钮后，上方对应通道的led灯点亮，同时显示屏相应通道也会有闪烁。  
右边5个按钮调节当前通道的脉宽，从上到下功能依次为：加100，加10，复位（设置为1500us），减10，减100。  
**脉宽调节没有范围限制！！**所以调节时一定要注意。  
开机后默认所有路通道脉宽为1500us。

# 硬件
【单片机】12C5A32S2  
【晶振】24MHz  
【引脚连接】

功能 | 引脚 | 备注
:-: | :-: | :-: 
舵机控制脚1 | P1.6 |
舵机控制脚2	| P1.5 |
舵机控制脚3	| P1.4 |
舵机控制脚4	| P1.3 |
舵机控制脚5	| P1.2 |
舵机控制脚6	| P1.1 |
舵机控制脚7	| P1.0 |
led1		| P0.6	| 推挽输出，高电平点亮，所有负极接在一起串联了电阻，一次只能点亮一个灯
led2		| P0.5 |
led3		| P0.4 |
led4		| P0.3 |
led5		| P0.2 |
led6		| P0.1 |
led7		| P0.0 |
通道开关1	| P2.0	| 所有开关按下是低电平
通道开关2	| P2.1 |
通道开关3	| P2.2 |
通道开关4	| P2.3 |
通道开关5	| P2.4 |
通道开关6	| P2.5 |
通道开关7	| P2.6 |
开关-100	| P3.7 |
开关-10		| P3.6 |
开关复位	| P3.5 |
开关+10		| P3.4 |
开关+100	| P3.3 |
显示屏SCL	| P0.7 |
显示屏SDA	| P1.7 |

# 程序结构
定时器10us一次中断，在定时器中断中完成 7路pwm通道脉宽控制 和 按键扫描 任务，其中按键扫描分成了三个函数分别进行。（广告：想了解这样做的原因，以及更多单片机编程结构相关的内容请访问<http://nicekwell.net/pages/dan-pian-ji-bian-cheng.html>）  
主进程完成显示屏显示任务。

定时器进程检测到按键操作后，会发送标志给主进程刷新显示。


