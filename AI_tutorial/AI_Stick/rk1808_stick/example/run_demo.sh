#!/usr/bin/env bash

TMP_SCRIPT='tmp.sh'

rm ${TMP_SCRIPT}
wget http://repo.rock-chips.com/rk1808/run_demo.sh -O ${TMP_SCRIPT}

source ${TMP_SCRIPT}