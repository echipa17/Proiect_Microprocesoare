# Proiect_Microprocesoare


# Echipa #17: Banu Teodora și Mistric Ioana

## Implementarea temei de proiect

### Configurații pentru modulele periferice:
- **UART**
  - Baud Rate: `9600`
  - Over Sampling Rate: `16`
  - Data Frame Order: `LSB First`
  - Data Inversion: `TX Inverted`

- **GPIO**
  - LED Sequence: `alb, roșu, mov, galben`

- **ADC**
  - PIT
  - Analog Sensor
  - Single Ended Precision: `16`
  - LED Sequence Duration: `836 ms`

### Tratarea datelor
- Valoarea digitală rezultată din conversia tensiunii datelor achiziționate va fi în intervalul `[DMIN - DMAX]`.
- Acest interval este împărțit în 3 subintervale: valori mici, medii și mari.
- Reprezentarea datelor convertite se va face prin numărul de acționări a unui buzzer.
