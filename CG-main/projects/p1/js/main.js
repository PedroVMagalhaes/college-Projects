/* global THREE */
'use strict';

function init() {
    renderer.setSize(window.innerWidth, window.innerHeight);
    document.body.appendChild(renderer.domElement);

    createScene();
    createCamera();

    window.addEventListener("resize", onResize);
    window.addEventListener("keydown", onKeydown);
    window.addEventListener("keyup", onKeyup);
}

function onResize() {
    renderer.setSize(window.innerWidth, window.innerHeight);

    if (window.innerHeight > 0 && window.innerWidth > 0) {
        const aspect = window.innerWidth / window.innerHeight;
        const halfWidth = cameraHalfHeight * aspect;
        camera.left = -halfWidth;
        camera.right = halfWidth;
        camera.top = cameraHalfHeight;
        camera.bottom = -cameraHalfHeight;
        camera.updateProjectionMatrix();
    }
}

function createScene() {
    scene = new THREE.Scene();

    articulate = createArticulateObject(0, -10, 0);
    articulate[0].scale.set(15, 15, 15);
    scene.add(articulate[0]);

    scene.add(createInanimateObjects());
}

function createCamera() {
    const aspect = window.innerWidth / window.innerHeight;
    const halfWidth = cameraHalfHeight * aspect;

    camera = new THREE.OrthographicCamera(-halfWidth, halfWidth, cameraHalfHeight, -cameraHalfHeight, 1, 1000);
    camera.position.set(0, 0, 500);
    camera.lookAt(scene.position);
}

function handleInput() {
    if (input.wireframe) {
        materials.forEach(material => {
            material.wireframe = !material.wireframe;
        });
        input.wireframe = false;
    }

    if (input.camera.changed) {
        switch (input.camera.number) {
            case 0:
                camera.position.set(0, 0, 500);
                break;
            case 1:
                camera.position.set(0, 500, 0);
                break;
            case 2:
                camera.position.set(500, 0, 0);
                break;
        }

        camera.lookAt(scene.position);
    }
}

function animate() {
    handleInput();
    const delta = clock.getDelta();
    updateArticulateObject(articulate, delta);

    render();
    requestAnimationFrame(animate);
}

function render() {
    renderer.render(scene, camera);
}
