# Industrial_Control_Dev_Boards
For many projects a deeply embeddable development board is needed with the capability to take standard industrial DC voltages (12V, 24V, 36V) as input, and reliably provide outputs to anything else in a control/automation system. There aren't great options that support single side mounting (a la Pi Pico) that provide standard peripherals as well as CAN and possibly LIN, so here are some boards!  
  
## Board Families
- Waffle
The Waffle series is based on the F2800154QXXX (C2000 / dual TMS320C28x core) offerings from TI, which are generally aimed at the automotive market and thus provide good interfacing abilities for most controls needs. Several options are offered in the Waffle family (or rather, will, as my needs change) that only vary in pin count.
  
- Curly
The Curly series is based around the RP2040 we all know and love with an integrated TI TCAN4551-Q1 or MCP25625T-E_ML for SPI -> CAN conversion. These ICs include integrated CAN transceivers as well! Dual RS232 channels are provided from the boards, tied to the hardware UART peripherals.

- Tornado
Waiting on the new TI M0+ parts w CAN to drop... (MSPM0G)  
  
## Using Boards in Designs
Once a design has been fabricated and verified a footprint and symbol (KiCad 7) will be included in its respective library. These can be used as AIO components in other projects easily.
  
## Board Fabrication
Many PCBA services exist, and all fabrication files used to order a board will be provided in their respective directories. JLC's PCBA is unique in that their parts stock is in-house, which can be a massive bonus or immense drag depending on what you're trying to do. For my smaller scale their methods generally work, as long as parts selection is done to best fit their stock (and optimize for "Basic" parts over "Extended" parts, which incurr a proportionally large fee for small runs), as such certain choices have been made to optimize costs. Given their internal parts sourcing anyone can simply use the files provided, click "Buy", and assembled boards will show up at your house.
  
If a board is of interest, but you don't want to order it yourself, please contact me. If there is enough interest the fixed costs for setup and shipping can be reduced massively per board.

- Waffle Mini v0
Not verified.
  
- Curly Mini v0
All files provided for JLC PCBA, can be effectively one click ordered fully assembled. Approx. $45 for 5 complete boards, 1/2 parts, 1/2 setup fees, board costs are negligible. 2 day shipping to CA approx. $18. 