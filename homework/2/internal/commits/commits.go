package commits

import (
	"bytes"
	"file-git-mermaid/config"
	"fmt"
	"os/exec"
	"strings"
)

type GraphMaker interface {
	// Взаимодействие с коммитами

	SetCommits() error
	SetFileName() error
	FilterCommitsByFileHash() ([]string, error)
	GetParentCommits(commit, repoPath string) ([]string, error)

	// Взаимодействие с графом

	BuildDependencyGraph(filteredCommits []string) (map[string][]string, error)
	GenerateMermaid(graph map[string][]string) string
}

type graphMaker struct {
	config   config.Config
	fileName string
	commits  []string
}

func NewGraphMaker(config config.Config) GraphMaker {
	return &graphMaker{config: config}
}

func (g *graphMaker) SetCommits() error {
	cmd := exec.Command("git", "-C", g.config.RepositoryPath, "rev-list", "--all")
	output, err := cmd.Output()
	if err != nil {
		return err
	}

	g.commits = strings.Split(strings.TrimSpace(string(output)), "\n")
	return nil
}

func (g *graphMaker) SetFileName() error {
	cmd := exec.Command("git", "-C", g.config.RepositoryPath, "ls-tree", "-r", "HEAD")
	var out bytes.Buffer
	cmd.Stdout = &out
	if err := cmd.Run(); err != nil {
		return fmt.Errorf("failed to get file name by hash: %w", err)
	}

	lines := strings.Split(out.String(), "\n")
	for _, line := range lines {
		parts := strings.Fields(line)
		if len(parts) >= 3 && parts[2] == g.config.FileHash {
			g.fileName = parts[3]
			fmt.Printf("File Name: %s\n", g.fileName)
			return nil
		}
	}

	return fmt.Errorf("file with hash %s not found", g.config.FileHash)
}

func (g *graphMaker) getFileHashInCommit(commit string) (string, error) {
	cmd := exec.Command("git", "-C", g.config.RepositoryPath, "ls-tree", commit, g.fileName)
	output, err := cmd.Output()
	if err != nil {
		return "", err
	}
	parts := strings.Fields(string(output))
	if len(parts) < 3 {
		return "", fmt.Errorf("unexpected output: %s", output)
	}
	return strings.TrimSpace(parts[2]), nil
}

func (g *graphMaker) FilterCommitsByFileHash() ([]string, error) {
	var filteredCommits []string
	for _, commit := range g.commits {
		hash, err := g.getFileHashInCommit(commit)
		if err != nil {
			continue
		}
		if hash != "" {
			filteredCommits = append(filteredCommits, commit)
		}
	}
	return filteredCommits, nil
}

func (g *graphMaker) GetParentCommits(commit, repoPath string) ([]string, error) {
	cmd := exec.Command("git", "-C", repoPath, "rev-list", "--parents", "-n", "1", commit)
	output, err := cmd.Output()
	if err != nil {
		return nil, err
	}
	parts := strings.Fields(strings.TrimSpace(string(output)))
	if len(parts) < 2 {
		return []string{}, nil
	}
	return parts[1:], nil
}

func (g *graphMaker) BuildDependencyGraph(filteredCommits []string) (map[string][]string, error) {
	graph := make(map[string][]string)
	commitSet := make(map[string]bool)
	for _, commit := range filteredCommits {
		commitSet[commit] = true
	}

	for _, commit := range filteredCommits {
		parents, err := g.GetParentCommits(commit, g.config.RepositoryPath)
		if err != nil {
			return nil, err
		}
		for _, parent := range parents {
			if commitSet[parent] {
				graph[commit] = append(graph[commit], parent)
			}
		}
	}
	return graph, nil
}

func (g *graphMaker) GenerateMermaid(graph map[string][]string) string {
	var mermaid strings.Builder
	mermaid.WriteString("graph TD\n")
	for child, parents := range graph {
		for _, parent := range parents {
			mermaid.WriteString(fmt.Sprintf("    %s --> %s\n", parent, child))
		}
	}
	return mermaid.String()
}
