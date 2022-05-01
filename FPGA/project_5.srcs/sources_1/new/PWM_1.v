`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2021/04/19 14:16:37
// Design Name: 
// Module Name: PWM_1
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////



module PWM(clk,pwm1,jiaodu); 
		input clk;
		input[4:0] jiaodu;
		output[1:0] pwm1;
	reg[1:0] pwm1;
	reg[32:0] counter;
	reg[32:0] counter1;
		reg[32:0] counterx;
	reg[32:0] counter1x;
	reg[31:0] jiaodu1;
		reg[31:0] jiaodu2;
	reg[31:0] stata=0;
	reg[31:0] stata1=0;
	always@(posedge clk)
		begin
		case(stata)
	       	3'd0:jiaodu1=15;
		    3'd1:jiaodu1=5;
		    3'd2:jiaodu1=25;
		endcase
		case(stata1)
	       	3'd0:jiaodu2=15;
		    3'd1:jiaodu2=5;
		    3'd2:jiaodu2=25;
		endcase
		counter = counter + 1;
		if(counter == 32'd10000)   //  50MHz  0.1ms
			begin
			counter = 0;
			counter1= counter1 + 1;
			end
			if(counter1 == 8'd1)
				pwm1[0]  <= 1;
			else if(counter1 ==jiaodu1)
				pwm1[0] <= 0;
			else if (counter1 == 16'd200)
				counter1=0;	
					
		counterx = counterx + 1;		
	if(counterx == 32'd10000)   //  50MHz  0.1ms
			begin
			counterx = 0;
			counter1x= counter1x + 1;
			end
			if(counter1x == 8'd1)
				pwm1[1]  <= 1;
			else if(counter1x ==jiaodu2)
				pwm1[1] <= 0;
			else if (counter1x == 16'd200)
				counter1x=0;		
  end
  
  
		always@(posedge clk)
		begin
		if(jiaodu[0]==1)stata=1;
		else if(jiaodu[1]==1)stata=2;
		else stata=0;
		if(jiaodu[2]==1)stata1=1;
		else if(jiaodu[3]==1)stata1=2;
		else stata1=0;
		end
	always@(posedge clk)
		begin
		case(stata)
	       	3'd0:jiaodu1=15;
		    3'd1:jiaodu1=5;
		    3'd2:jiaodu1=25;
		endcase
		case(stata1)
	       	3'd0:jiaodu2=15;
		    3'd1:jiaodu2=5;
		    3'd2:jiaodu2=25;
		endcase	
		end
endmodule

