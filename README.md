# CMU ICS (CSAPP) Labs Implementation

This repository tracks my implementation progress, defensive programming practices, and optimization paths for the lab assignments in Carnegie Mellon University's classic core course: **Introduction to Computer Systems (ICS)**.

Note that this is for self-study reference, not affiliated with CMU.

## 🛠️ Repository Structure & Current Progress

Each lab is fully encapsulated within its respective directory:

| Lab Name | Core Focus / Technical Challenge | Status |
| :--- | :--- | :--- |
| [📂 datalab](./datalab) | Bit manipulation constraints, IEEE 754 floating-point representation. | Completed |
| [📂 cachelab](./cachelab) | LRU Cache Simulator implementation, Matrix Transpose optimization via Cache Blocking. | Completed |
| 📂 malloclab | Custom dynamic memory allocator (Implicit/Explicit lists, Segregated free lists). | implementing |
| 📂 proxylab | Concurrent, caching web proxy utilizing multi-threading and network sockets. | Planned |

---

## 🚀 Key Lab Insights & Takeaways

### 1. Data Lab
* **The Challenge**: Implement complex logical operations and boundary checks under strict operator limitations (e.g., restricted to bitwise `~` and `&`).
* **Key Takeaway**: Mastered the mechanics of two's complement representation, integer overflows, and the precise physical binary layout of floating-point numbers in memory.

### 2. Cache Lab
* **The Challenge**: Build a software-defined cache simulator supporting the LRU eviction policy, then optimize $32 \times 32$ and $64 \times 64$ matrix transpose kernels to minimize cache miss rates.
* **Key Takeaway**: Gained deep structural insights into hardware cache hierarchy, cache trashing avoidance, and how temporal and spatial locality directly dictate raw application performance.

---
*Note: This repository is intended strictly for personal academic exploration and engineering competency demonstration, adhering strictly to academic integrity standards.*
