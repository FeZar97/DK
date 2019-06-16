# DigitalTract
SDR receiver management program and implementation of DSP function.
Main project in 2017-2019.

## Features:
* full control of the SDR receiver based on Realtek and RafaelMicro chips:
    + gain control; 
    + ADC sampling frequency; 
    + frequency of the local oscillator; 
    + operating mode ADC;
    + receiving digital signal from SDR;
* DSP:
    + bandpass filtration (FIR filter);
    + FFT calculation (size 1024);
    + shift signal's frequency;
    + decimation;
    + demodulation (AM, USB, LSB, FM);
* recording signal at various stages of digital processing in wav files;
* displaying spectrum of signal on spectrogram and sonogram;
* output of low-frequency demodulated signal via computer sound card;
* suppression of imbalance of quadrature components;

## Appearance of the main program window
