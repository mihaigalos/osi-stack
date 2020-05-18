### OSI Stack [![Build Status](https://travis-ci.org/mihaigalos/osi_stack.svg?branch=master)](https://travis-ci.org/mihaigalos/osi_stack) [![license](https://img.shields.io/badge/license-GPLv3-brightgreen.svg)](LICENSE)

Very rough model of an ISO/OSI communication stack.

This implementation intentionally uses raw pointers instead of smart pointers.

The reasoning is that it will be ported to small 8-bit hardware architectures such as AVR/Arduino, with limited support for modern C++.


![alt text](screenshots/iso_osi_layers.png "ISO/OSI Layers (Rough Model).")

### Build and Test

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