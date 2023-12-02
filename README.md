<h1 align="center">CISO8583 Static Type-A</h1>
<br>
<br>

<p align="center">
  <i>ISO8583 Server entity simulatorin C for testing</i>
</p>

<p align="center">
  <a href="./CONTRIBUTING.md">Contributing</a>
  ·
  <a href="https://github.com/ZhengLinLei/Kronus.py/issues">Issues</a>
</p>

<p align="center">
  <a href="https://opensource.org/licenses/Apache-2.0">
    <img src="https://img.shields.io/badge/License-Apache%202.0-blue.svg" alt="Project License" />
  </a>&nbsp;
  <a>
    <img src="https://img.shields.io/badge/version-S.A-brightgreen" alt="Version" />
  </a>
</p>

<hr>

## Table of Contents

- [Table of Contents](#table-of-contents)
- [About the Project](#about-the-project)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
- [Contributing](#contributing)

## About the Project

This project is a ISO8583 Server entity simulator in C for testing. It is a static type-A simulator, which means it can only send and receive static messages. The project is based on the [ISO8583](https://en.wikipedia.org/wiki/ISO_8583) where you can use it as a test tool for your Host system. The project is written in C and it is a console application.

## Getting Started

### Prerequisites

- [CMake](https://cmake.org/download/)
- [GCC](https://gcc.gnu.org/install/index.html)

### Installation

```log
HOW TO DEPLOY THE APPLICATION SERVER?
-------------------------------------

WITH DOCKER (RECOMMENDED)

    1. Make sure you have installed docker and docker-compose

    2. Execute the following commands:
        - ./BUILD.sh
        - ./RUN.sh

    3. To enter the container execute:
        - docker exec -it ciso8583_ots_a bash

    3. To stop the container execute:
        - ./STOP.sh

    4. To clean all the environment execute:
        - ./CLEAN.sh


    Notes:
        - If you are Mac user, you must to change ./RUN.sh file and replace the following line:
            - docker run -d -it --name $container_name --network=host -v ciso8583_ots_a_logs:/opt/Ciso8583/log $container_name
            to
            - docker run -d -it --name $container_name -p 9101:9101 -v ciso8583_ots_a_logs:/opt/Ciso8583/log $container_name

WITH NATIVE INSTALLATION

    1. Make sure you have installed this dependencies:
        - gcc
        - make

    2. Execute the following commands:
        - make clean
        - make

    3. The application server will be compiled following path:
        - ~/dist/ciso8583

    4. Execute the program:
        - ~/dist/ciso8583

    5. You will see the logs in the following path:
        - /opt/Ciso8583/logs/Ciso8583.log
```

## Contributing

Contributions are what make the open source community such an amazing place to be learn, inspire, and create. Any contributions you make are **greatly appreciated**. Please read [CONTRIBUTING.md](./CONTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests to us.