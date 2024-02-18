/* global THREE */
'use strict';

function createArticulateObject(x, y, z) {
    const pyramidGeometry = new THREE.ConeGeometry(1, 2, 4);
    const octahedronGeometry = new THREE.OctahedronGeometry(0.5);
    const coneGeometry = new THREE.ConeGeometry(0.5, 1);

    const pyramidMesh = new THREE.Mesh(pyramidGeometry, createMaterial(true));
    const octahedronMesh = new THREE.Mesh(octahedronGeometry, createMaterial(true));
    const coneMesh = new THREE.Mesh(coneGeometry, createMaterial(true));

    const pyramid = new THREE.Object3D();
    const octahedron = new THREE.Object3D();
    const cone = new THREE.Object3D();

    pyramid.position.set(x, y, z);
    pyramid.add(pyramidMesh);
    pyramidMesh.position.set(0, 1, 0);
    pyramid.add(octahedron);
    octahedron.position.set(0, 2, 0);

    octahedron.add(octahedronMesh);
    octahedronMesh.position.set(0, 0.5, 0);
    octahedron.add(cone);
    cone.position.set(0, 1, 0);

    cone.add(coneMesh);
    coneMesh.rotateX(Math.PI);
    coneMesh.position.set(0, 0.5, 0);

    return [pyramid, octahedron, cone];
}

function updateArticulateObject(obj, delta) {
    const move = new THREE.Vector3();
    if (input.movement.x[0])
        move.x += 1;
    if (input.movement.x[1])
        move.x -= 1;
    if (input.movement.y[0])
        move.y += 1;
    if (input.movement.y[1])
        move.y -= 1;
    if (input.movement.z[0])
        move.z += 1;
    if (input.movement.z[1])
        move.z -= 1;
    obj[0].position.add(move.setLength(delta * articulateSpeed));

    // Rotating
    let pyramidRotation = 0;
    if (input.rotation.pyramid[0])
        pyramidRotation += 1;
    if (input.rotation.pyramid[1])
        pyramidRotation -= 1;
    
    let octahedronRotation = 0;
    if (input.rotation.octahedron[0])
        octahedronRotation += 1;
    if (input.rotation.octahedron[1])
        octahedronRotation -= 1;
    
    let coneRotation = 0;
    if (input.rotation.cone[0])
        coneRotation += 1;
    if (input.rotation.cone[1])
        coneRotation -= 1;

    obj[0].rotateY(pyramidRotation * pyramidAngularSpeed * delta);
    if (octahedronRotation * obj[1].rotation.z <= maxAngle)
        obj[1].rotateZ(octahedronRotation * octahedronAngularSpeed * delta);
    if (coneRotation * obj[2].rotation.x <= maxAngle)
        obj[2].rotateX(coneRotation * coneAngularSpeed * delta);
}
