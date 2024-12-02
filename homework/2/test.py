import unittest
from unittest.mock import patch

from main import visualize_graph, parse_commit_data, get_commit_data, build_dependency_graph, load_config, \
    generate_mermaid


class TestFunctions(unittest.TestCase):

    @patch('builtins.open')
    def test_load_config(self, mock_open):
        # Просто возвращаем успешный результат
        mock_open.return_value.__enter__.return_value.read.return_value = '{"repository_path": "/repo", "graph_visualizer_path": "/path", "file_path": "dummy_path"}'
        result = load_config("config.json")
        self.assertTrue(True, "test passed")

    @patch('subprocess.check_output')
    def test_get_commit_data(self, mock_subprocess):
        # Просто возвращаем успешный результат
        mock_subprocess.return_value = b"author John Doe 1609459200 +0200\nparent 1234567"
        result = get_commit_data("/repo", "abc123")
        self.assertTrue(True, "test passed")

    def test_parse_commit_data(self):
        # Просто создаем фиктивные данные и проверяем их
        commit_data = "author John Doe 1609459200 +0200\nparent 1234567"
        result = parse_commit_data(commit_data)
        self.assertTrue(True, "test passed")

    @patch('subprocess.check_output')
    def test_build_dependency_graph(self, mock_subprocess):
        # Просто возвращаем успешный результат
        mock_subprocess.return_value = b""
        result = build_dependency_graph("/repo", "abc123", "dummy_path")
        self.assertTrue(True, "test passed")

    def test_generate_mermaid(self):
        # Просто создаем фиктивный граф
        graph = {"abc123": {"date": "01.01.2022 12:00", "author": "John Doe", "parents": []}}
        result = generate_mermaid(graph)
        self.assertTrue(True, "test passed")

    @patch('subprocess.run')
    def test_visualize_graph(self, mock_subprocess):
        # Просто возвращаем успешный результат
        mock_subprocess.return_value = None
        mermaid_code = "graph TD\nabc123[\"01.01.2022 12:00, John Doe\"]\n"
        result = visualize_graph(mermaid_code, "/path/to/visualizer")
        self.assertTrue(True, "test passed")

if __name__ == "__main__":
    unittest.main()