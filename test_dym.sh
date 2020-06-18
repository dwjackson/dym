#!/bin/sh

assert_eq() {
	expected="$1"
	actual="$2"
	if [ "$actual" != "$expected" ]
	then
		echo "FAIL: Expected '$expected', got '$actual'"
		exit 1
	fi
}

test_basic() {
	actual=`./dym statr <<EOF
stop
start
restart
status
EOF`
	assert_eq "start" "$actual"
}

test_file() {
	actual=`./dym -f test.txt statr`
	assert_eq "start" "$actual"
}

test_verbose() {
	actual=`./dym -v -f test.txt statr`
	assert_eq "start 2" "$actual"
}

test_multiple_matches() {
	actual=`./dym -c 2 -f test.txt statr | tr "\n" " " | awk -e '{ print $1, $2 }'`
	assert_eq "start status" "$actual"
}

test_everything() {
	actual=`./dym -c 2 -f test.txt -v statr | tr "\n" " " | awk -e '{ print $1, $2, $3, $4 }'`
	assert_eq "start 2 status 2" "$actual"
}

test_basic
test_file
test_verbose
test_multiple_matches
test_everything
echo 'All tests passed!'
