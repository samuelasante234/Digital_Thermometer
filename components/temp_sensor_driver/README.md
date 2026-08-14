# Temperature Sensor Driver (ADC10 & DTC)

This driver manages analog-to-digital conversions to read the temperature data from an analog LM35 sensor. It is highly optimized to minimize CPU overhead.

## Data Transfer Controller (DTC) Implementation
Instead of relying on the CPU to fetch every single ADC reading, this driver configures the **ADC10 Data Transfer Controller (DTC)**. 
* **Mechanism**: The ADC10 is set to single-channel repeat mode. The DTC is programmed to automatically transfer a block of 5 samples directly into the `temp_readings` RAM array.
* **Interrupt**: Only when all 5 transfers are complete does the ADC trigger the `ADC10_ISR` interrupt, which updates the `sampling_done_flag`. The CPU remains free (or asleep) during the sampling window.

## Calculation and String Formatting
* **Averaging**: The 5 samples are summed and averaged to act as a software low-pass filter, mitigating noise on the analog line.
* **Fixed-Point Math**: To avoid importing the bloated floating-point standard library, temperature is calculated using integer arithmetic (`(average * 15000) / 1023`).
* **String Conversion**: Extracts the integer and fractional parts using modulo-like arithmetic, formatting them via `snprintf` to a precise, display-ready 2-decimal point string.
