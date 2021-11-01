#!/bin/bash

SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
tr -dc A-Za-z0-9 </dev/urandom | head -c 63 > "$SCRIPT_DIR"/pwd
