#!/bin/sh

TEST_FILE_NAME=code-monkies-test
TEST_FILE="/tmp/${TEST_FILE_NAME}"
CONFIG_FILE="/tmp/base_config"
PORT=2020
PORT2=8080
EXIT_STATUS=0

CONFIG=$(cat <<'CONFIG_END'
# This is a comment.

port 2020;  # This is also a comment.

path / StaticHandler {
  root /tmp;
}

path /echo EchoHandler {}

path /status StatusHandler {}

path /delay DelayHandler {}

# Default response handler if no handlers match.
# TODO this doesnt work yet
default NotFoundHandler {}
CONFIG_END
)

CONFIG2=$(cat <<'CONFIG_END'
# This is a comment.

port 8080;  # This is also a comment.

path / StaticHandler {
  root scripts;
}

path /echo EchoHandler {}

path /status StatusHandler {}

path /proxy ProxyHandler {
  host localhost;
  host_port 2020;
}

path /delay DelayHandler {}

# Default response handler if no handlers match.
# TODO this doesnt work yet
default NotFoundHandler {}
CONFIG_END
)

echo "$CONFIG" > $CONFIG_FILE
echo "ABCDEFGH" > $TEST_FILE

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
sleep .5

direct_response="$(curl --silent localhost:${PORT}/${TEST_FILE_NAME})"
echo "$direct_response"

echo "$CONFIG2" > $CONFIG_FILE
bin/webserver $CONFIG_FILE 2>&1 >/dev/null &
sleep .5

proxy_response="$(curl --silent localhost:${PORT2}/proxy/${TEST_FILE_NAME})"
echo "$proxy_response"

if [ "$proxy_response" != "$direct_response" ]; then
  echo "Reverse Proxy Test failed - got different response than expected."
  EXIT_STATUS=1
else
  echo "Reverse Proxy Test Passed."
fi


killall webserver

rm $TEST_FILE
exit $EXIT_STATUS
