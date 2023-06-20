#include <maya/MFnPlugin.h>

#include "surfaceAttractNode.h"


MStatus initializePlugin(MObject obj) {
	MStatus status;
	MFnPlugin plugin(obj, "Robert Joosten", "1.0", "any");

	status = plugin.registerNode(
		SurfaceAttractNode::kName,
		SurfaceAttractNode::id,
		SurfaceAttractNode::creator,
		SurfaceAttractNode::initialize,
		MPxNode::kDeformerNode
	);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	return status;
}

MStatus uninitializePlugin(MObject obj) {
	MStatus status;
	MFnPlugin plugin(obj);

	status = plugin.deregisterNode(SurfaceAttractNode::id);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	return status;
}