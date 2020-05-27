#!/bin/bash

bazel analyze-profile --dump=text /tmp/build_profile || true
bazel analyze-profile --dump=text /tmp/test_profile || true
