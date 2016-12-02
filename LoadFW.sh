#!/bin/bash

clear

HEX_FILE="ChristmasCard.hex"
HEX_DIR=""

END_DIR=$HEX_FILE
echo "Loading firmware... to file $END_DIR" 
avrdude -p t2313 -c usbasp-clone -U flash:w:$END_DIR

if [ $? -eq 0 ]
then
  echo "Loading complete"
else
  echo "Error"
fi
