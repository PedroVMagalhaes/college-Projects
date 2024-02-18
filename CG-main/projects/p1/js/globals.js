/* global THREE */
'use strict';

let camera, scene;
let articulate;

const cameraHalfHeight = 100;
const renderer = new THREE.WebGLRenderer({ antialias: true });
const clock = new THREE.Clock();

const articulateSpeed = 80;
const pyramidAngularSpeed = 1;
const octahedronAngularSpeed = 2;
const coneAngularSpeed = 3;
const maxAngle = Math.PI * 3 / 4 - Math.atan(0.5);
