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
        print("Ошибка: Необходимо указать текст для баннера.")
        print("Пример использования: python banner.py 'Ваш текст здесь'")
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
