#!/bin/bash

bazel analyze-profile --dump=text /tmp/build_profile
bazel analyze-profile --dump=text /tmp/test_profile
