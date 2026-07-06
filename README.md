# ToolFlow

ToolFlow is a lightweight C++ framework for organizing and developing modular, reusable scientific analysis code based on CERN ROOT.

ROOT provides a comprehensive collection of libraries for scientific computing, data analysis, visualization, and many other tasks. Project organization, however, is left to the developer.

As a scientific analysis project evolves, new functionality is continuously added. Without a consistent project structure, code gradually becomes difficult to maintain. Similar functionality may be implemented multiple times, configuration parameters become scattered throughout the source code, and reusing existing analysis components becomes increasingly difficult.

ToolFlow provides a simple and structured environment for organizing ROOT-based analysis code. It encourages a modular design in which analysis components are implemented as independent, reusable executables called Tools.

From the developer's perspective, writing a Tool is very similar to writing a traditional ROOT macro. Everything that can be implemented in a ROOT macro can also be implemented in a Tool. The main difference is that a Tool is developed as a standard C++ application with its own source code, CMake configuration, and configuration file (INI format).

---


# Who is ToolFlow for?

ToolFlow is suitable for anyone who develops ROOT-based analysis code and wants to keep their code organized, modular and reusable.

It is particularly useful when

- analysis code gradually grows in size and complexity,
- different analysis stages are implemented in C++ and Python,
- common analysis components are reused across multiple workflows

Although originally motivated by high-energy physics analyses, ToolFlow can be applied to any ROOT-based scientific data analysis.

---

# Tool

A Tool is an independent executable that typically performs a specific task within a scientific analysis workflow. 

Since each Tool is a standard C++ application with its own source code, it can be extended as needed with additional source files, classes, and libraries to meet the user's requirements.

Every Tool has

- its own source code
- its own CMake configuration
- its own executable
- its own configuration file

allowing each Tool to be developed, tested and maintained independently.

---

# Configuration Files

Every Tool can be configured using an external INI file.

This allows you to

- reuse the same executable with different parameters,
- modify analysis settings without recompilation.

---


# ToolChain

A **ToolChain** is an optional workflow that executes multiple Tools in a user-defined sequence.

It is particularly useful when an analysis is divided into multiple processing stages and the output of one Tool is used by subsequent Tools. It also allows common analysis steps to be implemented once and reused across multiple workflows.

A ToolChain specifies which Tools should be executed and in what order. Rather than containing analysis code itself, it simply orchestrates the execution of existing Tools together with their corresponding configuration files.

This allows complex analyses to be organized as reusable analysis workflows while keeping individual Tools independent and interchangeable.

---

# Communication Between Tools in a toolchain

ToolFlow does **not** enforce a particular communication mechanism between Tools. A Tool may read any type of input and produce any type of output depending on the application.

For ROOT-based analyses, however, ROOT files provide a natural communication interface between consecutive Tools. In high-energy physics, both experimental and simulated data are commonly stored in ROOT files, where each event corresponds to one entry of a TTree and event-level quantities are stored as branches. Most HEP analyses process events independently, producing additional event-level derived quantities such as reconstructed variables or selection flags. A TTree naturally accommodates this workflow, making it an effective interface between consecutive Tools.

When ROOT files are used as the communication interface between consecutive Tools, a typical Tool performs the following steps:

1. Open an existing ROOT file.
2. Clone the input `TTree`.
3. Perform the analysis.
4. Store newly computed quantities as additional branches.
5. Write the updated tree to a new ROOT file.

```text
Input ROOT
      │
      ▼
+-------------+
|   Tool A    |
+-------------+
      │
      ▼
Output ROOT
      │
      ▼
+-------------+
|   Tool B    |
+-------------+
      │
      ▼
Output ROOT
```

This approach preserves all previously computed information while allowing each Tool to contribute only its own analysis results.

Although exchanging data through intermediate ROOT files introduces additional disk I/O, this is often a reasonable trade-off in offline analysis workflows.

Advantages of this approach include

- every Tool produces a persistent output that can be inspected independently,
- previous Tools do not need to be rerun if a later Tool fails,
- intermediate results can be validated and debugged easily,
- Tools remain loosely coupled by communicating only through ROOT files,


ROOT files also preserve the efficient column-oriented I/O model provided by `TTree`. Since a Tool typically reads only the branches required for its task, unnecessary disk I/O is minimized while maintaining efficient processing even for very large datasets.

Some analysis stages may be implemented as C++ Tools, while others—such as machine learning, or data manipulation—may be more conveniently implemented in Python. Since every stage exchanges data through standard ROOT files, each Tool can be implemented in the language best suited to its task.

```text
Tool A (C++)
        │
        ▼
Intermediate ROOT
        │
        ▼
Python Analysis
    (uproot)
        │
        ▼
Updated ROOT
        │
        ▼
Tool B (C++)
```

Although ROOT files provide the most natural interface for ROOT-based analyses, ToolFlow does not require this approach. Any suitable communication mechanism may be used whenever appropriate.

---

# Project Structure

A typical ToolFlow project has the following structure.

```text
ToolFlow/
│
├── tools/
├── toolChains/
├── shared/
├── iniReader/
├── createTool.sh
├── createToolChain.sh
├── CMakeLists.txt
└── README.md
```

---

# tools/

The `tools/` directory contains all independent Tools.

Each Tool resides in its own directory.

```
tools/
└── myTool/
    ├── main.cpp
    ├── CMakeLists.txt
    └── config.ini
    └── README.md
```

where

- `main.cpp` contains the Tool implementation.
- `CMakeLists.txt` defines how the Tool is compiled.
- `config.ini` stores runtime configuration parameters.

A single main.cpp file is sufficient for many analysis tasks. However, a Tool can be freely extended by adding additional source and header files (e.g. src/ and include/ directories). In this case, only the Tool's own CMakeLists.txt needs to be updated. ToolFlow does not impose any restrictions on the internal organization of a Tool.

New Tools can be created automatically using

```bash
./createTool MyTool
```

ToolFlow automatically maintains a registry of available Tools.

---

# toolChains/

The `toolChains/` directory contains all ToolChains.

Each ToolChain resides in its own directory and contains a workflow definition (.tchain file) together with the configuration files for the Tools it executes.

```
toolChains/
└── MyToolChain/
    ├── MyToolChain.tchain
    ├── tools.list
    ├── tool1.ini
    ├── tool2.ini
    └── tool3.ini
```

where

- `.tchain` defines the execution order.
- `tools.list` stores the Tools used by the ToolChain.
- each INI file contains the configuration of its corresponding Tool.

Create a new ToolChain

```bash
./createToolChain MyToolChain Tool1 Tool2 Tool3
```

ToolFlow automatically maintains a registry of available ToolChains.

---

# shared/

The `shared/` directory contains reusable C++ components shared by multiple Tools.

Typical examples include

- event-level objects such as detector hits, tracks or clusters
- utility classes
- helper algorithms

Using `shared/` is optional but recommended whenever multiple Tools require the same functionality.

ToolFlow includes a small set of example classes such as `Event`, `PMTHit`, and `EventReader`. These classes are intentionally simple and are provided solely to demonstrate how common components can be shared among multiple Tools. .

---

# iniReader/

ToolFlow uses a lightweight single-header INI parser (based on [inih](https://github.com/jtilly/inih)) for reading configuration files.

The parser provides a simple and convenient interface for accessing configuration parameters.

---

# Helper Scripts

ToolFlow provides two helper scripts for creating new Tools and ToolChains.

Create a Tool

```bash
./createTool toolName
```

Create a ToolChain

```bash
./createToolChain toolChainName Tool1 Tool2 Tool3
```

Both Tool and ToolChain registries are updated automatically.

---

# Building

ToolFlow uses CMake and supports out-of-source builds.

Create a build directory

```bash
mkdir build
cd build
```

Build all Tools

```bash
cmake ../<source_dir>
cmake --build . -j
```

Build selected Tools

```bash
cmake ../<source_dir> \
    -DTOOLS="tools/tool1;tools/tool2"

cmake --build . -j
```

Build all Tools required by one or more ToolChains

```bash
cmake ../<source_dir> \
    -DTOOLCHAINS="toolChains/myToolChain"

cmake --build . -j
```

Multiple ToolChains can also be specified

```bash
cmake ../<source_dir> \
    -DTOOLCHAINS="toolChains/chain1;toolChains/chain2"

cmake --build . -j
```

`TOOLS` and `TOOLCHAINS` may also be combined. In this case, ToolFlow builds the union of all specified Tools and all Tools required by the selected ToolChains.

```bash
cmake ../<source_dir> \
    -DTOOLS="tools/myExtraTool" \
    -DTOOLCHAINS="toolChains/myToolChain"

cmake --build . -j
```

---

# Running

Run an individual Tool

```bash
./bin/myTool ./configs/myTool/config.ini
```

Run an entire ToolChain

```bash
./bin/runPipeline ./toolChains/myToolChain/myToolChain.tchain
```

---

# Examples

ToolFlow provides several example Tools and a ToolChain demonstrating common usage patterns. These examples serve as starting points for developing new analysis workflows.

## tool1

Demonstrates the basic structure of a Tool, including INI-based configuration and an interactive ROOT graphical application. A simple 2D histogram is generated and displayed, similar to a traditional ROOT macro.

---

## tool2

Demonstrates how a Tool can be extended with additional source and header files. The analysis logic is implemented in a separate C++ class, illustrating how ToolFlow supports the development of larger and more maintainable applications.

---

## tool3

Demonstrates how common classes from the `shared/` directory can be used by a Tool. It uses the example `Event`, `PMTHit`, and `EventReader` classes to generate and process simple events.

---

## tool4

Demonstrates how a Tool can generate event-based data and store it in a ROOT `TTree`. The example creates a fake dataset and writes it to a ROOT file.

---

## tool5

Demonstrates how a Tool can read an existing ROOT `TTree`, perform event-by-event analysis, and store the computed quantities as additional branches in a new ROOT file.

---

## toolChain1

Demonstrates how multiple Tools can be combined into a complete analysis workflow. The ToolChain executes `tool4` to generate a ROOT file, followed by `tool5`, which processes the generated data and stores the computed quantities as additional branches in a new ROOT file.

