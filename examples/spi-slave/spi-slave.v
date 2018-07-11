module SPI_slave(
  input clk,
  input SCK,
  input SSEL,
  input MOSI,
  output MISO,
  output LED
);

  // clk domain crossing - this works by storing the last two sck states, and then compare them to
  // to determine if it's rising or falling.

  // sync SCK to the FPGA clock using a 3-bits shift register
  reg [2:0] SCKr;  always @(posedge clk) SCKr <= {SCKr[1:0], SCK};
  wire SCK_risingedge = (SCKr[2:1]==2'b01);  // now we can detect SCK rising edges
  wire SCK_fallingedge = (SCKr[2:1]==2'b10);  // and falling edges

  // same thing for SSEL
  reg [2:0] SSELr;  always @(posedge clk) SSELr <= {SSELr[1:0], SSEL};
  wire SSEL_active = ~SSELr[1];  // SSEL is active low
  wire SSEL_startmessage = (SSELr[2:1]==2'b10);  // message starts at falling edge
  wire SSEL_endmessage = (SSELr[2:1]==2'b01);  // message stops at rising edge

  // and for MOSI
  reg [1:0] MOSIr;  always @(posedge clk) MOSIr <= {MOSIr[0], MOSI};
  wire MOSI_data = MOSIr[1];


  /////////////////////////////////////
  // read in 8 bit message
  // we handle SPI in 8-bits format, so we need a 3 bits counter to count the bits as they come in
  reg [2:0] bitcnt;
  reg byte_received;  // high when a byte has been received
  reg [7:0] byte_data_received;

  always @(posedge clk)
  begin
    if(~SSEL_active)
      bitcnt <= 3'b000;
    else
    if(SCK_risingedge)
    begin
      bitcnt <= bitcnt + 3'b001;

      // implement a shift-left register (since we receive the data MSB first)
      byte_data_received <= {byte_data_received[6:0], MOSI_data};
    end
  end

  always @(posedge clk)
    byte_received <= SSEL_active && SCK_risingedge && (bitcnt==3'b111) ;

  always @(posedge clk)
    begin
      if(byte_received) 
        case (byte_data_received)
          8'hcc:
            LED <= 1;
          8'hdd:
            LED <= 0;
      endcase
    end
endmodule

module top (
  input  clk,
  output D1,
  output D2,
  output D3,
  output D4,
  output D5,
  input cs,
  input sclk,
  output miso,
  input mosi
);

  SPI_slave #()
  SPI_slave
    (
    .clk(clk),
    .SCK(sclk),
    .MOSI(mosi),
    .MISO(miso),
    .SSEL(cs),
    .LED(D1)
  );

  assign D2 = 0;
  assign D3 = 0;
  assign D4 = 0;
  assign D5 = sclk;
endmodule
