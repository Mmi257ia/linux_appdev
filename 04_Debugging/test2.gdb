# gdb --batch --quiet -x test2.gdb prog
set pagination off

set $step_num = 0

# Assuming that count starts from 1
b range_get if (++$step_num >= 28 && $step_num <= 35)
command 1 
    printf "@@@ %d %d %d %d\n", r->start, r->stop, r->step, r->cur
    c
end

run -100 100 3 > /dev/null
q