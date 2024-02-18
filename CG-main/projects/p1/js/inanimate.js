/* global THREE */
'use strict';

function createInanimateObjects() {
    const parent = new THREE.Object3D();
    
    for (let i = 0; i < 3; i++) {
        const pyramid = createPyramid(60 + i * 40, -70, -90 + i * 25);
        pyramid.scale.set(10 - i * 2, 10 - i * 2, 10 - i * 2);
        pyramid.rotateZ(Math.PI);
        parent.add(pyramid);

        const pyramid2 = createPyramid(40 + i * 40, -60, 90 - i * 25);
        pyramid2.scale.set(8 + i, 8 + i, 8 + i);
        parent.add(pyramid2);
    }

    const torus = createTorus(-60, 50, -75);
    torus.scale.set(20,20,20);
    parent.add(torus);

    const sphere = createSphere(-60, 50, 60);
    sphere.scale.set(5, 5, 5);
    sphere.rotateX(Math.PI / 2);
    parent.add(sphere);

    const torus2 = createTorus(100, 0, 25);
    torus2.scale.set(15,15,15);
    parent.add(torus2);

    const cylinder2 = createCylinder(100, 0, -25);
    cylinder2.scale.set(2, 2, 2);
    cylinder2.rotateX(Math.PI / 2);
    parent.add(cylinder2);

    const cylinderstar1 = createCylinderlong(-30, -60, 12);
    cylinderstar1.scale.set(3, 3, 3);
    parent.add(cylinderstar1);

    const cylinderstar2 = createCylinderlong(-30, -60, 24);
    cylinderstar2.scale.set(3, 2, 3);
    cylinderstar2.rotateZ(-Math.PI / 4);
    parent.add(cylinderstar2);

    const cylinderstar3 = createCylinderlong(-30, -60, 0);
    cylinderstar3.scale.set(3, 3, 3);
    cylinderstar3.rotateZ(Math.PI / 2);
    parent.add(cylinderstar3);

    const cylinderstar4 = createCylinderlong(-30, -60, 36);
    cylinderstar4.scale.set(3, 2, 3);
    cylinderstar4.rotateZ(Math.PI / 4);
    parent.add(cylinderstar4);

    const cube = createCube(0, 0, -150);
    cube.scale.set(200, 5, 5);
    cube.rotateZ(Math.PI/6);
    parent.add(cube);

    const cube2 = createCube(90, 0, -125);
    cube2.scale.set(200, 5, 5);
    cube2.rotateZ(-Math.PI/4);
    parent.add(cube2);

    const elipse = createSphere(-100, -20, 50);
    elipse.scale.set(60, 20, 20);
    elipse.rotateZ(Math.PI/4);
    elipse.rotateY(Math.PI/4);
    parent.add(elipse);

    return parent;
} 

function createPyramid(x, y, z) {
    const geometry = new THREE.ConeGeometry(1, 2, 6);
    const mesh = new THREE.Mesh(geometry, createMaterial());
    const pyramid = new THREE.Object3D();
    pyramid.add(mesh);
    pyramid.position.set(x, y, z);
    return pyramid;
}

function createTorus(x, y, z) {
    const geometry = new THREE.TorusGeometry(10, 3, 20, 20);
    const mesh = new THREE.Mesh(geometry, createMaterial());
    mesh.scale.set(0.1, 0.1, 0.1);
    const torus = new THREE.Object3D();
    torus.add(mesh);
    torus.position.set(x, y, z);
    return torus;
}

function createCylinder(x, y, z) {
    const geometry = new THREE.CylinderGeometry(3, 3, 4, 20);
    const mesh = new THREE.Mesh(geometry, createMaterial());
    const cylinder = new THREE.Object3D();
    cylinder.add(mesh);
    cylinder.position.set(x, y, z);
    return cylinder;
}

function createSphere(x, y, z) {
    const geometry = new THREE.SphereGeometry(1, 15, 15);
    const mesh = new THREE.Mesh(geometry, createMaterial());
    const sphere = new THREE.Object3D();
    sphere.add(mesh);
    sphere.position.set(x, y, z);
    return sphere;
}

function createCylinderlong(x, y, z) {
    const geometry = new THREE.CylinderGeometry(2, 2, 20, 20);
    const mesh = new THREE.Mesh(geometry, createMaterial());
    const cylinder = new THREE.Object3D();
    cylinder.add(mesh);
    cylinder.position.set(x, y, z);
    return cylinder;
}

function createCube(x, y, z) {
    const geometry = new THREE.BoxGeometry(1, 1, 1);
    const mesh = new THREE.Mesh(geometry, createMaterial());
    const cube = new THREE.Object3D();
    cube.add(mesh);
    cube.position.set(x, y, z);
    return cube;
}
