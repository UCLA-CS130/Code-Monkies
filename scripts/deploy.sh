#!/bin/bash

echo "Note that this utility must be run from the top level of the "
echo "Code-Monkies repository, as such: ./scripts/deploy.sh"
echo "If not called from here, it will not work."

if [[ $EUID -ne 0 ]]; then
    echo "This script must be run as root."
    exit 1
fi

if [ $# -ne 3 ]; then
    echo "Usage: $0 server_ip ssh_user ssh_key_file"
    exit 1
fi

server_ip="$1"
ssh_user="$2"
ssh_key_file="$3"

# Make sure directory is empty to minimize transfer time.
make clean

# Build docker image

./scripts/install_dependencies

docker build -t httpserver .

docker save -o <save image to path> <image name>

#ssh -oStrictHostKeyChecking=no -i "$ssh_key_file" "${ssh_user}@${server_ip}" \
#  "docker run httpserver"
