# Dockerfile to set up a 16-bit x86 GCC compiler toolchain.
# Setup:
#   docker build -t dockbuild-ia16 -f dockbuild-ia16.Dockerfile .
#   docker run --rm dockbuild-ia16 > dockbuild-ia16
#   chmod +x dockbuild-ia16
#   ./dockbuild-ia16 <cmd>

FROM dockbuild/ubuntu2004:latest
ENV DEFAULT_DOCKCROSS_IMAGE dockbuild-ia16

# For add-apt-repository
RUN apt-get update
RUN apt-get install -y software-properties-common

RUN add-apt-repository ppa:tkchia/build-ia16
RUN apt-get update
RUN apt-get install -y gcc-ia16-elf
