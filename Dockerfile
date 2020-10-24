FROM ubuntu:devel

# Use local mirror to get packages faster
ARG APT_PROXY="None"
RUN if [ "${APT_PROXY}" != "None" ]; then echo "Acquire::http { Proxy \"http://${APT_PROXY}:3142\"; };" >> /etc/apt/apt.conf.d/02-apt-proxy; fi

ENV DEBIAN_FRONTEND noninteractive
RUN sed -i '/deb-src/s/^# //' /etc/apt/sources.list \
 && apt update \
 && apt install -y \
     clang++ \
 && rm -rf /var/lib/apt/lists/*

COPY . /app/
WORKDIR /app
RUN ./build.sh

FROM ubuntu:devel

WORKDIR /app
COPY --from=0 /app/rtx rtx
COPY --from=0 /app/earthmap.jpg earthmap.jpg
