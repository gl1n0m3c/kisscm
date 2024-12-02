# Задание 1

## Описание проекта

Разработан эмулятор для языка оболочки ОС. Работа
эмулятора максимально похожа на сеанс shell в UNIX-подобной ОС.
Эмулятор запускается из реальной командной строки, а файл с
виртуальной файловой системой не распаковывается у пользователя.
Эмулятор принимает образ виртуальной файловой системы в виде файла формата
zip. Эмулятор работает в режиме GUI.

## Основные функции

- **ls**: Выводит все файлы и дирректории в текущем или переданном каталоге.
- **cd**: Переход из текущей директории в указанную
- **exit**: Выход из эмулятора
- **touch**: Обновление временной метки файла
- **chmod**: Выдача прав файлу

## Как использовать

```bash
python main.py [-h] zip_path log_path
```

## Тестирование

Весь функционал визуализатора покрыт тестами.