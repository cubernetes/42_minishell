#!/bin/sh

cd -- "$(git rev-parse --show-toplevel)"
cd tester4
./tester
read -p "Press enter to continue" choice

cd ../tester2
python3 minitester.py -c -a
read -p "Press enter to continue" choice

cd ../tester3
bash test.sh all
read -p "Press enter to continue" choice
