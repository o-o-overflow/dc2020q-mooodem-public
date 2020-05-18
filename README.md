# DEFCON 2020 Quals MOOODEM

```
::::    ::::   ::::::::   ::::::::   ::::::::  :::::::::  :::::::::: ::::    :::: 
+:+:+: :+:+:+ :+:    :+: :+:    :+: :+:    :+: :+:    :+: :+:        +:+:+: :+:+:+
+:+ +:+:+ +:+ +:+    +:+ +:+    +:+ +:+    +:+ +:+    +:+ +:+        +:+ +:+:+ +:+
+#+  +:+  +#+ +#+    +:+ +#+    +:+ +#+    +:+ +#+    +:+ +#++:++#   +#+  +:+  +#+
+#+       +#+ +#+    +#+ +#+    +#+ +#+    +#+ +#+    +#+ +#+        +#+       +#+
#+#       #+# #+#    #+# #+#    #+# #+#    #+# #+#    #+# #+#        #+#       #+#
###       ###  ########   ########   ########  #########  ########## ###       ###

        w e l c o m e   t o   t h e   m o o o d e m   b b s   s y s t e m
```

This challenge is a simple service in the style of an old school BBS system,
running on DOS, with a Bell 202 style modem frontend--players must interface
with the service by sending and receiving audio.

* [gcc-ia16](https://github.com/tkchia/gcc-ia16) toolchain is used to build the service
* Service runs on DOS inside QEMU, communicates via serial port
* [minimodem](https://github.com/kamalmostafa/minimodem) is used to encode/decode

The service provides a file download function that allows users to download
whitelisted files. Players are expected to download and reverse the binary of
the service which is provided on a disk image that can be easily booted in
QEMU.

Vulnerability
-------------
There is an unbound read in the bulletin listing functionality which allows an
attacker to control the stack if they are able to get admin access and create
a bulletin. Admin access is guarded by a hardcoded password in the binary.

Exploit
-------
The reference exploit redirects control flow into the `show_random_quote`
function at the point just before opening QUOTES.TXT, but instead of
QUOTES.TXT it loads FLAG.TXT.
