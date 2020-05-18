from ubuntu:18.04

# example:
# run apt-get -qq update && apt-get install -qq --no-install-recommends xinetd rsyslog qemu-system-x86 mtools libfftw3-dev
run apt-get -qq update && apt-get install -qq --no-install-recommends rsyslog qemu-system-x86 mtools libfftw3-dev socat xinetd
# python3-pip, etc.

run apt-get install -qq --no-install-recommends pv

# Note: anything that goes in public_files must be pre-built and checked in git
copy BBS.IMG /BBS.IMG

# the deployment tester will pass in the flag from the yaml
ARG THE_FLAG="OOO{this is a test flag}"
RUN touch /flag && chmod 644 /flag && echo $THE_FLAG > /flag

# Copy the real flag into the disk image
RUN mcopy -o -i /BBS.IMG /flag ::/FLAG.TXT
RUN rm /flag

# Just making sure qemu -snapshot works correctly
RUN chmod a-w /BBS.IMG

copy service.conf /service.conf
copy banner_fail /banner_fail
copy wrapper_modem /wrapper_modem
copy minimodem /minimodem
copy wrapper /wrapper
copy wrapper_qemu /wrapper_qemu
copy killondisconnect /killondisconnect

expose 5000
user nobody
cmd ["/usr/sbin/xinetd", "-syslog", "local0", "-dontfork", "-f", "/service.conf"]
