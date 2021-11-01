#!/bin/bash

SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
< /dev/urandom tr -cd "[:print:]" | tr -d '\\'  | tr -d '"' | head -c 63 > "$SCRIPT_DIR"/pwd