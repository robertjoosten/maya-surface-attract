#include <map>
#include <unordered_set>
#include <maya/MPxNode.h>
#include <maya/MPxDeformerNode.h>
#include <maya/MItGeometry.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MTypes.h>
#include <maya/MStatus.h>
#include <maya/MGlobal.h>
#include <maya/MMatrix.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MIntArray.h>
#include <maya/MVector.h>
#include <maya/MQuaternion.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MItMeshVertex.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MRampAttribute.h>


class SurfaceAttractNode : public MPxDeformerNode {
public:
	SurfaceAttractNode();
	virtual ~SurfaceAttractNode();
	virtual void postConstructor();
	static void* creator();

	static MStatus initialize();
	static MStatus initializeRamp(MObject& oParent, MObject& oColorRamp, int index, float position, float value, int interpolation);

	virtual MStatus deform(MDataBlock& block, MItGeometry& iter, const MMatrix& mat, unsigned int multiIndex);

	static MTypeId id;
	static const MString kName;
	static MObject aInputSurface;
	static MObject aAttractDistance;
	static MObject aAttractRamp;
    static MObject aAttractRampU;
	static MObject aAttractRampV;
	static MObject aBulgeDistance;
    static MObject aBulgeStrength;
    static MObject aBulgeRamp;
	static MObject aSmoothingStep;
	static MObject aSmoothingIterations;
};
