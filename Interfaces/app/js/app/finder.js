define(function () {
    let src = {
        x: 0,
        y: 0,
    };

    // Default values for tgt
    let tgt = {
        x: 5,
        y: 5,
    };

    let previous = null;

    function angle() {
        let x = tgt.x - src.x;
        let y = tgt.y - src.y;

        let angle = null;

        if (y > 0) {
            angle = toDegrees(Math.atan(x / y));
        } else if (y === 0) {
            if (x > 0) {
                angle = 180 + toDegrees(Math.atan(x / y));
            } else {
                angle = toDegrees(Math.atan(x / y));
            }
        } else {
            angle = 180 + toDegrees(Math.atan(x / y));
        }

        return angle;
    }

    function distance() {
        return Math.sqrt(Math.pow(src.x - tgt.x, 2) + Math.pow(src.y - tgt.y, 2));
    }

    function toDegrees(rad) {
        return rad * (180 / Math.PI);
    }

    function move(dir) {

        previous = src;

        if (dir === "up")
            src.y++;
        else if (dir === "down")
            src.y--;
        else if (dir === "right")
            src.x++;
        else if (dir === "left")
            src.x--;
        else
            console.log("Invalid direction!");
        this.move_callback();
    }

    return {
        move: move,
        move_callback: () => {},
        distance: distance,
        angle: angle,
        src: src,
        tgt: tgt
    }
});