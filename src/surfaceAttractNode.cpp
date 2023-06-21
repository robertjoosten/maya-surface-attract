#include "surfaceAttractNode.h"
#include <algorithm>


double const M_PI_HALF = M_PI * 0.5;

MTypeId SurfaceAttractNode::id(0x04);
MObject SurfaceAttractNode::aInputSurface;
MObject SurfaceAttractNode::aAttractDistance;
MObject SurfaceAttractNode::aAttractRamp;
MObject SurfaceAttractNode::aAttractRampU;
MObject SurfaceAttractNode::aAttractRampV;
MObject SurfaceAttractNode::aBulgeDistance;
MObject SurfaceAttractNode::aBulgeStrength;
MObject SurfaceAttractNode::aBulgeRamp;
MObject SurfaceAttractNode::aSmoothingStep;
MObject SurfaceAttractNode::aSmoothingIterations;

const MString SurfaceAttractNode::kName("surfaceAttract");


MStatus SurfaceAttractNode::initialize() {
	MStatus status;
	MFnTypedAttribute fnTypedAttr;
	MFnNumericAttribute fnNumericAttr;

	aInputSurface = fnTypedAttr.create("inputSurface", "inputSurface", MFnData::kNurbsSurface);
	fnTypedAttr.setStorable(true);
	fnTypedAttr.setKeyable(false);
	fnTypedAttr.setReadable(false);
	fnTypedAttr.setWritable(true);
	fnTypedAttr.setCached(false);
	addAttribute(aInputSurface);

	aAttractDistance = fnNumericAttr.create("attractDistance", "attractDistance", MFnNumericData::kFloat, 1);
	fnNumericAttr.setMin(0);
	fnNumericAttr.setStorable(true);
	fnNumericAttr.setKeyable(true);
	fnNumericAttr.setReadable(true);
	fnNumericAttr.setWritable(true);
	addAttribute(aAttractDistance);
	
	aAttractRamp = MRampAttribute::createCurveRamp("attractRamp", "attractRamp");
    addAttribute(aAttractRamp);

	aAttractRampU = MRampAttribute::createCurveRamp("attractRampU", "attractRampU");
    addAttribute(aAttractRampU);

    aAttractRampV = MRampAttribute::createCurveRamp("attractRampV", "attractRampV");
    addAttribute(aAttractRampV);

	aBulgeDistance = fnNumericAttr.create("bulgeDistance", "bulgeDistance", MFnNumericData::kFloat, 1);
	fnNumericAttr.setMin(0);
	fnNumericAttr.setStorable(true);
	fnNumericAttr.setKeyable(true);
	fnNumericAttr.setReadable(true);
	fnNumericAttr.setWritable(true);
	addAttribute(aBulgeDistance);

	aBulgeStrength = fnNumericAttr.create("bulgeStrength", "bulgeStrength", MFnNumericData::kFloat, 1);
	fnNumericAttr.setMin(0);
	fnNumericAttr.setStorable(true);
	fnNumericAttr.setKeyable(true);
	fnNumericAttr.setReadable(true);
	fnNumericAttr.setWritable(true);
	addAttribute(aBulgeStrength);
	
    aBulgeRamp = MRampAttribute::createCurveRamp("bulgeRamp", "bulgeRamp");
    addAttribute(aBulgeRamp);

    aSmoothingStep = fnNumericAttr.create("smoothingStep", "smoothingStep", MFnNumericData::kFloat, 0.5);
	fnNumericAttr.setMin(0);
	fnNumericAttr.setMax(1);
	fnNumericAttr.setStorable(true);
	fnNumericAttr.setKeyable(true);
	fnNumericAttr.setReadable(true);
	fnNumericAttr.setWritable(true);
	addAttribute(aSmoothingStep);

	aSmoothingIterations = fnNumericAttr.create("smoothingIterations", "smoothingIterations", MFnNumericData::kInt, 0);
	fnNumericAttr.setMin(0);
	fnNumericAttr.setStorable(true);
	fnNumericAttr.setKeyable(true);
	fnNumericAttr.setReadable(true);
	fnNumericAttr.setWritable(true);
	addAttribute(aSmoothingIterations);

	attributeAffects(aInputSurface, outputGeom);
	attributeAffects(aAttractDistance, outputGeom);
	attributeAffects(aAttractRamp, outputGeom);
	attributeAffects(aAttractRampU, outputGeom);
	attributeAffects(aAttractRampV, outputGeom);
	attributeAffects(aBulgeDistance, outputGeom);
	attributeAffects(aBulgeStrength, outputGeom);
    attributeAffects(aBulgeRamp, outputGeom);
	attributeAffects(aSmoothingStep, outputGeom);
	attributeAffects(aSmoothingIterations, outputGeom);

	status = MGlobal::executeCommandOnIdle("makePaintable -attrType multiFloat -sm deformer surfaceAttract weights");
    CHECK_MSTATUS_AND_RETURN_IT(status);

	return MS::kSuccess;
}


MStatus SurfaceAttractNode::initializeRamp(MObject& oParent, MObject& oValueRamp, int index, float position, float value, int interpolation)
{
    MStatus status;

    MPlug pValueRamp(oParent, oValueRamp);
    MPlug pValueRampElement = pValueRamp.elementByLogicalIndex(index, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    MPlug pValueRampPosition = pValueRampElement.child(0, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    status = pValueRampPosition.setFloat(position);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    MPlug pValueRampValue = pValueRampElement.child(1, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    status = pValueRampValue.setFloat(value);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    MPlug pValueRampInterpolation = pValueRampElement.child(2);
    pValueRampInterpolation.setInt(interpolation);

    return MS::kSuccess;
}


void* SurfaceAttractNode::creator() {
	return new SurfaceAttractNode();
}


void SurfaceAttractNode::postConstructor()
{
    initializeRamp(thisMObject(), aAttractRamp, 0, 0.0f, 0.0f, 2);
    initializeRamp(thisMObject(), aAttractRamp, 1, 1.0f, 1.0f, 2);
    initializeRamp(thisMObject(), aAttractRampU, 0, 0.0f, 1.0f, 1);
    initializeRamp(thisMObject(), aAttractRampU, 1, 1.0f, 1.0f, 1);
    initializeRamp(thisMObject(), aAttractRampV, 0, 0.0f, 1.0f, 1);
    initializeRamp(thisMObject(), aAttractRampV, 1, 1.0f, 1.0f, 1);
    initializeRamp(thisMObject(), aBulgeRamp, 0, 0.0f, 0.0f, 2);
    initializeRamp(thisMObject(), aBulgeRamp, 1, 0.5f, 1.0f, 2);
    initializeRamp(thisMObject(), aBulgeRamp, 2, 1.0f, 0.0f, 2);
}


SurfaceAttractNode::SurfaceAttractNode() {
}


SurfaceAttractNode::~SurfaceAttractNode() {
}


MStatus SurfaceAttractNode::deform(MDataBlock& block, MItGeometry& iter, const MMatrix& mat, unsigned int multiIndex) {
    MStatus status;

    MArrayDataHandle dInputs = block.inputValue(input, &status);
    status = dInputs.jumpToElement(multiIndex);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    MDataHandle dInput = dInputs.inputValue(&status);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    MDataHandle dGeom = dInput.child(inputGeom);
    MObject oGeom = dGeom.asMesh();
    MMatrix matrix = dGeom.geometryTransformMatrix();
    CHECK_MSTATUS_AND_RETURN_IT(status);

    MDataHandle dInputSurface = block.inputValue(aInputSurface, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	MObject oInputSurface = dInputSurface.asNurbsSurface();

	float weight = block.inputValue(envelope).asFloat();
	if (weight == 0.0f || oInputSurface.isNull()) {
        return MS::kSuccess;
    }

    MDataHandle dAttractDistance = block.inputValue(aAttractDistance, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	float attractDistance = dAttractDistance.asFloat();

	MRampAttribute fnAttractRampU(thisMObject(), aAttractRampU, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	MRampAttribute fnAttractRampV(thisMObject(), aAttractRampV, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	MRampAttribute fnAttractRamp(thisMObject(), aAttractRamp, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	MDataHandle dBulgeDistance = block.inputValue(aBulgeDistance, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	float bulgeDistance = dBulgeDistance.asFloat();

	MDataHandle dBulgeStrength = block.inputValue(aBulgeStrength, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	float bulgeStrength = dBulgeStrength.asFloat();

	MRampAttribute fnBulgeRamp(thisMObject(), aBulgeRamp, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	MDataHandle dSmoothingStep = block.inputValue(aSmoothingStep, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    float smoothingStep = dSmoothingStep.asFloat();

    MDataHandle dSmoothingIterations = block.inputValue(aSmoothingIterations, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    int smoothingIterations = dSmoothingIterations.asInt();

    int prevIndex;
    float dot;
    float distance;
    float attractNormal;
    float attractMultiplier;
    float attractWeight;
    float attractWeightU;
    float attractWeightV;
    float bulgeNormal;
    float bulgeMultiplier;
    float bulgeWeight;
    float vertexWeight;
    float smoothWeight;
    float smoothMultiplier;
    double u;
    double uStart;
    double uEnd;
    double v;
    double vStart;
    double vEnd;

    MVector delta;
    MVector normal;
    MIntArray vertices;
    MPointArray points;
    MPointArray pointsSmooth;

    std::map<int, int> indices;
    std::map<int, float> smoothing;
    std::map<int, std::unordered_set<int>> connectivity;

    int count = iter.count(&status);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    status = iter.allPositions(points);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    MItMeshVertex itGeom(oGeom, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    MFnNurbsSurface fnInputSurface(oInputSurface, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    status = fnInputSurface.getKnotDomain(uStart, uEnd, vStart, vEnd);
    CHECK_MSTATUS_AND_RETURN_IT(status);

    // construct a map between vertex index and ordered index, this will most
    // likely be the same unless the deformer was created using components.
    for (int i = 0; i < count; i++) {
        int index = iter.index(&status);
        CHECK_MSTATUS_AND_RETURN_IT(status);

        indices[index] = i;
        iter.next();
    }

    // calculate the initial displacement of the vertices and store the vertex
    // connectivity used for smoothing operations, these smoothing operations
    // will work hardest on vertices that have a 90 degree angle between
    // normal and closest point.
    for (auto const& index: indices) {
        MPoint point = points[index.second];
        vertexWeight = weightValue(block, multiIndex, index.first);
        if (weight * vertexWeight < 0.00001f) {
            points.set(point, index.second);
            continue;
        }

        point *= matrix;

        status = itGeom.setIndex(index.first, prevIndex);
        CHECK_MSTATUS_AND_RETURN_IT(status);
        prevIndex = index.first;

        status = itGeom.getNormal(normal, MSpace::kWorld);
        CHECK_MSTATUS_AND_RETURN_IT(status);

        MPoint pointOnSurface = fnInputSurface.closestPoint(point, &u, &v, 0.0, 0.0, MSpace::kWorld, &status);
        MVector normalOnSurface = fnInputSurface.normal(u, v, MSpace::kWorld, &status);

        fnAttractRampU.getValueAtPosition(u / uEnd, attractWeightU, &status);
        CHECK_MSTATUS_AND_RETURN_IT(status);

        fnAttractRampV.getValueAtPosition(v / vEnd, attractWeightV, &status);
        CHECK_MSTATUS_AND_RETURN_IT(status);

        delta = (MVector)pointOnSurface - (MVector)point;
        dot = delta.normal() * normalOnSurface;
        distance = delta.length();
        attractNormal = std::min(1.0f, std::max(0.0f, 1.0f - (distance / attractDistance))) * attractWeightU * attractWeightV;
        bulgeNormal = std::min(1.0f, std::max(0.0f, distance / bulgeDistance)) * attractWeightU * attractWeightV;

        fnAttractRamp.getValueAtPosition(attractNormal, attractWeight, &status);
        CHECK_MSTATUS_AND_RETURN_IT(status);

        fnBulgeRamp.getValueAtPosition(bulgeNormal, bulgeWeight, &status);
        CHECK_MSTATUS_AND_RETURN_IT(status);

        attractMultiplier = weight * attractWeight * vertexWeight;
        bulgeMultiplier = weight * bulgeWeight * vertexWeight;
        smoothMultiplier = std::max(attractMultiplier, bulgeMultiplier);

        if (smoothMultiplier > 0.00001f && smoothingIterations > 0) {
            fnAttractRamp.getValueAtPosition(smoothMultiplier, smoothWeight, &status);
            CHECK_MSTATUS_AND_RETURN_IT(status);
            smoothing[index.second] = smoothWeight;

            status = itGeom.getConnectedVertices(vertices);
            CHECK_MSTATUS_AND_RETURN_IT(status);

            for (int i = 0; i < vertices.length(); i++) {
                if (indices.find(vertices[i]) != indices.end()) {
                    connectivity[index.second].insert(indices[vertices[i]]);
                }
            }
        }

        if (dot > 0.0f) {
            attractMultiplier = weight * vertexWeight;
            bulgeMultiplier = 0.0f;
        }

        point = MPoint((MVector)point + (delta * attractMultiplier) + (normal * bulgeMultiplier * bulgeStrength));
        points.set(point * matrix.inverse(), index.second);
    }

    // smoothWeight the points using the vertex connectivity.
    for (int i = 0; i < smoothingIterations; ++i)
    {
        pointsSmooth.copy(points);

        for (auto const& connect : connectivity)
        {
            int num = connect.second.size();
            float smoothWeight = smoothing[connect.first] * smoothingStep;
            MVector point = (MVector)points[connect.first];
            MVector pointSmooth;

            for (auto const& index: connect.second)
            {
                pointSmooth += ((point / num) * (1 - smoothWeight)) + ((points[index] / num) * smoothWeight);
            }
            pointsSmooth.set(pointSmooth, connect.first);
        }
        points.copy(pointsSmooth);
    }

    iter.setAllPositions(points);

    return MS::kSuccess;
}