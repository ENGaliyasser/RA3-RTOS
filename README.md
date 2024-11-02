# RA3 RTOS

RA3 RTOS is a custom-designed, open-source Real-Time Operating System tailored for embedded applications, currently supporting ARM Cortex-M3 processors. The project was initiated to create a flexible, lightweight OS with efficient task management and scheduling capabilities for resource-constrained systems.

## Features

- **Task Management**: Support for task creation, activation, suspension, and termination.
- **Task Scheduling**: Utilizes a preemptive priority-based round-robin scheduling algorithm with plans for additional scheduling mechanisms.
- **Inter-task Communication**: Implements semaphores, mutexes, and event groups for efficient synchronization.
- **Memory Management**: Provides lightweight memory handling to optimize for embedded environments.
- **Sorting and Prioritization**: Task prioritization is implemented using an efficient, memory-conscious bubble sort.
- **Low Power Optimization**: Idle task hooks and CPU sleep modes are available to reduce power consumption.
- **SysTick and SVC Hooks**: Built-in support for system-level hooks to improve flexibility and control.
  
## Getting Started

1. **Download RA3 RTOS**: You can download the latest release as a [ZIP file from the Releases section](https://github.com/ENGaliyasser/RA3-RTOS/releases).
2. **Unzip and Integrate**: Simply unzip the package and add it to your project directory.
3. **Setup Instructions**: Detailed setup and integration instructions can be found [here](#).

### Requirements

- **Processor**: ARM Cortex-M3 or compatible processors.
- **Development Environment**: Any standard ARM development environment with support for C.

## How RA3 RTOS Works

RA3 RTOS employs a combination of preemptive and round-robin scheduling, designed to be efficient in both memory and processing overhead. By incorporating task prioritization and delayed scheduling, it meets the real-time requirements of embedded systems.
  
## Documentation

The RA3 RTOS documentation includes in-depth information on:

- **APIs**: Covering task management, semaphores, mutexes, FIFO operations, and memory management.
- **Code Examples**: Demonstrating how to use the core features.
- **Contributions**: Guidance on how to contribute to the project.

To explore more, please check the [full documentation here](https://github.com/ENGaliyasser/RA3-RTOS/blob/main/documentation/RA3_RTOS_documentation.pdf).

## Meet the Team

Currently, RA3 RTOS is developed and maintained by:

**Ali Yasser Ali Abdallah**  
I’m a passionate computer engineering student dedicated to contributing to the embedded industry through RA3 RTOS. This project embodies my commitment to open-source solutions for embedded systems. I'm always open to expanding the team—enthusiastic contributors are welcome!

## Contributing

To contribute to RA3 RTOS:

1. **Fork the Repository**: Make your desired changes in a separate branch.
2. **Follow Coding Standards**: Ensure your code adheres to RA3 RTOS coding rules.
3. **Submit a Pull Request**: Once your feature is ready, submit a pull request for review.

For any questions or suggestions, feel free to reach out at [engaliyasser7@gmail.com](mailto:engaliyasser7@gmail.com).

--- 

This README includes the links to the documentation PDF and the releases section based on the screenshot you provided. Let me know if you'd like any additional modifications.
