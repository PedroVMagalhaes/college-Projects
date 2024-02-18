define(function(require) {
        let $ = require('jquery');

        return {
            load_page : function(url) {
                $.get(url, function(data){
                    $("#content-area").html(data)
                });
            },

            load_dialog : function(url) {
                $.get(url, function(data){
                    $("#dialog-wrapper").show();
                    $("#dialog-area").html(data);
                })
            },

            close_dialog : function() {
                $("#dialog-wrapper").hide();
            }
        }
    }
);
