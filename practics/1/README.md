# Задание 1
### Условие

Вывести отсортированный в алфавитном порядке список имен пользователей в файле passwd (вам понадобится grep).

### Код

```bash
cut -d':' -f1 etc/passwd
```

### Вывод

```
root
daemon
bin
sys
sync
games
man
lp
mail
news
uucp
proxy
www-data
backup
list
irc
gnats
nobody
systemd-network
systemd-resolve
messagebus
systemd-timesync
syslog
_apt
uuidd
tcpdump
gl1n0m3s
```



# Задание 2
### Условие

Вывести данные /etc/protocols в отформатированном и отсортированном порядке для 5 наибольших портов, как показано в примере ниже:

```
[root@localhost etc]# cat /etc/protocols ...
142 rohc
141 wesp
140 shim6
139 hip
138 manet
```

### Код

```bash
cat etc/protocols | sort -k2,2 -r -n | head -n 5 | awk '{print $2, $1}'
```

### Вывод

```
142 rohc
141 wesp
140 shim6
139 hip
138 manet
```



# Задание 3
### Условие

Написать программу banner средствами bash для вывода текстов, как в следующем примере (размер баннера должен меняться!):

```
[root@localhost ~]# ./banner "Hello from RTU MIREA!"
+-----------------------+
| Hello from RTU MIREA! |
+-----------------------+
```

Перед отправкой решения проверьте его в ShellCheck на предупреждения.

### Код

Файл `banner.py`:
```python
#!/usr/bin/env python3
import sys

def create_banner(text):
    text_length = len(text)
    border = '+' + '-' * (text_length + 2) + '+'
    print(border)
    print('| ' + text + ' |')
    print(border)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python banner.py <your_text>")
    else:
        input_text = sys.argv[1]
        create_banner(input_text)

```

Командная строка:
```bash
./banner.py "Hello from RTU MIREA!"
```

### Вывод

```
+-----------------------+
| Hello from RTU MIREA! |
+-----------------------+
```



# Задание 4
### Условие

Написать программу для вывода всех идентификаторов (по правилам C/C++ или Java) в файле (без повторений).

Пример для hello.c:

```
h hello include int main n printf return stdio void world
```

### Код

```bash
grep -E -o '\b[a-zA-Z_][a-zA-Z0-9_]*\b' hello.c | uniq
```

### Вывод

```
include
stdio
h
int
main
printf
Hello
World
n
return
```



# Задание 5
### Условие

Написать программу для регистрации пользовательской команды (правильные права доступа и копирование в /usr/local/bin).

Например, пусть программа называется reg:

```
./reg banner
```

В результате для banner задаются правильные права доступа и сам banner копируется в /usr/local/bin.

### Код

Файл `reg`
```bash
#!/bin/bash
 
chmod +x $1
sudo cp $1 /usr/local/bin
```

Командная строка:
```bash
./reg banner
```

### Вывод

Пустой



# Задание 6
### Условие

Написать программу для проверки наличия комментария в первой строке файлов с расширением c, js и py.

### Код

Файл `check_comment.py`
```python
#!/usr/bin/env python3
import os
import sys

def check_comment(directory):
    extensions = {
        '.c': '//',
        '.js': '//',
        '.py': '#'
    }

    for entry in os.scandir(directory):
        if entry.is_file():
            ext = os.path.splitext(entry.name)[-1]
            if ext in extensions:
                with open(entry.path, 'r') as f:
                    first_line = f.readline().strip()
                    if first_line.startswith(extensions[ext]):
                        print(f"{entry.name}: Комментарий присутствует")
                    else:
                        print(f"{entry.name}: Комментарий отсутствует")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: ./main.py <path_to_directory>")
    else:
        directory = sys.argv[1]
        check_comment(directory)


```

Командная строка:
```bash
./check_comment.py "./"
```

### Вывод

```
check_comment.py: Комментарий присутствует
example.c: Комментарий присутствует
example.py: Комментарий присутствует
example.js: Комментарий отсутствует
```



# Задание 7
### Условие

Написать программу для нахождения файлов-дубликатов (имеющих 1 или более копий содержимого) по заданному пути (и подкаталогам).

### Код

Файл `duplicates.py`
```python
#!/usr/bin/env python3
import os
import hashlib

def hash_file(filepath):
    hasher = hashlib.sha256()
    with open(filepath, 'rb') as f:
        buf = f.read(65536)  # Читаем файл блоками по 65536 байт, так как файлы могут быть больших рамеров
        while len(buf) > 0:
            hasher.update(buf)
            buf = f.read(65536)
    return hasher.hexdigest()

def find_duplicates(start_dir):
    hashes = {}
    duplicates = []

    # Итерируемся по всем вложеным директориям от переданной
    for dirpath, dirnames, filenames in os.walk(start_dir):
        for filename in filenames:
            filepath = os.path.join(dirpath, filename)
            file_hash = hash_file(filepath)
            if file_hash in hashes:
                hashes[file_hash].append(filepath)
            else:
                hashes[file_hash] = [filepath]

    # Ищем группы дубликатов
    for paths in hashes.values():
        if len(paths) > 1:
            duplicates.append(paths)

    return duplicates

if __name__ == "__main__":
    import sys
    if len(sys.argv) > 1:
        directory = sys.argv[1]
        found_duplicates = find_duplicates(directory)
        if found_duplicates:
            print("Найдены дубликаты:")
            for duplicate_group in found_duplicates:
                print("\nГруппа дубликатов:")
                for filepath in duplicate_group:
                    print(filepath)
        else:
            print("Дубликаты не найдены.")
    else:
        print("Usage: ./ find_duplicates.py <directory>")
```

Командная строка:
```bash
./duplicates.py "./"
```

### Вывод

```
Найдены дубликаты:

Группа дубликатов:
./dupl1.c
./dupl/dupl2.c
```



# Задание 8
### Условие

Написать программу, которая находит все файлы в данном каталоге с расширением, указанным в качестве аргумента и архивирует все эти файлы в архив tar.

### Код

Файл `archive.py`
```python
#!/usr/bin/env python3
import os
import tarfile
import sys


def archive(directory, extension):
    tar_filename = f"archive_{extension.replace('.', '')}.tar"

    with tarfile.open(tar_filename, "w") as tar:
        for file in os.listdir(directory):
            if file.endswith(extension):
                tar.add(os.path.join(directory, file))
    print(f"Архив {tar_filename} создан.")


if __name__ == "__main__":
    if len(sys.argv) < 3 or sys.argv[2][0] != ".":
        print("Usage: ./archive.py <directory> <extension> (with .)")
    else:
        directory, extension = sys.argv[1], sys.argv[2]
        archive(directory, extension)
```

Командная строка:
```bash
./archive.py "./" ".py"
```
```bash
ls
```

### Вывод

```
Архив archive_py.tar создан.
```
```
archive.py  archive_py.tar  example.c  example.js  example.py
```



# Задание 9
### Условие

Написать программу, которая заменяет в файле последовательности из 4 пробелов на символ табуляции. Входной и выходной файлы задаются аргументами.

### Код

Файл `spaces_to_tabs.py`
```python
#!/usr/bin/env python3
import sys


def spaces_to_tabs(input, output):
    try:
        with open(input) as file:
            content = file.read()

        content_tabs = content.replace("    ", "\t")

        with open(output, "w") as file:
            file.write(content_tabs)

        print(f"Файл '{input}' был обработан, результат сохранён в '{output}'.")
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: spaces_to_tabs.py <input_file> <output_file>")
    else:
        input, output = sys.argv[1], sys.argv[2]
        spaces_to_tabs(input, output)
```

Командная строка:
```bash
./spaces_to_tabs.py "spaces_to_tabs.py" "new_spaces_to_tabs.py"
```

### Вывод

```
Файл 'spaces_to_tabs.py' был обработан, результат сохранён в 'new_spaces_to_tabs.py'.
```



# Задание 10
### Условие

Написать программу, которая выводит названия всех пустых текстовых файлов в указанной директории. Директория передается в программу параметром.

### Код

Файл `empty.py`
```python
#!/usr/bin/env python3
import os
import sys


def empty(directory):
    try:
        files = os.listdir(directory)
        empty_files = []

        for file in files:
            file_path = os.path.join(directory, file)
            if os.path.isfile(file_path) and os.path.getsize(file_path) == 0:
                empty_files.append(file)

        if empty_files:
            print("Пустые текстовые файлы в директории:")
            for file in empty_files:
                print(file)
        else:
            print("Пустые текстовые файлы не найдены.")
    except Exception as e:
        print(f"Произошла ошибка: {e}")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Использование: ./empty.py <directory>")
    else:
        directory = sys.argv[1]
        empty(directory)
```

Командная строка:
```bash
./empty.py "./"
```

### Вывод

```
Пустые текстовые файлы в директории:
example.с
example.py
example.js
```
