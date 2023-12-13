sleep 5 &

jobs
fg %1
bg %2
kill %3

ctrl + z
bg

kill -19 %3
kill -18 %3
