requirejs.config({
    baseUrl: 'js/lib',
    paths: {
        app: '../app',
        main: '../'
    }
});

requirejs(['app/pageManager', 'app/finder', 'app/wc'], function(pm, finder, wc) {
    $(document).ready(() => {
        $("#lock-btn").click(function () {
            if (pm.locked()) {
                $("#lock-btn").attr("src", "img/unlocked.png");
                pm.unlock();
            } else {
                $("#lock-btn").attr("src", "img/locked.png");
                pm.lock();
            }
        });

        $("#menu-btn").click(() => {
            pm.change_page("menu.html");
        });

        $("#back-btn").click(function() {
            pm.back();
        });

        $("#X").attr("placeholder", "X = " + finder.tgt.x);
        $("#Y").attr("placeholder", "Y = " + finder.tgt.y);

        $("#setpos").click(function() {
            finder.tgt.x = $("#X").val();
            finder.tgt.y = $("#Y").val();
        });

        $("#move_left").click(function() {
            finder.move("left");
        });

        $("#move_right").click(function() {
            finder.move("right");
        });

        $("#move_down").click(function() {
            finder.move("down");
        });

        $("#move_up").click(function() {
            finder.move("up");
        });

        wc.notify = () => {
            console.log("Timed out!");
            pm.open_dialog("wc-dialog.html");
        };

        $("#help-btn").click(() => { pm.change_page("help.html") });

        $("#dialog-area").click(pm.close_dialog);
        pm.change_page("home.html");
    });
});