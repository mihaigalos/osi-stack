# OSI Stack

[![Build Status](https://travis-ci.org/mihaigalos/osi_stack.svg?branch=master)](https://travis-ci.org/mihaigalos/osi_stack) [![codecov](https://codecov.io/gh/mihaigalos/osi_stack/branch/master/graph/badge.svg)](https://codecov.io/gh/mihaigalos/osi_stack) [![license](https://img.shields.io/badge/license-GPLv3-brightgreen.svg)](LICENSE) [![HitCount](http://hits.dwyl.com/mihaigalos/osi_stack.svg)](http://hits.dwyl.com/mihaigalos/osi_stack) [![LoC](https://tokei.rs/b1/github/mihaigalos/osi_stack)](https://github.com/Aaronepower/tokei)

![Last Logs](https://img.shields.io/endpoint?style=social&url=https%3A%2F%2Fapi.cl1p.net%2Fosi_stack%2Fclipboard)


Very rough model of an ISO/OSI communication stack.

This implementation intentionally uses raw pointers instead of smart pointers.

The reasoning is that it will be ported to small 8-bit hardware architectures such as AVR/Arduino, with limited support for modern C++.


![alt text](screenshots/iso_osi_layers.png "ISO/OSI Layers (Rough Model).")

### Build and Test

First, get the sources:
```
git clone https://github.com/mihaigalos/osi_stack.git && cd osi_stack
```

#### Baremetal

Building and testing is managed by `bazel`, including dependencies to other repos such as [containers](https://github.com/mihaigalos/containers) (transparent to the end-user).

```bash
bazel build //...
bazel test //...
```

#### Using Docker

```bash
docker run -it --rm -v $(pwd):/src -v /tmp:/tmp/bazel remoteapistesting/bazel-build /bin/bash -c "bazel --output_base=/tmp/bazel test //..."
```

#### Session Layer Authentification

The following code is a PlantUML diagram.

```plantuml
@startuml
    skinparam monochrome true

    actor User
    User -> "Session::Transmit()/Receive()" : username & password

    "Session::Transmit()/Receive()"-> "Session::Login()" : username & password
    activate "Session::Login()"
    "Session::Login()" -> "Session::Transmit()/Receive()" : LoginStatus::Success
    "Session::Transmit()/Receive()" -> User : "OK session_cookie"

    User -> "Session::Transmit()/Receive()" : "Logout"
    "Session::Transmit()/Receive()" -> "Session::Logout()"
    "Session::Logout()" -> "Session::Login()": reset cookie
    deactivate "Session::Login()"

@enduml
```