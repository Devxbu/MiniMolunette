# MiniMolunette - C Project Tester

MiniMolunette is a command-line tool designed to test C programming exercises, for piscine projects. It automates the process of compiling and testing your C code against expected outputs.

## Features

- Automated testing of C projects
- Color-coded test results
- Detailed diff output for failed tests
- Support for multiple test cases
- Easy to set up and use

## Prerequisites

- GCC (GNU Compiler Collection)
- Make
- Bash (for the alias setup)

## Installation

1. Clone this repository:

   ```bash
   git clone https://github.com/yourusername/miniMolunette.git
   cd miniMolunette
   ```
2. Build the project:

   ```bash
   make
   ```

## Setup Alias

To make it easier to use MiniMolunette from any directory, add this alias to your shell configuration file (~/.zshrc or ~/.bashrc):

```bash
alias miniMolunette='function _miniMolunette() {
    local project_dir=$(pwd)
    local project_name=$(basename "$project_dir" | tr "[:upper:]" "[:lower:]" | grep -o "c[0-9]\+")
  
    if [ -z "$project_name" ]; then
        echo "Error: Could not determine project name from directory"
        echo "Make sure your directory name contains the project name (e.g., C08, c09)"
        return 1
    fi
  
    /path/to/miniMolunette/bin/main "$project_name"
}; _miniMolunette'
```

Then reload your shell configuration:

```bash
source ~/.zshrc  # or source ~/.bashrc if using bash
```

## Usage

1. Navigate to your project directory:

   ```bash
   cd /path/to/your/cXX_project  # e.g., /Users/me/Projects/C08
   ```
2. Run MiniMolunette:

   ```bash
   miniMolunette
   ```

   The tool will automatically detect the project name from the directory (e.g., "c08" from "C08").

## Project Structure

Your project directory should have the following structure:

```
project_root/
├── c00/          # Exercise 00
│   ├── ft_putchar.c
│   └── ...
├── c01/          # Exercise 01
│   ├── ft_ft.c
│   └── ...
└── ...
```

## How It Works

1. The tool looks for exercise directories (c00, c01, etc.) in your current directory
2. For each exercise, it compiles your code and runs the tests
3. It compares the output with expected results
4. Displays a summary of passed/failed tests

## Troubleshooting

- **Compilation errors**: Make sure your code compiles without errors
- **Incorrect output**: Check the diff output to see how your output differs from expected
- **Missing exercises**: Ensure your exercise directories follow the cXX naming convention

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
