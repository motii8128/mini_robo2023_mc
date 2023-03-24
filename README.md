# mini_robo2023_mc
Microcomputer

# 概要
マイコン側のコードです
このコードだけでは動かないので[こちら](https://github.com/Kotakku/ros2mbed)をクローンし同じファイルに入れた上で使ってください。
引用：[kotakku](https://github.com/Kotakku)

# ノードの立ち上げ
Micro-XRCE-DDS-Agentを用いてます
```
git clone https://github.com/eProsima/Micro-XRCE-DDS-Agent.git
cd Micro-XRCE-DDS-Agent && git checkout v1.3.0
mkdir build && cd build
source /opt/ros/dashing/setup.bash # to share libraries with ros2
cmake ..
make
sudo make install
sudo ldconfig /usr/local/lib/
```
私はserial通信で使いました
```
cd Micro-XRCE-DDS-Agent/build
./MicroXRCEAgent serial --dev /dev/ttyACM0 -b 115200
```
詳しくは[こちら](https://github.com/Kotakku/ros2mbed)のREADMEを参考にしてください
# 環境
ubuntu 20.04


stm32 NUCLEO_f767zi
