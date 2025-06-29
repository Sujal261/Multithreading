## 🧩 Explanation of Key Parts

| Label | Part                  | Explanation |
|-------|-----------------------|-------------|
| **B** | `#include <thread>`   | The multithreading features like `std::thread` come from the `<thread>` header (new in C++11). Without it, `std::thread` won't work. |
| **C** | `hello()` function     | Every thread must start with a function. You cannot just write code inline—threads need a starting point, like `main()` for the main thread, or `hello()` for the new one. |
| **D** | `std::thread t(hello);` | This creates a **new thread** that starts executing the `hello()` function concurrently with `main()`. So now, **two threads** are running: `main()` and `hello()`. |
| **E** | `t.join();`            | This makes `main()` **wait** for the `hello()` thread to finish. Without `join()`, the main thread might exit before `hello()` runs, terminating the program prematurely. |
