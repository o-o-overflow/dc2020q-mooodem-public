FROM ubuntu:18.04
ENV TERM linux
run apt-get -qq update && apt-get install -qq --no-install-recommends python python-pip python-setuptools libfftw3-dev socat libffi-dev
run apt-get install -qq --no-install-recommends build-essential python-dev
run apt-get install -qq --no-install-recommends qemu-system-x86 

COPY requirements.txt /requirements.txt
run pip install -r requirements.txt

COPY exploit1.py /
COPY check1.py /
COPY minimodem /
COPY wrapper_modem /
