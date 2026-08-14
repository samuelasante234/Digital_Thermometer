# Register Bit Mapping

This component replaces standard bitwise operations with structured **Bit-Fields** for manipulating MSP430 peripheral registers.

## Why Use Bit-Fields?
Standard embedded C utilizes macros and bitwise logic to configure registers (e.g., `UCA0CTL1 |= UCSWRST | UCSSEL_2`). While functional, debugging these raw hexadecimal values can be tedious.

By defining C `union`s and packed `struct`s, we map exact hardware bit definitions to readable properties. 

## Advantages
1. **Readability**: Code becomes self-documenting. `UCA0CTL0_bits.bit5_UCMSB = 1` clearly indicates that the Most Significant Bit first mode is enabled.
2. **Safety**: Bit-fields limit the values that can be written to specific register segments, reducing the chance of accidentally overwriting adjacent configuration bits.
3. **Type-Punning**: The `union` structure allows the code to configure individual bits in the struct, and then write the entire compiled byte to the actual hardware register (`UCA0CTL1_reg`) in a single clock cycle.
