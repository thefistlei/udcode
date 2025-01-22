# UD Code
UD Code, powered by ollama LLM, performs automated source code analysis. It recursively traverses a given directory, reads each source code file, and queries the LLM with the code for interpretation. The generated documentation is saved as Markdown files mirroring the original file.

## Installation
On windows, run generate_sln.bat.

## Documentation
First, installation [ollama](https://github.com/ollama/ollama) on local mathine, then run udcode.

```cpp
.\udcode "e:/test"
```

![GIF演示](https://github.com/thefistlei/uncode/blob/main/run.gif)

## Example
![avatar](https://github.com/thefistlei/uncode/blob/main/example.jpg)

- To support more source file type, modify the JSON Configuration File `config.json`.
For `language_type`, the key is the file extension, the value is the md code format.

```json
    "source_file_value": {
        ".c": "c",
        ".cpp": "cpp",
        ".py": "py",
        ".java": "java",
        ".rs": "rust"
    },
```

- To support other lanuage, modify the JSON Configuration File `config.json`.
First, identify the custom language name with your native lanuage command thas is send to llm. Ensure the `language_type` is set accordingly or it will use chinese as default.

```json
    "language_type": "zh",
    "language_value": {
        "zh": "以中文回答, 解释下面代码",
        "en": "explain the code"
    }
```

## Todo

- The code files are being split at the byte level, which can cause code to be broken at arbitrary points. A more suitable approach is to split the code at the function level.







