/* global THREE */
'use strict';

const renderer = new THREE.WebGLRenderer();
const clock = new THREE.Clock();

const worldWidth = 800;
const worldHeight = 600;

let scene;
let rocket;
let trashVector;
let flip = false;

const planetRadius = 200;
let rocketPosition = { lat: Math.random() * Math.PI - Math.PI/2, lon: Math.random() * 2 * Math.PI };
const rocketRadiusPos = 1.2 * planetRadius;
const cameraRadiusPos = 1.5 * planetRadius;

const rocketColisionRadius = planetRadius/22;
const trashColisionRadius = planetRadius/22;
const angularSpeed = 0.9;

const cameras = {
    current: 0,
    cameras: [undefined, undefined, undefined],
};
const input = {
    camera: {
        changed: false,
        number: 0,
    },
    movement: {
        latitude: [false, false],
        longitude: [false, false],
    }
};

function init() {
    renderer.setSize(window.innerWidth, window.innerHeight);
    document.body.appendChild(renderer.domElement);

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
        case 37:
            input.movement.longitude[0] = false;
            break;
        case 38:
            input.movement.latitude[0] = false;
            break;
        case 39:
            input.movement.longitude[1] = false;
            break;
        case 40:
            input.movement.latitude[1] = false;
            break;
        case 49:
            cameras.current = 0;
            break;
        case 50:
            cameras.current = 1;
            break;
        case 51:
            cameras.current = 2;
            break;
    }
}

function onKeydown(e) {
    switch (e.keyCode) {
        case 37:
            input.movement.longitude[0] = true;
            break;
        case 38:
            input.movement.latitude[0] = true;
            break;
        case 39:
            input.movement.longitude[1] = true;
            break;
        case 40:
            input.movement.latitude[1] = true;
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
            cameras.cameras[0].left = -halfHeight * currentAspect;
            cameras.cameras[0].right = halfHeight * currentAspect;
            cameras.cameras[0].top = halfHeight;
            cameras.cameras[0].bottom = -halfHeight;
        } else {
            cameras.cameras[0].left = -halfWidth;
            cameras.cameras[0].right = halfWidth;
            cameras.cameras[0].top = halfWidth / currentAspect;
            cameras.cameras[0].bottom = -halfWidth / currentAspect;
        }
        cameras.cameras[0].updateProjectionMatrix();

        cameras.cameras[1].aspect = window.innerWidth / window.innerHeight;
        cameras.cameras[1].updateProjectionMatrix();

        cameras.cameras[2].aspect = window.innerWidth / window.innerHeight;
        cameras.cameras[2].updateProjectionMatrix();
    }
}

function createScene() {
    scene = new THREE.Scene();
    
    const planet = createPlanet(planetRadius);
    scene.add(planet);

    rocket = createRocket(rocketRadiusPos, planetRadius/11);
    scene.add(rocket);

    createTrash(planetRadius * 1.20, planetRadius/22);
    for (let i = 0; i < trashVector.length; i++) {
        for (let j = 0; j < trashVector[i].length; j++) {
            scene.add(trashVector[i][j]);
        }   
    }
}

function animate() {
    const delta = clock.getDelta();

    updateRocket(delta);
    handleColisions();
    
    render();
    requestAnimationFrame(animate);
}

function render() {
    renderer.render(scene, cameras.cameras[cameras.current]);
}

/* CAMERAS */
function createCameras() {
    const currentAspect = window.innerWidth / window.innerHeight;
    const targetAspect = worldWidth / worldHeight;

    const halfHeight = worldHeight / 2;
    const halfWidth = worldWidth / 2;

    if (currentAspect > targetAspect) {
        cameras.cameras[0] = new THREE.OrthographicCamera(-halfHeight * currentAspect, halfHeight * currentAspect, halfHeight, -halfHeight, 1, 1000);
    } else {
        cameras.cameras[0] = new THREE.OrthographicCamera(-halfWidth, halfWidth, halfWidth / currentAspect, -halfWidth / currentAspect, 1, 1000);
    }

    cameras.cameras[0].position.set(0, 0, 500);
    cameras.cameras[0].lookAt(scene.position);

    cameras.cameras[1] = new THREE.PerspectiveCamera(70, window.innerWidth/window.innerHeight, 1, 1000);
    cameras.cameras[1].position.set(0, 100, 400);
    cameras.cameras[1].lookAt(scene.position);

    cameras.cameras[2] = new THREE.PerspectiveCamera(70, window.innerWidth/window.innerHeight, 1, 1000);
    cameras.cameras[2].lookAt(new THREE.Vector3(0, 40, 40));
    const group = new THREE.Group();
    group.position.set(0, -40, -40);
    group.add(cameras.cameras[2]);
    rocket.add(group);
}

/* PLANET */

function createPlanet(radius) {
    const geometry = new THREE.SphereGeometry(radius, 64, 64);
    const texture = new THREE.TextureLoader().load( 'images/earth.png' );
    texture.magFilter = THREE.NearestFilter;
    texture.minFilter = THREE.NearestFilter;
    const material = new THREE.MeshBasicMaterial( { map: texture } );
    const mesh = new THREE.Mesh(geometry, material);
    const planet = new THREE.Object3D();
    planet.add(mesh);
    return planet;
}

/* ROCKET */

function createRocket(positionRadius, height) {
    const bodyRadius = height * 0.5;
    const topRadius = height * 0.1;
    const capsuleRadius = height * 0.125;

    const bodyHeight = height * 0.5;
    const topHeight = height * 0.25;
    const capsuleHeight = height * 0.375;
    
    const topGeometry = new THREE.CylinderGeometry(topRadius, bodyRadius, topHeight);
    const bodyGeometry = new THREE.CylinderGeometry(bodyRadius, bodyRadius, bodyHeight);
    const capsuleGeometry = new THREE.CapsuleGeometry(capsuleRadius, capsuleHeight);

    const materialRed = new THREE.MeshBasicMaterial({ color: 0xed1f1f });
    const materialBody = new THREE.MeshBasicMaterial({ color: 0xc9d3d6 });
    const topMesh = new THREE.Mesh(topGeometry, materialBody);
    const bodyMesh = new THREE.Mesh(bodyGeometry, materialBody);
    const capsuleMesh1 = new THREE.Mesh(capsuleGeometry, materialRed);
    const capsuleMesh2 = new THREE.Mesh(capsuleGeometry, materialRed);
    const capsuleMesh3 = new THREE.Mesh(capsuleGeometry, materialRed);
    const capsuleMesh4 = new THREE.Mesh(capsuleGeometry, materialRed);

    const rocket = new THREE.Object3D();
    const rocketPos = convertCoords(positionRadius, rocketPosition.lat, rocketPosition.lon);
    rocket.position.set(rocketPos.x, rocketPos.y, rocketPos.z);
    rocket.add(bodyMesh);
    
    rocket.add(topMesh);
    topMesh.position.set(0, bodyHeight/2 + topHeight/2, 0);

    rocket.add(capsuleMesh1);
    capsuleMesh1.position.set(0, -bodyHeight/2, bodyRadius);
    rocket.add(capsuleMesh2);
    capsuleMesh2.position.set(0, -bodyHeight/2, -bodyRadius);
    rocket.add(capsuleMesh3);
    capsuleMesh3.position.set(-bodyRadius, -bodyHeight/2, 0);
    rocket.add(capsuleMesh4);
    capsuleMesh4.position.set(bodyRadius, -bodyHeight/2, 0);

    return rocket;
}

/* TRASH */

function createTrash(positionRadius, radius) {
    const edge = 2*radius*Math.sqrt(3)/3;
    const geometry = new THREE.BoxGeometry(edge, edge, edge);
    const material = new THREE.MeshBasicMaterial({ color: 0x808080 });

    const trashParent = new THREE.Object3D();

    trashVector = new Array(4).fill([]);
    
    for (let i = 0; i < 20; i++) {
        const trash = new THREE.Object3D();
        const mesh = new THREE.Mesh(geometry, material);
        const lat = Math.random() * Math.PI - Math.PI/2;
        const lon = Math.random() * 2 * Math.PI;
        const trashPos = convertCoords(positionRadius, lat, lon);

        trash.position.set(trashPos.x, trashPos.y, trashPos.z);
        trash.add(mesh);
        trashParent.add(trash);
        if (lat >= 0) {
            if (lon <= Math.PI) {
                trashVector[0].push(trash);
            } else {
                trashVector[1].push(trash);
            }  
        } else {
            if (lon <= Math.PI) {
                trashVector[3].push(trash);
            } else {
                trashVector[2].push(trash);
            }  
        }
    }
}

function convertCoords(radius, latitude, longitude) {
    return new THREE.Vector3(radius * Math.cos(latitude) * Math.sin(longitude), radius * Math.cos(Math.PI/2 - latitude), radius * Math.cos(latitude) * Math.cos(longitude))
}

function getNormalizedLatitude() {
    const lat = rocketPosition.lat % (Math.PI * 2);
    return lat < 0 ? lat + Math.PI * 2 : lat;
}

function updateRocket(delta){ 
    const move = new THREE.Vector2();

    if (input.movement.latitude[0])
        move.x += 1;
    if (input.movement.latitude[1])
        move.x -= 1;
    if (input.movement.longitude[0])
        move.y -= 1;
    if (input.movement.longitude[1])
        move.y += 1;

    const movementLat = delta * angularSpeed * move.x;
    const lonSign = flip ? -1 : 1;
    const movementLon = delta * angularSpeed * move.y * lonSign;

    const lat1 = getNormalizedLatitude();
    rocketPosition.lat += movementLat;
    const lat2 = getNormalizedLatitude();

    const flipForward = movementLat > 0 && ((lat1 < Math.PI / 2 && lat2 > Math.PI / 2) || (lat1 < 3 * Math.PI / 2 && lat2 > 3 * Math.PI / 2));
    const flipBackward = movementLat < 0 && ((lat1 > Math.PI / 2 && lat2 < Math.PI / 2) || (lat1 > 3 * Math.PI / 2 && lat2 < 3 * Math.PI / 2));

    if (flipForward || flipBackward) {
        flip = !flip;
    }

    rocketPosition.lon += movementLon;

    const rocketPos = convertCoords(rocketRadiusPos, rocketPosition.lat, rocketPosition.lon);
    rocket.position.set(rocketPos.x, rocketPos.y, rocketPos.z);

    rocket.lookAt(scene.position);
    if (flip)
        rocket.rotateZ(Math.PI);
}

function handleColisions() {
    let quarters = new Array();
    if (rocket.position.y > -rocketColisionRadius) {
        if (rocket.position.x > -rocketColisionRadius) {
            quarters.push(0);
        }
        if (rocket.position.x < rocketColisionRadius) {
            quarters.push(1); 
        }
    }
    if (rocket.position.y < rocketColisionRadius) {
        if (rocket.position.x > -rocketColisionRadius) {
            quarters.push(3);
        }
        if (rocket.position.x < rocketColisionRadius) {
            quarters.push(2); 
        }
    }

    for (let i = 0; i < quarters.length; i++) {
        const quarter = quarters[i];
        for (let index = 0; index < trashVector[quarter].length; index++) {
            let element = trashVector[quarter][index];
            if ((rocketColisionRadius + trashColisionRadius)**2 >= (element.position.x - rocket.position.x)**2 + (element.position.y - rocket.position.y)**2 + (element.position.z - rocket.position.z)**2) {
                scene.remove(element);
            }
        }
    }
}