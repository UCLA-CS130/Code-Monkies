#!/bin/sh

CONFIG_FILE="/tmp/base_config-$(date +'%s')"
PORT=8080
EXIT_STATUS=0

# don't clobber pre-existing files
while [ -e $CONFIG_FILE ]; do
  CONFIG_FILE="/tmp/base_config-$(date +'%s')-$RANDOM"
done

CONFIG=$(cat <<'CONFIG_END'
server {
  listen 8080;
  echo {
    /echo1;
    /echo2;
  }
  serve {
    /static1 /var/www/html;
    /static2 /home/html;
  }
}
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

good_response="GET /echo1 HTTP/1.1\r\nUser-Agent: Mozilla/4.0\r\n"
good_response="${good_response}Host: localhost:8080\r\nAccept: */*\r\n\r\n"
# turn C-style escape sequences into actual carriage returns and newlines
good_response=$(printf '%b' "$good_response")

response="$(curl --silent -A "Mozilla/4.0" localhost:${PORT}/echo1)"

if [ "$response" != "$good_response" ]; then
  echo "Test failed - got different response than expected."
  echo "Expected:"
  printf '%s\n' "$good_response"
  echo "Got:"
  printf '%s\n' "$response"
  EXIT_STATUS=1
else
  echo "Test passed."
fi

killall webserver

rm $CONFIG_FILE

exit $EXIT_STATUS
