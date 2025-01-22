# UD Code
UD Code, powered by ollama LLM, performs automated source code analysis. It recursively traverses a given directory, reads each source code file, and queries the LLM with the code for interpretation. The generated documentation is saved as Markdown files mirroring the original file.

## Installation
On windows, run generate_sln.bat.

## Documentation
First, installation ollama on local mathine, then run udcode.

```cpp
.\udcode "e:/test"
```

![GIF演示](https://github.com/thefistlei/uncode/blob/main/run.gif)

## Example
![avatar](https://github.com/thefistlei/uncode/blob/main/example.jpg)

- To support other lanuage and source file type, modify the content to file config.json.

## Todo

- The code files are being split at the byte level, which can cause code to be broken at arbitrary points. A more suitable approach is to split the code at the function level.







