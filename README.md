# OSI Stack

![CI](https://github.com/mihaigalos/osi_stack/workflows/CI/badge.svg) [![codecov](https://codecov.io/gh/mihaigalos/osi_stack/branch/master/graph/badge.svg)](https://codecov.io/gh/mihaigalos/osi_stack) [![license](https://img.shields.io/badge/license-GPLv3-brightgreen.svg)](LICENSE) [![HitCount](http://hits.dwyl.com/mihaigalos/osi_stack.svg)](http://hits.dwyl.com/mihaigalos/osi_stack) [![LoC](https://tokei.rs/b1/github/mihaigalos/osi_stack)](https://github.com/Aaronepower/tokei)

![Last Logs](https://img.shields.io/endpoint?style=social&url=https%3A%2F%2Fapi.cl1p.net%2Fosi_stack%2Fclipboard)

OSI communication stack, initially written for AVR UART communication. 

This implementation intentionally uses raw pointers instead of smart pointers.

The reasoning is that it will be ported to small 8-bit hardware architectures such as AVR/Arduino, with limited support for modern C++.

<img src="http://www.plantuml.com/plantuml/proxy?cache=no&src=https://raw.githubusercontent.com/mihaigalos/osi_stack/master/doc/sources/iso_osi_layers.puml" alt="ISO/OSI Layers (Rough Model)." />

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
docker run -it --rm --cap-add SYS_PTRACE -v $(pwd):/src -v /tmp:/tmp/bazel docker.pkg.github.com/mihaigalos/docker/bazel /bin/zsh -c "cd /src && bazel --output_base=/tmp/bazel test //..."
```

#### Session Layer Authentification

The Session handshake is described in the following diagram:

<img src="http://www.plantuml.com/plantuml/proxy?cache=no&src=https://raw.githubusercontent.com/mihaigalos/osi_stack/master/doc/sources/session_handshake.puml" alt="ISO/OSI Layers (Rough Model)." />

#### Matter of principles

This code respects many of the Clean Code & Architecture Principles:

* Composition over inheritance
* Dependency injection
* DMT
* Docs as code
* DRY
* FIRST
* GRASP
* KISS
* Single source of truth
* SoC
* SOLID
* TDD
* Testing pyramid
