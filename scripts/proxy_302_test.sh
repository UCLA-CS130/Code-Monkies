#!/bin/sh

# The url in this test (ucla.edu) issues a 302 to redirect to www.ucla.edu

CONFIG_FILE="/tmp/base_config-$(date +'%s')"
PORT=8080
EXIT_STATUS=0

# don't clobber pre-existing files
while [ -e $CONFIG_FILE ]; do
  CONFIG_FILE="/tmp/base_config-$(date +'%s')-$RANDOM"
done

CONFIG=$(cat <<'CONFIG_END'
# This is a comment.

port 8080;  # This is also a comment.

path / StaticHandler {
  root /opt/static;
}

path /echo EchoHandler {}

path /status StatusHandler {}

path /proxy ProxyHandler {
  host ucla.edu;
  host_port 80;
}

path /delay DelayHandler {}

# Default response handler if no handlers match.
# TODO this doesnt work yet
default NotFoundHandler {}
CONFIG_END
)

printf '%s' "$CONFIG" > $CONFIG_FILE

if [ $? -ne 0 ]; then
  echo "Failed to write to config file $CONFIG_FILE - cannot test."
  exit 1
fi

# cd into directory of script, then out 1 dir to get to top level of project
cd $(dirname $0) && cd ..

if ! [ -e "bin/webserver" ]; then
  make
  if [ $? -ne 0 ]; then
    echo "Build failed - cannot test."
    exit 1
  fi
fi

# run server in background
bin/webserver $CONFIG_FILE 2>&1 >/dev/null &

# integration test for reverse proxy
sleep .5
proxy_response="$(curl --silent -I localhost:${PORT}/proxy | head -n 1 | tr -d '\r\n')"

if [ "$proxy_response" != "HTTP/1.1 200 OK" ]; then
  echo "302 Reverse Proxy Test failed - got different response than expected."
  echo "$proxy_response"
  EXIT_STATUS=1
else
  echo "302 Reverse Proxy Test Passed."
fi

killall webserver

rm $CONFIG_FILE

exit $EXIT_STATUS
