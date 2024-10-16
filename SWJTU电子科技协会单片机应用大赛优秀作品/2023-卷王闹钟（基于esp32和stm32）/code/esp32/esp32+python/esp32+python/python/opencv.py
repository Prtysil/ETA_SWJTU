import urllib.request
import cv2
import numpy as np
import dlib
import socket
import threading
from scipy.spatial import distance as dist 
from collections import OrderedDict

url='http://192.168.226.183/cam-hi.jpg'         #改成自己的ip地址+/cam-hi.jpg

################################
#########与esp32通信部分#########
################################

def dispose_client_request(tcp_client_1,tcp_client_address):
    # 5 循环接收和发送数据
    while True:
        recv_data = tcp_client_1.recv(4096)
        
        # 6 有消息就回复数据，消息长度为0就是说明客户端下线了
        if recv_data:
            print("客户端是:", tcp_client_address)
            print("客户端发来的消息是:", recv_data.decode())
            # send_data = "Buzzer_ON".encode()
            # tcp_client_1.send(send_data)
        else:
            print("%s 客户端下线了..." % tcp_client_address[1])
            tcp_client_1.close()
            break

tcp_server = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
tcp_server.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,True)
tcp_server.bind(("",61234))
tcp_server.listen(128)


# def Control_Buzzer():
    # tcp_server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # tcp_server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, True) # 设置端口号复用,保证传输可靠性
    # tcp_server.bind(("", 61234))
    # tcp_server.listen(128)

    # tcp_client, tcp_client_address= tcp_server.accept() # 只有客户端和服务端建立连接成功代码才会解阻塞，代码才能继续往下执行
    # print("客户端的ip地址和端口号:", tcp_client_address)
    # recv_data = tcp_client.recv(1024)
    # recv_content = recv_data.decode(encoding = "utf-8")
    # print("接收客户端的数据为:", recv_content)
    # send_data = "Buzzer_ON".encode(encoding = "utf-8")
    # tcp_client.send(send_data)
    # tcp_client.close()

# Control_Buzzer()

###########################
#########摄像头部分#########
###########################

# 设置判断参数
EYE_AR_THRESH = 0.2  # ear小于0.2判断为闭眼
EYE_AR_CONSEC_FRAMES = 3  # 连续三帧ear都小于0.2判断为眨眼
MOUTH_AR_THRESH = 0.8   # 大于0.8判断为哈欠
EYE_COUNTER = 0     # 初始化计数器
EYE_TOTAL = 0

FACIAL_LANDMARKS_68_IDXS = dict([
    ("mouth", (48, 68)),
    ("right_eyebrow", (17, 22)),
    ("left_eyebrow", (22, 27)),
    ("right_eye", (36, 42)),
    ("left_eye", (42, 48)),
    ("nose", (27, 36)),
    ("jaw", (0, 17))
])
def eye_aspect_ratio(eye):
    # 计算距离，竖直的
    A = dist.euclidean(eye[1], eye[5])
    B = dist.euclidean(eye[2], eye[4])
    # 计算距离，水平的
    C = dist.euclidean(eye[0], eye[3])
    # ear值
    ear = (A + B) / (2.0 * C)
    return ear
def mouse_aspect_ratio(mouse):
        A = dist.euclidean(mouse[2],mouse[9])
        B = dist.euclidean(mouse[4],mouse[7])
        C = dist.euclidean(mouse[0],mouse[6])
        mouth = (A+ B) / (2 * C)
        return mouth
def shape_to_np(shape, dtype="int"):
    # 创建68*2
    coords = np.zeros((shape.num_parts, 2), dtype=dtype)
    # 遍历每一个关键点
    # 得到坐标
    for i in range(0, shape.num_parts):
        coords[i] = (shape.part(i).x, shape.part(i).y)
    return coords

#加载dlib库中的人脸检测与关键点定位
detector = dlib.get_frontal_face_detector()
predictor = dlib.shape_predictor('shape_predictor_68_face_landmarks.dat')

#分别取两个眼睛和嘴区域
(lStart, lEnd) = FACIAL_LANDMARKS_68_IDXS["left_eye"]
(rStart, rEnd) = FACIAL_LANDMARKS_68_IDXS["right_eye"]
(int_mouth, mEnd) = FACIAL_LANDMARKS_68_IDXS["mouth"]

TEST = False
ClosedEyes = False

while True:
    tcp_client_1 , tcp_client_address = tcp_server.accept()
    # 创建多线程对象
    thd = threading.Thread(target = dispose_client_request, args = (tcp_client_1,tcp_client_address))
    # 设置守护主线程  即如果主线程结束了 那子线程中也都销毁了  防止主线程无法退出
    # thd.setDaemon(True)
    # 启动子线程对象
    thd.start()

    # 如果闭眼则发送数据
    if ClosedEyes == True:
        send_data = "Buzzer_ON".encode(encoding = "utf-8")
        tcp_client_1.send(send_data)
        ClosedEyes = False

    # if TEST:
        # Control_Buzzer()
        # TEST = False
################################################################################################################################
################################################################################################################################
################################################################################################################################
    

    imgResp=urllib.request.urlopen(url)
    imgNp=np.array(bytearray(imgResp.read()),dtype=np.uint8)
    frame=cv2.imdecode(imgNp,-1)      #导入的图片

    ##### all the opencv processing is done here #####

    #图片预处理
    (h, w) = frame.shape[:2]
    width=1200
    r = width / float(w)
    dim = (width, int(h * r))
    frame = cv2.resize(frame, dim, interpolation=cv2.INTER_AREA)
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    rects = detector(gray, 0)   #检测人脸
    for rect in rects:
        # 获取坐标
        shape = predictor(gray, rect)
        shape = shape_to_np(shape)

        # 分别计算ear值
        leftEye = shape[lStart:lEnd]
        rightEye = shape[rStart:rEnd]
        leftEAR = eye_aspect_ratio(leftEye)
        rightEAR = eye_aspect_ratio(rightEye)

        # 计算mouth值
        mouth = shape[int_mouth:mEnd]
        mouthEAR = mouse_aspect_ratio(mouth)

        # 计算两个眼睛的平均值
        ear = (leftEAR + rightEAR) / 2.0

        # 绘制眼睛和嘴区域
        leftEyeHull = cv2.convexHull(leftEye)
        rightEyeHull = cv2.convexHull(rightEye)
        mouthHull = cv2.convexHull(mouth)
        cv2.drawContours(frame, [leftEyeHull], -1, (0, 255, 0), 1)
        cv2.drawContours(frame, [rightEyeHull], -1, (0, 255, 0), 1)
        cv2.drawContours(frame, [mouthHull], -1, (0, 255, 0), 1)

        # 检查是否满足阈值
        if ear < EYE_AR_THRESH:
            EYE_COUNTER += 1
            cv2.putText(frame, "ClosedEyes", (10, 150),
                cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)
            ClosedEyes = True
            TEST = True
            #通过wifi发送数据到esp32，从而控制蜂鸣器

        else:
            # 如果连续几帧都是闭眼的，判断为一次闭眼
            if EYE_COUNTER >= EYE_AR_CONSEC_FRAMES:
                EYE_TOTAL += 1
            # 重置
                EYE_COUNTER = 0

        #判断是否打哈欠
        if mouthEAR > MOUTH_AR_THRESH:
            cv2.putText(frame, "Yawn", (10, 100),
            cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)
        

        # 显示
        cv2.putText(frame, "Blinks: {}".format(EYE_TOTAL), (10, 30),
            cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)
        cv2.putText(frame, "eye: {:.2f}".format(ear), (300, 30),
            cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)
        cv2.putText(frame, "mouth: {:.2f}".format(mouthEAR), (400, 30),
            cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)

    cv2.imshow('test',frame)        #显示图片

    print("OKKKKKK")

    #enter q to exit
    if ord('q')==cv2.waitKey(10):
        exit(0)

