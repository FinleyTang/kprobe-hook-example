cmd_/home/ftang/Kprobe-hooker/modules.order := {   echo /home/ftang/Kprobe-hooker/Kprobe-tcp.ko; :; } | awk '!x[$$0]++' - > /home/ftang/Kprobe-hooker/modules.order
