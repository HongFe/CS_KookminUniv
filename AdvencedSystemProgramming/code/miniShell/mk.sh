#!/bin/bash

p="/root/syspro/bin/"

gcc -o "mini" "miniShell.c"

gcc -o $p"od" $p"od.c"
gcc -o $p"pwd" $p"pwd.c"
gcc -o $p"ln" $p"ln.c"
gcc -o $p"touch" $p"touch.c"
gcc -o $p"mv" $p"mv.c"
gcc -o $p"who" $p"who.c"
gcc -o $p"ls" $p"ls.c"
gcc -o $p"chmod" $p"chmod.c"
gcc -o $p"find" $p"find.c"
gcc -o $p"cp" $p"cp.c"
gcc -o $p"cat" $p"cat.c"
gcc -o $p"rm" $p"rm.c"


