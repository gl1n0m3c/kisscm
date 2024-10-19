package config

import (
	"encoding/json"
	"os"
)

type Config struct {
	VisualizationToolPath string `json:"visualization_tool_path"`
	RepositoryPath        string `json:"repository_path"`
	FileHash              string `json:"file_hash"`
}

func InitConfig() Config {
	var config Config

	file, err := os.ReadFile("../config/config.json")
	if err != nil {
		panic(err)
	}

	if err := json.Unmarshal(file, &config); err != nil {
		panic(err)
	}

	return config
}
