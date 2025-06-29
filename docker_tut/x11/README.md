# DOCKER WITH X11 USING GRAPHIC.
## Build:

```text
docker build -t gui-x11 .
```
```text
xhost +local:docker  # allow docker to use X11
xhost -local:docker  # secure it again after
```
## Set up Xauthority for docker access.
At host, check Xinfo:
```text
xauth info
```
You will see line:
```text
Authority file:       /run/user/1000/gdm/Xauthority
```
This file Authority of X11 in host.
**Check permission file**
```text
ls -l /run/user/1000/gdm/Xauthority
```
You can see:
```text
-rw------- 1 devh devh 145 Jun 29 15:39 /run/user/1000/gdm/Xauthority
```
Temporary solution:
```text
sudo chmod o+r /run/user/1000/gdm/Xauthority
```
**CHECK EYES APP**
```text
xhost +local:root
xhost +SI:localuser:root
docker run -it --rm --net=host   -e DISPLAY=$DISPLAY   -e XAUTHORITY=/tmp/.Xauthority   -v /run/user/1000/gdm/Xauthority:/tmp/.Xauthority:ro   -v /tmp/.X11-unix:/tmp/.X11-unix   gui-x11:latest xeyes
```
If EYES APP not work: - Check file Authority of X11 be mounted to Docker ?
```text
docker run -it --rm --net=host -e DISPLAY=$DISPLAY   -e XAUTHORITY=/tmp/.Xauthority   -v /tmp/.X11-unix:/tmp/.X11-unix   -v /run/user/1000/gdm/Xauthority:/tmp/.Xauthority:ro   gui-x11:latest bash
# At docker check Authority:
apt-get update && apt-get install -y xauth
xauth list
# Some info about Authority will be display -> It OK.
```

