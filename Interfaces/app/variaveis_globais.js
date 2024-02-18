var options_quantity=[[0,0,0],
					[0,0,0,0],
					[0,0,0,0],
					[0,0,0,0,0],
					[0,0,0]];
var order_beginDate=new Array(10);
var order_total_price=new Array(10);
var order_minutes=new Array(10);
var order_total_minutes=new Array(10);
var order_extra_time=new Array(10);
var order_countdown_var=new Array(10);
var id=9;
var order_quantity=new Array(10);
for (var i = 0;i<order_quantity.length; i++) {
	order_quantity[i]=new Array(options_quantity.length);
	for(var j=0;j<options_quantity.length;j++)
		order_quantity[i][j]=new Array(options_quantity[j].length);
}

var bill=0;//usado por todo a lado, var global
var curr_section=[0,0];
var stack=['open_food_first_screen()']
