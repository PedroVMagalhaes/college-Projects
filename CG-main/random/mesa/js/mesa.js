/* global THREE */
'use strict';

let camera, scene, renderer;
let geometry, material, mesh;
let ball;

function init() {
    renderer = new THREE.WebGLRenderer({ antialias: true });
    renderer.setSize(window.innerWidth, window.innerHeight);
    document.body.appendChild(renderer.domElement);

    createScene();
    createCamera();

    window.addEventListener("resize", onResize);
    window.addEventListener("keydown", onKeydown);
}

function onResize() {
    renderer.setSize(window.innerWidth, window.innerHeight);

    if (window.innerHeight > 0 && window.innerWidth > 0) {
        camera.aspect = innerWidth / innerHeight;
        camera.updateProjectionMatrix();
    }
}

function onKeydown(e) {
    switch (e.keyCode) {
        case 65:
        case 97:
            scene.traverse(function (node) {
                if (node instanceof THREE.Mesh) {
                    node.material.wireframe = !node.material.wireframe;
                }
            });
            break;

        case 83:
        case 115:
            ball.userData.jumping = !ball.userData.jumping;
            break;
    }
}

function createScene() {
    scene = new THREE.Scene();
    scene.add(new THREE.AxesHelper(10));

    createTable(0, 0, 0);
    createBall(0, 0, 15);
}

function createCamera() {
    camera = new THREE.PerspectiveCamera(70, window.innerWidth / window.innerHeight, 1, 1000);
    camera.position.x = 50;
    camera.position.y = 50;
    camera.position.z = 50;
    camera.lookAt(scene.position);
}

function createBall(x, y, z) {
    ball = new THREE.Object3D();
    ball.userData = { jumping: true, step: 0 };

    material = new THREE.MeshBasicMaterial({ color: 0xff0000, wireframe: true });
    geometry = new THREE.SphereGeometry(4, 10, 10);
    mesh = new THREE.Mesh(geometry, material);

    ball.add(mesh);
    ball.position.set(x, y, z);
    scene.add(ball);
}

function addTableTop(obj, x, y, z) {
    geometry = new THREE.BoxGeometry(60, 2, 20);
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);
    obj.add(mesh);
}

function addTableLeg(obj, x, y, z) {
    geometry = new THREE.BoxGeometry(2, 6, 2);
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y - 3, z);
    obj.add(mesh);
}

function createTable(x, y, z) {
    const table = new THREE.Object3D();
    material = new THREE.MeshBasicMaterial({ color: 0x00ff00, wireframe: true });

    addTableTop(table, 0, 0, 0);
    addTableLeg(table, -25, -1, -8);
    addTableLeg(table, -25, -1, 8);
    addTableLeg(table, 25, -1, 8);
    addTableLeg(table, 25, -1, -8);

    scene.add(table);
    table.position.x = x;
    table.position.y = y;
    table.position.z = z;

}

function animate() {
    if (ball.userData.jumping) {
        ball.userData.step += 0.04;
        ball.position.y = Math.abs(30 * Math.sin(ball.userData.step));
        ball.position.z = 15 * Math.cos(ball.userData.step);
    }

    render();
    requestAnimationFrame(animate);
}

function render() {
    renderer.render(scene, camera);
}
