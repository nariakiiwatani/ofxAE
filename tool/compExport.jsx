#include "./exportUtil.jsxinc"
#include "./exportKeys.jsxinc"
(function() {

function proc(comp)
{
	var name = comp.name;
	var width = comp.width;
	var height = comp.height;
	var frameRate = comp.frameRate;
	var startTime = 0;//comp.workAreaStart;
	var duration = comp.duration;//comp.workAreaDuration;

function procMarkers(layer, markers)
{
	var ret = new Array();
	for(var i = 1; i <= markers.numKeys; ++i) {
		var obj = new Object();
		obj.name = markers.keyValue(i).comment;
		obj.from = Math.round((markers.keyTime(i)-startTime)*frameRate);
		obj.to = Math.round((markers.keyValue(i).duration)*frameRate)+obj.from;
		ret.push(obj);
	}
	return ret;
}

function procLayer(l)
{
	var obj = new Object();
	obj.name = l.name;
	if(l.parent) {
		obj.parent = l.parent.name;
	}
	obj.layerType = ExportUtil.getLayerType(l);
	// type specific
	switch(obj.layerType) {
		case ExportUtil.LayerType.COMPOSITION:
			obj.width = l.width;
			obj.height = l.height;
			obj.composition = proc(l.source);
			break;
		case ExportUtil.LayerType.STILL:
			obj.width = l.width;
			obj.height = l.height;
			obj.still = l.source.file.toString().replace(/.*\//,"");
			break;
		case ExportUtil.LayerType.SOLID:
			obj.width = l.width;
			obj.height = l.height;
			obj.solid = l.source.mainSource.color;
			break;
		case ExportUtil.LayerType.CAMERA:
			obj.camera = new Object();
			break;
	}

	// timeline
	obj.property = getLayerKeys(l, startTime, duration);
	// maker
	if(l.marker && l.marker.numKeys > 0) {
		obj.marker = procMarkers(l, l.marker);
	}

	return obj;
}

	var json = new Object();
	json.name = name;
	json.width = width;
	json.height = height;
	json.layer = new Array();
	var layers = comp.layers;
	for(var i = 1; i <= layers.length; ++i) {
		if(layers[i].enabled) {
			json.layer.push(procLayer(layers[i]));
		}
	}
	return json;
}

var comps = app.project.selection;
for(var i = 0; i < comps.length; ++i) {
	var exp = proc(comps[i]);
	var file = File.saveDialog(comps[i].name+":save file.","*.json");
	if(!file) return $.writeln("failed");
	var flag = file.open("w","txt","");
	if (!flag) return $.writeln("failed");
	file.write(ExportUtil.toSource(exp));
	file.close();
}

}());

