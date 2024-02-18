/* global THREE */
'use strict';

const colors = [
    0xff0000, 0x00ff00, 0x0000ff, 0xbbbbbb, 0xffff00,
    0x00ffff, 0xaa00ff, 0xffaa00, 0x0088ff, 0xf44611,
    0xff0000, 0x00ff00, 0x0000ff, 0xbbbbbb, 0xffff00,
    0x00ffff, 0xaa00ff, 0xffaa00, 0x0088ff, 0xf44611
];
const materials = [];

function createMaterial(isArticulate = false) {
    const material = isArticulate ? 
        new THREE.MeshNormalMaterial({ wireframe: true }) :
        new THREE.MeshBasicMaterial({ color: colors.pop(), wireframe: true });
    materials.push(material);
    return material;
}