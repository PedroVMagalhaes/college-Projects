/* global THREE */
'use strict';

const renderer = new THREE.WebGLRenderer();
const clock = new THREE.Clock();

const worldWidth = 200;
const worldHeight = 180;

let scene, camera, pauseScene;
let pausedCamera;
let paused = false;
const lights = [];
const lightState = [true, true, true, true];
const materials = [];
const objects = [];
const origamis = [];

let currentMaterial = 0;
const angularSpeed = 5;
const directionalLightIntensity = 0.6;
const spotlightIntensity = 1;

const cameras = {
    current: 0,
    cameras: [undefined, undefined, undefined],
};

const input = {
    reset: false,
    camera: {
        changed: false,
        number: 0,
    },
    lights: [false, false, false, false],
    lightsOff: false,
    rotation: [
        [false, false],
        [false, false],
        [false, false],
    ],
    material: false,
};

function init() {
    renderer.setSize(window.innerWidth, window.innerHeight);
    
    renderer.xr.enabled = true;
    renderer.setAnimationLoop(animate);
    document.body.appendChild(VRButton.createButton( renderer ));
    document.body.appendChild(renderer.domElement);
    
    createPauseScene();
    createScene();
    createCameras();
        
    window.addEventListener("resize", onResize);
    window.addEventListener("keydown", onKeydown);
    window.addEventListener("keyup", onKeyup);
}

function setCamera(index) {
    input.camera.changed = true;
    input.camera.number = index;
}

function onKeyup(e) {
    switch (e.keyCode) {
        case 68:
        case 100: 
            input.lights[0] = true;
            break;
        
        case 90:
        case 122:
            input.lights[1] = true;
            break;
        
        case 88:
        case 120:
            input.lights[2] = true;
            break;
        
        case 67:
        case 99:
            input.lights[3] = true;
            break;
        
        case 81:
        case 113:
            input.rotation[0][0] = false;
            break;
        case 87:
        case 119:
            input.rotation[0][1] = false;
            break;
        
        case 69:
        case 101:
            input.rotation[1][0] = false;
            break;
        case 82:
        case 114:
            input.rotation[1][1] = false;
            break;

        case 84:
        case 116:
            input.rotation[2][0] = false;
            break;
        case 89:
        case 121:
            input.rotation[2][1] = false;
            break;

        case 65:
        case 97:
            input.material = true;
            break;

        case 83:
        case 115:
            input.lightsOff = !input.lightsOff;
            break;

        case 49:
            setCamera(0)
            break;
        case 50:
            setCamera(1)
            break;

        case 51:
            input.reset = true;
            break;

        case 32:
            paused = !paused;
            break;
    }
}

function onKeydown(e) {
    switch (e.keyCode) {
        case 81:
        case 113:
            input.rotation[0][0] = true;
            break;
        case 87:
        case 119:
            input.rotation[0][1] = true;
            break;
        
        case 69:
        case 101:
            input.rotation[1][0] = true;
            break;
        case 82:
        case 114:
            input.rotation[1][1] = true;
            break;

        case 84:
        case 116:
            input.rotation[2][0] = true;
            break;
        case 89:
        case 121:
            input.rotation[2][1] = true;
            break;
    }
}

function onResize() {
    renderer.setSize(window.innerWidth, window.innerHeight);

    if (window.innerHeight > 0 && window.innerWidth > 0) {
        const currentAspect = window.innerWidth / window.innerHeight;
        const targetAspect = worldWidth / worldHeight;

        const halfHeight = worldHeight / 2;
        const halfWidth = worldWidth / 2;
        
        if (currentAspect > targetAspect) {
            cameras.cameras[1].left = -halfHeight * currentAspect;
            cameras.cameras[1].right = halfHeight * currentAspect;
            cameras.cameras[1].top = halfHeight;
            cameras.cameras[1].bottom = -halfHeight;

            pausedCamera.left = -halfHeight * currentAspect;
            pausedCamera.right = halfHeight * currentAspect;
            pausedCamera.top = halfHeight;
            pausedCamera.bottom = -halfHeight;
        } else {
            cameras.cameras[1].left = -halfWidth;
            cameras.cameras[1].right = halfWidth;
            cameras.cameras[1].top = halfWidth / currentAspect;
            cameras.cameras[1].bottom = -halfWidth / currentAspect;

            pausedCamera.left = -halfWidth;
            pausedCamera.right = halfWidth;
            pausedCamera.top = halfWidth / currentAspect;
            pausedCamera.bottom = -halfWidth / currentAspect;
        }
        cameras.cameras[1].updateProjectionMatrix();

        cameras.cameras[0].aspect = currentAspect;
        cameras.cameras[0].updateProjectionMatrix();

        pausedCamera.updateProjectionMatrix();
    }
}

function createPauseScene() {
    pauseScene = new THREE.Scene();

    const texture = new THREE.TextureLoader().load('textures/paused.png');
    const material = new THREE.MeshBasicMaterial({
        map: texture,
        transparent: true,
        opacity: 0.6,
    });
    const geometry = new THREE.PlaneGeometry(worldWidth, worldHeight);
    const mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(0, 0, 0);
    pauseScene.add(mesh);

    const currentAspect = window.innerWidth / window.innerHeight;
    const targetAspect = worldWidth / worldHeight;
    const halfWidth = worldWidth / 2;
    const halfHeight = worldHeight / 2;

    if (currentAspect > targetAspect) {
        pausedCamera = new THREE.OrthographicCamera(-halfHeight * currentAspect, halfHeight * currentAspect, halfHeight, -halfHeight, 1, 1000);
    } else {
        pausedCamera = new THREE.OrthographicCamera(-halfWidth, halfWidth, halfWidth / currentAspect, -halfWidth / currentAspect, 1, 1000);
    }

    pausedCamera.position.set(0, 0, 5);
    pausedCamera.lookAt(pauseScene.position);
}

function createScene() {
    scene = new THREE.Scene();
    createBackground();
    createOrigami();

    scene.position.set(0, -100, -100);
    const directionalLight = new THREE.DirectionalLight(0xffffff, directionalLightIntensity);
    directionalLight.position.set(90, 140, 100);
    directionalLight.target.position.set(0, -100, -100);
    directionalLight.target.updateMatrixWorld();

    scene.add(directionalLight);
    lights.push(directionalLight);

    const spotlight1 = createSpotlight(-45, 120, 25);
    const spotlight2 = createSpotlight(0, 120, 25);
    const spotlight3 = createSpotlight(45, 120, 25);

    scene.add(spotlight1);
    scene.add(spotlight2);
    scene.add(spotlight3);

    lights.push(spotlight1);
    lights.push(spotlight2);
    lights.push(spotlight3);
}

function createMaterials(options) {
    const phong = new THREE.MeshPhongMaterial(options);
    const lambert = new THREE.MeshLambertMaterial(options);
    const mats = [phong, lambert];
    materials.push(mats);
    return { id: materials.length - 1, materials: mats };
}

function createBackground() {
    const geometry = new THREE.PlaneGeometry(500, 500);
    const planeMaterials = createMaterials({ color: 0xffddaa });
    const plane = new THREE.Mesh(geometry, planeMaterials.materials[0]);
    plane.userData = { materialId: planeMaterials.id };
    plane.rotateX(- Math.PI / 2);
    scene.add(plane);
    objects.push(plane);

    const wood = createMaterials({color: 0xaa4400, specular: 0xffffff});
    const geometry2 = new THREE.BoxGeometry(200, 25, 100, 10);
    const cube = new THREE.Mesh(geometry2, wood.materials[0]);
    cube.userData = { materialId: wood.id };
    cube.position.set(0, 12.5, 0);
    scene.add(cube);
    objects.push(cube);

    const geometry3 = new THREE.BoxGeometry(180, 25, 80, 10);
    const cube2 = new THREE.Mesh(geometry3, wood.materials[0]);
    cube2.userData = { materialId: wood.id };
    cube2.position.set(0, 37.5, 0);
    scene.add(cube2);
    objects.push(cube2);

}

function createOrigami() {
    const texture = new THREE.TextureLoader().load( 'textures/2871.jpg' );
    
    const materialTexture = createMaterials({ map: texture, side: THREE.FrontSide });
    const materialPaper = createMaterials({ color: 0xffffff, side: THREE.BackSide });

    const geometry = new THREE.BufferGeometry();
    const vertices = new Float32Array( [
        0, -15, 0,
        15, 0,  -5,
        0,  15, 0,
        
        0,  15, 0,
        -15, 0,  -5,
        0, -15, 0,
    ] );

    var uvs = new Float32Array([
        0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        
        1.0, 1.0,
        0.0, 1.0,
        0.0, 0.0,
      ])
    
    geometry.setAttribute( 'position', new THREE.BufferAttribute( vertices, 3 ) );
    geometry.setAttribute('uv', new THREE.BufferAttribute( uvs, 2) )
    geometry.computeVertexNormals();

    const group = new THREE.Group();

    const front = new THREE.Mesh(geometry, materialPaper.materials[0]);
    objects.push(front);
    front.userData = { materialId: materialPaper.id };
    group.add(front);

    const back = new THREE.Mesh(geometry, materialTexture.materials[0]);
    back.userData = { materialId: materialTexture.id };
    objects.push(back);
    group.add(back);
    
    scene.add(group);
    group.position.set(-45,80,0)
    origamis.push(group);

    const geometry2 = new THREE.BufferGeometry();
    const vertices2 = new Float32Array( [
        0, -23, 0,
        4.2, -1.5, -0.85, 
        0, 0, 1, 

        0, -23, 0,
        0, 0, 1, 
        -4.2, -1.5, -0.85, 

        0, 0, 1,
        -4.2, -1.5, -0.85,
        -5, 2, -1,

        0, 0, 1,
        5, 2, -1,
        4.2, -1.5, -0.85,

        0, -23, 0,
        0, 7, 0, 
        -5, 2, -1, 

        0, -23, 0,
        5, 2, -1, 
        0, 7, 0, 

        0, -23, 0,
        0, -2, -0.5, 
        4.2, -1.5, -0.85, 
        
        0, -23, 0,
        -4.2, -1.5, -0.85, 
        0, -2, -0.5, 0
    ] );

    var uvs2 = new Float32Array([
        1.0, 0.0,
        0.82, 1.0,
        0.62, 1.0,
        1.0, 0.0,
        0.0, 0.68, 
        0.0, 0.18,
        0.0, 0.38,
        0.0, 0.18,
        0.0, 0.68,
        0.0, 0.38,
        0.68, 1.0,
        0.82, 1.0,
        1.0, 0.0,
        0.0, 1.0,
        0.0, 0.68,
        1.0, 0.0,
        0.68, 1.0,
        0.0, 1.0,
        1.0, 0.0,
        1.0, 1.0,
        0.82, 1.0,
        1.0, 0.0,
        0.0, 0.18,
        0.0, 0.0, 
      ])
      
    geometry2.setAttribute( 'position', new THREE.BufferAttribute( vertices2, 3 ) );
    geometry2.setAttribute('uv', new THREE.BufferAttribute( uvs2, 2) )
    geometry2.computeVertexNormals();

    const group2 = new THREE.Group();

    const front2 = new THREE.Mesh(geometry2, materialPaper.materials[0]);
    objects.push(front2);
    front2.userData = { materialId: materialPaper.id };
    group2.add(front2);

    const back2 = new THREE.Mesh(geometry2, materialTexture.materials[0]);
    back2.userData = { materialId: materialTexture.id };
    objects.push(back2);
    group2.add(back2);
    
    scene.add(group2);
    group2.position.set(0, 85, 0);

    origamis.push(group2);

    const geometry3 = new THREE.BufferGeometry();
    const vertices3 = new Float32Array( [
        0, 0, 1,
        6, 4.5, 0,
        -1.5, 3, 0,
        0, 0, 1,
        5, 0, 1,
        6, 4.5, 0,
        5, 0, 1,
        6, 4.5, 0,
        9, 0, 1,
        9, 0, 1,
        13, 5.5, 0,
        6, 4.5, 0,
        0, 0, 1,
        1, 11.5, 1,
        -1.5, 3, 0,
        -1.5, 3, 0,
        1, 11.5, 1,
        -0.5, 11, 0,
        -2, 10, 0,
        1, 11.5, 1,
        0.5, 12, 0,
        0, 0, -1,
        -1.5, 3, 0,
        6, 4.5, 0,
        0, 0, -1,
        6, 4.5, 0,
        5, 0, -1,
        5, 0, -1,
        9, 0, -1,
        6, 4.5, 0,
        9, 0, -1,
        6, 4.5, 0,
        13, 5.5, 0,
        0, 0, -1,
        -1.5, 3, 0,
        1, 11.5, -1,
        -1.5, 3, 0,
        -0.5, 11, 0,
        1, 11.5, -1,
        -2, 10, 0,
        0.5, 12, 0,
        1, 11.5, -1,
    ] );

    var uvs3 = new Float32Array([
        0.86, 0.79,
        0.0, 0.68,
        0.5, 0.5,
        0.86, 0.79,
        0.62, 1.0,
        0.0, 0.68,
        0.62, 1.0,
        0.0, 0.68,
        0.68, 1.0,
        0.68, 1.0,
        0.0, 1.0,
        0.0, 0.68,
        0.86, 0.79,
        0.95, 0.24,
        0.5, 0.5,
        0.5, 0.5,
        0.95, 0.24,
        0.0, 0.0,
        1.0, 0.0,
        0.95, 0.24,
        0.86, 0.14,

        0.86, 0.79,
        0.5, 0.5,
        0.0, 0.68,
        0.86, 0.79,
        0.0, 0.68,
        0.62, 1.0,
        0.62, 1.0,
        0.68, 1.0,
        0.0, 0.68,
        0.68, 1.0,
        0.0, 0.68,
        0.0, 1.0,
        0.86, 0.79,
        0.5, 0.5,
        0.95, 0.24,
        0.5, 0.5,
        0.0, 0.0,
        0.95, 0.24,
        1.0, 0.0,
        0.86, 0.14,
        0.95, 0.24,
      ])
    
    geometry3.setAttribute( 'position', new THREE.BufferAttribute( vertices3, 3 ) );

    geometry3.setAttribute('uv', new THREE.BufferAttribute( uvs3, 2) )
    geometry3.computeVertexNormals();

    const group3 = new THREE.Group();

    const front3 = new THREE.Mesh(geometry3, materialPaper.materials[0]);
    objects.push(front3);
    front3.userData = { materialId: materialPaper.id };
    group3.add(front3);

    const back3 = new THREE.Mesh(geometry3, materialTexture.materials[0]);
    back3.userData = { materialId: materialTexture.id };
    objects.push(back3);
    group3.add(back3);
    
    scene.add(group3);
    group3.position.set(42,70,0)
    origamis.push(group3);
}

function createSpotlight(x, y, z) {
    const spotlight = new THREE.SpotLight(0xffffff, spotlightIntensity, 150, Math.PI / 6);
    spotlight.position.set(x, y, z);
    spotlight.target.position.set(x, 50-100, z-25-100);
    spotlight.target.updateMatrixWorld();

    const coneGeometry = new THREE.ConeGeometry(10, 10, 10);
    const material = new THREE.MeshPhongMaterial({ color: 0xffffff });
    const cone = new THREE.Mesh(coneGeometry, material);
    spotlight.add(cone);

    const sphereGeometry = new THREE.SphereGeometry(3, 10, 10);
    const material2 = new THREE.MeshBasicMaterial({ color: 0xffff00 });
    const sphere = new THREE.Mesh(sphereGeometry, material2);
    sphere.position.set(0, -5, 0);
    cone.add(sphere);

    return spotlight;
}

function animate() {
    const delta = paused ? 0 : clock.getDelta();
    
    if (input.camera.changed) {
        cameras.current = input.camera.number;
    }
    
    if (input.reset && paused) {
        reset();
    }
    // Toggle materials
    if (input.material) {
        currentMaterial = currentMaterial == 0 ? 1 : 0;
        for (let i = 0; i < objects.length; i++) {
            objects[i].material = materials[objects[i].userData.materialId][currentMaterial];
        }
    }

    // Toggle lights
    for (let i = 0; i < input.lights.length; i++) {
        if (input.lights[i]) {
            lightState[i] = !lightState[i];
        }
    }

    if (input.lightsOff) {
        for (let i = 0; i < lights.length; i++) {
            lights[i].intensity = 0;
            if (i > 0)
                lights[i].children[0].children[0].material.color.setHex(0x000000);
        }
    } else {
        // Update lights
        for (let i = 0; i < lights.length; i++) {
            if (lightState[i] && lights[i].intensity == 0) {
                lights[i].intensity = i == 0 ? directionalLightIntensity : spotlightIntensity;
                if (i > 0)
                    lights[i].children[0].children[0].material.color.setHex(0xffff00);
            } else if (!lightState[i] && lights[i].intensity != 0) {
                lights[i].intensity = 0;
                if (i > 0)
                    lights[i].children[0].children[0].material.color.setHex(0x000000);
            }
        }
    }

    input.reset = false;
    input.lights = [false, false, false, false];
    input.material = false;
    

    for (let i = 0; i < origamis.length; i++) {
        let rotation = 0;
        if (input.rotation[i][0])       
            rotation += 1;
        if (input.rotation[i][1])
            rotation -= 1;
        
        origamis[i].rotateY(rotation * angularSpeed * delta);
    }

    render();
}

function render() {
    renderer.autoClear = false;
    renderer.clear();
    renderer.render(scene, cameras.cameras[cameras.current]);
    if (paused) {
        renderer.clearDepth();
        renderer.render(pauseScene, pausedCamera);
    }
}

function createCameras() {
    const currentAspect = window.innerWidth / window.innerHeight;
    const targetAspect = worldWidth / worldHeight;

    const halfHeight = worldHeight / 2;
    const halfWidth = worldWidth / 2;

    if (currentAspect > targetAspect) {
        cameras.cameras[1] = new THREE.OrthographicCamera(-halfHeight * currentAspect, halfHeight * currentAspect, halfHeight, -halfHeight, 1, 1000);
    } else {
        cameras.cameras[1] = new THREE.OrthographicCamera(-halfWidth, halfWidth, halfWidth / currentAspect, -halfWidth / currentAspect, 1, 1000);
    }

    cameras.cameras[1].position.set(0, -50, 150);
    cameras.cameras[1].lookAt(0,-50,0);

    cameras.cameras[0] = new THREE.PerspectiveCamera(70, window.innerWidth/window.innerHeight, 1, 1000);
    cameras.cameras[0].position.set(0, 150-100, 150-100);
    cameras.cameras[0].lookAt(scene.position);
}

function reset() {
    for (let i = 0; i < origamis.length; i++) {
        origamis[i].rotation.set(0, 0, 0);
    }

    input.lightsOff = false;
    for (let i = 0; i < lightState.length; i++) {
        lightState[i] = true;
    }

    if (currentMaterial == 1)
        input.material = true;

    setCamera(0); 
    
    paused = false;
}