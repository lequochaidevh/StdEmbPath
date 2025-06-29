# Set up docker with USER_ID = USER_ID_HOST
# Set up WORKDIR 
FROM ubuntu:22.04

RUN apt-get update && \
    apt-get install -y wget && \
    apt-get clean

WORKDIR /app

RUN apt-get update && apt-get install -y passwd

ARG USERNAME
ARG USER_ID
ARG GROUP_ID

RUN groupadd -g ${GROUP_ID} ${USERNAME} && \
    useradd -m -u ${USER_ID} -g ${GROUP_ID} ${USERNAME}

