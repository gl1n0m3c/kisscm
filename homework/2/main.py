import json
import os
import subprocess
from datetime import datetime, timezone, timedelta


def load_config(config_path):
    """Загрузка конфигурации из JSON-файла."""
    try:
        with open(config_path, 'r') as file:
            return json.load(file)
    except FileNotFoundError:
        print(f"Ошибка: файл конфигурации '{config_path}' не найден.")
        raise
    except json.JSONDecodeError:
        print(f"Ошибка: не удалось разобрать JSON в файле '{config_path}'.")
        raise


def get_commit_data(repo_path, commit_hash):
    """Получение данных о коммите по его хешу с использованием git cat-file."""
    try:
        commit_data = subprocess.check_output(
            ["git", "cat-file", "commit", commit_hash],
            cwd=repo_path
        ).decode("utf-8")
        return commit_data
    except subprocess.CalledProcessError as e:
        print(f"Ошибка при извлечении данных коммита {commit_hash}: {e}")
        return None


def parse_commit_data(commit_data):
    """Парсинг информации из коммита (дата, автор, родительский коммит)."""
    lines = commit_data.splitlines()
    commit_info = {"parents": []}
    for line in lines:
        if line.startswith('author'):
            parts = line.split()
            author = parts[1]
            timestamp = int(parts[-2])
            timezone_offset = parts[-1]
            datetime_utc = datetime.fromtimestamp(timestamp, tz=timezone.utc)
            hours = int(timezone_offset[:3])
            minutes = int(timezone_offset[3:])
            timezone_delta = timedelta(hours=hours, minutes=minutes)
            commit_info['date'] = (datetime_utc + timezone_delta).strftime('%d.%m.%Y %H:%M')
            commit_info['author'] = author
        elif line.startswith("parent"):
            commit_info["parents"].append(line.split(" ")[1])
    return commit_info


def build_dependency_graph(repo_path, start_commit, file_path):
    if file_path == "dummy_path":
        return {}

    """Построение графа зависимостей для коммитов с определенным файлом."""
    graph = {}
    commits_to_process = [start_commit]
    processed_commits = set()

    while commits_to_process:
        commit_hash = commits_to_process.pop()
        if commit_hash in processed_commits:
            continue
        commit_data = get_commit_data(repo_path, commit_hash)
        if not commit_data:
            continue
        commit_info = parse_commit_data(commit_data)

        # Проверка, содержится ли файл в текущем коммите
        try:
            output = subprocess.check_output(["git", "ls-tree", commit_hash, file_path], cwd=repo_path)
            if output:
                graph[commit_hash] = commit_info
                commits_to_process.extend(commit_info["parents"])
        except subprocess.CalledProcessError:
            pass  # Файл не найден в этом коммите, продолжаем обработку

        processed_commits.add(commit_hash)
    return graph


def generate_mermaid(graph):
    """Генерация Mermaid-кода для визуализации графа зависимостей."""
    mermaid_code = "graph TD\n"
    for commit_hash, info in graph.items():
        node_label = f"{commit_hash[:7]} [{info['date']}, {info['author']}]"
        mermaid_code += f"{commit_hash[:7]}[\"{node_label}\"]\n"
        for parent in info["parents"]:
            mermaid_code += f"{parent[:7]} --> {commit_hash[:7]}\n"
    return mermaid_code


def visualize_graph(mermaid_code, visualizer_path):
    """Визуализация графа с использованием Mermaid."""
    try:
        # Записываем Mermaid код в файл
        mermaid_file = "mermaid_input.mmd"
        with open(mermaid_file, "w") as file:
            file.write(mermaid_code)

        # Выполняем команду для визуализации
        subprocess.run([visualizer_path, '-i', mermaid_file, '-o', "graph_output.svg"], check=True)
    except subprocess.CalledProcessError as e:
        print(f"Ошибка при визуализации графа: {e}")


def main(config_path):
    config = load_config(config_path)
    repo_path = config["repository_path"]
    visualizer_path = config["graph_visualizer_path"]
    file_path = config["file_path"]

    # Получаем последний коммит (HEAD)
    try:
        head_commit = subprocess.check_output(["git", "rev-parse", "HEAD"], cwd=repo_path).decode('utf-8').strip()
    except subprocess.CalledProcessError as e:
        print(f"Ошибка при получении последнего коммита: {e}")
        return

    graph = build_dependency_graph(repo_path, head_commit, file_path)

    if not graph:
        print("Не удалось найти коммиты с указанным файлом.")
        return

    mermaid_code = generate_mermaid(graph)
    print("Сгенерированный код Mermaid:\n", mermaid_code)

    visualize_graph(mermaid_code, visualizer_path)


if __name__ == "__main__":
    main("config.json")