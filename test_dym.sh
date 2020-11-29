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
	echo 'test_file'
	actual=`./dym -f test.txt statr`
	assert_eq "start" "$actual"
}

test_verbose() {
	echo 'test_verbose'
	actual=`./dym -v -f test.txt statr`
	assert_eq "start 2" "$actual"
}

test_multiple_matches() {
	echo 'test_multiple_matches'
	actual=`./dym -c 2 -f test.txt statr | tr "\n" " " | awk -e '{ print $1, $2 }'`
	assert_eq "start status" "$actual"
}

test_everything() {
	echo 'test_everything'
	actual=`./dym -c 2 -f test.txt -v statr | tr "\n" " " | awk -e '{ print $1, $2, $3, $4 }'`
	assert_eq "start 2 status 2" "$actual"
}

test_ignore_case() {
	echo 'test_ignore_case'
	actual=`./dym -v -i -f test.txt StATr`
	assert_eq "start 2" "$actual"
}

test_explicit_list() {
	echo 'test_explicit_list'
	actual=`./dym -e 'stop start restart status' statr`
	assert_eq "start" "$actual"
}

test_explicit_list_with_delim() {
	echo 'test_explicit_list_with_delim'
	actual=`./dym -F ',' -e 'stop,start,restart,status' statr`
	assert_eq "start" "$actual"
}

test_explicit_list_with_delim_2() {
	echo 'test_explicit_list_with_delim_2'
	actual=`./dym -F ',' -e 'this,is,a,test' 'tset'`
	assert_eq "test" "$actual"
}

test_unicode() {
	echo 'test_unicode'
	actual=`./dym -v -i -e 'über uber' 'Über'`
	assert_eq "über 0" "$actual"
}

test_explicit_list_2() {
	echo 'text_explicit_list_2'
	actual=`./dym -e 'vpn disconnect rdp meeting standup' meetign`
	assert_eq "meeting" "$actual"
}

test_explicit_list_with_preceding_and_trailing_spaces() {
	echo 'text_explicit_list_with_trailing_spaces'
	actual=`./dym -e '  vpn disconnect rdp meeting standup   ' meetign`
	assert_eq "meeting" "$actual"
}

test_edit_distance() {
	echo 'test_edit_distance'
	actual=`./dym -d -e 'abc' 'ade'`
	assert_eq "2" "$actual"
}

test_basic
test_file
test_verbose
test_multiple_matches
test_everything
test_ignore_case
test_explicit_list
test_explicit_list_with_delim
test_unicode
test_explicit_list_2
test_edit_distance
test_explicit_list_with_preceding_and_trailing_spaces
test_explicit_list_with_delim_2
echo 'All tests passed!'
