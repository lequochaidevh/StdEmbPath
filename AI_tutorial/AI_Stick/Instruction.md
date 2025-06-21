# Quickstart rk1808 

## Make a Dockerfile_U1804 
## Build docker image
``` sudo docker build -t ubuntu1804:AI_Stick -f Dockerfile_U1804 .```

## __Allow docker used without sudo

```text
sudo groupadd docker
getent group docker
```
**Output:**:
```text
docker:x:1001:
```
### Allow docker use without sudo
```text
sudo usermod -aG docker $USER
ps aux | grep dockerd
# or
ls -l /var/run/docker.sock
```
**Expect:**
```text
srw-rw---- 1 root root 0 Jun 11 20:52 /var/run/docker.sock
```
**If not like expect, can try:**
```text
   sudo chown root:docker /var/run/docker.sock
   sudo chmod 660 /var/run/docker.sock
```

## Docker run and mount 
### Docker run with spec user and group id
```text   
#docker run -it --privileged \
#-v $(pwd):/home \
#--device /dev/bus/usb \
#--user $(id -u):$(id -g) \
#ubuntu1804:AI_Stick /bin/bash
```
### Docker run with user root
```text
#docker run -it --privileged \
#-v $(pwd):/home \
#--device /dev/bus/usb \
#--user root \
#ubuntu1804:AI_Stick /bin/bash
```
### Docker run and mount video from webcam
```text
#if mount video: (ex: webcam, ...,)
#docker run -it --privileged \
#-v $(pwd):/home \
#--device /dev/video0:/dev/video0 \
#--device /dev/bus/usb \
#ubuntu1804:AI_Stick /bin/bash
```

# Create container with the image.
```text
docker run -d --name ai_stick \
-it --privileged \
-v $(pwd):/home/dev_ai/ \
-v /media/devh/:/media/dev_ai \
--device /dev/video0:/dev/video0 \
--device /dev/bus/usb:/dev/bus/usb \
ubuntu1804:AI_Stick /bin/bash
```

# Start docker
```text
docker start -ai ai_stick
```

--------------------------------------------------------------------------------------------------------
# Set up in container:
## Create a new user to install few packages without warning and error.
**1. Create new user with path home /home/dev_ai**

```useradd -m -d /home/dev_ai -s /bin/bash dev_ai```

**2. Set password for dev_ai usr to use sudo**

```passwd dev_ai```

**3. (Optimize) Can add dev_ai to group sudo if want**

```usermod -aG sudo dev_ai```

**4. Checkout to dev_ai user**

```su - dev_ai```

### Verify own some file 
```text
ls -ld /home/dev_ai/.local
ls -ld /home/dev_ai/.local/lib
```
**Expect:** `drwx------ 4 dev_ai dev_ai 4096 Jun 15 19:55 /home/dev_ai/.local`

## rknn_toolkit quickstart --- recomment implement with NOT root user
### Set up


```text
# If you are dev_ai user
cd ~
export PATH=$PATH:/home/dev_ai/.local/bin
sudo wget http://repo.rock-chips.com/python/rknn_toolkit-1.2.1-cp36-cp36m-linux_x86_64.whl
sudo apt-get install -y  python3-opencv
pip3 install --user tensorflow==1.13.1
pip3 install --user rknn_toolkit-1.2.1-cp36-cp36m-linux_x86_64.whl
```
### Check lib install
```text
#PYTHONWARNINGS="ignore" python3
# to Ignore warning. Or
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
### Check usb rk1808
```text
lsusb
# expect: id 2207
# Bus 001 Device 007: ID 2207:0018
```

```text
ls -l /dev/bus/usb/001/007
# expect: crw-rw-rw- 1 root root 189, 6 Jun 15 21:20 /dev/bus/usb/001/007
```
### Download example

```text
wget http://repo.rock-chips.com/rk1808/mobilenet_v1.tar.gz
tar xvf mobilenet_v1.tar.gz
```

```text
cd mobilenet_v1/
sudo apt install adb -y
python3 test.py
```

**Expected result:**
```text
--> config model
done
--> Loading model
done
--> Building model
done
--> Export RKNN model
done
--> Init runtime environment
done
--> Running modelmobilenet_v1
-----TOP 5-----
[156]: 0.8837890625
[155]: 0.0677490234375
[188 205]: 0.00867462158203125
[188 205]: 0.00867462158203125
[263]: 0.0057525634765625
done
--> Begin evaluate model performance
========================================================================                               Performance                              ========================================================================
Total Time(us): 7140FPS: 140.06
========================================================================
done
```

# NOTE:
## Reproduce
# Join to docker running container  
`docker exec -it ai_stick /bin/bash`

### Install few packages. Can not need if you now have any error above.
```text
sudo apt update
sudo apt install usbutils udev

sudo udevadm control --reload-rules
sudo udevadm trigger
```

# NOT USE
### export PATH=$PATH:/root/.local/bin


# RUN useradd -m -d /home/dev_ai -s /bin/bash dev_ai \
#    && echo "dev_ai ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

# USER dev_ai
# WORKDIR /home/dev_ai
# ENV HOME /home/dev_ai


# USER root



# echo "dev_ai ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers


# RUN useradd -m -s /bin/bash dev_ai && \
#    echo "dev_ai ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

# to install packages
# apt install sudo 
# pip3 install --user tensorflow==1.13.1


# docker run -it --privileged    -v $(pwd):/home/dev_ai/    --device /dev/video0:/dev/video0    --device /dev/bus/usb    ubuntu1804:AI_Stick /bin/# bash

