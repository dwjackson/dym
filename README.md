# DYM: Did you mean..?

DYM is a utility that does "fuzzy" string matching. Given a particular input
string and set of valid possible strings, DYM will find the closest match(es).
It can be used, for example, to suggest corrections for mistyped commands.

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

## License

DYM is licensed under the MIT license.
