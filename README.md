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
        print("Usage: python find_duplicates.py <directory>")
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