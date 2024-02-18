define(function() {

    let ticketno = 0;

    function reqTicket(timeout = 0) {
        ticketno++;
        console.log("Requested ticket " + ticketno);

        setTimeout(() => {
            if (this.trigger)
                this.notify();
        }, timeout);
    }

    function isWaiting() {
        if (this.deadline != null) {
            return this.deadline >= new Date();
        }
        return false;
    }

    return {
        reqTicket: reqTicket,
        ticketno: ticketno,
        deadline: null,
        trigger : true,
        display_help: true,
        isWaiting: isWaiting,
        notify: () => {},
    }
});