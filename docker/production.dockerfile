FROM ubuntu:20.04

LABEL maintainer="zheng9112003@icloud.com"
LABEL version="0.1"
LABEL description="ISO8583 Server in C (Production layer, runnable with ./RUN.sh in /)"

RUN apt install -y vim gcc cmake
RUN mkdir log bin

WORKDIR /opt/Ciso8583

COPY ../tmp/ /bin

VOLUME /opt/Ciso8583/log

RUN chmod +x ./bin/Ciso8583

CMD ./bin/Ciso8583