#!/bin/bash

source ./$(dirname $0)/template/template.sh

test_file md5sum md5 base64
