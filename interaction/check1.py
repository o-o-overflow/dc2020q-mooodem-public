#!/usr/bin/env python
from pwn import *
import struct
import argparse
import codecs
import hashlib

# sha256sum ../service/src/bbs/BBS.ZIP
BBS_ZIP_sha256 = '490ef87e07a8628058d31ba8dfb1731410012516cda929ff15d1c79bfec1e09b'

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('host')
    ap.add_argument('port', type=int)
    ap.add_argument('--full_check', action='store_true')
    args = ap.parse_args()
    # r = remote(args.host, args.port)
    r = process(['./wrapper_modem', args.host, str(args.port)])
    r.timeout = 600

    def check_recv(x):
        data = r.recvuntil(x)
        assert(x in data), ('Failed to recv %s' % (repr(x)))
        return data

    print('Waiting for greeting')
    check_recv(b'What is your name?')

    print('Sending name')
    r.send(b'Checker\n')
    check_recv(b'Selection: ')

    print('Selecting file menu')
    r.send(b'F\n')
    check_recv(b' to cancel): ')

    if args.full_check:
        print('Selecting file')
        r.send(b'2\n')

        check_recv(b'Sending...')
        check_recv(b'\r\n')
        print('Downloading...')
        data = check_recv(b'\r\n')

        # Check that we returned to the menu
        check_recv(b'Selection: ')

        # Check hash of the downloaded zip file
        data = b''.join(data.rstrip(b'\r\n').split())
        data = codecs.decode(data, 'hex')
        # with open('output.bin', 'wb') as f:
            # f.write(sdata)

        print('Checking sha256')
        assert hashlib.sha256(data).hexdigest() == BBS_ZIP_sha256, 'Incorrect sha256 of BBS.ZIP!'
    print('All checks passed')

if __name__ == '__main__':
    main()
