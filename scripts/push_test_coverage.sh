#!/bin/bash

lcov --list bazel-out/_coverage/_coverage_report.dat
bash <(curl -s https://codecov.io/bash) -f bazel-out/_coverage/_coverage_report.dat || echo "Codecov did not collect coverage reports"
