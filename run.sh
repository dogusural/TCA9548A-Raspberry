


tput setaf 1
./bus 0
tput sgr0
./tests/v1_tests

tput setaf 1
./bus 1
tput sgr0
sudo ./tests/v2_tests

tput setaf 1
./bus 2
tput sgr0
./tests/v3_tests

r=$(( $RANDOM % 3 ))
tput setaf 1
./bus $r
tput sgr0
./tests/ses_tests
