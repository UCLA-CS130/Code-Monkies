#!/bin/bash

# Deploy webserver to any remote server running Docker.
# Requires that local machine is also running Docker.

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

# Build binary

docker build -t httpserver.build .

docker run httpserver.build > binary.tar

cd deploy && tar xvf ../binary.tar  && cd ..

rm binary.tar

# Build server image

docker build -t httpserver deploy

# Deploy server image to remote host

docker save -o deploy_img httpserver

scp -oStrictHostKeyChecking=no -i "$ssh_key_file" deploy_img \
  "${ssh_user}@${server_ip}:~/"

rm deploy_img

# Run server image on remote host as daemon

ssh -oStrictHostKeyChecking=no -i "$ssh_key_file" "${ssh_user}@${server_ip}" \
  "docker load -i deploy_img && docker run -d -p 8080:8080 httpserver"
