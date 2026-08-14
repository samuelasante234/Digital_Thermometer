# Application Logic and Build System

This directory houses the core application logic (`main.c`) and the custom build system (`Makefile`). 

## The Need for Modularity
Embedded systems code can quickly become unmaintainable when hardware configurations, display rendering, and application logic are written in a single file. This project strictly enforces separation of concerns:
* **`main.c`** only dictates the *flow* of the application (the super-loop). It does not care *how* SPI is initialized or *how* the ADC works.
* Drivers and HAL components are isolated in the `components/` directory, making them highly portable to other MSP430 projects.

## Makefile Implementation
Instead of relying on the Code Compiler Studio IDE which has its own overhead and large size not necessarily needed for the scope of this project, this project uses a `Makefile` written entirely by the creator through documentation from TI. 
* **`VPATH`**: Utilized to automatically locate `.c` files within the `components/` subdirectories, keeping the build rules clean.
* **Object Linking**: Compiles the HAL and Drivers independently before linking them into the final `firmware.elf` image.

## `main.c` Execution Logic
1. **Pre-Boot Routine**: The Watchdog Timer (WDT) is disabled immediately upon boot by placing the `disable_watchdog()` function in the `.crt_0042` section. This prevents reset loops during C runtime initialization.
2. **Initialization**: Global interrupts are enabled, followed by the ADC/DTC setup, SPI bus initialization, and the ST7789 wake-up sequence.
3. **Super-Loop**:
   * Enables the ADC conversion (`enable_enc()`).
   * Blocks execution until the Data Transfer Controller (DTC) pulls 5 samples and fires the `ADC10_ISR` (`while (!sampling_done_flag);`).
   * Calculates the average temperature using integer arithmetic.
   * Converts the value into a formatted C-string.
   * Renders the string to the ST7789 display.
   * Resets the flag and delays for 2 seconds before repeating.
