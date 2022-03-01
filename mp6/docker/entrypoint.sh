#!/bin/bash
export MP2_DIR=../mp2/
cd mp6
make png-extractGIF
flask run --host 0.0.0.0
