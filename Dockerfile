FROM archlinux:latest

# setup mirror
RUN pacman -Syy --noconfirm pacman-contrib
RUN curl 'https://www.archlinux.org/mirrorlist/?country=CN&protocol=https&ip_version=4' \
        | sed 's/^#Server/Server/' > /tmp/mirrorlist
RUN rankmirrors -n 3 /tmp/mirrorlist > /etc/pacman.d/mirrorlist

# install depdenency
RUN pacman -Syu --noconfirm
RUN pacman -S --noconfirm clang gcc cmake git make\
      tcpdump wireshark-cli socat openbsd-netcat\
      doxygen graphviz man-db man-pages-zh_cn diffutils

# clear cache
RUN pacman -Scc --noconfirm

RUN ln -sf /usr/share/zoneinfo/Asia/Shanghai /etc/localtime
