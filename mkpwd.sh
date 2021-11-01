#!/bin/bash

SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
dd if=/dev/urandom bs=48 count=1 status=none | base64 | tr +/ -_ > "$SCRIPT_DIR"/pwd
