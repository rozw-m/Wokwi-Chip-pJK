// Wokwi Custom Chip - For docs and examples see:
// https://docs.wokwi.com/chips-api/getting-started
//
// SPDX-License-Identifier: MIT
// Copyright 2023 Roz Wyatt-Millington

// Input / output names must match the pins defined in the chip's JSON file:
module wokwi (
    output reg Q,
    output reg Q_bar,
    input J,
    input K,
    input clk
  );
initial Q = 1'b1;
initial Q_bar =1'b0;

always @(posedge clk) begin
    case ({J, K})
        2'b00: begin // No change
            Q <= Q;
            Q_bar <= Q_bar;
        end
        2'b01: begin // Reset
            Q <= 1'b0;
            Q_bar <= 1'b1;
        end
        2'b10: begin // Set
            Q <= 1'b1;
            Q_bar <= 1'b0;
        end
        2'b11: begin // Toggle
            Q <= ~Q;
            Q_bar <= ~Q_bar;
        end
    endcase
end

endmodule
