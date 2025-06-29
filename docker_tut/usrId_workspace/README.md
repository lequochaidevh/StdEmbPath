# Build image with parameters - ADD USER:GROUP

### Build image with 3 args
```text
docker build -t image-viewer --build-arg USERNAME=$(whoami) --build-arg USER_ID=$(id -u) --build-arg GROUP_ID=$(id -g) -f Dockerfile .
```

### Run docker with USER and GROUP present in host
```text
docker run -it --rm --user $(id -u):$(id -g) -v $(pwd):/app image-viewer:latest bash
```

