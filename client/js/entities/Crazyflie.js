/**
 * @file <client/js/entitites/Crazyflie.js>
 *
 * @author Guillaume Ricard - <guillaume.ricard@polymtl.ca>
 *
 * @project ARGoS3-Webviz <https://github.com/NESTLab/argos3-webviz>
 *
 * MIT License
 * Copyright (c) 2020 NEST Lab
 * Copyright (c) 2022 MIST Lab
 */

class Crazyflie {
  constructor(entity, scale, EntityLoadingFinishedFn) {
    this.scale = scale;
    this.entity = entity;

    /* We do not have a Crazyflie model in memory */
    if (!window.CrazyflieModel) {
        var that = this;
        if (window.CrazyflieModel_isLoading) {
            /* Some other instance is already loading this same robot */
            /* Keep checking for loaded model */
            var t = setInterval(() => {
                if (!window.CrazyflieModel_isLoading) {
                    /* Loaded by other loader instance */
		    clearInterval(t);
		    that.buildModel(window.CrazyflieModel.clone(), entity, EntityLoadingFinishedFn);
                }
	    }, 10);
        } else {
	    window.CrazyflieModel_isLoading = true;
	    var objectLoader = new THREE.ObjectLoader();
	    objectLoader.load("/models/cf2.json", function(crazyflieBot) {
		crazyflieBot.scale.multiplyScalar(scale / 2);
		var boundingBox = new THREE.Box3().setFromObject(crazyflieBot);
		for (let i = 0; i < crazyflieBot.children.length; ++i) {
		    crazyflieBot.children[i].material = new THREE.MeshPhongMaterial({
                        color: 0x000000
		    });
		}
		window.CrazyflieModel = crazyflieBot;
		that.buildModel(crazyflieBot, entity, EntityLoadingFinishedFn);
	    });
	}
    } else {
	that.buildModel(window.CrazyflieModel.clone(), entity, EntityLoadingFinishedFn);
    }
  }

  buildModel(geometry, entity, EntityLoadingFinishedFn) {
    var mesh = new THREE.Mesh();
    mesh.add(geometry);

    mesh.rotation.setFromQuaternion(new THREE.Quaternion(
      entity.orientation.x,
      entity.orientation.y,
      entity.orientation.z,
      entity.orientation.w));

    mesh.position.x = entity.position.x * scale;
    mesh.position.y = entity.position.y * scale;
    mesh.position.z = entity.position.z * scale;

    this.mesh = mesh;

    EntityLoadingFinishedFn(this);
  }

  getMesh() {
    return this.mesh;
  }

  update(entity) {
    this.mesh.position.x = entity.position.x * this.scale;
    this.mesh.position.y = entity.position.y * this.scale;
    this.mesh.position.z = entity.position.z * this.scale;

    this.mesh.rotation.setFromQuaternion(new THREE.Quaternion(
      entity.orientation.x,
      entity.orientation.y,
      entity.orientation.z,
      entity.orientation.w));
  }
}
