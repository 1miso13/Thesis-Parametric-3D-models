#pragma once
#ifndef OPERATIONTYPES_HEADER
#define OPERATIONTYPES_HEADER

#include <vector>
#include "Command.h"
#include "OperationTypeEnum.h"
#include "GeometricObject.h"
namespace operationType {
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	enum ParameterTypesEnum {
		ParameterTypePOINT,
		ParameterTypeMULTIPLEPOINTS,
		ParameterTypeFLOAT,
		ParameterTypeLINE,
		ParameterTypeSURFACE,
		ParameterTypeTRIANGLE,
		ParameterTypeOBJECT3D
	};
	inline ObjectTypeEnum findTypeOfOperation(operationType::OperationTypeEnum operationType);
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	inline ObjectTypeEnum Find(std::vector <Command*> *GraphCommand, std::string objectName,int to =-1) {
		std::vector<Command*>::iterator it= (*GraphCommand).begin();
		std::vector<Command*>::iterator toit;
		if (to==-1)
		{
			toit = (*GraphCommand).end();
		}
		else
		{
			toit = it + to;
		}
		for (; it != toit && it != (*GraphCommand).end(); it++)
		{
			if ((*it)->name == objectName)
				return findTypeOfOperation((*it)->operationType);
		}
		return INVALIDObjectType;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	inline  bool CompareTypes(ObjectTypeEnum commandParType, ObjectTypeEnum tested) {
		/*POINT,
			LINE,
			SURFACE,
			CIRCLE,
			RECTANGLE,
			Polygon,
			TRIANGLE,
			PYRAMID,
			OBJECT3D*/
		if (commandParType == tested)
			return true;
		switch (tested)
		{
		case SURFACE:
		case CIRCLE:
		case RECTANGLE:
		case Polygon:
		case TRIANGLE:
			if (commandParType == SURFACE)
			{
				return true;
			}
			break;
		case PYRAMID:
		case OBJECT3D:
			if (commandParType == OBJECT3D)
			{
				return true;
			}
			break;
		default:
			break;
		}
		return false;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	inline bool IsFloat(const std::string& num) {
		try
		{
			std::stof(num);
		}
		catch (...)
		{
			return false;
		}
		return true;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	inline bool IsPercentage(const std::string& s) {
		const char *begin = s.c_str();
		char *end;
		double val = std::strtod(begin, &end);
		bool ret = false;
		if (end == begin) return false;
		size_t len = strlen(end);
		for (size_t i = 0; i < len; i++, end++) {
			if (*end == '%' && ret == false)
				ret = true;
			if (!isspace(*end) && *end != '%') {
				return false;
			}
		}
		return ret;

	}

	//TODO
	inline bool TestValidParameterType(ParameterTypesEnum parameterType, std::string paramValue, std::vector <Command*> *GraphCommand,int to=-1) {
		/*POINTObjectType,
	LINE,
	SURFACE,
	CIRCLE,
	RECTANGLE,
	Polygon,
	TRIANGLE,
	PYRAMID,
	OBJECT3D,
	INVALIDObjectType
	*/

		if (parameterType == ParameterTypeMULTIPLEPOINTS)//multiple points are defined as: point1;point2;point3;point4;...pointN
		{
			//parse points by ';' as delimeter
			size_t pos = 0;
				std::string token;
				bool retVal = true;
				while ((pos = paramValue.find(';')) != std::string::npos) {
					token = paramValue.substr(0, pos);
						if (!CompareTypes(POINTObjectType, Find(GraphCommand, token, to)))
						{
							retVal = false;
						}
					paramValue.erase(0, pos + 1);
				}
			if (!CompareTypes(POINTObjectType, Find(GraphCommand, paramValue, to)))
			{
				retVal = false;
			}

			return retVal;

		}
		switch (parameterType)
		{
		case ParameterTypePOINT:
			return CompareTypes(POINTObjectType, Find(GraphCommand, paramValue,to));
			break;
		case ParameterTypeFLOAT:
			return IsFloat(paramValue);
			break;
		case ParameterTypeLINE:
			return CompareTypes(LINE, Find(GraphCommand, paramValue, to));
			break;
		case ParameterTypeSURFACE:
			return CompareTypes(SURFACE, Find(GraphCommand, paramValue, to));
			break;
		case ParameterTypeTRIANGLE:
			return CompareTypes(TRIANGLE, Find(GraphCommand, paramValue,to));
			break;
		case ParameterTypeOBJECT3D:
			return CompareTypes(OBJECT3D, Find(GraphCommand, paramValue,to));
			break;
		default:
			return false;
			break;
		}

	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	inline OperationTypeEnum GetOperationType(
		std::string commandName,
		std::vector<std::string>* paramVectors/*contain vector of parameters*/,
		std::vector <Command*> *GraphCommand,
		size_t * typeOfParams
	) {
		size_t CountOfParams = paramVectors->size();
		*typeOfParams = 1;
		std::vector<std::string>::iterator it = (*paramVectors).begin();
		if (commandName == "Point")
		{
			if (CountOfParams == 3 || CountOfParams == 4)
			{
				if (!IsFloat(*it))
				{
					return INVALID;
				}
				it++;
				if (!IsFloat(*it))
				{
					return INVALID;
				}
				it++;
				if (!IsFloat(*it))
				{
					return INVALID;
				}
				it++;
				if (CountOfParams == 4)//with parent point
				{
					if (!CompareTypes(POINTObjectType, Find(GraphCommand, *it)))
					{
						return INVALID;
					}
					*typeOfParams = 2;
				}
				return  Point;
			}
			return INVALID;
			//Point(string name, Point Parent = NULL, bool visible = true) //- Create point on position 0,0,0. Parent - If entered, position is relative, else absolute
			//Point(string name, float X, float Y, float Z, Point Parent = NULL, bool visible = true) //-Create point on position XYZ
			///	Example:
			//		Point("Name of point 1")	//Name can by written with "" or '' or without, Point is on absolute position [0,0,0]
			//		Point(NameOfPoint2)		//Name can by written with "" or '' or without
			//		Point('NameOfPoint3')	//Name can by written with "" or '' or without
			//		Point(NameOfPoint4,1.1,2.2,3.3)	//- Create visible Point on position [1,2,3] 
			//		Point(NameOfPoint4,1,2,3,true)	//- Create visible Point on position [1,2,3] 
			//		Point(NameOfPoint4,1,2,3,false)	//- Create invisible Point on position [1,2,3]
			//		Point(NameOfPoint4,1,2,3,NameOfParentPoint,true)	//- Create visible Point on relative position [1,2,3] from ParentPoint
		}
		if (commandName == "LinearInterpolationDist")
		{
			if (CountOfParams == 3)
			{
				if (!CompareTypes(POINTObjectType, Find(GraphCommand, *it)))
				{
					return INVALID;
				}
				it++;
				if (!CompareTypes(POINTObjectType, Find(GraphCommand, *it)))
				{
					return INVALID;
				}
				it++;
				if (!IsFloat(*it))//distance or %
				{
					return INVALID;
				}
				return  LinearInterpolationDist;
			}
			return INVALID;//1 - %, 2- distance
		//LinearInterpolation(string name, Point fromPoint, Point toPoint, float distance / percentage, bool visible = true)
				//	Example:
				//		LinearInterpolation(PointName, FromPoint, ToPoint, 50%)	//- Create Point in middle of entered points
				//		LinearInterpolation(PointName, FromPoint, ToPoint, 25%)	//- Create Point on position with distance 20% of first point and 80% of second point
				//		LinearInterpolation(PointName, FromPoint, ToPoint, 100)	//- Create Point on position with distance 100 from first point in angle to second point 


		}
		if (commandName == "LinearInterpolationPerc")
		{
			if (CountOfParams == 3)
			{
				if (!CompareTypes(POINTObjectType, Find(GraphCommand, *it)))
				{
					return INVALID;
				}
				it++;
				if (!CompareTypes(POINTObjectType, Find(GraphCommand, *it)))
				{
					return INVALID;
				}
				it++;
				if (!IsFloat(*it))
				{
					return INVALID;
				}
				return  LinearInterpolationPerc;
			}
			return INVALID;//1 - %, 2- distance
		//LinearInterpolation(string name, Point fromPoint, Point toPoint, float  percentage, bool visible = true)
				//	Example:
				//		LinearInterpolation(PointName, FromPoint, ToPoint, 50%)	//- Create Point in middle of entered points
				//		LinearInterpolation(PointName, FromPoint, ToPoint, 25%)	//- Create Point on position with distance 20% of first point and 80% of second point
				//		LinearInterpolation(PointName, FromPoint, ToPoint, 100)	//- Create Point on position with distance 100 from first point in angle to second point 


		}
		if (commandName == "Intersection_Plane_Line")
		{
			if (CountOfParams != 2)
			{
				return INVALID;
			}
			if (!CompareTypes(LINE, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			it++;
			if (!CompareTypes(SURFACE, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			return  Intersection_Plane_Line;

			//Intersection_Plane_Line(string name, Line lineName, Sufrace surfaceName, bool visible = true)
					//	Alternative:
					//	Intersection(string name, Line lineName, Sufrace surfaceName)
					//	Example:
					//		Intersection_Plane_Line(PointName,  lineName, surfaceName) //- Create Point on position where Line with name "lineName" intersecting Surface with name "surfaceName"

		}
		if (commandName == "SurfaceCenterBoundingSquare")
		{
			if (CountOfParams != 1)
			{
				return INVALID;
			}
			if (!CompareTypes(SURFACE, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			return  SurfaceCenterBoundingSquare;
			//SurfaceCenterBoundingSquare(string name, Surface surfaceName, bool visible = true) //Create point on position of middle of entered surface
				//	Example:
				//		SurfaceCenterBoundingSquare(PointName, Circle)	//- Create Point on center of Circle
				//		SurfaceCenterBoundingSquare(PointName, Rectangle)	//- Create Point on middle of Rectangle
				//		SurfaceCenterBoundingSquare(PointName, Polygon)		//- Create Point on middle of Polygon - centroid (sum of points / count of points)

		}
		if (commandName == "SurfaceCenterAverage")
		{
			if (CountOfParams != 1)
			{
				return INVALID;
			}
			if (!CompareTypes(SURFACE, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			return  SurfaceCenterAverage;
			//SurfaceCenterAverage(string name, Surface surfaceName, bool visible = true) //Create point on position of middle of entered surface
				//	Example:
				//		SurfaceCenterAverage(PointName, Circle)	//- Create Point on center of Circle
				//		SurfaceCenterAverage(PointName, Rectangle)	//- Create Point on middle of Rectangle
				//		SurfaceCenterAverage(PointName, Polygon)		//- Create Point on middle of Polygon - centroid (sum of points / count of points)

		}
		if (commandName == "Centroid")
		{
			if (CountOfParams != 1)
			{
				return INVALID;
			}
			if (!CompareTypes(TRIANGLE, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			return Centroid;//(string name, Triangle triangleName, float visibility)
		}
		if (commandName == "Incenter")
		{
			if (CountOfParams != 1)
			{
				return INVALID;
			}
			if (!CompareTypes(TRIANGLE, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			return Incenter;//(string name, Triangle triangleName, float visibility)
		}
		if (commandName == "Circumcenter")
		{
			if (CountOfParams != 1)
			{
				return INVALID;
			}
			if (!CompareTypes(TRIANGLE, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			return Circumcenter;//(string name, Triangle triangleName, float visibility)
		}
		if (commandName == "Orthocenter")
		{
			if (CountOfParams != 1)
			{
				return INVALID;
			}
			if (!CompareTypes(TRIANGLE, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			return Orthocenter;//(string name, Triangle triangleName, float visibility)
		}
		if (commandName == "NinePointCenter")
		{
			if (CountOfParams != 1)
			{
				return INVALID;
			}
			if (!CompareTypes(TRIANGLE, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			return NinePointCenter;//(string name, Triangle triangleName, float visibility)
		}
		if (commandName == "ObjectCenterBoundingBox")
		{
			if (CountOfParams != 1)
			{
				return INVALID;
			}
			if (!CompareTypes(OBJECT3D, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			return  ObjectCenterBoundingBox;
			//ObjectCenterBoundingBox(string name, Object3D ObjectName, bool visible = true) //Create point on position of middle of entered object
		}
		if (commandName == "ObjectCenterAverage")
		{
			if (CountOfParams != 1)
			{
				return INVALID;
			}
			if (!CompareTypes(OBJECT3D, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			return  ObjectCenterAverage;
			//ObjectCenterAverage(string name, Object3D ObjectName, bool visible = true) //Create point on position of center of entered object

		}
		if (commandName == "LineFirstPoint")
		{
			if (CountOfParams != 1)
			{
				return INVALID;
			}
			if (!CompareTypes(LINE, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			return  LineFirstPoint;
			//LineFirstPoint(string name, Line lineName, bool visible = true)
		}
		if (commandName == "LineSecondPoint")
		{
			if (CountOfParams != 1)
			{
				return INVALID;
			}
			if (!CompareTypes(LINE, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			return  LineSecondPoint;
			//LineSecondPoint(string name, Line lineName, bool visible = true)




				///Line Commands:

		}
		if (commandName == "Line")
		{
			if (CountOfParams != 2)
			{
				return INVALID;
			}
			if (!CompareTypes(POINTObjectType, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			it++;
			if (!CompareTypes(POINTObjectType, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			return  Line;//Line(string lineName, Point p1, Point p2, bool visible = true) //create line, where p1 is start point and p2 is end point

		}
		if (commandName == "LineNormalize")
		{
			if (CountOfParams != 1)
			{
				return INVALID;
			}
			if (!CompareTypes(LINE, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			return  LineNormalize;//LineNormalize(string lineName, Line l, bool visible = true)

		}
		if (commandName == "LineChangeLengthDist")
		{
			if (CountOfParams != 2)
			{
				return INVALID;
			}
			if (!CompareTypes(LINE, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			it++;
			if (!IsFloat(*it)) {
				return INVALID;
			}
			return  LineChangeLengthDist;//LineChangeLengthDist(string lineName, Line l, float distance, bool visible = true)
		}
		if (commandName == "LineChangeLengthPerc")
		{
			if (CountOfParams != 2)
			{
				return INVALID;
			}
			if (!CompareTypes(LINE, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			it++;
			if (!IsFloat(*it)) {
				return INVALID;
			}
			return  LineChangeLengthPerc;//LineChangeLengthPerc(string lineName, Line l, float percent, bool visible = true) //percent = (0;100>



				// these commands are based on page http://paulbourke.net/geometry/pointlineplane/
		}
		if (commandName == "MinLineBetweenLineAndLine")
		{
			if (CountOfParams != 2)
			{
				return INVALID;
			}
			if (!CompareTypes(LINE, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			it++;
			if (!CompareTypes(LINE, Find(GraphCommand, *it))) {
				return INVALID;
			}
			return  MinLineBetweenLineAndLine;//MinLineBetweenLineAndLine(string lineName, Line l1, Line l2, bool visible = true)
		}
		if (commandName == "MinLineBetweenPointAndLine")
		{
			if (CountOfParams != 2)
			{
				return INVALID;
			}
			if (!CompareTypes(POINTObjectType, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			it++;
			if (!CompareTypes(LINE, Find(GraphCommand, *it))) {
				return INVALID;
			}
			return  MinLineBetweenPointAndLine;//MinLineBetweenPointAndLine(string lineName, Point p, Line l, bool visible = true)
		}
		if (commandName == "MinLineBetweenPointAndSurface")
		{
			if (CountOfParams != 2)
			{
				return INVALID;
			}
			if (!CompareTypes(POINTObjectType, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			it++;
			if (!CompareTypes(SURFACE, Find(GraphCommand, *it))) {
				return INVALID;
			}
			return  MinLineBetweenPointAndSurface;//MinLineBetweenPointAndSurface(string lineName, Point p, Surface s, bool visible = true)

				///Alternative:
		}
		if (commandName == "MinLine")
		{
			if (CountOfParams != 2)
			{
				return INVALID;
			}
			if (CompareTypes(POINTObjectType, Find(GraphCommand, *it)))
			{
				it++;
				if (CompareTypes(LINE, Find(GraphCommand, *it))) {
					*typeOfParams = 1;
					return MinLine;
				}
				if(CompareTypes(SURFACE, Find(GraphCommand, *it)))
				{
					*typeOfParams = 2;
					return MinLine;
						
				}
			}
			else
			{
				if (CompareTypes(LINE, Find(GraphCommand, *it))) {

					it++;
					if (CompareTypes(LINE, Find(GraphCommand, *it))) {
						*typeOfParams = 3;
						return MinLine;
					}
				}
			}

			return  INVALID;//MinLine(string lineName, Object o1, Object o2, bool visible = true) //Supported are only : [Line,Line], [Point,Line], [Point, Surface]



		}
		if (commandName == "SurfaceNormal")
		{
			if (CountOfParams != 1)
			{
				return INVALID;
			}
			if (!CompareTypes(SURFACE, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			return  SurfaceNormal;//SurfaceNormal(string lineName, Surface s, bool visible = true) // return normal vector of surface



		}
		if (commandName == "LineRelocationByPoint")
		{
			if (CountOfParams != 2)
			{
				return INVALID;
			}
			if (!CompareTypes(LINE, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			it++;
			if (!CompareTypes(POINTObjectType, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			return  LineRelocationByPoint;//LineRelocationByPoint(string lineName, Line l, Point p, bool visible = true)

		}
		/*if (commandName == "OrthogonalLeastSquares")
		{
			return  OrthogonalLeastSquares;//OrthogonalLeastSquares(string lineName, Point p1, Point p2, Point p3, ..., bool visible = true) //min 3

		}*/
		if (commandName == "CrossProduct")
		{
			if (CountOfParams != 2)
			{
				return INVALID;
			}
			if (!CompareTypes(LINE, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			it++;
			if (!CompareTypes(LINE, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			return  CrossProduct;
			//CrossProduct(string lineName, Line l1, Line l2, bool visible = true)



		///Surface Commands:

		}
		if (commandName == "RectangleFromLine")
		{//1 - surface point, 2 - normal vector
			if (CountOfParams != 4)
			{
				return INVALID;
			}
			if (CompareTypes(LINE, Find(GraphCommand, *it)))
			{
				it++;
				if (IsFloat(*it))
				{

					it++;
					if (CompareTypes(POINTObjectType, Find(GraphCommand, *it)) || CompareTypes(LINE, Find(GraphCommand, *it)))
					{

						*typeOfParams = 1;
						if (CompareTypes(LINE, Find(GraphCommand, *it)))
						{

							*typeOfParams = 2;
						}
						it++;
						if (IsFloat(*it))//type is short TODO
						{
							return RectangleFromLine;
						}
					}
				}
			}
			return  INVALID;
			//RectangleFromLine(string surfaceName, Line l, float width, Point surfacePoint, short type, bool visible = true)
			//RectangleFromLine(string surfaceName, Line l, float width, Vector3 normalVector, short type, bool visible = true)
			//create Rectangle from Line l
			/*type:
				0 - width/2 to left, width/2 to right
				1 - width to left
				2 - width to right
				*/
				//if normal vector is not perpendicular to line, as normal is used normalized dot product between line and normal vector
				//if normal vector is same direction as line normal, exception occure
				//If surface point is not on line l, exception occure
		}
		if (commandName == "Circle")
		{
			if (CountOfParams != 3)
			{
				return INVALID;
			}
			if (CompareTypes(POINTObjectType, Find(GraphCommand, *it)))
			{
				it++;
				if (IsFloat(*it))
				{
					it++;
					if (CompareTypes(LINE, Find(GraphCommand, *it)))
					{
						*typeOfParams = 1;
						return Circle;
					}
				}
				else if (CompareTypes(POINTObjectType, Find(GraphCommand, *it)))
				{
					it++;
					if (CompareTypes(POINTObjectType, Find(GraphCommand, *it)))
					{
						*typeOfParams = 2;
						return Circle;
					}
				}
			}
			//1 - radius + line normal
			//2 - 3 points
			return INVALID;
			//Circle(string surfaceName, Point center, float radius, Line lineNormal, bool visible = true)
			//Circle(string surfaceName, Point center, Point outlinePoint, Point planePoint, bool visible = true)

		}
		if (commandName == "Triangle")
		{
			if (CountOfParams == 3)
			{
				if (CompareTypes(POINTObjectType, Find(GraphCommand, *it)))
				{
					it++;
					if (CompareTypes(POINTObjectType, Find(GraphCommand, *it)))
					{
						it++;
						if (CompareTypes(POINTObjectType, Find(GraphCommand, *it)))
						{
							*typeOfParams = 2;
							return Triangle;
						}
					}
				}
			}
			if (CountOfParams == 2)
			{
				if (CompareTypes(LINE, Find(GraphCommand, *it)))
				{
					it++;
					if (CompareTypes(POINTObjectType, Find(GraphCommand, *it)))
					{
						*typeOfParams = 1;
						return Triangle;
					}
				}
			}
			//1 - Line + point
			//2 - 3 Points
			return  INVALID;
			//Triangle(string surfaceName, Line l, Point p, bool visible = true)
			//Triangle(string surfaceName, Point p1, Point p2, Point p3, bool visible = true)

				//rectangle
		}
		if (commandName == "Rectangle")
		{
			if (CountOfParams != 5)
			{
				return INVALID;
			}

			if (!CompareTypes(POINTObjectType, Find(GraphCommand, *it)))//center
			{
				return INVALID;
			}
			it++;
			if (!IsFloat(*it))//X
			{
				return INVALID;
			}
			it++;
			if (!IsFloat(*it))//Y
			{
				return INVALID;
			}
			it++;
			if (!IsFloat(*it))//roll
			{
				return INVALID;
			}
			it++;
			if (!CompareTypes(LINE, Find(GraphCommand, *it)))//normal
			{
				return INVALID;
			}
			return  Rectangle;
			//Rectangle(string surfaceName, Point center, float X, float Y, float Roll/*[0,360]*/, Line normal, bool visible = true)

		}
		if (commandName == "Polygon")
		{
			for (size_t i = 0; i < CountOfParams; i++)
			{
				if (!CompareTypes(POINTObjectType, Find(GraphCommand, *it)))//normal
				{
					return INVALID;
				}
				it++;
			}
			return  Polygon;
			//Polygon(string surfaceName, Point p1, Point p2, Point p3, ..., bool visible = true)//minimum 3 points 


		}
		if (commandName == "Circumscribed")
		{
			if (CountOfParams != 1)
			{
				return INVALID;
			}

			if (!CompareTypes(TRIANGLE, Find(GraphCommand, *it)))//center
			{
				return INVALID;
			}
			return  Circumscribed;
			//Circumscribed(string surfaceName, Triangle t, bool visible = true) //Create circle over triangle
		}
		if (commandName == "Inscribed")
		{
			if (CountOfParams != 1)
			{
				return INVALID;
			}

			if (!CompareTypes(TRIANGLE, Find(GraphCommand, *it)))//center
			{
				return INVALID;
			}
			return  Inscribed;
			//Inscribed(string surfaceName, Triangle t, bool visible = true)		//Create circle in triangle

			///Objects Commands:

		}
		if (commandName == "Pyramid")
		{
			if (CountOfParams != 2)
			{
				return INVALID;
			}

			if (CompareTypes(SURFACE, Find(GraphCommand, *it)))//center
			{
				it++;
				if (IsFloat(*it))//center
				{
					*typeOfParams = 1;
					return Pyramid;
				}
				if (CompareTypes(POINTObjectType, Find(GraphCommand, *it)))
				{
					*typeOfParams = 2;
					return Pyramid;
				}

			}
			//1 - distance
			//2 - point
			return  INVALID;
			//Pyramid(string objectName, Surface s, float distance, bool visible = true) //Create Pyramid by distance from center
			//Pyramid(string objectName, Surface s, Point p, bool visible = true) //Create Pyramid by Point

		}
		if (commandName == "Extrude")
		{
			if (CountOfParams != 2)
			{
				return INVALID;
			}

			if (!CompareTypes(SURFACE, Find(GraphCommand, *it)))//center
			{
				return INVALID;
			}
			it++;
			if (!IsFloat(*it))//center
			{
				return INVALID;
			}
			return  Extrude;
			//Extrude(string objectName, Surface s, float distance, bool visible = true) //add width to 

		}
		//if (commandName == "SpericalCurvedSurface")
		//{
		//	if (CountOfParams != 2)
		//	{
		//		return INVALID;
		//	}

		//	if (!CompareTypes(SURFACE, Find(GraphCommand, *it)))//center
		//	{
		//		return INVALID;
		//	}
		//	it++;
		//	if (!IsFloat(*it))//center
		//	{
		//		return INVALID;
		//	}
		//	return  SpericalCurvedSurface;
			//SpericalCurvedSurface(string objectName, Surface s, float distance, bool visible = true)


		//}
		//if (commandName == "Cylinder")
		//{
		//	if (CountOfParams != 2)
		//	{
		//		return INVALID;
		//	}

		//	if (!CompareTypes(LINE, Find(GraphCommand, *it)))//center
		//	{
		//		return INVALID;
		//	}
		//	it++;
		//	if (!IsFloat(*it))//center
		//	{
		//		return INVALID;
		//	}
		//	return  Cylinder;
		//	//Cylinder(string objectName, Line l, float radius, bool visible = true)

		//}
		return INVALID;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	inline ObjectTypeEnum findTypeOfOperation(operationType::OperationTypeEnum operationType) {
		/*POINT,
		LINE,
		SURFACE,
		CIRCLE,
		RECTANGLE,
		Polygon,
		TRIANGLE,
		PYRAMID,
		OBJECT3D*/
		switch (operationType) {
		case Point:
		case LinearInterpolationDist:
		case LinearInterpolationPerc:
		case Intersection_Plane_Line:
		case SurfaceCenterAverage:
		case SurfaceCenterBoundingSquare:
		case ObjectCenterBoundingBox:
		case ObjectCenterAverage:
		case LineFirstPoint:
		case LineSecondPoint:
		case Centroid:
		case Incenter:
		case Circumcenter:
		case Orthocenter:
		case NinePointCenter:
			return POINTObjectType;
			///Line Commands:
		case Line:
		case LineNormalize:
		case LineChangeLengthDist:
		case LineChangeLengthPerc:
		case MinLineBetweenLineAndLine:
		case MinLineBetweenPointAndLine:
		case MinLineBetweenPointAndSurface:
		case MinLine:
		case SurfaceNormal:
		case LineRelocationByPoint:
			//case OrthogonalLeastSquares:
		case CrossProduct:
		//case CrossProductLP:
			return LINE;
			///Surface Commands:


		case RectangleFromLine:
		case Circle:
			return CIRCLE;
		case Triangle:
			return TRIANGLE;
		case Rectangle:
			return  RECTANGLE;
		case Polygon:
			return POLYGON;
		case Circumscribed:
			return CIRCLE;
		case Inscribed:
			return CIRCLE;
			break;

			///Objects Commands:


			break;
		case Pyramid:
			return PYRAMID;
			break;
		case Extrude:
			return OBJECT3D;
			break;
		//case SpericalCurvedSurface:
		//	return OBJECT3D;
		//case Cylinder:
		//	return OBJECT3D;
		default://INVALID
			return POINTObjectType;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	inline OperationTypeEnum GetOperationParameters(
		std::string commandName,
		std::vector < 
				std::vector<
						ParameterTypesEnum
					>*
			>** paramVectors,/*contain vector of parameters*/
		std::vector <
			std::vector<
				std::string
				>*
			>** paramVectorsInfo = NULL,bool info=false
	) {
		*paramVectors = new std::vector < std::vector<ParameterTypesEnum>*>();
		if (info)
		{
			*paramVectorsInfo = new std::vector <
										std::vector<
											std::string
										>*
									>();
		}


		if (commandName == "Point")
		{
			std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
			params1->push_back(ParameterTypeFLOAT);
			params1->push_back(ParameterTypeFLOAT);
			params1->push_back(ParameterTypeFLOAT);

			std::vector<ParameterTypesEnum>* params2 = new std::vector<ParameterTypesEnum>();
			params2->push_back(ParameterTypeFLOAT);
			params2->push_back(ParameterTypeFLOAT);
			params2->push_back(ParameterTypeFLOAT);
			params2->push_back(ParameterTypePOINT);
			(*paramVectors)->push_back(params1);
			(*paramVectors)->push_back(params2);



			if (info)
			{
				std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
				paramInfo1->push_back("Create point with absolut position");
				paramInfo1->push_back("Position X");
				paramInfo1->push_back("Position Y");
				paramInfo1->push_back("Position Z");
				std::vector<std::string> *paramInfo2 = new std::vector<std::string>();
				paramInfo2->push_back("Create point with relative position of entered point");
				paramInfo2->push_back("Position X");
				paramInfo2->push_back("Position Y");
				paramInfo2->push_back("Position Z");
				paramInfo2->push_back("Parent point");

				(*paramVectorsInfo)->push_back(paramInfo1);
				(*paramVectorsInfo)->push_back(paramInfo2);
			}
			return  Point;
			
			//Point(string name, Point Parent = NULL, bool visible = true) //- Create point on position 0,0,0. Parent - If entered, position is relative, else absolute
			//Point(string name, float X, float Y, float Z, Point Parent = NULL, bool visible = true) //-Create point on position XYZ
			///	Example:
			//		Point("Name of point 1")	//Name can by written with "" or '' or without, Point is on absolute position [0,0,0]
			//		Point(NameOfPoint2)		//Name can by written with "" or '' or without
			//		Point('NameOfPoint3')	//Name can by written with "" or '' or without
			//		Point(NameOfPoint4,1.1,2.2,3.3)	//- Create visible Point on position [1,2,3] 
			//		Point(NameOfPoint4,1,2,3,true)	//- Create visible Point on position [1,2,3] 
			//		Point(NameOfPoint4,1,2,3,false)	//- Create invisible Point on position [1,2,3]
			//		Point(NameOfPoint4,1,2,3,NameOfParentPoint,true)	//- Create visible Point on relative position [1,2,3] from ParentPoint
		}
		if (commandName == "LinearInterpolationDist")
		{
			std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
			params1->push_back(ParameterTypePOINT);
			params1->push_back(ParameterTypePOINT);
			params1->push_back(ParameterTypeFLOAT);

			(*paramVectors)->push_back(params1);
			if (info)
			{
				std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
				paramInfo1->push_back("Create point in distance from first point to second");
				paramInfo1->push_back("Point from");
				paramInfo1->push_back("Point to");
				paramInfo1->push_back("Distance from first point to second point");

				(*paramVectorsInfo)->push_back(paramInfo1);
			}
			return  LinearInterpolationDist;
			//LinearInterpolation(string name, Point fromPoint, Point toPoint, float distance / percentage, bool visible = true)
					//	Example:
					//		LinearInterpolation(PointName, FromPoint, ToPoint, 50%)	//- Create Point in middle of entered points
					//		LinearInterpolation(PointName, FromPoint, ToPoint, 25%)	//- Create Point on position with distance 20% of first point and 80% of second point
					//		LinearInterpolation(PointName, FromPoint, ToPoint, 100)	//- Create Point on position with distance 100 from first point in angle to second point 


		}
		if (commandName == "LinearInterpolationPerc")
		{
			std::vector<ParameterTypesEnum>* params2 = new std::vector<ParameterTypesEnum>();
			params2->push_back(ParameterTypePOINT);
			params2->push_back(ParameterTypePOINT);
			params2->push_back(ParameterTypeFLOAT);
			(*paramVectors)->push_back(params2);
			if (info)
			{
				std::vector<std::string> *paramInfo2 = new std::vector<std::string>();
				paramInfo2->push_back("Create point in percentual distance from first point to second");
				paramInfo2->push_back("Point from");
				paramInfo2->push_back("Point to");
				paramInfo2->push_back("Percentual distance from first point to second point (50 is in middle)");

				(*paramVectorsInfo)->push_back(paramInfo2);
			}
			return  LinearInterpolationPerc;
			//LinearInterpolation(string name, Point fromPoint, Point toPoint, float distance / percentage, bool visible = true)
					//	Example:
					//		LinearInterpolation(PointName, FromPoint, ToPoint, 50%)	//- Create Point in middle of entered points
					//		LinearInterpolation(PointName, FromPoint, ToPoint, 25%)	//- Create Point on position with distance 20% of first point and 80% of second point
					//		LinearInterpolation(PointName, FromPoint, ToPoint, 100)	//- Create Point on position with distance 100 from first point in angle to second point 


		}
		if (commandName == "Intersection_Plane_Line")
		{
			/*if (CountOfParams != 2)
			{
				return INVALID;
			}
			if (!CompareTypes(LINE, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			it++;
			if (!CompareTypes(SURFACE, Find(GraphCommand, *it)))
			{
				return INVALID;
			}*/
			std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
			params1->push_back(ParameterTypeLINE);
			params1->push_back(ParameterTypeSURFACE); 

			(*paramVectors)->push_back(params1);
			if (info)
			{
				std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
				paramInfo1->push_back("Create Point on position where Line intersecting Surface");
				paramInfo1->push_back("Line");
				paramInfo1->push_back("Surface");

				(*paramVectorsInfo)->push_back(paramInfo1);
			}
			return  Intersection_Plane_Line;

			//Intersection_Plane_Line(string name, Line lineName, Sufrace surfaceName, bool visible = true)
					//	Alternative:
					//	Intersection(string name, Line lineName, Sufrace surfaceName)
					//	Example:
					//		Intersection_Plane_Line(PointName,  lineName, surfaceName) //- Create Point on position where Line with name "lineName" intersecting Surface with name "surfaceName"

		}
		if (commandName == "SurfaceCenterBoundingSquare")
		{
			/*if (CountOfParams != 1)
			{
				return INVALID;
			}
			if (!CompareTypes(SURFACE, Find(GraphCommand, *it)))
			{
				return INVALID;
			}*/

			std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
			params1->push_back(ParameterTypeSURFACE);

			(*paramVectors)->push_back(params1);
			if (info)
			{
				std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
				paramInfo1->push_back("Create point on position in middle of bounding box of entered surface");
				paramInfo1->push_back("Surface");

				(*paramVectorsInfo)->push_back(paramInfo1);
			}
			return  SurfaceCenterBoundingSquare;
			//SurfaceCenterBoundingSquare(string name, Surface surfaceName, bool visible = true) //Create point on position of middle of entered surface
				//	Example:
				//		SurfaceCenterBoundingSquare(PointName, Circle)	//- Create Point on center of Circle
				//		SurfaceCenterBoundingSquare(PointName, Rectangle)	//- Create Point on middle of Rectangle
				//		SurfaceCenter(PointName, Polygon)		//- Create Point on middle of Polygon 
				//		SurfaceCenterBoundingSquare(PointName, Polygon)		//- Create Point on middle of Polygon - centroid (sum of points / count of points)

		}
		if (commandName == "SurfaceCenterAverage")
		{
			/*if (CountOfParams != 1)
			{
				return INVALID;
			}
			if (!CompareTypes(SURFACE, Find(GraphCommand, *it)))
			{
				return INVALID;
			}*/

			std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
			params1->push_back(ParameterTypeSURFACE);

			(*paramVectors)->push_back(params1);
			if (info)
			{
				std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
				paramInfo1->push_back("Create point on avarage position of all points of entered surface");
				paramInfo1->push_back("Surface");

				(*paramVectorsInfo)->push_back(paramInfo1);
			}
			return  SurfaceCenterAverage;
			//SurfaceCenter(string name, Surface surfaceName, bool visible = true) //Create point on position of middle of entered surface
				//	Example:
				//		SurfaceMiddle(PointName, Circle)	//- Create Point on center of Circle
				//		SurfaceMiddle(PointName, Rectangle)	//- Create Point on middle of Rectangle
				//		SurfaceCenter(PointName, Polygon)		//- Create Point on middle of Polygon 
				//		SurfaceMiddle(PointName, Polygon)		//- Create Point on middle of Polygon - centroid (sum of points / count of points)

		}
		if (commandName == "Centroid")
		{

			std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
			params1->push_back(ParameterTypeTRIANGLE);

			(*paramVectors)->push_back(params1);
			if (info)
			{
				std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
				paramInfo1->push_back("Create point on centroid of triangle");
				paramInfo1->push_back("Triangle");

				(*paramVectorsInfo)->push_back(paramInfo1);
			}
			return  Centroid;//(string name, Triangle triangleName, float visibility)
		}
		if (commandName == "Incenter")
		{

			std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
			params1->push_back(ParameterTypeTRIANGLE);

			(*paramVectors)->push_back(params1);
			if (info)
			{
				std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
				paramInfo1->push_back("Create point on center of inscribed circle in triangle");
				paramInfo1->push_back("Triangle");

				(*paramVectorsInfo)->push_back(paramInfo1);
			}
			return  Incenter;//(string name, Triangle triangleName, float visibility)
		}
			if (commandName == "Circumcenter")
			{

				std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
				params1->push_back(ParameterTypeTRIANGLE);

				(*paramVectors)->push_back(params1);
				if (info)
				{
					std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
					paramInfo1->push_back("Create point on center of circumscribed circle over triangle");
					paramInfo1->push_back("Triangle");

					(*paramVectorsInfo)->push_back(paramInfo1);
				}
				return  Circumcenter;//(string name, Triangle triangleName, float visibility)
			}
			if (commandName == "Orthocenter")
			{

				std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
				params1->push_back(ParameterTypeTRIANGLE);

				(*paramVectors)->push_back(params1);
				if (info)
				{
					std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
					paramInfo1->push_back("Create orthocentrum of tringle");
					paramInfo1->push_back("Triangle");

					(*paramVectorsInfo)->push_back(paramInfo1);
				}
				return  Orthocenter;//(string name, Triangle triangleName, float visibility)
			}
			if (commandName == "NinePointCenter")
			{

				std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
				params1->push_back(ParameterTypeTRIANGLE);

				(*paramVectors)->push_back(params1);
				if (info)
				{
					std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
					paramInfo1->push_back("Create point at center of nine points circle of triangle");
					paramInfo1->push_back("Triangle");

					(*paramVectorsInfo)->push_back(paramInfo1);
				}
				return  NinePointCenter;//(string name, Triangle triangleName, float visibility)
			}
		if (commandName == "ObjectCenterBoundingBox")
		{
			/*if (CountOfParams != 1)
			{
				return INVALID;
			}
			if (!CompareTypes(OBJECT3D, Find(GraphCommand, *it)))
			{
				return INVALID;
			}*/
			std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
			params1->push_back(ParameterTypeOBJECT3D);

			(*paramVectors)->push_back(params1);
			if (info)
			{
				std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
				paramInfo1->push_back("Create point on position in middle of bounding box of entered object");
				paramInfo1->push_back("3D object");

				(*paramVectorsInfo)->push_back(paramInfo1);
			}
			return  ObjectCenterBoundingBox;
			//ObjectCenterBoundingBox(string name, Object3D ObjectName, bool visible = true) //Create point on position of middle of entered object
		}
		if (commandName == "ObjectCenterAverage")
		{
			/*if (CountOfParams != 1)
			{
				return INVALID;
			}
			if (!CompareTypes(OBJECT3D, Find(GraphCommand, *it)))
			{
				return INVALID;
			}*/
			std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
			params1->push_back(ParameterTypeOBJECT3D);

			(*paramVectors)->push_back(params1);
			if (info)
			{
				std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
				paramInfo1->push_back("Create point on avarage position of all points of entered object");
				paramInfo1->push_back("3D object");

				(*paramVectorsInfo)->push_back(paramInfo1);
			}
			return  ObjectCenterAverage;
			//ObjectCenterAverage(string name, Object3D ObjectName, bool visible = true) //Create point on position of center of entered object

		}
		if (commandName == "LineFirstPoint")
		{
			/*if (CountOfParams != 1)
			{
				return INVALID;
			}
			if (!CompareTypes(LINE, Find(GraphCommand, *it)))
			{
				return INVALID;
			}*/
			std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
			params1->push_back(ParameterTypeLINE);
			(*paramVectors)->push_back(params1);
			if (info)
			{
				std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
				paramInfo1->push_back("Return beginning point of Line");
				paramInfo1->push_back("Line");

				(*paramVectorsInfo)->push_back(paramInfo1);
			}
			return  LineFirstPoint;
			//LineFirstPoint(string name, Line lineName, bool visible = true)
		}
		if (commandName == "LineSecondPoint")
		{
			/*if (CountOfParams != 1)
			{
				return INVALID;
			}
			if (!CompareTypes(LINE, Find(GraphCommand, *it)))
			{
				return INVALID;
			}*/
			std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
			params1->push_back(ParameterTypeLINE);

			(*paramVectors)->push_back(params1);
			if (info)
			{
				std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
				paramInfo1->push_back("Return end point of Line");
				paramInfo1->push_back("Line");

				(*paramVectorsInfo)->push_back(paramInfo1);
			}
			return  LineSecondPoint;
			//LineSecondPoint(string name, Line lineName, bool visible = true)




				///Line Commands:

		}
		if (commandName == "Line")
		{
			/*if (CountOfParams != 2)
			{
				return INVALID;
			}
			if (!CompareTypes(POINTObjectType, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			it++;
			if (!CompareTypes(POINTObjectType, Find(GraphCommand, *it)))
			{
				return INVALID;
			}*/
			std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
			params1->push_back(ParameterTypePOINT);
			params1->push_back(ParameterTypePOINT);

			(*paramVectors)->push_back(params1);
			if (info)
			{
				std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
				paramInfo1->push_back("Create line, where p1 is start point and p2 is end point");
				paramInfo1->push_back("Beginning point");
				paramInfo1->push_back("End point");

				(*paramVectorsInfo)->push_back(paramInfo1);
			}
			return  Line;//Line(string lineName, Point p1, Point p2, bool visible = true) //create line, where p1 is start point and p2 is end point

		}
		if (commandName == "LineNormalize")
		{
			/*if (CountOfParams != 1)
			{
				return INVALID;
			}
			if (!CompareTypes(LINE, Find(GraphCommand, *it)))
			{
				return INVALID;
			}*/
			std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
			params1->push_back(ParameterTypeLINE);

			(*paramVectors)->push_back(params1);
			if (info)
			{
				std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
				paramInfo1->push_back("Normalize line to distance 1.0. Beginning point is same as beginning point of entered line, end point is in distance 1.0 in direction to end point of line");
				paramInfo1->push_back("Line to be normalized");

				(*paramVectorsInfo)->push_back(paramInfo1);
			}
			return  LineNormalize;//LineNormalize(string lineName, Line l, bool visible = true)

		}
		if (commandName == "LineChangeLengthDist") //redundant TODO
		{
			/*if (CountOfParams != 2)
			{
				return INVALID;
			}
			if (!CompareTypes(LINE, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			it++;
			if (!IsFloat(*it)) {
				return INVALID;
			}*/
			std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
			params1->push_back(ParameterTypeLINE);
			params1->push_back(ParameterTypeFLOAT);

			(*paramVectors)->push_back(params1);
			if (info)
			{
				std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
				paramInfo1->push_back("Beginning point is same as beginning point of entered line, end point is in entered distance in direction to end point of line");
				paramInfo1->push_back("Line");
				paramInfo1->push_back("Distance from begining point to end point");

				(*paramVectorsInfo)->push_back(paramInfo1);
			}
			return  LineChangeLengthDist;//LineChangeLengthDist(string lineName, Line l, float distance, bool visible = true)
		}
		if (commandName == "LineChangeLengthPerc")
		{
			/*if (CountOfParams != 2)
			{
				return INVALID;
			}
			if (!CompareTypes(LINE, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			it++;
			if (!IsFloat(*it)) {
				return INVALID;
			}*/
			std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
			params1->push_back(ParameterTypeLINE);
			params1->push_back(ParameterTypeFLOAT);

			(*paramVectors)->push_back(params1);
			if (info)
			{
				std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
				paramInfo1->push_back("Beginning point is same as beginning point of entered line, end point is in entered percentual distance in direction to end point of line");
				paramInfo1->push_back("Line");
				paramInfo1->push_back("Percentual distance (100 is same length) from begining point to end point");

				(*paramVectorsInfo)->push_back(paramInfo1);
			}
			return  LineChangeLengthPerc;//LineChangeLengthPerc(string lineName, Line l, float percent, bool visible = true) //percent = (0;100>



				// these commands are based on page http://paulbourke.net/geometry/pointlineplane/
		}
		if (commandName == "MinLineBetweenLineAndLine")
		{
			/*if (CountOfParams != 2)
			{
				return INVALID;
			}
			if (!CompareTypes(LINE, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			it++;
			if (!CompareTypes(LINE, Find(GraphCommand, *it))) {
				return INVALID;
			}*/
			std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
			params1->push_back(ParameterTypeLINE);
			params1->push_back(ParameterTypeLINE);

			(*paramVectors)->push_back(params1);
			if (info)
			{
				std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
				paramInfo1->push_back("Return minimal line between two lines");
				paramInfo1->push_back("first line");
				paramInfo1->push_back("second line");

				(*paramVectorsInfo)->push_back(paramInfo1);
			}
			return  MinLineBetweenLineAndLine;//MinLineBetweenLineAndLine(string lineName, Line l1, Line l2, bool visible = true)
		}
		if (commandName == "MinLineBetweenPointAndLine")
		{
			/*if (CountOfParams != 2)
			{
				return INVALID;
			}
			if (!CompareTypes(POINTObjectType, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			it++;
			if (!CompareTypes(LINE, Find(GraphCommand, *it))) {
				return INVALID;
			}*/
			std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
			params1->push_back(ParameterTypePOINT);
			params1->push_back(ParameterTypeLINE);

			(*paramVectors)->push_back(params1);
			if (info)
			{
				std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
				paramInfo1->push_back("Return minimal line between point and line");
				paramInfo1->push_back("Point");
				paramInfo1->push_back("Line");

				(*paramVectorsInfo)->push_back(paramInfo1);
			}
			return  MinLineBetweenPointAndLine;//MinLineBetweenPointAndLine(string lineName, Point p, Line l, bool visible = true)
		}
		if (commandName == "MinLineBetweenPointAndSurface")
		{
			/*if (CountOfParams != 2)
			{
				return INVALID;
			}
			if (!CompareTypes(POINTObjectType, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			it++;
			if (!CompareTypes(SURFACE, Find(GraphCommand, *it))) {
				return INVALID;
			}*/
			std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
			params1->push_back(ParameterTypePOINT);
			params1->push_back(ParameterTypeSURFACE);

			(*paramVectors)->push_back(params1);
			if (info)
			{
				std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
				paramInfo1->push_back("Return minimal line between point and surface");
				paramInfo1->push_back("Point");
				paramInfo1->push_back("Surface");

				(*paramVectorsInfo)->push_back(paramInfo1);
			}
			return  MinLineBetweenPointAndSurface;//MinLineBetweenPointAndSurface(string lineName, Point p, Surface s, bool visible = true)

				///Alternative:
		}
		if (commandName == "MinLine")
		{
			

			std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
			params1->push_back(ParameterTypeLINE);
			params1->push_back(ParameterTypeLINE);

			(*paramVectors)->push_back(params1);

			std::vector<ParameterTypesEnum>* params2 = new std::vector<ParameterTypesEnum>();
			params2->push_back(ParameterTypePOINT);
			params2->push_back(ParameterTypeLINE);

			(*paramVectors)->push_back(params2);

			std::vector<ParameterTypesEnum>* params3 = new std::vector<ParameterTypesEnum>();
			params3->push_back(ParameterTypePOINT);
			params3->push_back(ParameterTypeSURFACE);

			(*paramVectors)->push_back(params3);
			if (info)
			{
				std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
				paramInfo1->push_back("Return minimal line between two lines");
				paramInfo1->push_back("First line");
				paramInfo1->push_back("Second line");

				std::vector<std::string> *paramInfo2 = new std::vector<std::string>();
				paramInfo2->push_back("Return minimal line between point and line");
				paramInfo2->push_back("Point");
				paramInfo2->push_back("Line");
				std::vector<std::string> *paramInfo3 = new std::vector<std::string>();
				paramInfo3->push_back("Return minimal line between point and surface");
				paramInfo3->push_back("Point");
				paramInfo3->push_back("Surface");

				(*paramVectorsInfo)->push_back(paramInfo1);
				(*paramVectorsInfo)->push_back(paramInfo2);
				(*paramVectorsInfo)->push_back(paramInfo3);
			}
			return MinLine;
			
			//MinLine(string lineName, Object o1, Object o2, bool visible = true) //Supported are only : [Line,Line], [Point,Line], [Point, Surface]



		}
		if (commandName == "SurfaceNormal")
		{
			/*if (CountOfParams != 1)
			{
				return INVALID;
			}
			if (!CompareTypes(SURFACE, Find(GraphCommand, *it)))
			{
				return INVALID;
			}*/
			std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
			params1->push_back(ParameterTypeSURFACE);

			(*paramVectors)->push_back(params1);
			if (info)
			{
				std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
				paramInfo1->push_back("Return normal vector of surface");
				paramInfo1->push_back("Surface");

				(*paramVectorsInfo)->push_back(paramInfo1);
			}
			return  SurfaceNormal;//SurfaceNormal(string lineName, Surface s, bool visible = true) // return normal vector of surface



		}
		if (commandName == "LineRelocationByPoint")
		{
			/*if (CountOfParams != 2)
			{
				return INVALID;
			}
			if (!CompareTypes(LINE, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			it++;
			if (!CompareTypes(POINTObjectType, Find(GraphCommand, *it)))
			{
				return INVALID;
			}*/
			std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
			params1->push_back(ParameterTypeLINE);
			params1->push_back(ParameterTypePOINT);

			(*paramVectors)->push_back(params1);
			if (info)
			{
				std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
				paramInfo1->push_back("Line is moved to new location. Beginning point is entered point and end point is in distance of entered line with same direction.");
				paramInfo1->push_back("Line");
				paramInfo1->push_back("New location");

				(*paramVectorsInfo)->push_back(paramInfo1);
			}
			return  LineRelocationByPoint;//LineRelocationByPoint(string lineName, Line l, Point p, bool visible = true)

		}
		/*if (commandName == "OrthogonalLeastSquares")
		{
			return  OrthogonalLeastSquares;//OrthogonalLeastSquares(string lineName, Point p1, Point p2, Point p3, ..., bool visible = true) //min 3

		}*/
		if (commandName == "CrossProduct")
		{
			/*if (CountOfParams != 2)
			{
				return INVALID;
			}
			if (!CompareTypes(LINE, Find(GraphCommand, *it)))
			{
				return INVALID;
			}
			it++;
			if (!CompareTypes(LINE, Find(GraphCommand, *it)))
			{
				return INVALID;
			}*/
			std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
			params1->push_back(ParameterTypeLINE);
			params1->push_back(ParameterTypeLINE);

			(*paramVectors)->push_back(params1);
			if (info)
			{
				std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
				paramInfo1->push_back("Create cross-product of entered lines");
				paramInfo1->push_back("First line");
				paramInfo1->push_back("Second line");

				(*paramVectorsInfo)->push_back(paramInfo1);
			}
			return  CrossProduct;
			//CrossProduct(string lineName, Line l1, Line l2, bool visible = true)



		///Surface Commands:

		}
		if (commandName == "RectangleFromLine")
		{//1 - surface point, 2 - normal vector
			/*if (CountOfParams != 4)
			{
				return INVALID;
			}
			if (CompareTypes(LINE, Find(GraphCommand, *it)))
			{
				it++;
				if (IsFloat(*it))
				{

					it++;
					if (CompareTypes(POINTObjectType, Find(GraphCommand, *it)) || CompareTypes(LINE, Find(GraphCommand, *it)))
					{
						it++;
						//	if ()type is short TODO
						{*/

			std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
			params1->push_back(ParameterTypeLINE);
			params1->push_back(ParameterTypeFLOAT);
			params1->push_back(ParameterTypePOINT);
			params1->push_back(ParameterTypeFLOAT);

			(*paramVectors)->push_back(params1);

			std::vector<ParameterTypesEnum>* params2 = new std::vector<ParameterTypesEnum>();
			params2->push_back(ParameterTypeLINE);
			params2->push_back(ParameterTypeFLOAT);
			params2->push_back(ParameterTypeLINE);
			params2->push_back(ParameterTypeFLOAT);

			(*paramVectors)->push_back(params2);
			if (info)
			{
				std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
				paramInfo1->push_back("Create rectangle surface from line");
				paramInfo1->push_back("Line to be enlarged");
				paramInfo1->push_back("Width");
				paramInfo1->push_back("Point on surface ");
				paramInfo1->push_back("Type:0 - width/2 to left, width/2 to right; 1 - width to left; 2 - width to right.");
				std::vector<std::string> *paramInfo2 = new std::vector<std::string>();
				paramInfo2->push_back("Create rectangle surface from line");
				paramInfo2->push_back("Line to be enlarged");
				paramInfo2->push_back("Width");
				paramInfo2->push_back("Normal of surface");
				paramInfo2->push_back("Type:0 - width/2 to left, width/2 to right; 1 - width to left; 2 - width to right.");

				(*paramVectorsInfo)->push_back(paramInfo1);
				(*paramVectorsInfo)->push_back(paramInfo2);
			}
			return RectangleFromLine;
						/*}
					}
				}
			}*/
			//return  INVALID;
			//RectangleFromLine(string surfaceName, Line l, float width, Point surfacePoint, short type, bool visible = true)
			//RectangleFromLine(string surfaceName, Line l, float width, Vector3 normalVector, short type, bool visible = true)
			//create Rectangle from Line l
			/*type:
				0 - width/2 to left, width/2 to right
				1 - width to left
				2 - width to right
				*/
				//if normal vector is not perpendicular to line, as normal is used normalized dot product between line and normal vector
				//if normal vector is same direction as line normal, exception occure
				//If surface point is not on line l, exception occure
		}
		if (commandName == "Circle")
		{
			/*if (CountOfParams != 3)
			{
				return INVALID;
			}
			if (CompareTypes(POINTObjectType, Find(GraphCommand, *it)))
			{
				it++;
				if (IsFloat(*it))
				{
					it++;
					if (CompareTypes(LINE, Find(GraphCommand, *it)))
					{
						return Circle;
					}
				}
				else if (CompareTypes(POINTObjectType, Find(GraphCommand, *it)))
				{
					it++;
					if (CompareTypes(POINTObjectType, Find(GraphCommand, *it)))
					{*/


			std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
			params1->push_back(ParameterTypePOINT);
			params1->push_back(ParameterTypeFLOAT);
			params1->push_back(ParameterTypeLINE);

			(*paramVectors)->push_back(params1);
			std::vector<ParameterTypesEnum>* params2 = new std::vector<ParameterTypesEnum>();
			params2->push_back(ParameterTypePOINT);
			params2->push_back(ParameterTypePOINT);
			params2->push_back(ParameterTypePOINT);

			(*paramVectors)->push_back(params2);
			if (info)
			{
				std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
				paramInfo1->push_back("Create circle by center, radius and normal vector");
				paramInfo1->push_back("Center of circle");
				paramInfo1->push_back("Radius of circle");
				paramInfo1->push_back("Normal of surface");
				std::vector<std::string> *paramInfo2 = new std::vector<std::string>();
				paramInfo2->push_back("Create circle by 3 points");
				paramInfo2->push_back("Center of circle");
				paramInfo2->push_back("Outline point");
				paramInfo2->push_back("Plane point");

				(*paramVectorsInfo)->push_back(paramInfo1);
				(*paramVectorsInfo)->push_back(paramInfo2);
			}
			return Circle;
			//Circle(string surfaceName, Point center, float radius, Line lineNormal, bool visible = true)
			//Circle(string surfaceName, Point center, Point outlinePoint, Point planePoint, bool visible = true)

		}
		if (commandName == "Triangle")
		{
			/*if (CountOfParams == 3)
			{
				if (CompareTypes(POINTObjectType, Find(GraphCommand, *it)))
				{
					it++;
					if (CompareTypes(POINTObjectType, Find(GraphCommand, *it)))
					{
						it++;
						if (CompareTypes(POINTObjectType, Find(GraphCommand, *it)))
						{
							return Triangle;
						}
					}
				}
			}
			if (CountOfParams == 2)
			{
				if (CompareTypes(LINE, Find(GraphCommand, *it)))
				{
					it++;
					if (CompareTypes(POINTObjectType, Find(GraphCommand, *it)))
					{*/
			std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
			params1->push_back(ParameterTypeLINE);
			params1->push_back(ParameterTypePOINT);

			(*paramVectors)->push_back(params1);
			std::vector<ParameterTypesEnum>* params2 = new std::vector<ParameterTypesEnum>();
			params2->push_back(ParameterTypePOINT);
			params2->push_back(ParameterTypePOINT);
			params2->push_back(ParameterTypePOINT);

			(*paramVectors)->push_back(params2);
			if (info)
			{
				std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
				paramInfo1->push_back("Create triangle by line and point");
				paramInfo1->push_back("Line");
				paramInfo1->push_back("Point");
				std::vector<std::string> *paramInfo2 = new std::vector<std::string>();
				paramInfo2->push_back("Create triangle by 3 points");
				paramInfo2->push_back("First point");
				paramInfo2->push_back("Second point");
				paramInfo2->push_back("Third point");

				(*paramVectorsInfo)->push_back(paramInfo1);
				(*paramVectorsInfo)->push_back(paramInfo2);
			}
			return Triangle;
			//1 - Line + point
			//2 - 3 Points
			//Triangle(string surfaceName, Line l, Point p, bool visible = true)
			//Triangle(string surfaceName, Point p1, Point p2, Point p3, bool visible = true)

				//rectangle
		}
		if (commandName == "Rectangle")
		{
			/*if (CountOfParams != 5)
			{
				return INVALID;
			}

			if (!CompareTypes(POINTObjectType, Find(GraphCommand, *it)))//center
			{
				return INVALID;
			}
			it++;
			if (!IsFloat(*it))//X
			{
				return INVALID;
			}
			it++;
			if (!IsFloat(*it))//Y
			{
				return INVALID;
			}
			it++;
			if (!IsFloat(*it))//roll
			{
				return INVALID;
			}
			it++;
			if (!CompareTypes(LINE, Find(GraphCommand, *it)))//normal
			{
				return INVALID;
			}*/
			std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
			params1->push_back(ParameterTypePOINT);
			params1->push_back(ParameterTypeFLOAT);
			params1->push_back(ParameterTypeFLOAT);
			params1->push_back(ParameterTypeFLOAT);
			params1->push_back(ParameterTypeLINE);

			(*paramVectors)->push_back(params1);
			if (info)
			{
				std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
				paramInfo1->push_back("Create recangle");
				paramInfo1->push_back("Center Point");
				paramInfo1->push_back("Size X");
				paramInfo1->push_back("Size Y");
				paramInfo1->push_back("Rotation of surface [0;360]");
				paramInfo1->push_back("Normal vector");

				(*paramVectorsInfo)->push_back(paramInfo1);
			}
			return  Rectangle;
			//Rectangle(string surfaceName, Point center, float X, float Y, float Roll/*[0,360]*/, Line normal, bool visible = true)

		}
		if (commandName == "Polygon")//TODO
		{
			/*for (size_t i = 0; i < CountOfParams; i++)
			{
				if (!CompareTypes(POINTObjectType, Find(GraphCommand, *it)))//normal
				{
					return INVALID;
				}
				it++;
			}*/

			std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
			//params1->push_back(ParameterTypePOINT);
			params1->push_back(ParameterTypeMULTIPLEPOINTS);

			if (info)
			{
				std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
				paramInfo1->push_back("Create Polygon by connecting points");
				//paramInfo1->push_back("Origin point");
				paramInfo1->push_back("Multiple points - ; divider");

				(*paramVectorsInfo)->push_back(paramInfo1);
			}
			(*paramVectors)->push_back(params1);
			return  Polygon;
			//Polygon(string surfaceName, Point p1, Point p2, Point p3, ..., bool visible = true)//minimum 3 points 


		}
		if (commandName == "Circumscribed")
		{
			/*if (CountOfParams != 1)
			{
				return INVALID;
			}

			if (!CompareTypes(TRIANGLE, Find(GraphCommand, *it)))//center
			{
				return INVALID;
			}*/
			std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
			params1->push_back(ParameterTypeTRIANGLE);

			(*paramVectors)->push_back(params1);
			if (info)
			{
				std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
				paramInfo1->push_back("Create circumscribed circle around triangle");
				paramInfo1->push_back("Triangle");

				(*paramVectorsInfo)->push_back(paramInfo1);
			}
			return  Circumscribed;
			//Circumscribed(string surfaceName, Triangle t, bool visible = true) //Create circle over triangle
		}
		if (commandName == "Inscribed")
		{
			/*if (CountOfParams != 1)
			{
				return INVALID;
			}

			if (!CompareTypes(TRIANGLE, Find(GraphCommand, *it)))//center
			{
				return INVALID;
			}*/
			std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
			params1->push_back(ParameterTypeTRIANGLE);

			(*paramVectors)->push_back(params1);
			if (info)
			{
				std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
				paramInfo1->push_back("Create inscribed circle in triangle");
				paramInfo1->push_back("Triangle");

				(*paramVectorsInfo)->push_back(paramInfo1);
			}
			return  Inscribed;
			//Inscribed(string surfaceName, Triangle t, bool visible = true)		//Create circle in triangle

			///Objects Commands:

		}
		if (commandName == "Pyramid")
		{
			/*if (CountOfParams != 2)
			{
				return INVALID;
			}

			if (CompareTypes(SURFACE, Find(GraphCommand, *it)))//center
			{
				it++;
				if (IsFloat(*it) || CompareTypes(POINTObjectType, Find(GraphCommand, *it)))//center
				{*/
			std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
			params1->push_back(ParameterTypeSURFACE);
			params1->push_back(ParameterTypeFLOAT);

			(*paramVectors)->push_back(params1);
			std::vector<ParameterTypesEnum>* params2 = new std::vector<ParameterTypesEnum>();
			params2->push_back(ParameterTypeSURFACE);
			params2->push_back(ParameterTypePOINT);

			(*paramVectors)->push_back(params2);
			if (info)
			{
				std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
				paramInfo1->push_back("Create pyramid from surface and distance from center");
				paramInfo1->push_back("Base");
				paramInfo1->push_back("Base");
				std::vector<std::string> *paramInfo2 = new std::vector<std::string>();
				paramInfo2->push_back("Create pyramid from surface and specific point");
				paramInfo2->push_back("Base");
				paramInfo2->push_back("Point");

				(*paramVectorsInfo)->push_back(paramInfo1);
				(*paramVectorsInfo)->push_back(paramInfo2);
			}
			return Pyramid;
				/*}

			}*/
			//1 - distance
			//2 - point
			//return  INVALID;
			//Pyramid(string objectName, Surface s, float distance, bool visible = true) //Create Pyramid by distance from center
			//Pyramid(string objectName, Surface s, Point p, bool visible = true) //Create Pyramid by Point

		}
		if (commandName == "Extrude")
		{
			/*if (CountOfParams != 2)
			{
				return INVALID;
			}

			if (!CompareTypes(SURFACE, Find(GraphCommand, *it)))//center
			{
				return INVALID;
			}
			it++;
			if (!IsFloat(*it))//center
			{
				return INVALID;
			}*/
			std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
			params1->push_back(ParameterTypeSURFACE);
			params1->push_back(ParameterTypeFLOAT);

			(*paramVectors)->push_back(params1);
			if (info)
			{
				std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
				paramInfo1->push_back("Enlarge surface to 3D with distance");
				paramInfo1->push_back("Base");
				paramInfo1->push_back("Distance");

				(*paramVectorsInfo)->push_back(paramInfo1);
			}
			return  Extrude;
			//Extrude(string objectName, Surface s, float distance, bool visible = true) //add width to 

		}
		//if (commandName == "SpericalCurvedSurface") //TODO
		//{
		//	std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
		//	params1->push_back(ParameterTypeSURFACE);
		//	params1->push_back(ParameterTypeFLOAT);

		//	(*paramVectors)->push_back(params1);
		//	if (info)
		//	{
		//		std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
		//		paramInfo1->push_back("Create spherical curved surfaces from surface");
		//		paramInfo1->push_back("Base");
		//		paramInfo1->push_back("Distance");

		//		(*paramVectorsInfo)->push_back(paramInfo1);
		//	}
		//	return  SpericalCurvedSurface;
		//	//SpericalCurvedSurface(string objectName, Surface s, float distance, bool visible = true)


		//}
		//if (commandName == "Cylinder")
		//{
		//	/*if (CountOfParams != 2)
		//	{
		//		return INVALID;
		//	}

		//	if (!CompareTypes(LINE, Find(GraphCommand, *it)))//center
		//	{
		//		return INVALID;
		//	}
		//	it++;
		//	if (!IsFloat(*it))//center
		//	{
		//		return INVALID;
		//	}*/
		//	std::vector<ParameterTypesEnum>* params1 = new std::vector<ParameterTypesEnum>();
		//	params1->push_back(ParameterTypeLINE);
		//	params1->push_back(ParameterTypeFLOAT);

		//	(*paramVectors)->push_back(params1);
		//	if (info)
		//	{
		//		std::vector<std::string> *paramInfo1 = new std::vector<std::string>();
		//		paramInfo1->push_back("Create cylinder from line and radius");
		//		paramInfo1->push_back("Line");
		//		paramInfo1->push_back("Radius");

		//		(*paramVectorsInfo)->push_back(paramInfo1);
		//	}
		//	return  Cylinder;
		//	//Cylinder(string objectName, Line l, float radius, bool visible = true)

		//}
		return INVALID;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	inline std::string OperationToString(operationType::OperationTypeEnum operationType) {
		/*POINT,
		LINE,
		SURFACE,
		CIRCLE,
		RECTANGLE,
		Polygon,
		TRIANGLE,
		PYRAMID,
		OBJECT3D*/
		switch (operationType) {
//Point Commands:
		case Point:
			return "Point";
		case LinearInterpolationDist:
			return "LinearInterpolationDist";
		case LinearInterpolationPerc:
			return "LinearInterpolationPerc";
		case Intersection_Plane_Line:
			return "Intersection_Plane_Line";
		case SurfaceCenterBoundingSquare:
			return "SurfaceCenterBoundingSquare";
		case SurfaceCenterAverage:
			return "SurfaceCenterAverage";
		case Centroid:
			return "Centroid";
		case Incenter:
			return "Incenter";
		case Circumcenter:
			return "Circumcenter";
		case Orthocenter:
			return "Orthocenter";
		case NinePointCenter:
			return "NinePointCenter";
		case ObjectCenterBoundingBox:
			return "ObjectCenterBoundingBox";
		case ObjectCenterAverage:
			return "ObjectCenterAverage";
		case LineFirstPoint:
			return "LineFirstPoint";
		case LineSecondPoint:
			return "LineSecondPoint";
//Line Commands:
		case Line:
			return "Line";
		case LineNormalize:
			return "LineNormalize";
		case LineChangeLengthDist:
			return "LineChangeLengthDist";
		case LineChangeLengthPerc:
			return "LineChangeLengthPerc";
		case MinLineBetweenLineAndLine:
			return "MinLineBetweenLineAndLine";
		case MinLineBetweenPointAndLine:
			return "MinLineBetweenPointAndLine";
		case MinLineBetweenPointAndSurface:
			return "MinLineBetweenPointAndSurface";
		case MinLine:
			return "MinLine";
		case SurfaceNormal:
			return "SurfaceNormal";
		case LineRelocationByPoint:
			return "LineRelocationByPoint";
		case CrossProduct:
			return "CrossProduct";
//Surface Commands:


		case RectangleFromLine:
			return "RectangleFromLine";
		case Circle:
			return "Circle";
		case Triangle:
			return "Triangle";
		case Rectangle:
			return "Rectangle";
		case Polygon:
			return "Polygon";
		case Circumscribed:
			return "Circumscribed";
		case Inscribed:
			return "Inscribed";

//Objects Commands:


		case Pyramid:
			return "Pyramid";
		case Extrude:
			return "Extrude";
		/*case SpericalCurvedSurface:
			return "SpericalCurvedSurface";
		case Cylinder:
			return "Cylinder";*/
		default://INVALID
			return "INVALID";
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	inline std::string ParameterToString(operationType::ParameterTypesEnum ParameterType) {
		{
			switch (ParameterType)
			{
				case ParameterTypePOINT:
					return "POINT";
				case ParameterTypeMULTIPLEPOINTS:
					return "MULTIPOINT";
				case ParameterTypeFLOAT:
					return "FLOAT";
				case ParameterTypeLINE:
					return "LINE";
				case ParameterTypeSURFACE:
					return "SURFACE";
				case ParameterTypeTRIANGLE:
					return "TRIANGLE";
				case ParameterTypeOBJECT3D:
					return "OBJECT3D";
			default:
				break;
			}
		};
		return "";
	}
}

#endif // !OPERATIONTYPES_HEADER