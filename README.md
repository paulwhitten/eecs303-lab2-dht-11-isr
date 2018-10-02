# pi-dht-11-isr

Pi application that will interact with
the DHT 11 sensor using interrupts.

minimal-sampling.txt is sample output
from the application.

dht11-timings.xlsx shows the interpretation
of one reading.

Clone the repo on the pi by running:
```
git clone https://github.com/paulwhitten/eecs303-lab2-dht-11-isr.git
```

Compile the application by running the following in the repo folder:
```
gcc dht-11-minimal-isr.c -o dht-11-minimal-isr -lwiringPi
```

Run by executing:
```
sudo ./dht-11-minimal-isr
```


