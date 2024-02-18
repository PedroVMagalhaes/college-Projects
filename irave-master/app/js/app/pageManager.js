define(function (require) {
    let loader = require('app/loader');

    let close_fn = () => {}; // Allow to use blocking ACK dialogs

    // Decouple page stack from current and previous
    let pagestack = [];

    let current = null;
    let previous = null;
    let locked = true;
    let dialog_active = false;

    function change_page(page, updateStack = true) {

        console.log("Stack: " + pagestack);
        console.log({
            locked: locked,
            page: page,
            current: current,
            previous: previous,
        });

        if (!(ok_to_change(page)))
            return;

        if (previous !== current)
            previous = current;

        current = page;

        if (updateStack) {
            push_page(previous);
        }

        console.log("Looading " + page);
        loader.load_page(page);
    }

    function ok_to_change(page) {
        let conds = [page !== null,
            current !== page,
            (!locked || page === "home.html"),
            !dialog_active];

        //console.log("Conditions " + conds);
        return conds.every((v) => {return v});
    }

    function push_page(page) {
        if (page != null && !(pagestack.includes(page)))
            pagestack.push(page);
    }

    function pop_page() {
        if (pagestack.length > 0) {
            change_page(pagestack.pop(), false);
        }
    }

    function lock() {
        locked = true;
        change_page("home.html", false);
        this.callbacks.lock();
    }

    function unlock() {
        locked = false;
        change_page(previous, false);
        this.callbacks.unlock();
    }

    function open_dialog(page, fn = () => {}) {
        loader.load_dialog(page);
        close_fn = fn;
        dialog_active = true;
    }

    function close_dialog() {
        dialog_active = false;
        close_fn();
        loader.close_dialog();
    }

    function dialogActive() {
        return dialog_active;
    }

    return {
        // Override lock and back to define specific behaviour
        locked: () => {return locked},
        back: pop_page,
        lock: lock,
        unlock: unlock,
        change_page: change_page,
        open_dialog: open_dialog,
        close_dialog: close_dialog,
        callbacks: {
            lock: () => {},
            unlock: () => {},
        }
    }
});

console.log("Loaded page manager!");
