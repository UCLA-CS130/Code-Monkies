#!/bin/sh
## Goal
# Write output of an echo call and a delay call to a file
# The delay is defaulted at 5 seconds, which will definitely
# be longer than the delay call on any machine

CONFIG_FILE="/tmp/base_config-$(date +'%s')"
OUTPUT_FILE="/tmp/output-$(date +'%s')"
PORT=8080
EXIT_STATUS=0

if [ $# -ne 1 ]; then
    echo "Usage: $0 num_requests"
    exit 1
fi

num_reqs=$(($1 - 1))

# don't clobber pre-existing files
while [ -e $CONFIG_FILE ]; do
  CONFIG_FILE="/tmp/base_config-$(date +'%s')-$RANDOM"
done

while [ -e $OUTPUT_FILE ]; do
  OUTPUT_FILE="/tmp/output-$(date +'%s')-$RANDOM"
done

CONFIG=$(cat <<'CONFIG_END'
# This is a comment.

port 8080;  # This is also a comment.

path / StaticHandler {
  root /opt/static;
}

path /echo EchoHandler {}

path /status StatusHandler {}

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

touch "$OUTPUT_FILE";

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

# Run delay first
curl --silent localhost:${PORT}/delay >> $OUTPUT_FILE &

# Run all requests
for i in `seq 1 $num_reqs`; do
    echo "Curling localhost:${PORT}/echo..."
    curl --silent localhost:${PORT}/echo >> $OUTPUT_FILE
    echo "Done"
done

# Wait for delay call to finish
sleep 10

EXPECTED_OUTPUT=""
for i in `seq 1 $num_reqs`; do
    EXPECTED_OUTPUT=`printf '%s' "GET /echo HTTP/1.1\r\nUser-Agent: curl/7.35.0\r\nHost: localhost:8080\r\nAccept: */*\r\n\r\n${EXPECTED_OUTPUT}"`
done
EXPECTED_OUTPUT=`echo "${EXPECTED_OUTPUT}Response delayed by 5 seconds"`
ACTUAL_OUTPUT=`cat $OUTPUT_FILE`

if [ "$EXPECTED_OUTPUT" != "$ACTUAL_OUTPUT" ]; then
  echo "Test failed - got different output than expected."
  echo "Expected:"
  printf '%s\n' "$EXPECTED_OUTPUT"
  echo "Got:"
  printf '%s\n' "$ACTUAL_OUTPUT"
  EXIT_STATUS=1
else
  echo "Test passed."
fi

killall webserver

rm $CONFIG_FILE
rm $OUTPUT_FILE

exit $EXIT_STATUS
