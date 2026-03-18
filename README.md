# 🍽️ OS Project – Food Delivery System (C Implementation)

A collection of **Operating System concepts implemented in C** using a real-world **Food Delivery System simulation**.

This project demonstrates core OS concepts like **Scheduling, Deadlocks, Synchronization, and Memory Management** using practical and easy-to-understand scenarios.

---

## 📌 Project Overview

This repository contains multiple OS lab activities implemented using:

- **C Programming**
- **POSIX Threads (pthreads)**
- **Semaphores & Mutex Locks**

Each module simulates a real-world food delivery workflow to explain OS concepts clearly.

---

## 🚀 Implemented Concepts

### 1️⃣ Multi-Level Feedback Queue (MLFQ Scheduling)
- Simulates restaurant order processing
- Uses multiple queues with different priorities
- Implements:
  - Time slicing (Round Robin)
  - Priority demotion
  - Aging for starvation prevention

---

### 2️⃣ Deadlock Demonstration
- Simulates delivery partners competing for resources:
  - Order
  - Vehicle
  - GPS
  - Delivery Bag
- Demonstrates all **4 conditions of deadlock**:
  - Mutual Exclusion
  - Hold and Wait
  - No Preemption
  - Circular Wait

---

### 3️⃣ Deadlock Prevention
- Fixes deadlock by enforcing **resource ordering**
- Ensures all threads acquire locks in the same sequence
- Eliminates circular wait condition

---

### 4️⃣ Producer-Consumer Problem
- Simulates:
  - Order Takers → Producers
  - Chefs → Consumers
- Uses:
  - **Semaphores (`empty`, `full`, `mutex`)**
  - Circular buffer
- Prevents:
  - Buffer overflow
  - Buffer underflow

---

### 5️⃣ Cache Algorithms (Memory Management)
- Simulates menu request caching
- Implements:
  - **LRU (Least Recently Used)**
  - **FIFO (First In First Out)**
- Tracks:
  - Cache hits
  - Cache misses

---

## 🧱 Project Structure

OSPROJECT-FOOD-DELIVERY-SYSTEM/
│
├── mlfq.c                     - Multi-Level Feedback Queue Scheduling
├── deadlock_demo.c           - Deadlock demonstration using threads
├── deadlock_prevention.c     - Deadlock-free implementation (resource ordering)
├── producer_consumer.c       - Producer-Consumer using semaphores
├── cache_algorithms.c        - LRU & FIFO cache implementation
│
└── README.md

---

## ⚙️ Requirements

- GCC Compiler  
- Linux / Windows (MinGW / WSL)  
- pthread library support  

---

## ▶️ Execution Commands

### 🔹 Compile (for programs using pthread)
- gcc filename.c -o output -lpthread

### 🔹 Compile (for normal C programs)
- gcc filename.c -o output

### 🔹 Run
- ./output

---

## 📌 Examples

### 1. Deadlock Program
gcc deadlock_demo.c -o deadlock -lpthread
./deadlock

### 2. Deadlock Prevention
gcc deadlock_prevention.c -o safe -lpthread
./safe

### 3. Producer-Consumer
gcc producer_consumer.c -o pc -lpthread
./pc

### 4. MLFQ Scheduling
gcc mlfq.c -o mlfq
./mlfq

### 5. Cache Algorithms (LRU & FIFO)
gcc cache_algorithms.c -o cache
./cache

---

## 💡 Tip
- On Windows (Command Prompt / PowerShell):
  output.exe
