#!/bin/bash

pushd scripts

invocation=$(grep -m 1 https://app.buildbuddy.io/invocation /tmp/test.log | sed -e "s/.*\(invocation\)/\1/g")
full_url="https://app.buildbuddy.io/${invocation}"
json={\"schemaVersion\":1,\"label\":\"LastLogs\",\"message\":\"${full_url}\",\"color\":\"green\"}

python3 cl1p.py osi_stack/clipboard "${json}" 10

popd