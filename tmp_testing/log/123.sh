#!/bin/bash

 sed '/Layer m/d' result/converted_log | cut -d ' ' -f 6,7 > ../temp123
