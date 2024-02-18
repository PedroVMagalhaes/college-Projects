define(function() {

    let id = 9;
    let options_quantity=[[0,0,0],
        [0,0,0,0],
        [0,0,0,0],
        [0,0,0,0,0],
        [0,0,0]];

    let order_quantity = new Array(10);
    for (var i = 0;i<order_quantity.length; i++) {
        order_quantity[i]=new Array(options_quantity.length);
        for(var j=0;j<options_quantity.length;j++)
            order_quantity[i][j]=new Array(options_quantity[j].length);
    }



    return {
        options_quantity: options_quantity,
        order_quantity: order_quantity,
        order_countdown_estado:new Array(10), // cena com string para meter no countdown_timmer;
        order_beginDate:new Array(10),
        order_total_price:new Array(10),
        order_minutes:new Array(10),
        order_total_minutes:new Array(10),
        order_extra_time:new Array(10),
        order_countdown_var:new Array(10),
        id: 9,
        bill:0,
        time: 0,
        confirm_fn: () => {},
        curr_section:[0,0],
        stack:['open_food_first_screen()']
    }
});
