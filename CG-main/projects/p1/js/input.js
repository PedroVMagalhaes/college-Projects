/* global THREE */
'use strict';

const input = {
    wireframe: false,
    camera: {
        changed: false,
        number: 0,
    },
    rotation: {
        pyramid: [false, false],
        octahedron: [false, false],
        cone: [false, false],
    },
    movement: {
        x: [false, false],
        y: [false, false],
        z: [false, false],
    }
};

function setCamera(index) {
    input.camera.changed = true;
    input.camera.number = index;
}

function onKeydown(e) {
    switch (e.keyCode) {
        case 81:
            input.rotation.pyramid[0] = true;
            break;
        case 87:
            input.rotation.pyramid[1] = true;
            break;

        case 65:
            input.rotation.octahedron[0] = true;
            break;
        case 83:
            input.rotation.octahedron[1] = true;
            break;

        case 90:
            input.rotation.cone[0] = true;
            break;
        case 88:
            input.rotation.cone[1] = true;
            break;

        case 37:
            input.movement.x[1] = true;
            break;
        case 38:
            input.movement.y[0] = true;
            break;
        case 39:
            input.movement.x[0] = true;
            break;
        case 40:
            input.movement.y[1] = true;
            break;
        case 99:
        case 67:
            input.movement.z[0] = true;
            break;
        case 100:
        case 68:
            input.movement.z[1] = true;
            break;
    }
}

function onKeyup(e) {
    switch (e.keyCode) {
        case 49:
            setCamera(0);
            break;
        case 50:
            setCamera(1);
            break;
        case 51:
            setCamera(2);
            break;
        case 52:
            input.wireframe = true;
            break;

        case 81:
            input.rotation.pyramid[0] = false;
            break;
        case 87:
            input.rotation.pyramid[1] = false;
            break;
     
        case 65:
            input.rotation.octahedron[0] = false;
            break;
        case 83:
            input.rotation.octahedron[1] = false;
            break;
        case 90:
            input.rotation.cone[0] = false;
            break;
        case 88:
            input.rotation.cone[1] = false;
            break;

        case 37:
            input.movement.x[1] = false;
            break;
        case 38:
            input.movement.y[0] = false;
            break;
        case 39:
            input.movement.x[0] = false;
            break;
        case 40:
            input.movement.y[1] = false;
            break;
        case 99:
        case 67:
            input.movement.z[0] = false;
            break;
        case 100:
        case 68:
            input.movement.z[1] = false;
            break;

    }
}


