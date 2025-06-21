# Quickstart rk1808 

## Make a Dockerfile_U1804 
## Build docker image

``` sudo docker build -t ubuntu1804:AI_Stick -f Dockerfile_U1804 .```

## Allow docker used without sudo

```text
sudo groupadd docker
getent group docker
```
```text
###OUTPUT: docker:x:1001:
```

sudo usermod -aG docker $USER
ps aux | grep dockerd
    ls -l /var/run/docker.sock
srw-rw---- 1 root root 0 Jun 11 20:52 /var/run/docker.sock
Expect:
   sudo chown root:docker /var/run/docker.sock
   sudo chmod 660 /var/run/docker.sock

### Docker run and mount 

# Docker run with spec user and group id
```text   
docker run -it --privileged \
>   -v $(pwd):/home \
>   --device /dev/bus/usb \
>   --user $(id -u):$(id -g) \
>   ubuntu1804:AI_Stick /bin/bash
```

# Docker run with user root
```text
docker run -it --privileged \
>   -v $(pwd):/home \
>   --device /dev/bus/usb \
>   --user root \
>   ubuntu1804:AI_Stick /bin/bash
```

# Docker run and mount video from webcam
```text
if mount video: (ex: webcam, ...,)
docker run -it --privileged \
   -v $(pwd):/home \
   --device /dev/video0:/dev/video0 \
   --device /dev/bus/usb \
   ubuntu1804:AI_Stick /bin/bash
```

### rknn_toolkit quickstart
# Set up
```text
wget http://repo.rock-chips.com/python/rknn_toolkit-1.2.1-cp36-cp36m-linux_x86_64.whl
apt-get install -y  python3-opencv
pip3 install --user tensorflow==1.13.1
pip3 install --user rknn_toolkit-1.2.1-cp36-cp36m-linux_x86_64.whl
```
### Check lib install

```text
python3
>>> from rknn.api import RKNN
>>>
```

### Plugin rk1808 stick to PC
```text
cp tool/update_rk1808_ai_cs_rule.sh  ~/rknn -f
chmod +x ~/rknn/update_rk1808_ai_cs_rule.sh
```

```text
cd  ~/rknn
sudo ./update_rk1808_ai_cs_rule.sh
```

```text
lsusb
```
export PATH=$PATH:/root/.local/bin

export PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/root/.local/bin:/root/.local/bin:/root/.local/bin

# 1. Tạo user mới dev_ai với thư mục home /home/dev_ai
useradd -m -d /home/dev_ai -s /bin/bash dev_ai

# 2. Đặt mật khẩu cho user (nếu cần)
passwd dev_ai

# 3. (Tùy chọn) Thêm user vào nhóm sudo nếu muốn
usermod -aG sudo dev_ai

# 4. Chuyển đổi sang user dev_ai
su - dev_ai

RUN useradd -m -d /home/dev_ai -s /bin/bash dev_ai \
    && echo "dev_ai ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

USER dev_ai
WORKDIR /home/dev_ai
ENV HOME /home/dev_ai


USER root



echo "dev_ai ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers


RUN useradd -m -s /bin/bash dev_ai && \
    echo "dev_ai ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

# to install packages
apt install sudo 
pip3 install --user tensorflow==1.13.1


docker run -d --name ai_stick \
-it --privileged \
-v $(pwd):/home/dev_ai/ \
--device /dev/video0:/dev/video0 \
--device /dev/bus/usb \
ubuntu1804:AI_Stick /bin/bash

