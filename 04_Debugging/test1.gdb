# gdb --batch --quiet -x test1.gdb prog
set pagination off

b range_get if r->cur % 5 == 0
command 1
    printf "@@@ %d %d %d %d\n", r->start, r->stop, r->step, r->cur
    c
end

run 1 12 > /dev/null
q