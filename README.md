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

The server simulator use the version ISO 8583-2:1993 version. All the MTIs must start with 1 and the format must be 1xxx.
```
1xxx ----> version of ISO 8583 (1993 version)
x1xx ----> class of the Message (Authorization Message)
xx1x ----> function of the Message (Request Response)
xxx0 ----> who is began the communication (Acquirer)
```

For more detailed information, read the documentation of the protocol [ISO.org](https://www.iso.org/obp/ui/#iso:std:iso:8583:-1:ed-1:v1:en), [IBM.com](https://www.ibm.com/docs/en/integration-bus/10.0?topic=formats-iso8583-messaging-standard) or [PDF](https://github.com/moov-io/iso8583/blob/master/docs/iso8583.pdf) from user of [GITHUB.com](https://github.com/moov-io/iso8583)

In addition the server use DATA-LENGTH as Header of the message. The DATA-LENGTH is the length of the message without the header. The DATA-LENGTH is a 2 bytes length and it is in binary. 

The server use the following format:
```
DATA-LENGTH (2 bytes) + MTI (2 bytes) + BITMAP(P) + BITMAP(S) + DATA-FIELDS
```

Example of MTI: 1200 Message
```
007c12007034051908c1080116888880366790225500000000000000057100005023120200523449124d313031303136353430344302862312020003061365443030353020202020202020203030303030303031202020202020202020202020202020000930303630313838383801020000020000001787b59700000000

===== 

Parsed:

HEADER: 007c -> 124 bytes
[000] 1200
[002] 8888803667902255
[003] 000000
[004] 000000000571
[011] 000050
[012] 231202005234
[014] 4912
[022] M1010165404C
[024] 286
[028] 231202
[029] 003
[032] 136544
[037] 0050        
[041] 00000001
[042] ...............
[048] 006018888
[053] .....
[064] ...
```

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



To execute with PODMAN, you only need to add podman as argument of any script. For example:

    ./BUILD.sh podman

----------

To execute all scripts with PODMAN, you can use the following command:

    ./BUILD.sh podman && ./RUN.sh podman && ./STOP.sh podman

----------

By default if you don't specify any argument, the scripts will use docker as container engine.
```

## Contributing

Contributions are what make the open source community such an amazing place to be learn, inspire, and create. Any contributions you make are **greatly appreciated**. Please read [CONTRIBUTING.md](./CONTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests to us.