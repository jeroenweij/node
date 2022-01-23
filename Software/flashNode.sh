#/bin/bash

NODE_ID="$1"
echo "flashing node ${NODE_ID}"

mkdir -p build
cd build
cmake .. -DNODE_ID=${NODE_ID}
make burnWithEeprom 
