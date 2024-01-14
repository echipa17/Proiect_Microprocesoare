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
  - Analog Sensor: `On-board Temperature Sensor`
  - Single Ended Precision: `16`
 
- **PIT**
  - LED Sequence Duration: `836 ms`

### Tratarea datelor
- Valoarea digitală rezultată din conversia tensiunii datelor achiziționate va fi în intervalul `[DMIN - DMAX]`.
- Acest interval este împărțit în 3 subintervale: valori mici, medii și mari.
- Reprezentarea datelor convertite se va face prin numărul de acționări a unui buzzer.

## Modul Senzor de Touch

În cadrul proiectului nostru, am integrat un **senzor de touch** care permite utilizatorilor să interacționeze direct cu sistemul. Acesta este configurat pentru a declanșa un algoritm de alertă, contribuind astfel la interactivitatea și eficiența sistemului.

### Caracteristici
- Sensibilitate ajustabilă pentru diferite aplicații
- Interfață simplă și intuitivă
- Integrare cu modulul de alarmă pentru o reacție rapidă la atingere

### Implementare
Utilizăm senzorul de touch pentru a activa un buzzer, oferind feedback imediat utilizatorului. Codul de configurare este optimizat pentru a asigura o detecție fiabilă și precisă a atingerilor.

