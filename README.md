# DYM: Did you mean...?

DYM is a utility that does "fuzzy" string matching. Given a particular input
string and set of valid possible strings, DYM will find the closest match(es).
It can be used, for example, to suggest corrections for mistyped commands. It
does this by computing the
[edit distance](https://en.wikipedia.org/wiki/Edit_distance)--more specifically
the [Levenshtein distance](https://en.wikipedia.org/wiki/Levenshtein_distance)
of the input against each string in the valid set and finding the one with the
smallest edit distance from the input string.

## Usage

The simplest possible usage is to directly call the `dym` command and use
`stdin` for the input. Each possible correct value against which to match must
be on its own line.

```sh
$ dym 'statr'
start
stop
restart
```

By default, `dym` will print the closest match in response.

## Options

The `dym` command supports the following command line options:

* `-m [MESSAGE]` print a message before the match(es)
* `-f [FILENAME]` read the list of possible matches from a file
* `-c [COUNT]` return up to COUNT closest matches, ties printed alphabetically
* `-i` ignore case
* `-v` verbose, print the edit distance from the match(es)
* `-V` print version information
* `-h` print help

## Examples

To find the closest match...

from an explicit list:
```sh
$ dym -F ',' -e 'stop,start,status' 'statr'
```

reading from a file:

```sh
$ dym -f words 'statr'
```

To find the closest 3 matches:
```sh
$ dym -c 3 -f words 'statr'
```

## License

DYM is licensed under the MIT license.
