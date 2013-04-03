# Substitute

Command line utility for substitute string in file.

## Usage ##

    substi [options] filename old-string [new-string]

Options:

- `-remove` - Remove `old-string` from file.
- `-nobackup|-rewrite` - Don't create backup file `<filename.original>` or rewrite backup file if exists.

Use only ANSI characters and/or c/c++ escape sequences.
Supported: \\n \\t \\v \\b \\r \\f \\a \\\\ \\? \\' \\\" \\xH \\xHH \\0.

## Examples ##

	substi file.txt hi hello
	substi file.txt "General Public License" GPL
	substi -rewrite file.txt \t "    "
	substi -nobackup file.txt \r\n \n
	substi -remove file.txt \0

## Download ##

[Win32 exe files](http://ge.tt/9xoIssc)
