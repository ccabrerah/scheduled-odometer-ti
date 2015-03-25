# scheduled-odometer-ti
This is an exploration of writing a scheduled framework for an **ARM M3** embedded system. Runs on the Texas Instruments LM3S3748 Stellaris Evaluation Board.

Features a small set of tasks that are scheduled and controlled using the [Protothreads](http://dunkels.com/adam/pt/) library, which allow it to be ported to another C-supporting architecture later. The system's initial objective is to collect rotary encoder data, calculate velocity and acceleration, and provide optional log messages on an attached screen or over CIO when connected to a host. Future functionality would include a thermal printer interface, travel data offload to flash memory and exporting over bluetooth.

This is a Code Composer Studio IDE project.