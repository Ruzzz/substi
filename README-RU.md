# substi

Замена строки в файле.

## Использование ##

    substi [опции] файл старая-строка [новая-строка]

Опции:

- `-remove` - Удалить из файла `старая-строка`.
- `-nobackup|-rewrite` - Не создавать резервный файл `<целевой_файл.original>` или переписать резервный файл, если он присутствует на диске.

Используйте тольсо ANSI символы и/или экранированные символы си/c++.
Поддерживаются: \\n \\t \\v \\b \\r \\f \\a \\\\ \\? \\' \\\" \\xH \\xHH \\0.

## Примеры ##

	substi file.txt hi hello
	substi file.txt "General Public License" GPL
	substi -rewrite file.txt \t "    "
	substi -nobackup file.txt \r\n \n
	substi -remove file.txt \0

## Скачать ##

[Исполняемые файлы](https://github.com/Ruzzz/Substitute/releases)
