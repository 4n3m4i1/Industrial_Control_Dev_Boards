# Industrial_Control_Dev_Boards
For many projects a deeply embeddable development board is needed with the capability to take standard industrial DC voltages (12V, 24V, 36V) as input, and reliably provide outputs to anything else in a control/automation system. There aren't great options that support single side mounting (a la Pi Pico) that provide standard peripherals as well as CAN and possibly LIN, so here are some boards!  
  
## Board Families
- Waffle
The Waffle series is based on the F2800154QXXX (C2000 / dual TMS320C28x core) offerings from TI, which are generally aimed at the automotive market and thus provide good interfacing abilities for most controls needs. Several options are offered in the Waffle family (or rather, will, as my needs change) that only vary in pin count.
  
- Curly
The Curly series is based around the RP2040 we all know and love with an integrated TI TCAN4551-Q1 or MCP25625T-E_ML for SPI -> CAN conversion. These ICs include integrated CAN transceivers as well! 

- Tornado
Waiting on the new TI M0+ parts w CAN to drop... (MSPM0G)