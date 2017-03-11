#!/bin/bash

echo "Note that this utility must be run from the top level of the "
echo "Code-Monkies repository, as such: ./scripts/deploy.sh"
echo "If not called from here, it will not work."

if [ $# -ne 3 ]; then
    echo "Usage: $0 server_ip ssh_user ssh_key_file"
    exit 1
fi

server_ip="$1"
ssh_user="$2"
ssh_key_file="$3"

archive_file="archive.tgz"

# Make sure directory is empty to minimize transfer time.
make clean

if [ -e "$archive_file" ]; then
  echo "removing old archive file"
  rm "$archive_file"
fi

# Build docker image

#sudo ./scripts/install_and_run.sh

# need this to go into archive after un-tar-ing
cwd="${PWD##*/}"

tar czvf "$archive_file" "../$cwd"

# Copy this directory over to server
echo "scp -i $ssh_key_file $archive_file ${ssh_user}@${server_ip}:/tmp/$archive_file"
scp -oStrictHostKeyChecking=no -i "$ssh_key_file" "$archive_file" \
  "${ssh_user}@$server_ip:/tmp/$archive_file"

ssh -oStrictHostKeyChecking=no -i "$ssh_key_file" "${ssh_user}@${server_ip}" \
  "cd /tmp; tar zxvf $archive_file; cd $cwd; docker build -t httpserver deploy"

ssh -oStrictHostKeyChecking=no -i "$ssh_key_file" "${ssh_user}@${server_ip}" \
  "docker run httpserver"
