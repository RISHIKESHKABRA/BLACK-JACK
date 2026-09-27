# BLACK-JACK

# C++ Console Blackjack Game

[![C++ Standard](https://img.shields.io/badge/C%2B%2B-11%2F03-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)](#building--running)

A lightweight, object-oriented console implementation of **Blackjack (21)** written in C++. Designed for zero-dependency execution across legacy compilers (Dev-C++, Turbo C++) and modern GCC/Clang tooling.

---

## Features

- **Bankroll System:** Track player chip balance persistently across rounds with wager validation.
- **Double Down Action:** Double initial wagers mid-hand with automated single-card draw rules.
- **3:2 Blackjack Payouts:** Automatic natural Blackjack detection with standard casino payout calculations.
- **Cross-Compiler Compatible:** Written with fallback conversion routines (`ostringstream`) and standard ASCII output for terminal compatibility across standard compilers.

---

## Rules of Play

1. **Objective:** Beat the Dealer's total without exceeding 21.
2. **Card Values:** 
   - 2 through 10 = Face value
   - J, Q, K = 10
   - Ace = 11 (automatically reduces to 1 if hand total exceeds 21)
3. **Actions:**
   - `(h)it`: Draw another card.
   - `(s)tand`: End turn and keep current total.
   - `(d)ouble down`: Double initial bet, draw exactly one card, and end turn.

---

## Building & Running

### Option A: Direct GCC Compilation (Terminal)
```bash
# Clone the repository
git clone [https://github.com/your-username/cpp-blackjack-game.git](https://github.com/your-username/cpp-blackjack-game.git)
cd cpp-blackjack-game

# Compile using g++
g++ -Iinclude src/*.cpp -o Blackjack

# Run the game
./Blackjack
