# Waffle
The Waffle series is based on the F2800154Qxxx family of automotive processors centered around the TMS320C28x processors from TI. While dual core the default behavior is a lockstep mode with hardware comparisons done between each core in order to maintain a higher degree of safety. As with all things this can be customized in software, and disabled to allow full dual core operation if desired.

[Datasheet](https://www.ti.com/lit/gpn/tms320f2800154-q1)
[Reference Manual](https://www.ti.com/lit/ug/spruiy4a/spruiy4a.pdf?ts=1703646925193&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FTMS320F2800152-Q1)
[Errata](https://www.ti.com/lit/er/sprz507b/sprz507b.pdf?ts=1703652510267&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252Fko-kr%252FTMS320F2800154-Q1)
  
Different Waffle sizes maintain similar feature sets, with varying amounts of GPIO. Each Waffle board has the following peripheral guaranteed:  
- 1 CAN2.0B / FD
- 3 UART
- 1 LIN
- 10+ PWM Channels
  
  
  
## Waffle Sizes
- Waffle Mini
	- F2800154QRHBRQ1, 20 GPIO
    - 35.56 mm x 17.78 mm
    - 1.4" x 0.7"
    - Breadboard compatible
    - Can be surface mounted w/ castelated pins (single side components)
    - Tag Connect 6 pin cJTAG interface (Odd Even Numbering)
        - 1: 3V3
        - 2: JTAG TMS
        - 3: XRSn (Reset)
        - 4: JTAG TCK
        - 5: GND
        - 6: N/C
