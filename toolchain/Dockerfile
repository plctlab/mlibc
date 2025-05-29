FROM ubuntu:16.04

ARG USER_NAME=name

RUN ln -sf /usr/share/zoneinfo/Asia/Shanghai /etc/localtime && \
    echo "Asia/Shanghai" > /etc/timezone
RUN sed -i 's/archive.ubuntu.com/mirrors.aliyun.com/g' /etc/apt/sources.list
RUN apt-get update && apt-get install -y sudo locales language-pack-en-base bash bash-completion git curl vim python3 python3-pip tmux bison flex file texinfo \
    && locale-gen en_US.UTF-8 \
    && echo "${USER_NAME} ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers \
    && apt-get install -y chrpath cpio diffstat gawk wget zstd

RUN apt-get install -y -qq lib32ncurses5 lib32z1

ENV LANG=en_US.UTF-8
ENV LC_ALL=en_US.UTF-8

WORKDIR /home/${USER_NAME}
