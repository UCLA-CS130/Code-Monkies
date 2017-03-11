#!/bin/bash

if [ $# -ne 3 ]; then
    echo "Usage: $0 server_ip ssh_user ssh_key_file"
    exit 1
fi

server_ip="$1"
ssh_user="$2"
ssh_key_file="$3"

cwd="$(pwd)"

archive_file="/tmp/archive.tgz"

# Navigate to top level directory.
# TODO this isn't really a legit of locating our position in a hfs but it's
# good enough for most sane use cases.
if [ ${cwd: -5} == "scripts" ]; then
    cd ..
elif [ ${cwd: -9} != "webserver" ]; then
    echo "This command must be run from the top level of the webserver repository."
    exit 1
else
    echo "${cwd: -5}"
    echo "${cwd: -9}"
fi

if [ -e "$archive_file" ]; then
    rm "$archive_file"
fi

tar czvf "$archive_file" .

echo "scp -i $ssh_key_file $archive_file ${ssh_user}@${server_ip}"
scp -i "$ssh_key_file" "$archive_file" "${ssh_user}@${server_ip}"
