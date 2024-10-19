package main

import (
	"file-git-mermaid/config"
	"file-git-mermaid/internal/commits"
	"fmt"
	"os"
)

func saveHashToFile(content string) error {
	return os.WriteFile("../output.md", []byte(content), 0644)
}

func main() {
	// Инициализация конфигурации
	cfg := config.InitConfig()
	fmt.Printf("Config: %#v\n", cfg)

	// Структура взаимодействия
	commitGraph := commits.NewGraphMaker(cfg)

	// Получение всех коммитов, связанных с файлом
	err := commitGraph.SetCommits()
	if err != nil {
		panic(fmt.Sprintf("Error getting commits: %v", err))
	}

	// Получение имени файла по его хешу
	err = commitGraph.SetFileName()
	if err != nil {
		panic(fmt.Sprintf("Error getting file name by hash: %v", err))
	}

	// Фильтруем коммиты по хешу файла
	filteredCommits, err := commitGraph.FilterCommitsByFileHash()
	if err != nil {
		panic(fmt.Sprintf("Error filtering commits: %v", err))
	}
	if len(filteredCommits) == 0 {
		fmt.Println("No commits found with the specified file hash.")
		return
	}

	// Построение графа зависимостей
	graph, err := commitGraph.BuildDependencyGraph(filteredCommits)
	if err != nil {
		panic(fmt.Sprintf("Error building dependency graph: %v", err))
	}

	// Генерация кода Mermaid
	mermaidCode := commitGraph.GenerateMermaid(graph)
	err = saveHashToFile(mermaidCode)
	if err != nil {
		panic(fmt.Sprintf("Error while saving file: %v", err))
	}

	fmt.Println("Success!")
}
