
gcc -Wall "$1" 2>&1 | grep "warning\|error:" | (echo "The number of errors is:" && wc -l)
