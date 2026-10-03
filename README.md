# 🏦 Bank Management System (BMS) in C++

A cross-platform, console-based Bank Management System developed in **C++** implementing Role-Based Access Control (RBAC), low-level binary data serialization, and comprehensive banking transaction processing. 

Compatible with both **Linux (Ubuntu)** and **Windows**.

---

## 📌 Project Overview

This project was built to demonstrate core concepts in:
* **Object-Oriented Programming (OOP)** & Data Encapsulation
* **Low-Level Memory & Binary File I/O** via `reinterpret_cast`
* **POSIX Terminal Control** on Linux (`termios.h`, ANSI escape sequences)
* **Computer Architecture** (memory layout, 64-bit struct padding, word alignment)
* **Transaction Atomicity & Systems Engineering**

---

## ✨ Features

### 👤 Role-Based Access Control (RBAC)
* **Administrator Mode:**
  * View all registered customer accounts in a structured table.
  * Customer Account Registration (with unique account number validation).
  * Edit existing customer details.
  * Delete customer accounts.
  * Search accounts by **Account Number** or **Customer Name** (case-insensitive).
  * Access full transaction suite.
* **Standard User Mode:**
  * Access restricted to customer banking transactions only.

### 💳 Banking Transactions
* **Balance Inquiry:** Real-time account balance retrieval.
* **Cash Deposit:** Safely deposit funds with input validation.
* **Cash Withdrawal:** Balance verification with overdraft protection and withdrawal fee computation.
* **Fund Transfer:** Validated atomic funds transfer between accounts.
* **Interest Calculator:** Acquired interest computation based on annual rates.

### 🛡️ Security & Reliability Enhancements
* **Masked Password Input:** Passwords typed are masked using `*` across both Linux and Windows.
* **Brute-Force Protection:** Maximum 3 failed authentication attempts before session lockout.
* **Stack Overflow Prevention:** Pure iterative event-loop architecture replacing recursive function calls.
* **Buffer Overflow Guards:** Strict input length bounding using stream manipulators (`setw`).
* **POSIX Terminal Synchronization:** Automatic input queue flushing (`tcflush`) to prevent phantom keystrokes.

---

## 🔑 Default Login Credentials

| Role | Username | Password | Access Level |
| :--- | :--- | :--- | :--- |
| **Administrator** | `admin` | `admin` | Full Management & Transactions |
| **Standard User** | `user` | `user` | Transactions Only |

---

## 🛠️ Build and Run Instructions

### 🐧 On Linux (Ubuntu / Debian / WSL)

1. **Prerequisites:** Install `g++` and build essentials:
   ```bash
   sudo apt update
   sudo apt install build-essential g++
