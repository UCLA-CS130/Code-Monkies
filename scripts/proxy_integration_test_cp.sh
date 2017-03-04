#!/bin/sh

TEST_FILE="/tmp/code-monkies-test"
CONFIG_FILE="/tmp/base_config-$(date +'%s')"
PORT=80
PORT2=8080
EXIT_STATUS=0

# don't clobber pre-existing files
while [ -e $CONFIG_FILE ]; do
  CONFIG_FILE="/tmp/base_config-$(date +'%s')-$RANDOM"
done

CONFIG=$(cat <<'CONFIG_END'
# This is a comment.

port 80;  # This is also a comment.

path / StaticHandler {
  root scripts;
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
  host_port 80;
}

path /delay DelayHandler {}

# Default response handler if no handlers match.
# TODO this doesnt work yet
default NotFoundHandler {}
CONFIG_END
)


printf '%s' "$CONFIG" > $CONFIG_FILE
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
bin/webserver $CONFIG_FILE &
sleep .5

# integration test for reverse proxy
direct_response="$(curl --silent -I localhost:${PORT}/test)"
echo "Direct response:"
echo "$direct_response"



killall webserver
rm $CONFIG_FILE
rm $TEST_FILE
exit $EXIT_STATUS
