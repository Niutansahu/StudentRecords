# Student Record Management System

A console program in C for adding, viewing, searching, updating, and deleting student records. Data is saved to `students.dat` between runs.

## Project layout

```
StudentRecords/
├── .vscode/
│   ├── tasks.json          Build and run tasks (Ctrl+Shift+B)
│   ├── launch.json         Debug configuration (F5)
│   └── c_cpp_properties.json   IntelliSense settings
├── student_record_system.c Source code
├── build_and_run.bat       Build and run from any terminal
└── README.md
```

## One-time setup (Windows)

1. Install MSYS2 from https://www.msys2.org and open the **MSYS2 MINGW64** shell.
2. Install the compiler and debugger:
   ```
   pacman -S --needed mingw-w64-x86_64-gcc mingw-w64-x86_64-gdb
   ```
3. Add `C:\msys64\mingw64\bin` to your Windows PATH:
   Start menu > "Edit the system environment variables" > Environment Variables > Path > New.
   Close and reopen VS Code afterward.
4. Check that it works: `gcc --version` in a new terminal.
5. In VS Code, install the **C/C++** extension (Microsoft).

If MSYS2 is installed somewhere other than `C:\msys64`, update the paths in `.vscode/c_cpp_properties.json` and `.vscode/launch.json`.

## Build and run

- **Build:** press Ctrl+Shift+B.
- **Run:** Terminal > Run Task > `run`, or double-click `build_and_run.bat`.
- **Debug:** press F5. Click in the left margin to set breakpoints.

The program is built as `student_records.exe` in the project folder. Run it from a terminal with:

```
.\student_records.exe
```

## Notes

- Student records are stored in `students.dat`, which is created the first time you save.
- The program holds up to 100 students. Change `MAX_STUDENTS` in the source to raise the limit.
