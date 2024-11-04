# Lab 05 for Semrah and Thomas

![example workflow](https://github.com/uofu-emb/2024-lab05-Semrah-Thomas/actions/workflows/main.yml/badge.svg)

## Table
Here is the table showing our measured values.

| Code Type   | Desired Frequency | Scope Freqency | Period | Duty Cycle | Jitter | Dift Over Hour |
|-------------|-------------------|----------------|--------|------------|--------|----------------|
| Sleep       | 1 Hz              |      1Hz       | 1 S    |50%         | 0      |N/A             |
| Sleep Delay | 1 Hz              |      0.833Hz   | 1.2 S  |50%         | 200ms  |12 Minutes      |
| Timer       | 1 Hz              |      1Hz       | 1 S    |50%         | 0      |N/A             |
| Timer Delay | 1 Hz              |      1Hz       | 1 S    |50%         | 0      |N/A             |
| Task        | 1 Hz              |      1Hz       | 1 S    |50%         | 0      |N/A             |
| Timer Delay | 1 Hz              |      0.833Hz   | 1.2 S  |50%         | 200ms  |12 Minutes      |

## Sleep
These are the oscilliscope images for the sleep without delay first followed by with delay.

![SleepNoDelay](!Data/Images/SLEEP_ND.PNG)
![SleepDelay](!Data/Images/SLEEP_D.PNG)

## Timer
These are the oscilliscope images for the timer without delay first followed by with delay.

![TimerNoDelay](!Data/Images/TIMER_ND.PNG)
![TimerDelay](!Data/Images/TIMER_L.PNG)

## Task Delay
These are the oscilliscope images for the task_delay function without delay first followed by with delay.

![TaskNoDelay](!Data/Images/TASK_ND.PNG)
![TaskDelay](!Data/Images/TASK_D.PNG)

## GPIO Interrupt
These are the oscilliscope images for the gpio_interrupt function without delay first followed by with delay.

![GPIONoDelay](!Data/Images/GPIO.PNG)
![GPIODelay](!Data/Images/GPIO_D.PNG)