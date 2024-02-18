define(function(require) {
    let g = require('app/globals');

    //stack.pop()   stack.push()
    var curr_screen=document.getElementById('food_first_screen');
    function back(){
        var estado_atual=g.stack.pop()
        if(g.stack.length==0){
            eval(estado_atual);//se e o ecra incial nao ha mais nada para voltar para tras
        }
        eval(g.stack.pop());//trocar para estado anterior
    }
    function return_to_food(){
        eval(g.stack.pop());
    }

    var categories=['Hamburgers','Sandwiches','Pizza','Drinks','Ice Cream'];

    var options_img=[['Happy_Meal.png','Big_Mac.png','CBO.png'],
        ['Simple.png','Egg.jpg','Meat_Ball.png','Chicken.png'],
        ['Classic.png','Hawaiian.jpg','4Cheese.png','Peperoni.jpg'],
        ['Lemonade.jpg','Water.jpg','Compal.jpg','Coca-Cola.jpg','Fanta.jpg'],
        ['Epa.jpg','Cone.jpg','Magnum.jpg']];

    var options_price=[[1.3,3.5,5.5],
        [0.90,1.2,3.3,3.3],
        [2,2.7,2.7,2.7,3],
        [0.9,1.5,1.8,2.5,2.8],
        [0.9,12.3,20.3]];

    var options_name=[['Happy Meal','Big Mac','CBO',],
        ['Simple','Egg','Meat Ball','Chicken'],
        ['Classic','Hawaiian','4Cheese','Peperoni'],
        ['Lemonade','Water','Compal','Coca-Cola','Fanta'],
        ['Epá','Cone','Magnum']];

    var options_time=[[2,1,2],
        [1,2,3,1],
        [7,10,4,10],
        [3,1,2,1,1],
        [5,7,5]];


    var expected_line_time=0;//apenas usados no menu buy para nao variar tempo enquanto se adiciona quantidade
    var time_per_quantity=0;//apenas usados no menu buy para nao variar tempo enquanto se adiciona quantidade

    var time=0;//pertence ao purchase menu, usada para calcular maior tempo de todos os items


    function make_purchase(){
        /*<div id="food_view_orders" class="scrollable_hmax not_inicial_screen">
            <div class="food_view_orders_order">
                <div class="food_view_orders_order_time">
                    12:30
                </div>
                <div class="food_view_orders_order_total_price">
                    XXX.XX€
                </div>
                <div class="food_view_orders_order_view_bill_button">
                    View Bill
                </div>
                <img class="food_view_orders_order_direction_button" src="replacer.png">
                <div class="food_view_orders_order_countdown_timmer">
                    XXmin
                </div>
                <img class="food_view_orders_order_enable_notification" src="replacer.png">
                <img class="food_view_orders_order_disable_notification" src="replacer.png">
            </div>
        </div>*/
        g.id=(g.id+1)%10;
        var current_date= new Date();
        console.log(current_date.getHours()+"Hours"+current_date.getMinutes()+"Minutes"+current_date.getSeconds()+"Seconds")
        g.order_total_price[g.id]=g.bill;
        g.order_beginDate[g.id]=current_date;
        g.order_minutes[g.id]=time;
        console.log("time to prepare="+time);
        g.order_extra_time[g.id]=Math.floor(Math.random()*10);
        g.order_total_minutes[g.id]=g.order_minutes[g.id]+g.order_extra_time[g.id];
        console.log("extra_time="+g.order_extra_time[g.id]);
        var curr_order=g.id;
        var old_order;
        var new_order=document.createElement('div');
        var order_time=document.createElement('div');
        var price=document.createElement('div');
        var view_bill_button=document.createElement('div');
        var direction_button=document.createElement('img');
        var countdown_timmer=document.createElement('div');
        var enable_notification=document.createElement('img');
        var disable_notification=document.createElement('img');
        var countdown;
        if((old_order=document.getElementById("food_view_orders_order_"+g.id))!=null)
            old_order.parentNode.removeChild(old_order);
        new_order.id="food_view_orders_order_"+g.id;
        new_order.className="food_view_orders_order";
        order_time.innerHTML=current_date.getHours()+":"+numberpad2(current_date.getMinutes());
        order_time.className="food_view_orders_order_time";
        new_order.appendChild(order_time);
        price.innerHTML=parseFloat(g.bill).toFixed(2)+"€";
        price.className="food_view_orders_order_total_price";
        new_order.appendChild(price);
        view_bill_button.innerHTML="View Bill";
        view_bill_button.className="food_view_orders_order_view_bill_button";
        view_bill_button.onclick=function(){showPastBill(curr_order)};
        new_order.appendChild(view_bill_button);
        direction_button.src="replacer.png";
        direction_button.className="food_view_orders_order_direction_button";
        new_order.appendChild(direction_button);
        countdown_timmer.innerHTML=time+'min';
        countdown_timmer.id="food_view_orders_order_countdown_timmer_"+g.id;
        countdown_timmer.className="food_view_orders_order_countdown_timmer";
        new_order.appendChild(countdown_timmer);
        enable_notification.src="replacer.png";
        enable_notification.id="food_view_orders_order_enable_notification_"+g.id;
        enable_notification.className="food_view_orders_order_enable_notification";
        new_order.appendChild(enable_notification);
        disable_notification.src="replacer.png";
        disable_notification.id="food_view_orders_order_disable_notification_"+g.id;
        disable_notification.className="food_view_orders_order_disable_notification";
        new_order.appendChild(disable_notification);
        //insere em primeiro a ordem acabada de criar
        var order_list=document.getElementById('food_view_orders')
        order_list.insertBefore(new_order, order_list.firstChild);
        g.order_countdown_var[g.id]=setInterval(function(){function_countdown_timmer(curr_order); }, 1000);

        time=0;
        g.bill=0;
        var updated_bill_number=document.getElementById("food_bill_number");
        updated_bill_number.innerHTML="0.00€";
        for (var i = 0; i < g.options_quantity.length; i++)
            for(var j =0;j<g.options_quantity[i].length;j++){
                g.order_quantity[g.id][i][j]=g.options_quantity[i][j];
                g.options_quantity[i][j]=0;
            }
        g.stack=[];
        open_food_first_screen();

    }

    function open_food_first_screen(){
        curr_screen.style.display="none";
        document.getElementById('food_bill_bar').style.display='none';
        curr_screen=document.getElementById('food_first_screen');
        curr_screen.style.display="block";
        g.stack.push("open_food_first_screen()");
    }

    function function_countdown_timmer(order) {

        var aux_div;
        var time_aux;
        g.order_minutes[order]--;
        var countdown_timmer=document.getElementById("food_view_orders_order_countdown_timmer_"+order);
        countdown_timmer.innerHTML =g.order_minutes[order]+'min';//update no timmer
        if(g.order_minutes[order]==0){
            //open dialog??+disable real buttons while dialog open.
            console.log("minutes=0!!");
            aux_div=document.getElementById("food_view_orders_order_enable_notification_"+order);
            aux_div.parentNode.removeChild(aux_div);
            aux_div=document.getElementById("food_view_orders_order_disable_notification_"+order);
            aux_div.parentNode.removeChild(aux_div);
            aux_div=document.getElementById("food_view_orders_order_"+order);
            aux_div.style.backgroundColor="yellow";
        }
        if((-g.order_minutes[order])==g.order_extra_time[order]){//pessoa foi levantar encomenda
            console.log("minutes=extra_time!!");
            clearInterval(g.order_countdown_var[order]);
            time_aux=g.order_beginDate[order].getMinutes()+g.order_total_minutes[order];
            countdown_timmer.innerHTML=((g.order_beginDate[order].getHours()+Math.floor(time_aux/60))%24)+':'+numberpad2(time_aux%60);
            countdown_timmer.style.color="green";
            aux_div=document.getElementById("food_view_orders_order_"+order);
            aux_div.style.backgroundColor="lightgray";

        }
        else if(g.order_minutes[order]==-5){//pessoa chegou tarde demais
            console.log("minutes=-5!!");
            clearInterval(g.order_countdown_var[order]);
            countdown_timmer.innerHTML="EXPIRED";
            countdown_timmer.style.color="black";
            aux_div=document.getElementById("food_view_orders_order_"+order);
            aux_div.style.backgroundColor="OrangeRed ";
        }
    }
    //usado no countdown e no purchase
    function numberpad2(number){
        if(number<10)
            return'0'+number;
        return number;
    }

    function open_food_select_category(){
        curr_screen.style.display="none";
        curr_screen=document.getElementById('food_select_category');
        curr_screen.style.display="block";
        curr_screen.scrollTop=0;
        document.getElementById("food_bill_bar").style.display="block";
        g.stack.push('open_food_select_category()');

    }

    function open_food_view_orders(){
        var div_cell;
        if(g.order_quantity[g.id][0][0]==undefined){
            if(document.getElementById("no_orders_made")==null){
                div_cell=document.createElement('div');
                console.log('no orders made');
                div_cell.innerHTML='no orders made';
                div_cell.id="no_orders_made";
                div_cell.className='zero_items_msg';
                document.getElementById('food_view_orders').appendChild(div_cell);
            }
        }
        else if((div_cell=document.getElementById("no_orders_made"))!=null){
            div_cell.parentNode.removeChild(div_cell);
            console.log('no orders made sign removed');
        }
        curr_screen.style.display="none";
        curr_screen=document.getElementById('food_view_orders');
        curr_screen.style.display='block';
        g.stack.push('open_food_view_orders()');

    }
    function save_buy(){
        back();
    }

    function pick(option){
        console.log(option);
        console.log(options_img[option]);
        var length=options_img[option].length;
        var MAXlength=5;
        var i;
        curr_screen.style.display= "none";
        for (i = 0; i < length ; i++) {
            document.getElementById('food_option_'+i).style.display= "block";;
            document.getElementById('food_option_'+i+'_quant').innerHTML=
                'x'+g.options_quantity[option][i];
            document.getElementById('food_option_'+i+'_price').innerHTML=
                parseFloat(options_price[option][i]).toFixed(2)+'€';
            document.getElementById('food_option_'+i+'_name').innerHTML=
                options_name[option][i];
            document.getElementById('food_option_'+i+'_time').innerHTML=
                Math.round(options_time[option][i]*(Math.random()*0.2+0.4)*(1+g.options_quantity[option][i]))+'min';
        }
        for (; i < MAXlength ; i++) {
            curr_screen=document.getElementById('food_option_'+i);
            curr_screen.style.display= "none";

        }
        curr_screen=document.getElementById('options_menu');
        curr_screen.style.display= "block";
        g.curr_section[0]=option;
        document.getElementById('options_menu').scrollTop=0;
        document.getElementById("food_bill_bar").style.display="block";
        g.stack.push('pick('+option+')');
    }

    function buy2(option1,option2){
        g.curr_section[0]=option1;
        buy(option2);
    }


    function buy(option){
        curr_screen.style.display= "none";
        g.curr_section[1]=option;
        expected_line_time=options_time[g.curr_section[0]][g.curr_section[1]]*(Math.random()*0.2+0.4);
        time_per_quantity=
            (Math.random()*0.2+0.4)*options_time[g.curr_section[0]][g.curr_section[1]];
        document.getElementById('food_buy_img').src=options_img[g.curr_section[0]][g.curr_section[1]];
        document.getElementById('food_buy_price').innerHTML=
            parseFloat(options_price[g.curr_section[0]][g.curr_section[1]]).toFixed(2)+'€';
        document.getElementById('food_buy_name').innerHTML=
            options_name[g.curr_section[0]][g.curr_section[1]];
        document.getElementById('food_buy_display_quantity').innerHTML=
            g.options_quantity[g.curr_section[0]][g.curr_section[1]];
        document.getElementById('food_buy_time').innerHTML=
            Math.round(expected_line_time+time_per_quantity*g.options_quantity[g.curr_section[0]][g.curr_section[1]])+'min';
        curr_screen=document.getElementById('food_buy');
        curr_screen.style.display= "block";
        document.getElementById("food_bill_bar").style.display="block";
        g.stack.push('buy2('+g.curr_section[0]+','+g.curr_section[1]+')');
    }

    function add_quantity(){
        if(g.options_quantity[g.curr_section[0]][g.curr_section[1]]<99){
            if(g.bill+(+(options_price[g.curr_section[0]][g.curr_section[1]]).toFixed(2))>=1000){
                console.log('bill is over 1000.00€');
                console.log('bill is'+g.bill);
                console.log('price is'+(options_price[g.curr_section[0]][g.curr_section[1]]).toFixed(2));
                console.log('the sum is'+(g.bill+(+(options_price[g.curr_section[0]][g.curr_section[1]]).toFixed(2))));
                document.getElementById("info_msg").innerHTNL="Bill can't get to 1000.00€";
                document.getElementById("info_msg").style.display='block';
                return;
            }
            document.getElementById('food_buy_display_quantity').innerHTML=
                ++g.options_quantity[g.curr_section[0]][g.curr_section[1]];
            g.bill+= +(options_price[g.curr_section[0]][g.curr_section[1]]).toFixed(2);
            document.getElementById('food_bill_number').innerHTML=parseFloat(g.bill).toFixed(2)+'€';
            document.getElementById('food_buy_time').innerHTML=
                Math.round(expected_line_time+time_per_quantity*g.options_quantity[g.curr_section[0]][g.curr_section[1]])+'min';

        }

    }
    function subtract_quantity(){
        if(g.options_quantity[g.curr_section[0]][g.curr_section[1]]>0){
            document.getElementById("info_msg").style.display='none';
            document.getElementById('food_buy_display_quantity').innerHTML=
                --g.options_quantity[g.curr_section[0]][g.curr_section[1]];
            g.bill= +(Math.abs(g.bill-options_price[g.curr_section[0]][g.curr_section[1]])).toFixed(2);
            document.getElementById('food_bill_number').innerHTML=
                parseFloat(g.bill).toFixed(2)+'€';
            document.getElementById('food_buy_time').innerHTML=
                Math.round(expected_line_time+time_per_quantity*g.options_quantity[g.curr_section[0]][g.curr_section[1]])+'min';
        }
    }


    function open_purchase(){
        curr_screen.style.display="none";
        var aux;
        for (var i =  0; i < g.options_quantity.length; i++) {
            for (var j =  0; j < g.options_quantity[i].length; j++) {
                if(g.options_quantity[i][j]>0 &&
                    ((aux=Math.round(options_time[i][j]*(Math.random()*0.2+0.4)*(1+g.options_quantity[i][j])))>time)){
                    time=aux;
                }
            }
        }
        document.getElementById("food_purchase_total").innerHTML=
            'total:'+parseFloat(g.bill).toFixed(2)+'€';
        document.getElementById("food_purchase_time").innerHTML=time+'min';
        if(g.bill==0){
            document.getElementById('food_purchase_order_button').style.backgroundColor='lightgray';
            document.getElementById('food_purchase_order_button').onclick=function(){};
        }
        else{
            document.getElementById('food_purchase_order_button').style.backgroundColor='FireBrick';
            document.getElementById('food_purchase_order_button').
                onclick=function(){make_purchase()};
        }
        document.getElementById("food_bill_bar").style.display="none";
        curr_screen=document.getElementById("food_purchase");
        curr_screen.style.display="block";
        g.stack.push("open_purchase()");
    }

    function showBill(){
        var div_cell;
        var div_name;
        var div_quant;
        var div_time;
        var div_total_price;
        var save_i;
        var save_j;
        var current_bill = document.getElementById("current_bill");
        current_bill.parentNode.removeChild(current_bill);
        current_bill=document.createElement('div');
        current_bill.id='current_bill';
        current_bill.className='scrollable_hmax';
        document.getElementById('ecra_total').appendChild(current_bill);
        current_bill.scrollTop=0;
        if(g.bill==0){
            div_cell=document.createElement('div');
            console.log('no items to show');
            div_cell.innerHTML='no items added';
            div_cell.className='zero_items_msg';
            current_bill.appendChild(div_cell);
        }
        var contador;
        for (var i =  0; i < g.options_quantity.length; i++) {
            contador=0;
            for (var j = 0; j < g.options_quantity[i].length; j++){
                if(g.options_quantity[i][j]!=0){
                    contador++;
                }
            }
            if (contador==0){
                continue;
            }
            div_cell=document.createElement('div');
            div_cell.innerHTML=categories[i]+':';
            div_cell.style.fontSize='0.17in';
            div_cell.setAttribute("onClick", "pick("+i+");" );
            current_bill.appendChild(div_cell);
            for (var j = 0; j < g.options_quantity[i].length; j++) {
                if(g.options_quantity[i][j]!=0){
                    div_cell=document.createElement('div');
                    div_name=document.createElement('div');
                    div_quant=document.createElement('div');
                    div_time=document.createElement('div');
                    div_total_price=document.createElement('div');
                    div_cell.className='food_bill_cell';
                    div_cell.setAttribute("onClick", "buy2("+i+","+j+");" );
                    div_name.className='food_bill_cell_name';
                    div_name.innerHTML=options_name[i][j];
                    div_quant.className='food_bill_cell_quant';
                    div_quant.innerHTML='x'+g.options_quantity[i][j];
                    div_time.className='food_bill_cell_time';
                    div_time.innerHTML=
                        Math.round(options_time[i][j]*(Math.random()*0.2+0.4)*
                            (1+g.options_quantity[i][j]))+'min';
                    div_total_price.className='food_bill_cell_total_price';
                    div_total_price.innerHTML=
                        parseFloat(options_price[i][j]*g.options_quantity[i][j]).toFixed(2)+'€';
                    div_cell.appendChild(div_name);
                    div_cell.appendChild(div_quant);
                    div_cell.appendChild(div_time);
                    div_cell.appendChild(div_total_price);
                    current_bill.appendChild(div_cell);
                }
            }
        }
        curr_screen.style.display="none";
        curr_screen=current_bill;
        document.getElementById('food_bill_bar').style.display='none';
        g.stack.push("showBill()");
    }



    function showPastBill(order){
        var div_cell;
        var div_name;
        var div_quant;
        var div_total_price;
        var current_bill = document.getElementById("current_bill");
        current_bill.parentNode.removeChild(current_bill);
        current_bill=document.createElement('div');
        current_bill.id='current_bill';
        current_bill.className='scrollable_hmax';
        document.getElementById('ecra_total').appendChild(current_bill);
        current_bill.scrollTop=0;
        var contador;
        for (var i =  0; i <  g.order_quantity[order].length; i++) {
            contador=0;
            for (var j = 0; j < g.order_quantity[order][i].length; j++){
                if(g.order_quantity[order][i][j]!=0){
                    contador++;
                }
            }
            if (contador==0){
                continue;
            }
            div_cell=document.createElement('div');
            div_cell.innerHTML=categories[i]+':';
            div_cell.style.fontSize='0.17in';
            current_bill.appendChild(div_cell);
            for (var j = 0; j < g.order_quantity[order][i].length; j++) {
                if(g.order_quantity[order][i][j]!=0){
                    div_cell=document.createElement('div');
                    div_name=document.createElement('div');
                    div_quant=document.createElement('div');
                    div_total_price=document.createElement('div');
                    div_cell.className='food_bill_cell';
                    div_name.className='food_bill_cell_name';
                    div_name.innerHTML=options_name[i][j];
                    div_quant.className='food_bill_cell_quant';
                    div_quant.innerHTML='x'+g.order_quantity[order][i][j];
                    div_total_price.className='food_bill_cell_total_price';
                    div_total_price.innerHTML=
                        parseFloat(options_price[i][j]*g.order_quantity[order][i][j])
                            .toFixed(2)+'€';
                    div_cell.appendChild(div_name);
                    div_cell.appendChild(div_quant);
                    div_cell.appendChild(div_total_price);
                    current_bill.appendChild(div_cell);
                }
            }
        }
        curr_screen.style.display="none";
        curr_screen=current_bill;
        g.stack.push("showPastBill("+order+")");
    }



    function showWorks(){
        curr_screen.style.display="none";
    }
    return_to_food();

<div id="food_first_screen_view_orders_button" class="half_screen_button" onclick="open_food_view_orders()">
        <div class="half_screen_button_text">
        View Orders
    </div>
    </div>
    <div id="food_first_screen_order_button" class="half_screen_button" onclick="open_food_select_category()">

        $()
});