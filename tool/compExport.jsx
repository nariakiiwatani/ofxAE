#include "./exportUtil.jsxinc"
#include "./exportKeys.jsxinc"
#include "./getCompMarker.jsxinc"
#include "./itemUtil.jsxinc"
(function() {

var FOLDER = Folder.selectDialog('choose a folder to export files into');
if(!FOLDER) {
	$.writeln("canceled");
	return;
}

function proc(comp)
{
	var name = comp.name;
	var width = comp.width;
	var height = comp.height;
	var frameRate = comp.frameRate;
	var startTime = 0;
	var duration = comp.duration;
	// var startTime = comp.workAreaStart;
	// var duration = comp.workAreaDuration;

	function procMarkers(markers, offsetTime)
	{
		var ret = new Array();
		for(var i = 1; i <= markers.numKeys; ++i) {
			var obj = new Object();
//			obj.comment = markers.keyValue(i).comment;
			obj.comment = markers.keyValue(i).comment.replace(/[\r\n]+/g, '\\r\\n');
			obj.from = Math.round((markers.keyTime(i)-offsetTime)*frameRate);
			obj.length = Math.round((markers.keyValue(i).duration)*frameRate);
			ret.push(obj);
		}
		return ret;
	}

	function procLayer(l)
	{
		var obj = new Object();
		obj.name = l.name;
		obj.index = l.index;
		if(l.parent) {
			obj.parent = l.parent.index;
		}
		if(ExportUtil.isAVLayer(l)) {
			obj.width = l.width;
			obj.height = l.height;
			obj.is3d = l.threeDLayer;
			if(l.canSetCollapseTransformation) {
				obj.isCollapse = l.collapseTransformation;
			}
			obj.blendingMode = ExportUtil.getDrawMode(l.blendingMode);
		}
		obj.layerType = ExportUtil.getLayerType(l);
		var source = l.source.useProxy ? l.source.proxySource : l.source.mainSource;
		if(source) {
			obj.sourceDirectory = getItemFolder(source);
		}
		// type specific
		switch(obj.layerType) {
			case ExportUtil.LayerType.COMPOSITION:
				obj.source = l.source.name+'.json';
				break;
			case ExportUtil.LayerType.IMAGE:
				if(source.file.toString().match(/\.(ai|psd)$/)) {
					if(source.name.match(/\//)) {
						var sp = source.name.split('/');
						obj.sourceDirectory += sp[1]+'/';
						obj.source = (sp[0] + '.png').replace(/ /g,'-');
					}
					else {
						obj.sourceDirectory += source.name+'/';
						obj.source = source.name.replace(/\.(ai|psd)$/,"") + '.png';
					}
				}
				else {
					obj.source = source.file.toString().replace(/.*\//,"");
					copyItem(source, FOLDER.toString());
				}
				break;
			case ExportUtil.LayerType.MOVIE:
				obj.source = source.file.toString().replace(/.*\//,"");
				copyItem(source, FOLDER.toString());
				break;
			case ExportUtil.LayerType.SEQUENCE:
				obj.source = source.name;
				copySequenceItem(source, FOLDER.toString());
				break;
			case ExportUtil.LayerType.SOLID:
				obj.color = source.color;
				break;
			case ExportUtil.LayerType.CAMERA:
				break;
		}

		// timeline
		obj.frameOffset = (l.startTime-startTime)*frameRate;
		obj.inFrame = (l.inPoint-l.startTime)*frameRate;
		obj.outFrame = (l.outPoint-l.startTime)*frameRate;
		obj.property = getLayerKeys(l, obj.frameOffset, obj.inFrame, obj.outFrame);
		// maker
		if(l.marker && l.marker.numKeys > 0) {
			obj.marker = procMarkers(l.marker, l.startTime-startTime);
		}

		return obj;
	}

	var json = new Object();
	json.name = name;
	json.width = width;
	json.height = height;
	json.frameRate = frameRate;
	json.length = duration*frameRate;
	var markers = getCompMarker(comp);
	if(markers && markers.numKeys > 0) {
		json.marker = procMarkers(markers, startTime);
	}
	json.layer = new Array();
	var layers = comp.layers;
	for(var i = 1; i <= layers.length; ++i) {
		if(layers[i].enabled) {
			json.layer.push(procLayer(layers[i]));
		}
	}
	return json;
}

function getCompArrayToProc(proj) {
	function isSelected(comp, checkParent) {
		return comp.selected || (checkParent && function(comp) {
			for(var i = 0; i < comp.usedIn.length; ++i) {
				if(isSelected(comp.usedIn[i], true)) {
					return true;
				}
			}
			return false;
		}(comp));
	}
	var ret = [];
	for(var i = 1; i <= proj.numItems; ++i) {
		if(!proj.item(i).useProxy && proj.item(i) instanceof CompItem && isSelected(proj.item(i), true)) {
			ret.push(proj.item(i));
		}
	}
	return ret;
}

var comps = getCompArrayToProc(app.project);
for(var i = 0; i < comps.length; ++i) {
	var exp = proc(comps[i]);
	var dir = new Folder(FOLDER.toString()+'/'+getItemFolder(comps[i]));
	if(!dir.exists){
		dir.create();
	}
	var file = new File(dir.toString()+'/'+comps[i].name+'.json');
	if(!file) return $.writeln("failed");
	var flag = file.open("w","txt","");
	if (!flag) return $.writeln("failed");
	file.write(ExportUtil.toSource(exp));
	file.close();
}

}());

