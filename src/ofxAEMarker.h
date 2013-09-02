#pragma once

namespace ofxAE {
	class Layer;
}

namespace ofxAE {
class Marker {
	friend class Loader;
public:
private:
	string name_;
	int from_;
	int to_;
};
}

/* EOF */