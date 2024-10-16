# 三、外观设计（SketchUp）
在外形方面，我们设计了两版结构。
## 3-1第一版设计过程
### 3-1-1第一版激光切割加工图
![第一版激光切割加工图](https://img-blog.csdnimg.cn/7b1125a50c6a4708b711aad09b234c86.png)
### 3-1-2第一版3d模型图
![第一版3d模型图](https://img-blog.csdnimg.cn/7cfd56d264194135b4a82d95f5ac64c7.png)
### 3-1-3设计优缺点分析
可以很清楚地看到，第一版的设计有以下缺点：
1. 耗费木材过多
2. 灯板的高度不能自己调节
3. 没有设计手机支架
为了解决以上问题，我们采用了另外一种方案
## 3-2第二版设计过程
### 3-2-1第二版激光切割加工图
![第二版激光切割加工图](https://img-blog.csdnimg.cn/6cb84b3c5e894793b005f783a6c440bd.png)
### 3-2-2第二版实物图
![第二版实物图](https://img-blog.csdnimg.cn/9b8905f626994d6ba581980f15280e61.png)
### 3-2-3第二版优缺点分析
该版设计有以下优点：
1. 相比于第一版，节省很多木料
2. 灯板的高度可由用户自行调节
3. 设计了一个凹槽用于手机支撑
### 3-3-1加工工艺分析
1. 采用了激光切割的方式进行加工
2. 激光切割参数
	- 最小功率：82%
	- 最大功率：82%
	- 速度：12
3. 激光切割误差
	- 激光切割会损耗掉一部分材料
	- 为了使激光切割出来的侧壁能完美插入底板，我设计了一排测试孔。如图![测试孔](https://img-blog.csdnimg.cn/d84add20c130439283741e9ea6f7f4b1.png)
	- 最终测量出了最合适的孔距：1.6mm。
## 3-3-2插入效果![插入效果](https://img-blog.csdnimg.cn/0b88823a581a40f683456783334460fd.jpeg)
可以很清楚地看到，在没有上胶的情况下，也能使底座悬空，不会移动。
# 四、云端开发（python）
## 4-1图像处理
图像处理主要用来做姿态识别，姿态识别采用谷歌开源的mediapipe框架进行识别。这个模型有以下优势：
- Mediapipe具有高效的实时性能，可以在低功耗设备上实现姿态识别。
- Mediapipe可以实现端到端的解决方案，从视频流捕获到最终的姿态估计，而无需手动提取特征。
- Mediapipe提供了一系列可重用的模块，可以节省开发时间。
- Mediapipe支持多种输入源，包括视频流、图像序列和RGB图像。
- Mediapipe可以支持多种模型，可以根据应用场景选择合适的模型。
### 4-1-1图像预处理
对于姿态识别来说，预处理最容易遇到的问题是光线不足，所以预处理需要提高亮度，核心代码如下

```python
def liandu(src1, a, g):#第一个1.2为对比度  第二个为亮度数值越大越亮
    h, w, ch = src1.shape#获取shape的数值，height和width、通道
    src2 = np.zeros([h, w, ch], src1.dtype)
    dst = cv2.addWeighted(src1, a, src2, 1-a, g)#addWeighted函数说明如下
    return dst

```

### 4-1-2关节提取
运用了mediapipe模型进行骨骼关键点提取，核心代码如下：
```python
result=pose.process(frame)#加载模型
mp_drawing.draw_landmarks(frame,result.pose_landmarks,mp_pose.POSE_CONNECTIONS)#关节点绘制  
if result.pose_landmarks:#读取关节点坐标
    shoulder_left=result.pose_landmarks.landmark[11]
    elbow_left=result.pose_landmarks.landmark[13] 
    wrist_left=result.pose_landmarks.landmark[15]
    shoulder_right=result.pose_landmarks.landmark[12]
    elbow_right=result.pose_landmarks.landmark[14]
    wrist_right=result.pose_landmarks.landmark[16]
    head_nose=result.pose_landmarks.landmark[0]
```
### 4-1-3姿态识别
根据提取到的关节点，计算肢体的方向向量，从而得出肢体之间的夹角，从而进行姿态判断。
- 坐姿判断

```python
#计算方向向量
        S1=xl(shoulder_left,elbow_left)#左上臂
        S2=xl(elbow_left,wrist_left)#左下臂
        S3=xl(shoulder_right,elbow_right)#右上臂
        S4=xl(elbow_right,wrist_right)#右下臂
        S5=xl(shoulder_left,shoulder_right)#两肩
        S6=[0,1]
        #计算余弦角
        C1=xlj(S1,S2)
        C2=xlj(S2,S3)
        C3=xlj(S5,S6)
        print(head_nose.z)
        if C3>0.234:
            lamp_flag=0
            return "2"#脊柱歪了，坐直
        elif C3<-0.15:
            lamp_flag=0
            return "3"#脊柱歪了，坐直
        elif head_nose.y<-3.3:
            lamp_flag=0
            return "4"#驼背了，抬起头来
        else:
            lamp_flag=1
            return "1"#坐姿合格
```
- 运动姿态判断（伸展运动）

```python
        H1=shoulder_left.y-elbow_left.y
        H2=shoulder_right.y-elbow_right.y
        print("左臂"+str(H1))
        print("右臂"+str(H2))
        if H1<-0.05:
            lamp_flag=0
            return "2"#抬高左臂
        elif  H2<-0.05:
            lamp_flag=0
            return "3"#抬高右臂
        else:
            lamp_flag=1
            return "1"
```
## 4-2通信部分
主要通过python中的flask库构建网络接口，与APP和台灯进行通信。
### 4-2-1GET接口
用于向台灯返回标志位，核心代码如下：

```python
@app.route("/b", methods=["GET"])
def type_esp32():
    print(lamp_flag)
    return str(lamp_flag)
```

### 4-2-2POST接口
用于接受app发送的图片，并返回判断结果，核心代码如下：

```python
@app.route("/a", methods=["POST"])#图像处理函数
def type_study():
    data = request.files
#     print(type(data))
#     print(data)
    file = data['file']
#     print(file.filename)
#     print(type(request.headers))
#     print(request.headers)
#     print(request.headers.get('File-Name'))
    # 文件写入磁盘
    file.save(file.filename)   
```
### 4-2-3返回值定义
- 坐姿检测中：
	- return "1"：代表坐姿合格
	- return "2"：代表脊椎歪右边
	- return "3"：代表脊椎歪左边
	- return "4"：代表用户驼背
	- return "5"：代表没有检测到用户
- 动作判断中：
	- return "1"：代表动作标准
	- return "2"：代表左手没有抬起
	- return "3"：代表右手没有抬起
	- return "4"：代表没有检测到人像
- 台灯控制中：
	- return "1"：代表控制台灯亮
	- return "0"：代表控制台灯灭
## 4-3内网穿透
用花生壳将内网ip映射到公网
![内网穿透](https://img-blog.csdnimg.cn/19de19e676a6486999c1b9df6c7db63e.jpeg)

# 五、APP设计部分（iApp）
主要使用基于JAVA的裕语言（iApp）进行开发。
- 可在手机上同时进行开发和测试，开发便利。
- 开发界面简介明了。
## 5-1界面设计部分
利用可画绘制素材
## 5-2悬浮窗部分
主要采用悬浮窗的方式来进行锁机
### 5-2-1创建悬浮窗
核心代码如下

```python
s width=-1
s height=-1
s gravity="top|left"
uxf("study.iyu",width,height,gravity,sss.a)
```
### 5-2-2激活悬浮窗内的控件
核心代码如下
```python
gvs(sss.a,study.5,x)
gvs(sss.a,study.3,i)
gvs(sss.a,study.7,r)
us(r,"text",sss.cycle)
```
## 5-3摄像头部分
### 5-3-1开启摄像头
开启手机前置摄像头，并设置图片旋转角、长宽、质量
```python
usxq(sss.p,x, 90, 224, 224, 20)
```
### 5-3-2保存捕获到根目录

```python
usx(sss.p,"shot","%123.jpg",-90,false)
```
## 5-4网络传输部分
向网络接口发送图片

```python
s ip="https://xxxxxxxx"
s bao="filename=123.jpg&test=img1"
s frame="%123.jpg"
huf(ip,bao,frame,d,e)
//e为返回值
```
## 5-5 权限获取

```python
javags(SDK_INT,null,"android.os.Build$VERSION","SDK_INT")

f(SDK_INT >= 23) {

  sit(intent,"action","android.settings.action.MANAGE_OVERLAY_PERMISSION")

  java(pn,activity,"android.content.Context.getPackageName")

  ss("package:" + pn, uri)

  sit(intent,"data",uri)

  sit(intent,"flags",268435456)

  uit(intent,"result",0)

  } else {

  tw("系统版本太低")

  }
 
  
  
```
# 六、[硬件和控制部分](https://blog.csdn.net/YaoJinTao2003/article/details/129680777?csdn_share_tail=%7B%22type%22:%22blog%22,%22rType%22:%22article%22,%22rId%22:%22129680777%22,%22source%22:%22YaoJinTao2003%22%7D)
