#!/usr/bin/env bash

set -e
set -x


temp_dir=$(mktemp -d)
trap "rm -rd $temp_dir" EXIT
g++ stego.cpp -g -o "$temp_dir/stego"
cp test.jpg "$temp_dir"
cd "$temp_dir"

# Tests
echo "Test: wrong arguments"
set +e
result=$(./stego foo bar 2>&1)
exit_code="$?"
set -e
[[ "$exit_code" -eq 1  ]]
[[ "$result" = "Usage: ./stego write|read|delete path/to/image.jpg [message to write]" ]] 

echo "Test: write clean & read"
./stego write test.jpg "Hello world"
result=$(./stego read test.jpg)
[[ "$result" = "Message: Hello world" ]]

echo "Test: write filled & read"
./stego write test.jpg "Hello world"
./stego write test.jpg "foo bar"
result=$(./stego read test.jpg)
[[ "$result" = "Message: foo bar" ]]

echo "Test: delete & read"
./stego delete test.jpg
set +e
result=$(./stego read test.jpg 2>&1)
exit_code="$?"
set -e
[[ "$exit_code" -eq 2  ]]
[[ "$result" = "Message not found!" ]]

echo "Test: delete & write & read"
./stego delete test.jpg
./stego write test.jpg "Hello world"
result=$(./stego read test.jpg)
[[ "$result" = "Message: Hello world" ]]

echo "Done!"
