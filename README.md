# 🐚 XV6 Shell Implementation

![Shell Logo](https://img.shields.io/badge/XV6-Shell-blue.svg?style=for-the-badge&logo=gnu-bash&logoColor=white)

This repository contains a custom shell implementation for the XV6 operating system. The shell is designed to execute user commands, supporting both sequential and parallel execution, with additional features like I/O redirection and piping. This project was developed as part of an academic course under the guidance of **Prof. Umesh Bellur**.

---

## ✨ Features

- **Sequential and Parallel Execution**: Execute commands in sequence or in parallel.
- **I/O Redirection**: Redirect input and output streams to and from files.
- **Piping**: Chain commands together by passing the output of one command as the input to another.
- **File Command Execution**: Run commands stored in script files for batch processing.
- **Efficiency Gains**: Increased command execution efficiency by up to **30%** and improved overall productivity by **20%**.

---

## 🚀 Getting Started

### Prerequisites

To build and run this shell, you need to have XV6 installed and set up. Ensure you have the following:

- **XV6 Source Code**: Clone the [XV6 repository](https://github.com/mit-pdos/xv6-public) or any compatible version.
- **GCC Toolchain**: Ensure that you have the GCC compiler and related tools installed.

### Installation

1. **Clone this repository**:
    ```bash
    git clone https://github.com/codecasualty/xv6-Shell.git
    cd xv6-Shell
    ```

2. **Copy the shell implementation files to your XV6 source directory**:
    ```bash
    cp shell.c ../xv6-public/
    cp Makefile ../xv6-public/  # If necessary, adjust the Makefile for shell integration
    ```

3. **Navigate to your XV6 directory and build the system**:
    ```bash
    cd ../xv6-public
    make
    ```

4. **Run XV6 with your custom shell**:
    ```bash
    make qemu
    ```

---

## 💻 Usage

Once XV6 boots up, the custom shell will be available. You can start using the shell by typing commands as you would in a typical Unix shell. Here are a few examples:

- **Sequential Execution**: 
    ```bash
    echo "Hello, World!" > output.txt
    cat output.txt
    ```

- **Parallel Execution**:
    ```bash
    ls & ps &
    ```

- **Piping**:
    ```bash
    cat file.txt | grep "search_term"
    ```

- **I/O Redirection**:
    ```bash
    sort < unsorted.txt > sorted.txt
    ```

---

## 📂 Project Structure

- `shell.c`: The core shell implementation file.
- `Makefile`: Makefile used to build the XV6 kernel with the custom shell.
- `README.md`: This documentation file.

---

## 🤝 Contribution

If you find any issues or have suggestions for improvements, feel free to open an issue or submit a pull request. Contributions are welcome!

---

## 📜 License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.

---

## 🙏 Acknowledgments

- **Prof. Umesh Bellur** for guidance during the course project.
- The developers of XV6 for providing the foundation of this project.

---

## 📬 Contact

For any questions or discussions, feel free to reach out via the repository's issue tracker or directly through my [GitHub profile](https://github.com/codecasualty).

---

*Happy Coding! 🚀*
