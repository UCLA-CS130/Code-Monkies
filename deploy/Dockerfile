FROM busybox:ubuntu-14.04

WORKDIR /opt/httpserver
COPY . /opt/httpserver
EXPOSE 8080:8080

CMD ["./bin/webserver", "base_config"]