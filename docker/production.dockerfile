FROM ubuntu:20.04

LABEL maintainer="zheng9112003@icloud.com"
LABEL version="0.1"
LABEL description="ISO8583 Server in C (Production layer, runnable with ./RUN.sh in /)"

RUN apt update
RUN apt install -y vim
RUN mkdir /opt/Ciso8583
WORKDIR /opt/Ciso8583

RUN mkdir log bin

COPY tmp bin
COPY docker/runProduction.sh .

# COPY docker/showTree.sh .
# RUN chmod +x ./showTree.sh
# CMD ["sleep", "infinity"]

VOLUME /opt/Ciso8583/log

RUN chmod +x ./runProduction.sh
RUN chmod +x ./bin/ciso8583

CMD ./runProduction.sh