# PolySort - The Adaptive Hybrid Sorting Algorithm 🧠

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![Languages](https://img.shields.io/github/languages/count/rambo1111/PolySort?color=informational)](https://github.com/rambo1111/PolySort)
[![Stars](https://img.shields.io/github/stars/rambo1111/PolySort?style=social)](https://github.com/rambo1111/PolySort/stargazers)

PolySort is an intelligent sorting framework that analyzes your data and picks the most efficient algorithm to sort it. Instead of a one-size-fits-all approach, it acts as a smart manager, delegating the work to the right tool for the job.

## 🤔 Why PolySort?

Standard sorting algorithms have strengths and weaknesses. **Quicksort** is fast but has a dreadful O(n²) worst case. **Merge Sort** is consistent but uses extra space. **Radix Sort** is lightning-fast for integers but useless for other data types.

PolySort combines their strengths and avoids their weaknesses. It examines the data first and chooses the best strategy, leading to optimized and reliable performance across diverse datasets.

---

## ⚙️ How It Works

The process is a simple two-phase approach: **Analyze** and **Execute**.

1.  **Input Array**: An unsorted array is passed to PolySort.
2.  **Heuristic Analysis Engine**: A lightweight engine inspects a sample of the array for key patterns:
    * **Is it nearly sorted?** ➡️ If yes, it's a job for **Merge Sort** (as a stand-in for Timsort).
    * **Is it all non-negative integers?** ➡️ If yes, **Radix Sort** will be the fastest.
    * **Does it have many duplicates (low cardinality)?** ➡️ **Quicksort** is a great choice.
    * **Is it generic, random data?** ➡️ The robust, default **Quicksort** is chosen.
3.  **Algorithm Execution**: The chosen algorithm sorts the array.
4.  **Output Array**: A sorted array, achieved with the optimal strategy.

---

## ✨ Features

* **Adaptive Algorithm Selection**: Intelligently chooses between Merge Sort, Radix Sort, and Quicksort.
* **Worst-Case Avoidance**: Avoids performance pitfalls like Quicksort's O(n²) complexity by not using it on data that triggers its worst case.
* **Optimized for Small Arrays**: Automatically uses Insertion Sort for small arrays and partitions, where it is fastest.
* **Multi-Language Support**: Comes with clean, modular, and commented implementations in **C** and **Python**.
* **Educational**: An excellent resource for understanding algorithm design, heuristics, and performance optimization.

---
