#!/bin/bash

if [[ $EUID -ne 0 ]]; then
    echo "This script must be run as root."
    exit 1
fi

# Install docker dependencies

apt-get install -y \
    linux-image-extra-$(uname -r) \
    linux-image-extra-virtual

apt-get install -y \
    apt-transport-https \
    ca-certificates \
    curl \
    software-properties-common

# Install docker

curl -fsSL https://download.docker.com/linux/ubuntu/gpg | \
    apt-key add - &>/dev/null

res="$(apt-key fingerprint 0EBFCD88 | \
    egrep '9DC8 5822 9FC7 DD38 854A *E2D8 8D81 803C 0EBF CD88')"

if [ -z "$res" ]; then
    echo "Invalid fingerprint on docker repository."
    apt-key del 0EBFCD88 &>/dev/null
    exit 1
fi

add-apt-repository \
    "deb [arch=amd64] https://download.docker.com/linux/ubuntu \
    $(lsb_release -cs) \
    stable" &>/dev/null

apt-get update -y

apt-get install -y docker-ce

docker build -t httpserver deploy

docker run httpserver > binary.tar

#docker run --rm -t -p 8080:8080 httpserver
