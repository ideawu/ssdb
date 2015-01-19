FROM ubuntu
MAINTAINER wendal "wendal1985@gmail.com"

# Set the env variable DEBIAN_FRONTEND to noninteractive
ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && \
  apt-get install -y --force-yes git make gcc g++ && apt-get clean && \
  git clone --depth 1 https://github.com/ideawu/ssdb.git ssdb && \
  cd ssdb && make && make install && cp ssdb-server /usr/bin && \
  apt-get remove -y --force-yes git make gcc g++ && \
  apt-get autoremove -y && \
  rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/* && \
  cp ssdb.conf /etc && cd .. && yes | rm -r ssdb

RUN mkdir -p /var/lib/ssdb && \
  sed \
    -e 's@home.*@home /var/lib@' \
    -e 's/loglevel.*/loglevel info/' \
    -e 's@work_dir = .*@work_dir = /var/lib/ssdb@' \
    -e 's@pidfile = .*@pidfile = /run/ssdb.pid@' \
    -e 's@level:.*@level: info@' \
    -e 's@ip:.*@ip: 0.0.0.0@' \
    -i /etc/ssdb.conf


ENV TZ Asia/Shanghai
EXPOSE 8888
VOLUME /var/lib/ssdb
ENTRYPOINT /usr/bin/ssdb-server /etc/ssdb.conf