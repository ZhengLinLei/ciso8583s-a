FROM ubuntu:20.04

LABEL maintainer="zheng9112003@icloud.com"
LABEL version="0.1"
LABEL description="ISO8583 Server in C (Compilation layer, runnable with ./BUILD.sh in /)"

RUN apt update
RUN apt install -y vim gcc make
RUN mkdir /opt/Ciso8583

# Set root path
WORKDIR /opt/Ciso8583

# Copy project
COPY src src
COPY docker/buildContainer.sh .

# Create environment
RUN mkdir bin build dist tmp
RUN chmod +x ./buildContainer.sh

# CMD ["ls", "-l"]
CMD ./buildContainer.sh
