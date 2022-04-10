# Hash table


## Table of Contents
1. [Overview](#overview)
2. [Hash comparation](#hash-comparation)
3. [AVX 256 optimizations](#avx-256-optimizations)


## Overview
[Hash table] (also called hash map) is a data structure that implements 
an associative array abstract data type, a structure that can map keys to values. 

<pre>
    ┌────────────┐
    │ Hash table │ 
    ├────────────┤ 
    ├────────────┤  ┌─────────┐  ┌─────────┐      ┌─────────┐
    │ slot 1     ├─>┤ entry 0 ├─>┤ entry 1 ├─...──┤ entry n ├─...
    ├────────────┤  └─────────┘  └─────────┘      └─────────┘
    ├────────────┤  ┌─────────┐  ┌─────────┐  ┌─────────┐
    │ slot 2     ├─>┤ entry 0 ├─>┤ entry 1 ├─>┤ entry 2 ├─...
    ├────────────┤  └─────────┘  └─────────┘  └─────────┘
    │ ...        │ 
    ├────────────┤
    │ slot 255   ├─...
    └────────────┘
</pre>

A **chained hash table** fundamentally consists of an array of linked lists. 
Each list forms a **slot** in which we place all elements hashing to a specific 
position in the array (see figure above).


## Hash comparation
Hash collision is when two pieces of data in a hash table share the same hash value. 
The hash value in this case is derived from a hash function.

<pre>
╔══════════════════╦════════════════════════════════════════════════╗
║ Hash function    ║ Description                                    ║
╚══════════════════╩════════════════════════════════════════════════╝
┌──────────────────┬────────────────────────────────────────────────┐
│ one_hash         │ h(x) = 1                                       │
├──────────────────┼────────────────────────────────────────────────┤
│ first_ascii_hash │ h(x) = x[0]                                    │
├──────────────────┼────────────────────────────────────────────────┤
│ sum_ascii_hash   │ h(x) = x[0] + x[1] + ... + x[len(x)]           │
├──────────────────┼────────────────────────────────────────────────┤
│ ror_hash         │ for i in len(x): h(x) = x[i] xor (h(x) ror 1)  │
├──────────────────┼────────────────────────────────────────────────┤
│ length_hash      │ h(x) = len(x)                                  │
├──────────────────┼────────────────────────────────────────────────┤
│ wsum_ascii_hash  │ h(x) = 1*x[0] + 2*x[1] + ...                   │
├──────────────────┼────────────────────────────────────────────────┤
│ custom           │ h(x)                                           │
└──────────────────┴────────────────────────────────────────────────┘
</pre>

Collisions test is described on the figure below:

<pre>
                                           
  ╔════════╗     ╔═════════════════════╗     ╔════════════════╗
  ║ 824528 ║     ║ Load to hash table  ║     ║ Get collisions ║
  ║ words  ║ >>> ║ and calculate words ║ >>> ║ number         ║
  ║ array  ║     ║ duplicates          ║     ║                ║
  ╚════════╝     ╚═════════════════════╝     ╚════════════════╝

</pre>                                      

After analyzing the figures below, we can say that 
hashes **sum_ascii_hash**, **wsum_ascii_hash** and **ror_hash** can 
be used as 'good' hash functions.

<img src="img/plot.png" alt="Hash collisions" style="height: 750px; width:500px;"/>







## AVX 256 optimizations
<!--links-->
[Hash table]: https://en.wikipedia.org/wiki/Hash_table
[]