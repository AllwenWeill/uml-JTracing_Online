always_ff @(posedge clk) //flip-flop������
begin 
	int a = 0;
	if (a) a = 1;
	else a = 2;
end
